# ta-obv-ema-cross-01

## Purpose

Isolate **`ta.obv`** — On-Balance Volume, used as a SERIES property (no parens:
`obv = ta.obv`). OBV is an unbounded cumulative volume-flow sum: on each bar it
adds the full bar volume when `close > close[1]`, subtracts it when
`close < close[1]`, and carries forward unchanged on a flat close, accumulating
from the very first bar. The probe crosses OBV against its own `ta.ema(obv, 21)`
signal line and trades the cross (long on cross up, short on cross down).

Because OBV's magnitude is dominated by the cumulative seed (the running sum from
bar 0) and the per-bar direction tie-break (`close == close[1]`), only a real
TradingView export can adjudicate the engine's `ta::OBV` against TV. The crossing
of OBV with its EMA is sensitive to both the cumulative offset and the per-bar
volume sign, so a parity match here exercises the indicator end to end.

Engine run (corpus default ETH feed): **5089 trades** (7444 raw),
net_profit=1919.50, bars=53929. See `engine_trades.csv`.

## TradingView export recipe

1. **Symbol:** `ETH-USDT-USDT` perpetual (the corpus reference feed — the same
   ETH/USDT perpetual the corpus `ohlcv_ETH-USDT-USDT_15m_warmup6m.csv` was
   sampled from; use your exchange's ETHUSDT perpetual contract).
2. **Timeframe:** `15m`.
3. **Chart timezone:** `Asia/Taipei (UTC+8)` — the corpus default. The verifier
   parses TV trade timestamps as UTC+8 wall time and engine timestamps as UTC.
   If you export under a different chart tz, record it as `tv_trades_csv_tz` in
   `inputs.json` (e.g. `"utc"`) so the comparison stays aligned.
4. **strategy() settings:** none to change — the script declares
   `initial_capital=1000000`, `commission_value=0`, `slippage=0`,
   `default_qty_value=1`, `pyramiding=1`, `process_orders_on_close=false`.
   Leave the Strategy Tester properties at the script-declared values.
5. Apply `strategy.pine` to the chart.
6. Open **Strategy Tester -> List of Trades**.
7. **Export** the trade list and save it as `tv_trades.csv` in this directory.
8. Move this directory into `corpus/validation/` and run `./scripts/run_corpus.sh`
   (or `scripts/verify_corpus.py`) to score parity against `engine_trades.csv`.
