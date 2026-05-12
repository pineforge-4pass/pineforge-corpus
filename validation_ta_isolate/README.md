# validation_ta_isolate

Minimal-repro TA-only probes that isolate single Pine TA functions end-to-end. Each probe runs one indicator with one threshold/cross trade rule, no risk management, no MTF, no session filters, no compositional logic. Any TV vs engine drift on these probes localizes the cause to (a) that one TA function's math OR (b) OHLCV input drift between TV chart and shipped corpus data.

## Probes

| Probe | Isolates | Indirectly diagnoses |
|---|---|---|
| `ta-isolate-01-rsi14-cross-50` | `ta.rsi(close, 14)` mid-band | `validation/37-regex-string-filter`, `validation_typed_matrix/typed-matrix-probe-01-bool-regime-mask` |
| `ta-isolate-02-rsi14-bands-30-70` | `ta.rsi(close, 14)` extrema | typed-matrix-01 (uses RSI > 60 / < 45) |
| `ta-isolate-03-macd-12-26-9-cross` | `ta.macd(close, 12, 26, 9)` EMA chain | 37-regex MACD path |
| `ta-isolate-04-hma-55-close-cross` | `ta.hma(close, 55)` (nested WMA + sqrt rounding) | `community/MarketShift` HMA path |
| `ta-isolate-05-sma-152-close-cross` | `ta.sma(close, 152)` sliding-sum precision | MarketShift SMA path |

## Workflow

1. **Pick a probe** from the table.
2. **Verify chart feed** matches shipped corpus `data/ohlcv_ETH-USDT-USDT_15m_warmup6m.csv`. Per probe 01's README, scroll to TV's bar at `2024-10-19 21:00 UTC` (the warmup6m start) and compare TV's O/H/L/C against shipped CSV's first row. If they agree to ±$0.05, feed is correct. If not, switch venues (BINANCE / BYBIT / OKX / MEXC / BITGET) until match.
3. **Apply `strategy.pine`** to the verified chart.
4. **Export** `Strategy Tester → List of Trades → CSV` → save as `tv_trades.csv` next to the strategy.
5. **Run validator** (single probe or include in full corpus sweep). Read result label.
6. **Interpret** per the per-probe diagnostic table.

## Interpretation matrix

If after exports:
- All 5 probes `excellent` → engine TA is bit-exact across the four functions used by gap probes. The 37-regex / typed-matrix-01 / MarketShift gaps are caused by **OHLCV input drift** in those probes' captured TV trades — solution is to re-export their `tv_trades.csv` against current TV charts (whose OHLCV will then match).
- Some probes `excellent`, others not → bug localized to the failing probe's specific TA function. Re-open engine task with the failing probe as minimal repro.
- All 5 probes drift → either feed-mismatch on the verified exchange (re-do step 2 more carefully), or systemic engine math regression (unlikely given Task B's per-bar Wilder validation).

## Notes
- All probes default to `corpus/data/ohlcv_ETH-USDT-USDT_15m_warmup6m.csv` via the validator's auto-detection — no per-probe OHLCV override needed.
- `inputs.json` is currently a comment placeholder; future validator extensions can add per-probe `tv_ohlcv_csv` keys here once supported.
- TV export TZ defaults to chart timezone (typically UTC for ETH-USDT-USDT). If your TV chart's timezone differs, add `"tv_trades_csv_tz": "America/New_York"` (etc) to that probe's `inputs.json`.
