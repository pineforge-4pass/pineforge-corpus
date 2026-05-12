# validation_ta_isolate

Minimal-repro TA-only probes that isolate single Pine TA functions or compositional patterns end-to-end. Each probe runs one indicator with one trade rule, no risk management, no MTF, no session filters. Drift on any probe localizes the cause to that one TA function or one Pine semantic.

## Probes

### Tier 1 — pure TA function isolation (DONE — all 5 excellent)

| Probe | Isolates | Trigger | Status |
|---|---|---|---|
| `ta-isolate-01-rsi14-cross-50` | `ta.rsi(close, 14)` | `crossover` event | excellent (TV=4689, eng=4689) |
| `ta-isolate-02-rsi14-bands-30-70` | `ta.rsi(close, 14)` extrema | `crossover/under` events | excellent (TV=255, eng=255) |
| `ta-isolate-03-macd-12-26-9-cross` | `ta.macd(close,12,26,9)` EMA chain | `crossover` event | excellent (TV=3021, eng=3021) |
| `ta-isolate-04-hma-55-close-cross` | `ta.hma(close, 55)` nested WMA + sqrt | `crossover` event | excellent (TV=4838, eng=4838) |
| `ta-isolate-05-sma-152-close-cross` | `ta.sma(close, 152)` long-window | `crossover` event | excellent (TV=1616, eng=1616) |

**Conclusion**: engine RSI(14), MACD(12,26,9), HMA(55), SMA(152) bit-exact vs TV when fed shipped corpus OHLCV. The remaining gap-probe drift is NOT in TA math.

### Tier 2 — compositional pattern isolation (PENDING USER EXPORT)

Diagnoses why `validation/37-regex-string-filter` and `validation_typed_matrix/typed-matrix-probe-01-bool-regime-mask` still drift even though tier-1 proves their underlying TA math is exact.

| Probe | Isolates | Diagnoses |
|---|---|---|
| `ta-isolate-06-rsi14-gt-50-continuous-state` | value-cmp `rsi > 50` + `if cond entry else close` pattern | 37-regex pattern (vs probe 01 crossover) |
| `ta-isolate-07-macd-line-gt-signal-continuous-state` | value-cmp `macd > sig` + same pattern | 37-regex MACD path |
| `ta-isolate-08-rsi-and-macd-continuous-state` | full 37-regex AND-condition WITHOUT `str.match` | localizes if 37-regex bug is in str.match vs in AND-combination |
| `ta-isolate-09-rsi14-gt60-lt45-no-matrix` | typed-matrix-01 RSI > 55 / RSI < 45 trigger pair MINUS matrix bool ops | localizes if typed-matrix-01 bug is in matrix dispatch vs in entry/exit pattern |

### Diagnostic decision matrix (tier 2)

After all 4 tier-2 probes have `tv_trades.csv` + are validator-scored:

| 06 | 07 | 08 | 09 | Conclusion |
|---|---|---|---|---|
| ex | ex | ex | ex | 37-regex bug → engine `str.match()`. typed-matrix-01 bug → matrix bool dispatch (`matrix.new<bool>` / `matrix.set/get` / `matrix.transpose` / hotCount loop). |
| fail | * | * | * | Continuous-state value-gating diverges (engine bug in `>` operator boundary equality OR in same-bar entry-after-close ordering). Affects both gap probes. |
| ex | fail | * | * | MACD value-cmp `>` drifts despite crossover bit-exact — engine boundary-equality bug at exact `macd == sig`. |
| ex | ex | fail | * | AND-combination of two value-cmp conds differs. Engine evaluates `(a AND b)` differently from TV at bar where one side flips. |
| ex | ex | ex | fail | typed-matrix-01 bug → RSI > 55/45 trigger pair with explicit `position_size` guards (different from probes 01/02 which don't use those guards). |

## Existing tooling
- `~/code/tradingview-scraper` (`Streamer.stream`) — programmatic OHLCV verification
- `~/code/pineforge-utils/per-bar-trace/fetch_tv_indicator.py` — per-bar TV indicator values via WS scraper
- `~/code/pineforge-utils/per-bar-trace/pine-helpers/tv_ta_basic_helper.pine` — publishes RSI(14), MACD(12,26,9) for per-bar diff vs engine `// @pf-trace` JSON

## Workflow

### Step 1 — verify chart feed matches shipped corpus OHLCV (one-time, applies to all probes)

Shipped `data/ohlcv_ETH-USDT-USDT_15m_warmup6m.csv` is verified to be **BINANCE:ETHUSDT.P** 15m. Validator default tz `utc_plus_8` matches TV chart export tz. No per-probe verification needed if you use the same TV chart consistently.

### Step 2 — apply `strategy.pine` and export trades

1. Open BINANCE:ETHUSDT.P 15m chart on TradingView (UTC+8 tz)
2. Pine Editor → paste this probe's `strategy.pine` → Add to chart
3. `Strategy Tester → Performance Summary → List of Trades → Export → CSV`
4. Save as `tv_trades.csv` next to the strategy

### Step 3 — run validator

```bash
EIGEN3_INCLUDE_DIR=/opt/homebrew/include/eigen3 \
python ~/code/pineforge-utils/validator/validate.py \
  ~/code/pineforge-engine/corpus/validation_ta_isolate \
  --engine-repo ~/code/pineforge-engine \
  --codegen-repo ~/code/pineforge-codegen \
  --workers 9 --out-md /tmp/p.md --out-json /tmp/p.json
```

### Step 4 (optional) — per-bar TA diff for deep root-cause

If a tier-2 probe drifts unexpectedly, fetch TV's per-bar values via `tv_ta_basic_helper.pine` (RSI/MACD already published) + diff against engine `// @pf-trace` JSON. See `~/code/pineforge-utils/per-bar-trace/README.md`.

## Notes
- All probes use shipped `data/ohlcv_ETH-USDT-USDT_15m_warmup6m.csv` via validator auto-detection
- TV exports trades in chart timezone — UTC+8 by default; validator default matches
- Probes use only `strategy.entry/close` — no `plot()` (Strategy Tester export only emits trade rows, not plots)
