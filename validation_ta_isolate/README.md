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

## Existing tooling (use these, do not reinvent)

These live outside the corpus:

- `~/code/pineforge-utils/per-bar-trace/fetch_tv_indicator.py` — streams per-bar OHLCV + indicator plot values from any published Pine indicator on TV via the `tradingview-scraper` WebSocket. Output CSV format: `time,open,high,low,close,volume,plot1,plot2,...`.
- `~/code/pineforge-utils/per-bar-trace/pine-helpers/tv_ta_basic_helper.pine` — TV-side helper publishing 43 plot columns (RSI/MACD/SMA/EMA/HMA/etc., common short lengths). Use to fetch per-bar reference values for any of those functions.
- `~/code/pineforge-utils/per-bar-trace/diff_engine_vs_tv.py` — diffs a `// @pf-trace` engine JSON dump against a TV CSV from the fetcher.
- `~/code/tradingview-scraper` — patched fork supporting `request_more_data` pagination on the prodata endpoint (~21k bars per session vs free 5k cap). Uses cookie auth.
- `~/code/tradingview-scraper/examples/extract_sp500_15m_history.py` — full reference for fetching long history bypassing the 5k cap.

## Workflow

### Step 1 — verify chart feed matches shipped corpus OHLCV (programmatic)

Shipped `data/ohlcv_ETH-USDT-USDT_15m_warmup6m.csv` first bar:
```
timestamp = 1729371600000  →  2024-10-19 21:00 UTC
```

Use the scraper to fetch TV's first ~100 bars at the same timestamp, then diff vs the shipped CSV:

```python
# pseudocode — adapt from extract_sp500_15m_history.py
from tradingview_scraper.symbols.stream.streamer import Streamer
streamer = Streamer(...)
df = streamer.stream("BINANCE:ETHUSDT.P", "15m", n_bars=200)  # try BINANCE / BYBIT / OKX / MEXC / BITGET
# pandas-merge df['close'] vs shipped CSV close on shared timestamps; pick venue with smallest L∞ diff
```

If no exchange matches the shipped CSV to ±$0.05 across ≥95% of overlapping bars, the corpus OHLCV provenance needs re-sourcing — flag back, do not export trades against a mismatched feed.

### Step 2 — apply `strategy.pine` to the verified chart and export trades

1. Open the verified TV chart (the venue that passed step 1)
2. Pine Editor → paste this probe's `strategy.pine` → Add to chart
3. `Strategy Tester → Performance Summary → List of Trades → Export → CSV`
4. Save as `tv_trades.csv` next to the strategy

### Step 3 — run validator

```bash
EIGEN3_INCLUDE_DIR=/opt/homebrew/include/eigen3 \
python ~/code/pineforge-utils/validator/validate.py \
  ~/code/pineforge-engine/corpus/validation_ta_isolate/<probe-dir> \
  --engine-repo ~/code/pineforge-engine \
  --codegen-repo ~/code/pineforge-codegen \
  --workers 1 --out-md /tmp/p.md --out-json /tmp/p.json
```

### Step 4 (optional, deeper diagnosis) — per-bar TA diff

If a probe drifts and you want bar-by-bar engine-vs-TV TA values for root cause:

1. **For RSI(14) / MACD(12,26,9)**: tv_ta_basic_helper.pine already has these. Publish it (or already-published version), then:
   ```bash
   python ~/code/pineforge-utils/per-bar-trace/fetch_tv_indicator.py \
     BINANCE:ETHUSDT.P 15m PUB;<scriptIdPart> 1.0 5000 \
     --out /tmp/tv_ta_basic.csv
   ```
2. **For HMA(55) / SMA(152)**: extend `tv_ta_basic_helper.pine` (add `float t_hma55 = ta.hma(close, 55)` etc., re-publish), or fork it under `pine-helpers/tv_ta_isolate_helper.pine`.
3. Run engine with `// @pf-trace name=expr` directives capturing the same values, dump JSON via the engine's trace harness.
4. Diff: `python diff_engine_vs_tv.py --engine-trace out.json --tv-csv /tmp/tv_ta_basic.csv` — first column with `mismatches > 0` = the runtime function to fix.

## Interpretation matrix

After all 5 probes have `tv_trades.csv` + validator-scored:
- All 5 `excellent` → engine TA bit-exact across the four functions used by gap probes. The 37-regex / typed-matrix-01 / MarketShift gaps are caused by **OHLCV input drift** in those probes' captured TV trades. Re-export their `tv_trades.csv` against current TV charts to fix.
- Some `excellent`, others not → bug localized to the failing probe's TA function. Open engine task with that probe as minimal repro.
- All 5 drift → either feed-mismatch on the verified exchange (re-do step 1 with stricter L∞ check), or systemic engine math regression (unlikely given Task B's Wilder per-bar validation).

## Notes
- All probes default to `corpus/data/ohlcv_ETH-USDT-USDT_15m_warmup6m.csv` via the validator's auto-detection — no per-probe OHLCV override.
- `inputs.json` is a comment placeholder; future validator extensions can add `tv_ohlcv_csv` here.
- TV exports trades in the chart's display timezone. If your TV chart is not on UTC, set `"tv_trades_csv_tz": "America/New_York"` (etc.) in that probe's `inputs.json`.
- Probes use `strategy.entry` only — `plot()` calls were intentionally omitted because Strategy Tester's trade-list export does not include plots, only `strategy.entry/exit/close` rows.
