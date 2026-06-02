# leverage-margin-call-perp-5x-01

## Purpose

Isolate TradingView's broker-emulator **forced liquidation (Margin Call)** for an
**in-position long held through an adverse move at 5x leverage**.

The strategy declares `margin_long=20, margin_short=20` — 20% required margin,
i.e. **5x leverage**. A single, oversized long (notional ≈ 5 × `initial_capital`)
is entered **once** near the start of the feed (first bar at/after `bar_index 50`,
while flat and with no prior closed trades) and is **never exited by the script**.
There is no `strategy.close` / `strategy.exit` anywhere.

If price falls far enough that the position's losses exhaust the posted 20%
margin, TradingView's emulator injects a **Margin Call** exit row that the script
itself never issued.

### Expected divergence (this is the finding)

The PineForge engine currently has **no in-position liquidation path**: it holds
the entry indefinitely and produces no margin-call exit. This probe is **expected
to expose that gap** — TV emits a forced-liquidation exit, the engine does not.
The divergence is the point of the probe, not a bug to mask. `inputs.json` records
`expect_tv_match: false` accordingly.

When exporting, **capture the TV "Margin Call" rows** (TV labels the forced exit
explicitly in the Type / Signal column).

## Feed

This probe is **not** runnable against the corpus default feed unattended (it is
filed as a non-ETH-feed / instrument probe). The maintainer who exports
`tv_trades.csv` supplies the matching **BINANCE:ETHUSDT.P** (Binance USDT-M
perpetual) **15m** OHLCV CSV and points `ohlcv_csv` in `inputs.json` at it. Use
the same perpetual 15m bars that produced the TV export so the adverse move that
triggers the Margin Call is reproduced bar-for-bar. The corpus
`ohlcv_ETH-USDT-USDT_15m*.csv` feeds are the same instrument family and are the
recommended source.

## TradingView export recipe

1. **Symbol:** `BINANCE:ETHUSDT.P` (Binance USDT-M perpetual).
2. **Timeframe:** `15m`.
3. **Chart timezone:** `Asia/Taipei (UTC+8)` — the corpus default. The verifier
   parses TV trade timestamps as UTC+8 wall time and engine timestamps as UTC.
   `inputs.json` records `engine_chart_timezone: "Asia/Taipei"` and
   `tv_trades_csv_tz: "utc+8"`. If you export under a different chart tz, update
   `tv_trades_csv_tz` to match (e.g. `"utc"`).
4. **strategy() settings:** the script header sets
   `initial_capital=1000000`, `currency=USD`, `commission_value=0`, `slippage=0`,
   `default_qty_value=1`, `pyramiding=1`, `process_orders_on_close=false`, and
   crucially `margin_long=20, margin_short=20` (**5x leverage**). Open the
   Strategy Tester **Properties** tab and **verify the margin shows 20% long /
   20% short** — this is what enables the Margin Call. Leave everything else at
   the script-declared values.
5. Apply `strategy.pine` to the chart.
6. Open **Strategy Tester -> List of Trades**.
7. **Export** the trade list. **IMPORTANT:** make sure the export includes the
   **Margin Call** row(s) — that forced-liquidation exit is the whole point of the
   probe. Save the file as `tv_trades.csv` in this directory.
8. Move this directory into `corpus/validation/` and run `./scripts/run_corpus.sh`
   (or `scripts/verify_corpus.py`). The expected result is a documented
   divergence: TV has a Margin Call exit, the engine holds the position open.
