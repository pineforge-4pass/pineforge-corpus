# mtf-htf-monthly-ema-cross-01

## Purpose

Isolates **calendar-MONTH** HTF aggregation through `request.security`. Unlike
the fixed-width HTF buckets already covered by the corpus (`"60"`, `"240"`,
`"D"`), the `"M"` timeframe rolls on **variable-length calendar months**
(28-31 days). This probe drives:

```pine
float mema = request.security(syminfo.tickerid, "M", ta.ema(close, 6), lookahead=barmerge.lookahead_off)
```

so `ta.ema(close, 6)` advances **once per closed calendar month** and is held
flat across every intraday (15m) bar of that month. Trades fire on the LTF
`close` crossing the held monthly EMA (`ta.crossover` long / `ta.crossunder`
short), so any month-boundary detection, roll/seed, or held-value drift
surfaces as extra / early / missing crossings end-to-end.

## Status — BLOCKED on engine support (transpiled + compiled, did NOT run)

- `strategy.pine` transpiles cleanly; `generated.cpp` emits
  `register_security_eval(0, "M", input_tf_, ...)`.
- `generated.cpp` compiles to `strategy.dylib`.
- The **run is rejected** by the engine:
  `request.security: invalid timeframe literal 'M'`.

Root cause: `BacktestEngine::validate_security_timeframes`
(`src/engine_security.cpp:140`) rejects any requested TF whose
`safe_tf_to_seconds(...) <= 0`. For monthly, `tf_to_seconds("M")` returns
`-1` (calendar marker, `src/timeframe.cpp`), so it is rejected — even though
the aggregation machinery already supports it:

- `tf_ratio(input, "M")` returns `-1` for the calendar case
  (`src/timeframe.cpp`).
- `register_security_eval` builds a CALENDAR `TimeframeAggregator` when
  `ratio == -1` (`src/engine_security.cpp`).
- `calendar_period_for("M")` maps to `CalendarPeriod::MONTH`, and the
  aggregator implements CALENDAR mode.

So monthly `request.security` is plumbed end-to-end but **unreachable**
because the validation guard fires first. Once
`validate_security_timeframes` admits calendar periods (negative
`requested_seconds`) as valid HTF, this probe runs as-is and becomes a
ready TradingView parity test. No engine source was modified by this probe.

## TradingView export recipe

1. Symbol: **ETH-USDT-USDT** (the same instrument as
   `corpus/data/ohlcv_ETH-USDT-USDT_15m.csv`). Use the BINANCE perpetual feed
   that the corpus 15m CSV was exported from.
2. Chart timeframe: **15m**.
3. Chart timezone: **UTC** (matches the engine's UTC bar timestamps and the
   monthly calendar-boundary detection, which is computed in UTC via
   `gmtime_r`).
4. Strategy settings: leave the `strategy(...)` defaults baked into
   `strategy.pine` (initial_capital=1,000,000 USD, 0 commission, 0 slippage,
   default_qty=1, pyramiding=1, process_orders_on_close=false). No overrides.
5. Apply `strategy.pine` to the chart.
6. Open **Strategy Tester -> List of Trades -> Export** and save the CSV as
   `tv_trades.csv` in this directory.
7. Once the engine accepts monthly `request.security`, regenerate
   `engine_trades.csv`:

   ```bash
   EIGEN=/opt/homebrew/include/eigen3
   clang++ -std=c++17 -O2 -fPIC -shared -I include -I "$EIGEN" \
       proposed-probes/mtf-htf-monthly-ema-cross-01/generated.cpp \
       -Wl,-force_load,build/lib/libpineforge.a \
       -o proposed-probes/mtf-htf-monthly-ema-cross-01/strategy.dylib
   python3 scripts/run_strategy.py proposed-probes/mtf-htf-monthly-ema-cross-01 \
       --so-name strategy.dylib
   ```

8. Move this directory into `corpus/validation/` for the parity sweep.
