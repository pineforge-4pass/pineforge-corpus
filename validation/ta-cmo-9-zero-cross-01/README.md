# ta-cmo-9-zero-cross-01

## Purpose

Isolate `ta.cmo(series, length)` — the Chande Momentum Oscillator — against
TradingView parity. CMO sums up-moves and down-moves over `length` bars and
returns `100 * (sumUp - sumDown) / (sumUp + sumDown)`, bounded to [-100, +100].

This probe computes `cmo = ta.cmo(close, 9)` and drives a flat long/short flip
purely off the zero-line sign change:

- `ta.crossover(cmo, 0)`  -> `strategy.entry("L", strategy.long)`
- `ta.crossunder(cmo, 0)` -> `strategy.entry("S", strategy.short)`

No signal line, no thresholds, no filters — the only moving part is CMO itself,
so a TV List-of-Trades export adjudicates the engine's CMO warmup window and
up/down accumulation directly. CMO has no TradingView-parity coverage in the
corpus yet.

## Engine run (already done)

- Feed: ETH-USDT-USDT 15m, 6-month warmup (corpus default feed).
- Result: 6189 trades, net_profit=1442.20, 53929 bars -> `engine_trades.csv`.

## TradingView export recipe

1. Open TradingView, chart symbol **ETHUSDT** (the perpetual / USDT-margined
   feed matching `ETH-USDT-USDT`; use the same exchange the corpus feed was
   sourced from).
2. Set the timeframe to **15m**.
3. Set the **chart timezone to UTC** (right-click the time axis -> Timezone ->
   UTC) so exported timestamps line up with the engine's UTC bars.
4. Paste the contents of `strategy.pine` into the Pine Editor and click
   **Add to chart**. Do NOT change any `strategy()` setting — the script already
   pins initial_capital=1000000, commission=0, slippage=0, default_qty=1,
   pyramiding=1, process_orders_on_close=false.
5. Open **Strategy Tester** -> **List of Trades** tab.
6. Click **Export** (download icon) and save the CSV as `tv_trades.csv` in this
   directory (`proposed-probes/ta-cmo-9-zero-cross-01/`).
7. Move this whole directory into `corpus/validation/` so the corpus sweep
   (`./scripts/run_corpus.sh`) picks it up.

## Files

- `strategy.pine`    — clean-room Pine v6 source.
- `generated.cpp`    — transpiled engine strategy (`ta::CMO`).
- `strategy.dylib`   — compiled engine strategy.
- `engine_trades.csv`— engine run output (6189 trades).
- `inputs.json`      — feed note (ETH-USDT-USDT 15m warmup6m).
- `tv_trades.csv`    — (you add this) TradingView export for parity.
