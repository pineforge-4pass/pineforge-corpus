# ta-accdist-ema-cross-01

## Feature under test
Isolates `ta.accdist` — the no-arg Accumulation/Distribution running-sum SERIES
variable. A/D is a cumulative-over-all-bars accumulator (like `ta.obv`): each bar
adds `money flow volume = CLV * volume`, where the close-location value
`CLV = (2*close - low - high) / (high - low)` and is forced to 0 when
`high == low`. This probe drives entries on `ta.crossover` / `ta.crossunder` of
A/D against its own `ta.ema(ad, 21)` signal.

## Expected behavior
- `ad = ta.accdist` seeds on the first bar and accumulates every subsequent bar;
  any divergence in the first-bar seed or the `high==low` CLV guard compounds for
  the entire run, so the trade list is a sensitive end-to-end check of the
  accumulator.
- `sig = ta.ema(ad, 21)` smooths the A/D line; crossovers flip a single-unit
  long/short position (pyramiding=1, fixed qty 1).
- Pass = engine `List of trades` matches the TV export exactly. Any drift
  implicates the A/D accumulator formula, its seeding, or the CLV zero-range guard
  rather than EMA/cross plumbing (those are already corpus-covered elsewhere).

## Engine result (this run)
- Feed: `corpus/data/ohlcv_ETH-USDT-USDT_15m.csv`, 15m, ~6-month window.
- 5487 closed trades, net_profit ≈ -3243.70 (informational; parity is judged on
  the trade list, not PnL).
- `engine_trades.csv` written by `scripts/run_strategy.py`.

## TradingView export recipe
1. Symbol: **ETH-USDT-USDT** (the exact source of
   `corpus/data/ohlcv_ETH-USDT-USDT_15m.csv`).
2. Timeframe: **15m**.
3. Chart timezone: **UTC** (match the engine chart timezone; the corpus feed is
   UTC-stamped).
4. Strategy `strategy()` settings: leave defaults from `strategy.pine`
   (initial_capital=1000000, commission 0%, slippage 0, qty 1, pyramiding 1,
   `process_orders_on_close=false`). No setting needs changing.
5. Add `strategy.pine` to the chart → open **Strategy Tester** → **List of Trades**
   → **Export** → save as `tv_trades.csv` in this directory.
6. Move this directory into `corpus/validation/` to enroll it in the parity sweep.

## Regression value
First TV-parity coverage of the cumulative `ta.accdist` accumulator in the corpus.
Complements the OBV/PVT/NVI/PVI family (same no-arg running-sum codegen path) and
guards the CLV money-flow formula and its first-bar seeding against drift.
