# ta-isolate-01-rsi14-cross-50

## Purpose
Isolate `ta.rsi(close, 14)` (Wilder seed + smoothing) end-to-end. Any TV vs engine drift on this probe narrows root cause to:
- RSI math: engine `RMA::compute` / `RSI::compute` (now Pine bit-exact post fc22c34) — should be 0 ULP drift
- OHLCV input drift: TV chart's bar OHLC ≠ shipped corpus OHLCV — diagnosable via first-bar OHLC check below

Indirectly diagnoses gap probes:
- `validation/37-regex-string-filter` (RSI(14) + MACD)
- `validation_typed_matrix/typed-matrix-probe-01-bool-regime-mask` (RSI(14))

## Chart setup (TV)
- Symbol: **ETH-USDT-USDT** (perpetual swap)
- Exchange: pick the exchange whose first 15m bar at the corpus OHLCV start matches the shipped CSV. See "First-bar OHLC check" below.
- Timeframe: **15m**
- Range: cover the entire shipped window — at minimum from `2024-10-19 21:00 UTC` (warmup6m start) through `2026-05-04 03:00 UTC` (last bar).

## First-bar OHLC check (REQUIRED before export)
Shipped corpus `data/ohlcv_ETH-USDT-USDT_15m_warmup6m.csv` first bar:
- `timestamp = 1729371600000` → 2024-10-19 21:00 UTC
- expected OHLC = (read from CSV first row at this timestamp)

On TV: scroll to `2024-10-19 21:00 UTC` 15m bar. Hover. If TV's O/H/L/C at this bar matches the shipped CSV to ±$0.05, exchange feed is correct. If it differs, switch venues (BINANCE → BYBIT → OKX → MEXC) until match.

If no exchange matches: report back — corpus OHLCV provenance needs to be re-sourced separately.

## TV export
1. Apply `strategy.pine` to the verified chart.
2. `Strategy Tester → Performance Summary → List of Trades → Export → CSV`.
3. Save as `tv_trades.csv` in this directory.
4. (Optional but recommended) `Chart → Export Chart Data → CSV` over the same window. Save as `tv_ohlcv.csv` for cross-comparison even though the validator doesn't consume it yet.

## Validator usage
After `tv_trades.csv` is in place, runs as part of full corpus parity:
```bash
EIGEN3_INCLUDE_DIR=/opt/homebrew/include/eigen3 python /Users/haoliangwen/code/pineforge-utils/validator/validate.py \
  /path/to/pineforge-engine/corpus/validation_ta_isolate/ta-isolate-01-rsi14-cross-50 \
  --engine-repo /path/to/pineforge-engine \
  --codegen-repo /path/to/pineforge-codegen --workers 1 \
  --out-md /tmp/p01.md --out-json /tmp/p01.json
```

## Diagnostic interpretation
| Result | Conclusion |
|---|---|
| `excellent` (0 trade-count drift, pnl_p90 < 0.001) | Engine RSI(14) is bit-exact vs TV. The 37-regex / typed-matrix-01 gaps are **not** caused by RSI math — they are caused by OHLCV input drift OR by a non-RSI part of those strategies. |
| `strong/moderate` with 1-3 trade drift | Threshold-equality tie-break at `rsi == 50.0` ULP boundaries. Investigate engine `crossover` precision around exact threshold. |
| `moderate/weak` with >5% trade drift | RSI math itself drifts. Re-open Task B with this probe as the minimal repro. |
