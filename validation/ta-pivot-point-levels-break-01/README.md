# ta-pivot-point-levels-break-01

## Purpose
Isolate the free-function intrinsic `ta.pivot_point_levels("Traditional", anchor)` and
its array return shape. Validates:
- **Array return + indexing**: `array.get(pivots, 0)=P`, `array.get(pivots, 1)=R1`,
  `array.get(pivots, 2)=S1` — the engine's `std::vector<double>` ordering must match
  Pine's `[P, R1, S1, R2, S2, ...]` interleaving (verified in `src/ta_misc.cpp`:
  `out[0]=P; out[1]=R1; out[2]=S1; ...`).
- **Traditional pivot math**: `P=(H+L+C)/3`, `R1=2P-L`, `S1=2P-H` computed from the
  PREVIOUS bar's HLC. With a constant `anchor=true`, every bar opens its own period,
  so each bar's pivot set is derived from the last closed bar — exactly how the
  codegen lowers the call to `ta::pivot_point_levels("Traditional", _s_high[1],
  _s_low[1], _s_close[1])`.
- **na propagation** on the first bar (previous-bar HLC is NaN → 11-NaN vector → no
  crossover signal).

The trigger is a pure `close` vs level crossover/crossunder, so any divergence
isolates the pivot computation + array access, not entry/sizing/exit logic.

## TradingView export recipe
1. Symbol: **ETH-USDT-USDT** (the corpus default feed; any ETH-USDT perp on your
   data source that matches `data/ohlcv_ETH-USDT-USDT_15m_warmup6m.csv`).
2. Timeframe: **15m**.
3. Chart timezone: **UTC** (Exchange/UTC — must match the engine bar timestamps).
4. Strategy settings: leave `strategy(...)` defaults as declared in `strategy.pine`
   (initial_capital=1000000, commission 0, slippage 0, qty=1 fixed, pyramiding=1,
   process_orders_on_close=false). No setting changes required.
5. Apply `strategy.pine` to the chart → open **Strategy Tester** → **List of Trades**
   → **Export** → save as `tv_trades.csv` in this directory.
6. Move this directory into `corpus/validation/` to enroll it in the parity sweep.

## Notes on `anchor`
In native Pine, `anchor` is a per-period boolean (e.g. `timeframe.change("1D")`). This
probe uses a **constant `anchor=true`** so the period is a single bar and the pivot is
recomputed every bar from the prior bar's HLC. This is the form the transpiler models
exactly (it consumes `_s_high[1]/_s_low[1]/_s_close[1]` regardless of the anchor value),
so the constant-`true` anchor is what produces a clean engine↔TV comparison. Do not
swap in a sparser anchor for this probe — it would diverge by construction.

## Diagnostic interpretation
| Result | Conclusion |
|---|---|
| `excellent` | Engine `pivot_point_levels` Traditional math + array indexing bit-match TV. |
| `strong/moderate` 1-3 trades | Threshold-equality at `close == R1/S1` ULP boundaries; inspect `crossover` boundary semantics, not the pivot math. |
| `moderate/weak` significant drift | Either (a) array index ordering mismatch (R1/S1 swapped), (b) wrong HLC source bar (current vs previous), or (c) Traditional formula drift. Check `src/ta_misc.cpp::pivot_point_levels` against `out[1]=R1=2P-L`, `out[2]=S1=2P-H`, and the codegen `_s_*[1]` lowering in `pineforge-codegen/.../visit_call.py`. |
