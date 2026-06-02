# symbol-fx-5dp-eurusd-01

## Purpose
FX 5-decimal-pricing probe. Drives a plain `ta.sma(close, 20)` / `ta.sma(close, 50)`
crossover (long on crossover, short on crossunder, `strategy.close` the opposite
leg first) on **OANDA:EURUSD 15m** — a 5-decimal forex instrument with
`mintick = 0.00001`.

This isolates two behaviors the ETH-USDT-USDT corpus feed cannot exercise:

1. **5-decimal price scale.** Prices are ~1.0xxxx instead of ~1800, so any latent
   assumption about price magnitude, tick granularity, or float precision in the
   fill / reporting path surfaces here.
2. **Sub-pip slippage rounding.** `slippage=2` means 2 ticks * `0.00001` =
   `0.00002` added (long) / subtracted (short) at fill. The engine computes
   `slip = slippage_ * syminfo_mintick_`, so the result must land exactly on the
   5th decimal and match TradingView's fill prices to the pip fraction.

The SMA math is deliberately trivial (no Wilder smoothing, no state) so that any
parity drift points squarely at price-scale / slippage-rounding, not indicator
math.

## Required feed (NOT the corpus ETH feed)
This probe needs a EURUSD 15m OHLCV CSV. It is **not** runnable against the
shipped `data/ohlcv_ETH-USDT-USDT_*.csv` feed, so it ships un-run
(`compiled=false`, `ran=false`). The maintainer who exports `tv_trades.csv` must
supply the matching OHLCV CSV and set the tick size.

- `inputs.json` -> `ohlcv_csv`: `ohlcv_OANDA_EURUSD_15m.csv` (drop the EURUSD 15m
  feed next to this file with that name, or repoint the key).
- `inputs.json` -> `mintick` / `runtime_overrides.syminfo_metadata.mintick`:
  `0.00001`. The engine's slippage rounding uses `syminfo.mintick`; it defaults
  to `0.01`, which is wrong for EURUSD and will mis-scale the slippage. Ensure
  the harness wires `syminfo.mintick = 0.00001` before claiming parity.

## TradingView export recipe
1. Open a TradingView chart.
2. Symbol: **OANDA:EURUSD**. Timeframe: **15m**.
3. Chart timezone: set to **UTC** (bottom-right clock -> Etc/UTC). The export's
   trade timestamps will then be UTC, matching `tv_trades_csv_tz: "utc"` and
   `engine_chart_timezone: "Etc/UTC"` in `inputs.json`.
4. Strategy settings: leave `strategy()` as declared in `strategy.pine`
   (`initial_capital=1000000`, `commission_value=0`, **`slippage=2`**,
   `default_qty_value=1`, `pyramiding=1`, `process_orders_on_close=false`).
   Do not change Properties in the Strategy Tester — the `strategy(...)` header
   already encodes them.
5. Apply `strategy.pine` to the chart (Pine Editor -> Add to chart).
6. Open **Strategy Tester -> List of Trades**.
7. Click **Export** and save the CSV as `tv_trades.csv` in this directory.
8. Capture the same EURUSD 15m OHLCV window TradingView charted and save it as
   `ohlcv_OANDA_EURUSD_15m.csv` in this directory (or repoint
   `inputs.json -> ohlcv_csv`).
9. Move this directory into `corpus/validation/` to enroll it in the parity sweep.

## Diagnostic interpretation
| Result | Conclusion |
|---|---|
| `excellent` (0 trade-count drift, tight pnl) | Engine handles 5-dp pricing and sub-pip (`slippage*0.00001`) rounding bit-exact vs TV. |
| fill prices off by ~0.00001-0.00002 | Slippage rounding / mintick scaling. Verify `syminfo.mintick = 0.00001` reached the engine (default `0.01` mis-scales `slip = slippage_ * syminfo_mintick_`). |
| broad trade-count / pnl drift | OHLCV input mismatch: the EURUSD CSV bars differ from TV's charted bars. Re-capture the exact charted window. |
