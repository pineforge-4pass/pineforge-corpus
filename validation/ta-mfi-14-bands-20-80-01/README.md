# TA isolate probe — MFI(14) bands 20/80

**Indicator isolated**: `ta.mfi(hlc3, 14)` — Money Flow Index, a volume-weighted
RSI variant. Internally MFI accumulates positive vs. negative raw money flow
(typical price × volume) over the lookback, so its `.compute()` takes an
*explicit* source plus an *implicitly-appended* `volume`. This probe is the only
MFI TradingView-parity case in the corpus.

**Trigger**: static integer bands make the signal depend purely on the MFI value.
- Long on `ta.crossover(mfi, 20)` (cross up out of oversold).
- Short on `ta.crossunder(mfi, 80)` (cross down out of overbought).

Any trade-list drift implicates the money-flow accumulation (typical-price ×
volume, positive/negative flow split, RSI-style ratio), not the band logic.

**Engine baseline**: 442 trades (639 raw), net_profit=-186.09 over 53929 bars on
the default ETH-USDT-USDT 15m feed (warmup 6m).

## TradingView export recipe

1. Symbol: **ETH-USDT-USDT** (the corpus default feed; the exchange/pair must
   match the engine `ohlcv_csv` exactly so volume is identical — volume drives MFI).
2. Timeframe: **15m**.
3. Chart timezone: **UTC** (Exchange/UTC; corpus default).
4. Bar Magnifier: **OFF** in Strategy Tester settings.
5. Strategy properties — leave at script defaults (the `strategy(...)` call already
   pins initial_capital=1000000, commission 0, slippage 0, qty 1, pyramiding 1,
   process_orders_on_close=false). Do not override.
6. Apply `strategy.pine` to the chart.
7. Open **Strategy Tester -> List of Trades**.
8. **Export** the list -> save as `tv_trades.csv` in this directory.
9. Move this directory into `corpus/validation/` to enroll it in the parity sweep.
