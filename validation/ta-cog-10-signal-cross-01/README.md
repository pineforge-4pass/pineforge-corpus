# ta-cog-10-signal-cross-01

## Purpose
Isolate `ta.cog(close, 10)` — Ehlers **Center of Gravity** oscillator. Definition:

```
cog = -sum(src[i] * (i + 1), length) / sum(src[i], length)   for i in [0, length-1]
```

This is a SMA-weighted ratio with two parity-sensitive traits the engine must
match bit-for-bit:
- the leading **negative sign**,
- the **1-based position weight** `(i + 1)` applied to the most-recent bar first
  (`close[0]` weighted 1, `close[length-1]` weighted `length`),
- divide-by-`sum(src)` normalization, including window-seeding on the first
  `length` bars.

`ta.ema(cog, 5)` is the signal line; `crossover`/`crossunder(cog, sig)` is the
trigger only — it is not the subject under test, it merely converts COG dynamics
into a trade stream a TV export can adjudicate.

## Setup + export (TradingView)
See `../README.md` for the shared conventions. Quick reference:
- **Symbol / timeframe:** `ETH-USDT-USDT`, `15m` (BINANCE ETHUSDT.P or the matching
  corpus venue), full warmup6m window.
- **Chart timezone:** UTC (`Etc/UTC`) — matches the corpus engine_chart_timezone.
- **strategy() settings:** none to change. Defaults baked into `strategy.pine`
  (initial_capital=1000000, commission 0, slippage 0, pyramiding=1,
  process_orders_on_close=false).

Recipe:
1. Open the chart at `ETH-USDT-USDT` `15m`, chart timezone = UTC.
2. Add `strategy.pine` to the chart.
3. Strategy Tester → **List of Trades** → **Export** → save as `tv_trades.csv`
   in this directory.
4. Move this directory into `corpus/validation/` to enroll it in `run_corpus.sh`.

## Diagnostic interpretation
| Result | Conclusion |
|---|---|
| `excellent` | Engine `ta.cog(10)` bit-exact: sign, 1-based weighting, and sum normalization all correct. |
| `strong/moderate`, few trades | Threshold-equality at `cog == sig` ULP boundaries near the EMA(5) cross; inspect engine `crossover` boundary semantics, not COG itself. |
| `moderate/weak`, significant drift | Engine COG math drifts. Prime suspects: weight index direction (most-recent vs oldest bar gets weight 1), the negative sign, or window-seed behavior on the first 10 bars. Extend a `tv_ta_isolate_helper.pine` with `float t_cog10 = ta.cog(close, 10)` and per-bar diff against the engine series. |
