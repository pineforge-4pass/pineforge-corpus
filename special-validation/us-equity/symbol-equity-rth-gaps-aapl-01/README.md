# symbol-equity-rth-gaps-aapl-01

**Status:** TV_RUN_NEEDED (instrument feed required — not an ETH-feed probe)

## Purpose

US-equity Regular-Trading-Hours (RTH) + overnight-gap parity probe.

Entries are plain `ta.rsi(close, 14)` oversold/overbought crosses:
- `ta.crossover(rsi, 30)` → `strategy.entry("Long", strategy.long)`
- `ta.crossunder(rsi, 70)` → `strategy.close("Long")`

Every order is gated by `session.ismarket`, so only bars inside the regular
trading window (09:30–16:00 America/New_York) can trade. This isolates three
behaviors at once:

1. **RTH session filter** — `session.ismarket` backed by `syminfo.session`
   (`"0930-1600"`), injected via `runtime_overrides`.
2. **Overnight gap fills** — the RSI(14) series is continuous across the
   16:00→09:30 overnight gap (no per-day reset), so a cross can latch on the
   first RTH bar after the gap. Verifies the engine carries series state across
   non-session bars exactly as TradingView does.
3. **Exchange-tz vs chart-tz day boundary** — `timezone = America/New_York`
   must match the chart timezone so the RTH window aligns to the correct
   calendar day for each bar.

## Required feed (INSTRUMENT — not ETH)

This probe needs a user-provided **NASDAQ:AAPL 5m** OHLCV CSV. It is NOT runnable
against the bundled ETH 15m reference feed, so `generated.cpp` was transpiled but
NOT compiled/run here (`compiled=false`, `ran=false`).

Place the feed next to this file as the name referenced in `inputs.json`:

```
ohlcv_NASDAQ-AAPL_5m.csv
```

Recommended characteristics:
- 5-minute bars, NASDAQ:AAPL.
- Include the full session day (RTH 09:30–16:00 ET; pre/post bars may be present —
  `session.ismarket` will exclude them). Ensure the file contains the overnight
  gaps between trading days.
- Timestamps interpreted in chart timezone `America/New_York`
  (`chart_timezone` in `inputs.json`).

## TradingView export recipe (capture tv_trades.csv)

1. Open a **NASDAQ:AAPL** chart, timeframe **5m**.
2. Set the **chart timezone** to **America/New_York** (Exchange tz):
   right-click the time axis → *Timezone* → *(UTC-05:00/-04:00) New York*,
   or bottom-right clock → New York.
3. Ensure the chart shows the **regular session only** so RTH bars match
   (Chart settings → Symbol → *Regular Trading Hours*), matching the
   `0930-1600` session override.
4. Apply this probe's `strategy.pine` (Pine Editor → paste → *Add to chart*).
5. Use the **default `strategy(...)` settings** baked into the script
   (initial_capital 1,000,000 USD; commission 0%; slippage 0; qty 1 contract;
   pyramiding 1; process_orders_on_close = false). Do not override them in the
   Strategy Tester Properties tab.
6. Pick a contiguous date range that includes several overnight gaps
   (e.g. ~3 months of 5m data) and note it for reproducibility.
7. Strategy Tester → **List of Trades** → **Export** → save as
   `tv_trades.csv` **in this directory**.
8. Move this directory into `corpus/validation/` (suggested:
   `corpus/validation/symbol-specified/AAPL/`) so the corpus runner picks it up.

## Files

- `strategy.pine` — clean-room Pine v6 source.
- `generated.cpp` — transpiler output (transpiled OK; compiled/run deferred to
  feed availability).
- `inputs.json` — harness config: `ohlcv_csv`, `chart_timezone`,
  `tv_trades_csv_tz`, and `runtime_overrides.session = "0930-1600"` /
  `timezone = "America/New_York"`.
