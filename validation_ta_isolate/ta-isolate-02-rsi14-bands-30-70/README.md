# ta-isolate-02-rsi14-bands-30-70

## Purpose
Same isolated `ta.rsi(close, 14)` as probe 01 but with mean-reversion thresholds at 30 and 70. Cross-validates engine RSI at deeper extrema where Wilder smoothing can show different ULP-boundary behavior than mid-50 thresholds.

Indirectly diagnoses gap probes whose RSI threshold-cross logic uses non-50 bands.

## Chart setup (TV)
- Symbol: **ETH-USDT-USDT**
- Exchange: feed verified per probe 01's first-bar OHLC check (use the same exchange across all `validation_ta_isolate/*` probes)
- Timeframe: **15m**
- Range: full shipped warmup6m window (2024-10-19 21:00 UTC → 2026-05-04 03:00 UTC)

## TV export
1. Apply `strategy.pine` to chart.
2. `Strategy Tester → List of Trades → Export → CSV` → save as `tv_trades.csv`.

## Diagnostic interpretation
| Result | Conclusion |
|---|---|
| `excellent` AND probe 01 `excellent` | Engine RSI is bit-exact across mid + extrema. RSI math fully validated. |
| `excellent` but probe 01 `strong/moderate` | Drift is at the mid-50 boundary specifically. Investigate `crossover(x, 50.0)` precision. |
| `strong/moderate` while probe 01 `excellent` | Drift surfaces only at extrema — possible Wilder-saturation bug at RSI close to 0 or 100. Check engine RSI bounds. |
| Both `strong/moderate` | RSI math drifts globally OR OHLCV mismatch. Compare to per-bar trace from Task B. |
