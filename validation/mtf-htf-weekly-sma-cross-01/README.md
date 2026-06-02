# mtf-htf-weekly-sma-cross-01

## Purpose

Isolates **calendar WEEK (`"W"`) higher-timeframe aggregation** end-to-end via
`request.security`. The weekly SMA is computed *inside* the security context:

```pine
float wsma = request.security(syminfo.tickerid, "W", ta.sma(close, 10), lookahead=barmerge.lookahead_off)
```

The LTF (15m) `close` then drives `ta.crossover` / `ta.crossunder` against that
weekly SMA, flipping long/short. This is the only corpus probe whose trades are
driven off a **calendar-week** `request.security` chain, so it exercises:

- ISO Monday-based week-boundary bucketing in the engine
  (`CalendarPeriod::WEEK`, `calendar_week_open_local_ms`),
- HTF→LTF roll-forward of the weekly SMA into each 15m bar,
- `lookahead_off` semantics (the current, still-forming week's SMA value is the
  one confirmed at the *previous* week close, rolled forward).

A TradingView "List of trades" export adjudicates the week-open alignment and
the roll-forward against the engine.

## Engine result (for reference)

- Feed: `corpus/data/ohlcv_ETH-USDT-USDT_15m_warmup6m.csv` (default corpus feed).
- Engine: **96 trades**, net_profit ≈ 4736.94, 53929 bars. See `engine_trades.csv`.

## TradingView export recipe

1. Open a TradingView chart for symbol **ETHUSDT.P** on the **BINANCE** exchange
   (perpetual; the corpus feed is `ETH-USDT-USDT`). Use whichever data source
   matches the corpus OHLCV — the corpus default is Binance ETH-USDT perpetual.
2. Set the chart **timeframe to 15m**.
3. Set the chart **timezone to UTC** (right-click the time axis → Timezone → UTC).
   The corpus runs the engine on its UTC fast path; the chart TZ must match or
   the weekly buckets shift and trades diverge.
4. Pine Editor → paste the contents of `strategy.pine` → **Add to chart**.
5. Do **not** change any `strategy(...)` settings — the script already pins
   `initial_capital=1000000`, `commission_value=0`, `slippage=0`,
   `default_qty_value=1`, `pyramiding=1`, `process_orders_on_close=false`.
   In Strategy Tester → Properties, confirm Commission = 0, Slippage = 0,
   Order size = 1 contract, Pyramiding = 1, Recalculate = off,
   "Fill orders on bar close" = off.
6. Strategy Tester → **List of Trades** → **Export** (download icon).
7. Save the exported CSV as **`tv_trades.csv`** in this directory.
8. Move this whole directory into `corpus/validation/` to enroll it in the
   parity sweep (`./scripts/run_corpus.sh`).

### Notes on alignment

- The engine comparison window matches the non-warmup reference feed
  (`corpus/data/ohlcv_ETH-USDT-USDT_15m.csv`), which begins **2025-04-20 UTC**.
  Warmup bars before that prime the weekly SMA so the first compared trade is
  not a warmup artifact. Export a TV window that comfortably covers this range.
- If your TV chart and the corpus feed disagree on a few early weekly bars, the
  expected divergence is confined to the very first weekly-SMA window
  (`ta.sma(close,10)` on weekly bars = ~10 weeks of priming).
