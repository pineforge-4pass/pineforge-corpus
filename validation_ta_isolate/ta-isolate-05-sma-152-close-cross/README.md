# ta-isolate-05-sma-152-close-cross

## Purpose
Isolate `ta.sma(close, 152)` over a long window (~38 hours on 15m TF). Tests engine sliding-window sum precision. Naive running-sum has ULP drift `~1.5e-13` per update; over 30k bars that compounds to ~1e-9 — measurable at threshold crossings.

Targets gap probe `community/MarketShift` which uses SMA(close, 152).

## Chart setup (TV)
- Symbol: **ETH-USDT-USDT**
- Exchange: same feed as probes 01-04 (per first-bar OHLC check)
- Timeframe: **15m**
- Range: full shipped warmup6m window

## TV export
1. Apply `strategy.pine`.
2. `Strategy Tester → List of Trades → Export → CSV` → `tv_trades.csv`.

## Diagnostic interpretation
| Result | Conclusion |
|---|---|
| `excellent` | Engine SMA sliding-sum is bit-exact at length 152. MarketShift SMA path is fine — combined with probe 04 result, isolates the MarketShift bug to its non-TA logic OR to OHLCV input. |
| `strong/moderate` 1-3 trades | Sliding-sum running-error at threshold boundary. Consider engine periodic-recompute every N bars to bound drift. |
| `moderate/weak` significant drift | Algorithmic difference: TV may use full N-bar resum each bar (slow but bit-exact); engine may use running-sum (fast but drifty). Switch engine to per-bar resum at long lengths. |
