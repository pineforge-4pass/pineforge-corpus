# ta-pvt-ema-cross-01

## Purpose
Isolate `ta.pvt` (Price-Volume Trend), a cumulative volume-state SERIES built-in.
Pine definition (running accumulator, seeded on first valid bar):

```
pvt := nz(pvt[1]) + volume * (close - close[1]) / close[1]
```

The only logic is `ta.crossover/crossunder(pvt, ta.ema(pvt, 21))`, so any parity
drift is attributable to the `ta.pvt` series alone. Validates:
- Cumulative seeding on the first bar (no `close[1]` available → contribution skipped)
- Division by previous close `(close - close[1]) / close[1]` per bar
- Running sum carried across the full window (drift compounds, surfacing any
  off-by-one in the accumulator faster than a non-cumulative indicator would)
- Feeding a raw cumulative series into `ta.ema(...)` and crossover boundary semantics

`ta.pvt` shares the running-accumulator path with `ta.obv/accdist/nvi/pvi` and has
ZERO TradingView-parity coverage in this corpus.

## Setup + export
See `../README.md`. Quick reference:
- Symbol/TF: **ETH-USDT-USDT 15m**, full warmup6m window (corpus default feed)
- Chart timezone: **UTC** (engine `engine_chart_timezone` default for this feed)
- No non-default `strategy()` settings to change — declaration matches corpus baseline
  (initial_capital=1000000, commission 0, slippage 0, default_qty 1, pyramiding 1,
  process_orders_on_close=false)
- Apply `strategy.pine` → Strategy Tester → List of Trades → Export → save as
  `tv_trades.csv` next to this README → move this dir into `corpus/validation/`

## Diagnostic interpretation
| Result | Conclusion |
|---|---|
| `excellent` | Engine `ta.pvt` cumulative accumulator is bit-exact vs TV. Volume-state running-sum path validated; clears the shared accumulator family (obv/accdist/nvi/pvi). |
| `strong/moderate` 1-3 trades | Threshold-equality at `pvt == sig` ULP boundaries, or a single-bar seed difference on the first valid bar. Inspect engine `crossover` boundary semantics and the first-bar `nz(pvt[1])` seed. |
| `moderate/weak` significant drift | Engine PVT accumulator drifts. Likely candidates: seed bar index (first contribution included vs skipped), `(close - close[1]) / close[1]` ordering/precision, or `volume` source alignment. Because PVT is cumulative the error compounds — confirm with a per-bar `float t_pvt = ta.pvt` diff in the TA helper. |

## Engine run (reference)
- `python3 scripts/run_strategy.py . --so-name strategy.dylib` → 2749 trades (4076 raw),
  net_profit=3738.63, bars=53929. `engine_trades.csv` committed for diffing.
