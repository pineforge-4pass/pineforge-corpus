# ta-nvi-pvi-cross-01

## Purpose
Isolate the two cumulative volume-conditioned built-in SERIES properties
`ta.nvi` (Negative Volume Index) and `ta.pvi` (Positive Volume Index), neither
of which currently has any TradingView-parity coverage in the corpus.

Both indices are stateful and update only conditionally:
- **PVI** changes only on bars where `volume > volume[1]` (volume rises),
  accumulating the period's percentage price change; it is carried forward
  unchanged on flat/falling-volume bars.
- **NVI** changes only on bars where `volume < volume[1]` (volume falls),
  accumulating the percentage price change; carried forward otherwise.

The probe drives directional entries off each index versus its own EMA(255):
- **Long** when `ta.crossover(ta.pvi, ta.ema(ta.pvi, 255))`.
- **Short** when `ta.crossunder(ta.nvi, ta.ema(ta.nvi, 255))`.

This exercises (a) the seed/initial value of each index, (b) the strict
volume-comparison branch (`>` vs `<`, and the equal-volume carry-forward case),
and (c) the percentage-change accumulation feeding a long EMA. A drift here
points at the volume-comparison branch or the carry-forward (hold) semantics
rather than the EMA, which is already corpus-covered elsewhere.

Note: spec offered a `ta.pvi`-alone fallback if either index were unsupported.
Both are supported by the transpiler (emitted as `ta::PVI` / `ta::NVI`), so the
full NVI+PVI form is used; no fallback was needed.

## Setup + export (TradingView)
1. Symbol: **BINANCE:ETHUSDT.P** (ETH-USDT-USDT perpetual), timeframe **15m**.
2. Chart timezone: **UTC** (Etc/UTC). The corpus feed is UTC-stamped.
3. Load enough history to cover the corpus warmup window
   (`data/ohlcv_ETH-USDT-USDT_15m_warmup6m.csv`, first bar 2024-10-19 21:00 UTC;
   the EMA(255) needs ample warmup so include the full window before the test
   period begins).
4. Apply `strategy.pine` (no `strategy()` settings need changing — defaults in
   the script: initial_capital=1000000, USD, commission 0%, slippage 0,
   default_qty 1, pyramiding 1, process_orders_on_close=false).
5. Strategy Tester -> **List of Trades** -> **Export** -> save as
   `tv_trades.csv` in this directory.
6. Move this directory into `corpus/validation/` to enroll the probe.

## Engine run (already done)
- Feed: `corpus/data/ohlcv_ETH-USDT-USDT_15m_warmup6m.csv` (15m, warmup6m).
- `engine_trades.csv`: 271 closed trades (385 raw fills), net_profit 1196.34.
- Reproduce:
  ```
  EIGEN=/opt/homebrew/include/eigen3
  clang++ -std=c++17 -O2 -fPIC -shared -I include -I "$EIGEN" \
    proposed-probes/ta-nvi-pvi-cross-01/generated.cpp \
    -Wl,-force_load,build/lib/libpineforge.a \
    -o proposed-probes/ta-nvi-pvi-cross-01/strategy.dylib
  python3 scripts/run_strategy.py proposed-probes/ta-nvi-pvi-cross-01 --so-name strategy.dylib
  ```

## Diagnostic interpretation
| result | conclusion |
|---|---|
| `excellent` | Engine `ta.nvi`/`ta.pvi` match TV: seed value, volume-comparison branch, and equal-volume carry-forward all correct. |
| `strong/moderate` | Likely a carry-forward (hold-on-non-triggering-bar) or seed-value offset in NVI/PVI; the EMA path is corpus-proven, so suspect the index accumulation, not the smoothing. |
| `weak/fail` | Volume-comparison branch divergence (e.g. `>=`/`>` boundary on equal volume, or wrong base/seed) shifting the whole index series. |
