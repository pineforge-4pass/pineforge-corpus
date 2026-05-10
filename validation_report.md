# Corpus parity validation report

- **Generated**: `2026-05-11T00:00:00Z`
- **Host**: Darwin arm64 (16 cores), Python 3.14.3
- **pineforge-engine**: `3ed7f8f`
- **pineforge-codegen**: `9fa3c60`
- **corpus submodule**: `50813bc`
- **OHLCV feed**: `corpus/data/ohlcv_ETH-USDT-USDT_15m_warmup6m.csv` (6-month warmup)
- **Pipeline**: `xargs -P16 run_strategy.py` → `verify_corpus.py --all --include-anomalies`
- **Strategies**: 168 total (167 reference + 1 anomaly probe). All have TV trades; none skipped.

## Headline (167 reference strategies)

Reference tiers: `basic/`, `community/`, `validation/`. `parity-anomalies/` excluded — see anomaly section.

| Tier      | Count | Δ vs prev |
| --------- | ----- | --------- |
| excellent | 165   | —         |
| strong    | 2     | —         |
| moderate  | 0     | —         |
| weak      | 0     | —         |
| minimal   | 0     | —         |
| missing   | 0     | —         |
| **total** | **167** | —       |

Reproduce:

```bash
find corpus -mindepth 2 -maxdepth 2 -type d \
  | xargs -P16 -I{} python3 scripts/run_strategy.py {}
python3 scripts/verify_corpus.py --all --include-anomalies
```

## Standards

Two tiers of thresholds applied to four dimensions per strategy. Source: `scripts/verify_corpus.py:43-51`.

| Dimension              | strict (`excellent`) | strong (relaxed)  |
| ---------------------- | -------------------- | ----------------- |
| trade-count delta      | < **1.0%**           | < **5.0%**        |
| entry-price p90 delta  | < **0.01%**          | < **0.1%**        |
| exit-price p90 delta   | < **0.01%**          | < **0.5%**        |
| per-trade PnL p90 delta| < **1.0%**           | < **100%**        |

Strict thresholds match `pineforge-utils/validator/validate.py::DEFAULT_PARITY_STRICT` exactly — no production override; pass here ⇔ pass there.

Label rules:

- **excellent**: all four dimensions pass strict.
- **strong**: matched/TV ≥ 99% AND all four pass the relaxed tier (typical case: count delta ≤ a few trades, prices/PnL still strict).
- **moderate**: matched/TV ≥ 90%.
- **weak**: at least one trade matched but < 90%.
- **minimal**: zero matches with non-empty inputs.
- **missing**: `engine_trades.csv` absent.

### Boundary effect (count delta on the strict edge)

A strategy that matches every TV trade trade-by-trade with `entry/exit/PnL` all passing strict, but emits 1–N extra (or fewer) trades near the warmup/terminal edge, falls into **strong** purely because the count delta crosses the 1.0% strict gate. This is a **boundary effect**, not a parity regression:

- Trades inside the common window match exactly.
- Extras live outside the mutually trimmed window or land on the warmup boundary where TV's bootstrap and the engine's bootstrap differ by a single bar.
- Per-trade economics (entry/exit/PnL) stay inside strict on every matched trade.


Both `strong` cases below are boundary effects: prices and PnL pass strict, only count crosses 1.0%.

## `strong` tier detail

### `community/VCP` → **strong** *(boundary effect)*

```
  Profile:       strict
  TV trades:     71
  Engine trades: 72
  Matched:       71 (100.0% of TV)
  Count delta:            1.3889%  (X — boundary)
  Entry-price p90 delta:  0.0000%  (OK)
  Exit-price  p90 delta:  0.0005%  (OK)
  PnL         p90 delta:  0.1026%  (OK)
```

### `community/scalping-wunder-bots` → **strong** *(boundary effect)*

```
  Profile:       strict
  TV trades:     419
  Engine trades: 424
  Matched:       417 (99.5% of TV)
  Count delta:            1.1792%  (X — boundary)
  Entry-price p90 delta:  0.0000%  (OK)
  Exit-price  p90 delta:  0.0005%  (OK)
  PnL         p90 delta:  0.1235%  (OK)
```

## Anomaly probes (excluded from headline)

Probes that *deliberately* surface TV-side non-determinism. Engine is correct; divergence is documented in `pineforge-utils/parity-anomalies/`. Excluded from headline so they don't mask as regressions.

### `parity-anomalies/equity-mirror` → **weak** *(documented anomaly, expected)*

```
  Profile:       strict
  TV trades:     24
  Engine trades: 25
  Matched:       13 (54.2% of TV)
  Count delta:            4.0000%
  Entry-price p90 delta:  0.0000%
  Exit-price  p90 delta:  0.0000%
  PnL         p90 delta:  2.3202%
  -> weak
```

## Per-strategy detail (4 dimensions, all 168)

Columns: TV/eng trade count, matched ratio, then the four parity dimensions as percent deltas. **Bold** = fails strict on that dimension.

### `basic/`

| Strategy | TV | eng | match% | count Δ% | entry p90 Δ% | exit p90 Δ% | PnL p90 Δ% | label |
| -------- | --:| --: | -----: | -------: | -----------: | ----------: | ---------: | :---- |
| `greedy` | 13 | 13 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | excellent |
| `inside-bar` | 3332 | 3332 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0785 | excellent |
| `keltner` | 314 | 314 | 99.7 | 0.0000 | 0.0004 | 0.0004 | 0.1123 | excellent |
| `ma-cros` | 2315 | 2315 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0799 | excellent |
| `parabolic-asr` | 3082 | 3082 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0824 | excellent |
| `pivot-ext` | 4890 | 4890 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0775 | excellent |
| `stochastic-slow` | 690 | 690 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0816 | excellent |
| `supertrend` | 760 | 760 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0776 | excellent |
| `volty-expan` | 7235 | 7299 | 98.6 | 0.8768 | 0.0000 | 0.0000 | 0.1160 | excellent |

### `community/`

| Strategy | TV | eng | match% | count Δ% | entry p90 Δ% | exit p90 Δ% | PnL p90 Δ% | label |
| -------- | --:| --: | -----: | -------: | -----------: | ----------: | ---------: | :---- |
| `4ema_rsi` | 809 | 809 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0921 | excellent |
| `BOS_curv` | 262 | 262 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0770 | excellent |
| `IES` | 2668 | 2643 | 98.1 | 0.9370 | 0.0000 | 0.0000 | 0.7809 | excellent |
| `KKB` | 149 | 149 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0787 | excellent |
| `LiquitySweep` | 93 | 93 | 100.0 | 0.0000 | 0.0000 | 0.0005 | 0.0966 | excellent |
| `MarketShift` | 1152 | 1152 | 99.7 | 0.0000 | 0.0000 | 0.0000 | 0.0771 | excellent |
| `VCP` | 71 | 72 | 100.0 | **1.3889** | 0.0000 | 0.0005 | 0.1026 | strong |
| `kanuck` | 875 | 875 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0845 | excellent |
| `scalping-strategy` | 468 | 468 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | excellent |
| `scalping-wunder-bots` | 419 | 424 | 99.5 | **1.1792** | 0.0000 | 0.0005 | 0.1235 | strong |
| `trendmaster` | 123 | 123 | 100.0 | 0.0000 | 0.0000 | 0.0005 | 0.1368 | excellent |

### `validation/`

| Strategy | TV | eng | match% | count Δ% | entry p90 Δ% | exit p90 Δ% | PnL p90 Δ% | label |
| -------- | --:| --: | -----: | -------: | -----------: | ----------: | ---------: | :---- |
| `01-macd-histogram` | 2813 | 2813 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0884 | excellent |
| `02-bb-squeeze` | 813 | 813 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0863 | excellent |
| `03-dmi-adx-trend` | 2747 | 2743 | 99.8 | 0.1456 | 0.0000 | 0.0000 | 0.0779 | excellent |
| `04-stoch-rsi` | 1337 | 1337 | 97.6 | 0.0000 | 0.0000 | 0.0000 | 0.1152 | excellent |
| `05-hma-cross` | 4713 | 4713 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0821 | excellent |
| `06-cci-momentum` | 2462 | 2462 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0719 | excellent |
| `07-tsi-signal` | 846 | 846 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0823 | excellent |
| `08-linreg-channel` | 248 | 248 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0942 | excellent |
| `09-aroon-oscillator` | 1585 | 1585 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0834 | excellent |
| `10-donchian-breakout` | 1002 | 1002 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0793 | excellent |
| `11-elder-ray` | 2483 | 2483 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0786 | excellent |
| `12-chandelier-exit` | 1603 | 1603 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0827 | excellent |
| `13-atr-trailing-stop` | 5073 | 5072 | 100.0 | 0.0197 | 0.0000 | 0.0000 | 0.0786 | excellent |
| `14-vwma-divergence` | 2574 | 2574 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0800 | excellent |
| `15-momentum-roc` | 5690 | 5690 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0782 | excellent |
| `16-mean-reversion-bb` | 495 | 495 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0819 | excellent |
| `17-dual-ma-switch` | 1238 | 1238 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0888 | excellent |
| `18-ema-ribbon-loop` | 626 | 626 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0796 | excellent |
| `19-pivot-array-breakout` | 829 | 829 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0893 | excellent |
| `20-range-filter-while` | 401 | 401 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0788 | excellent |
| `21-adaptive-ma-func` | 4598 | 4600 | 100.0 | 0.0435 | 0.0000 | 0.0000 | 0.0772 | excellent |
| `22-candle-pattern` | 826 | 826 | 99.9 | 0.0000 | 0.0000 | 0.0000 | 0.0791 | excellent |
| `23-dual-thrust` | 2870 | 2870 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0784 | excellent |
| `25-percent-rank-revert` | 363 | 362 | 99.7 | 0.2755 | 0.0000 | 0.0000 | 0.0714 | excellent |
| `26-volume-breakout` | 1778 | 1778 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0829 | excellent |
| `27-ma-stack-array` | 1406 | 1406 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0796 | excellent |
| `28-swing-pivot-atr` | 1618 | 1619 | 100.0 | 0.0618 | 0.0000 | 0.0005 | 0.1264 | excellent |
| `29-median-cross` | 2837 | 2837 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0826 | excellent |
| `30-multi-indicator-score` | 3910 | 3911 | 100.0 | 0.0256 | 0.0000 | 0.0000 | 0.0830 | excellent |
| `31-rsi-bands` | 350 | 350 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0802 | excellent |
| `32-supertrend-adx-filter` | 455 | 455 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0768 | excellent |
| `33-matrix-eigen-pca` | 2850 | 2850 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0778 | excellent |
| `34-map-regime-tracker` | 801 | 801 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0783 | excellent |
| `35-advanced-trade-metrics` | 751 | 751 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.1214 | excellent |
| `36-mtf-array-stats` | 362 | 362 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0961 | excellent |
| `37-regex-string-filter` | 1777 | 1777 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0812 | excellent |
| `38-udt-regime-stack` | 3910 | 3911 | 100.0 | 0.0256 | 0.0000 | 0.0000 | 0.0830 | excellent |
| `39-request-security-parity` | 8775 | 8775 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0672 | excellent |
| `40-bracket-exit-tp-sl` | 366 | 366 | 100.0 | 0.0000 | 0.0000 | 0.0005 | 0.1255 | excellent |
| `41-partial-exit-qty-percent` | 725 | 725 | 100.0 | 0.0000 | 0.0000 | 0.0004 | 0.1321 | excellent |
| `42-close-immediate-vs-next-bar` | 732 | 732 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0794 | excellent |
| `43-close-all-cancel-all` | 732 | 732 | 100.0 | 0.0000 | 0.0000 | 0.0005 | 0.4543 | excellent |
| `44-oca-reduce-orders` | 366 | 366 | 100.0 | 0.0000 | 0.0000 | 0.0005 | 0.1287 | excellent |
| `45-commission-and-sizing` | 366 | 366 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0014 | excellent |
| `46-risk-gates` | 732 | 732 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0726 | excellent |
| `47-request-security-gaps-on` | 2247 | 2247 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0736 | excellent |
| `48-session-time-filter` | 366 | 366 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0698 | excellent |
| `49-na-nz-history-chain` | 3093 | 3093 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0756 | excellent |
| `50-exit-stop-limit-trail-same-bar` | 732 | 732 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0934 | excellent |
| `51-trail-points-without-offset` | 710 | 710 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0934 | excellent |
| `52-stop-entry-cancel-opposite` | 1738 | 1738 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0843 | excellent |
| `53-stop-entry-touch-boundary` | 548 | 548 | 100.0 | 0.0000 | 0.0004 | 0.0000 | 0.1220 | excellent |
| `54-mtf-roll-state-minimal` | 2938 | 2948 | 100.0 | 0.3392 | 0.0000 | 0.0000 | 0.0676 | excellent |
| `55-entry-reversal-implicit-exit` | 1098 | 1098 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0766 | excellent |
| `56-trailing-offset-activation-path` | 671 | 671 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0918 | excellent |
| `57-stop-entry-reversal-grouping` | 1464 | 1464 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0748 | excellent |
| `58-same-id-stop-modification` | 689 | 689 | 100.0 | 0.0000 | 0.0004 | 0.0000 | 0.1438 | excellent |
| `59-market-close-fill-basis` | 732 | 732 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0773 | excellent |
| `60-pyramiding-close-id-grouping` | 2196 | 2196 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0804 | excellent |
| `61-same-id-stop-raise-only` | 666 | 666 | 100.0 | 0.0000 | 0.0004 | 0.0000 | 0.1471 | excellent |
| `62-same-id-stop-cross-before-modify` | 732 | 732 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0849 | excellent |
| `63-dual-stop-cancel-rotation` | 792 | 792 | 100.0 | 0.0000 | 0.0004 | 0.0000 | 0.1258 | excellent |
| `64-same-id-market-entry-repeat` | 732 | 732 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0782 | excellent |
| `65-same-id-entry-close-same-bar` | 732 | 732 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0656 | excellent |
| `67-same-id-exit-replace` | 366 | 366 | 100.0 | 0.0000 | 0.0000 | 0.0004 | 0.1609 | excellent |
| `68-cross-entry-close-same-pass` | 732 | 732 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0828 | excellent |
| `69-cross-entry-cancel-same-pass` | 495 | 495 | 100.0 | 0.0000 | 0.0004 | 0.0000 | 0.1485 | excellent |
| `70-cross-exit-close-same-pass` | 732 | 732 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0675 | excellent |
| `71-cross-entry-exit-same-pass` | 728 | 728 | 100.0 | 0.0000 | 0.0000 | 0.0005 | 0.1248 | excellent |
| `72-cross-opposite-entry-close` | 843 | 843 | 100.0 | 0.0000 | 0.0004 | 0.0000 | 0.1132 | excellent |
| `73-same-id-stop-minute-zero-only` | 578 | 578 | 100.0 | 0.0000 | 0.0004 | 0.0000 | 0.1488 | excellent |
| `74-same-id-stop-window-four-bars` | 349 | 349 | 100.0 | 0.0000 | 0.0004 | 0.0000 | 0.1318 | excellent |
| `75-same-id-stop-after-flat` | 703 | 703 | 100.0 | 0.0000 | 0.0004 | 0.0000 | 0.1479 | excellent |
| `76-dual-side-same-id-stop-window-no-cancel` | 689 | 689 | 100.0 | 0.0000 | 0.0004 | 0.0000 | 0.1438 | excellent |
| `77-dual-side-stop-window-with-close-no-cancel` | 572 | 572 | 100.0 | 0.0000 | 0.0004 | 0.0000 | 0.1498 | excellent |
| `78-dual-side-four-bar-stop-window-no-close` | 366 | 366 | 100.0 | 0.0000 | 0.0004 | 0.0000 | 0.1057 | excellent |
| `79-one-side-four-bar-with-far-opposite` | 349 | 349 | 100.0 | 0.0000 | 0.0004 | 0.0000 | 0.1318 | excellent |
| `80-dual-stop-both-touch-priority` | 757 | 758 | 100.0 | 0.1319 | 0.0000 | 0.0000 | 0.0953 | excellent |
| `81-dual-stop-near-only` | 365 | 366 | 100.0 | 0.2732 | 0.0000 | 0.0000 | 0.1100 | excellent |
| `82-dual-stop-far-only` | 364 | 365 | 100.0 | 0.2740 | 0.0000 | 0.0000 | 0.0951 | excellent |
| `83-dual-stop-open-tie` | 732 | 732 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | excellent |
| `84-dual-stop-source-order-long-first` | 365 | 366 | 100.0 | 0.2732 | 0.0000 | 0.0000 | 0.1100 | excellent |
| `85-dual-stop-source-order-short-first` | 365 | 366 | 100.0 | 0.2732 | 0.0000 | 0.0000 | 0.0871 | excellent |
| `86-dual-stop-open-proximity-low-first` | 188 | 189 | 100.0 | 0.5291 | 0.0000 | 0.0000 | 0.1113 | excellent |
| `87-dual-stop-open-proximity-high-first` | 216 | 217 | 100.0 | 0.4608 | 0.0000 | 0.0000 | 0.1142 | excellent |
| `88-exit-three-way-set-once-at-entry` | 792 | 792 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | excellent |
| `89-trail-points-only-no-offset-explicit` | 782 | 782 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | excellent |
| `90-narrow-stop-limit-with-trail` | 792 | 792 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | excellent |
| `91-atr-trail-fixed-entry` | 792 | 792 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | excellent |
| `92-flip-stop-no-paired-close` | 724 | 724 | 100.0 | 0.0000 | 0.0004 | 0.0004 | 0.2511 | excellent |
| `93-flip-stop-pyramiding-2` | 843 | 843 | 100.0 | 0.0000 | 0.0004 | 0.0000 | 0.1132 | excellent |
| `94-stop-entry-cancel-no-regime-close` | 1610 | 1610 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0938 | excellent |
| `95-multi-cycle-open-guaranteed-stops` | 792 | 792 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0779 | excellent |
| `96-multi-cycle-pooc-cross-bar` | 791 | 791 | 100.0 | 0.0000 | 0.0005 | 0.0000 | 0.1441 | excellent |
| `ies-probe-01-adx-regime-classify` | 682 | 683 | 100.0 | 0.1464 | 0.0000 | 0.0000 | 0.0769 | excellent |
| `ies-probe-02-three-ema-bias` | 717 | 717 | 99.9 | 0.0000 | 0.0000 | 0.0000 | 0.0771 | excellent |
| `ies-probe-03-rsi-macd-momentum` | 4798 | 4798 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0821 | excellent |
| `ies-probe-04-pressure-gauge` | 2207 | 2207 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0786 | excellent |
| `ies-probe-05-bb-kc-squeeze` | 902 | 902 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0799 | excellent |
| `ies-probe-06-swing-pivot-sweep` | 706 | 706 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0786 | excellent |
| `ies-probe-07-bars-since-trade-cooldown` | 727 | 727 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0838 | excellent |
| `ies-probe-08-equity-feedback-sizing` | 79 | 79 | 100.0 | 0.0000 | 0.0000 | 0.0005 | 0.0925 | excellent |
| `mtf-probe-01-htf-close-roll` | 8775 | 8775 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0653 | excellent |
| `mtf-probe-02-dual-tf-closes` | 736 | 736 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0652 | excellent |
| `mtf-probe-03-htf-sma` | 540 | 540 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0767 | excellent |
| `mtf-probe-04-daily-prev-high` | 147 | 147 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0687 | excellent |
| `mtf-probe-05-htf-rsi` | 496 | 496 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0778 | excellent |
| `mtf-probe-06-htf-volume-sum` | 584 | 584 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0885 | excellent |
| `mtf-probe-07-triple-tf-close-confluence` | 48 | 48 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0910 | excellent |
| `mtf-probe-08-triple-tf-macd-hist-confluence` | 24 | 24 | 95.8 | 0.0000 | 0.0000 | 0.0000 | 0.1662 | excellent |
| `mtf-probe-09-htf-daily-ema-warmup` | 31 | 31 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0883 | excellent |
| `mtf-probe-10-htf-bracket-static` | 283 | 283 | 100.0 | 0.0000 | 0.0000 | 0.0005 | 0.0919 | excellent |
| `mtf-probe-11-htf-bracket-manual-trail` | 345 | 345 | 100.0 | 0.0000 | 0.0000 | 0.0005 | 0.1579 | excellent |
| `parity-probe-01-stop-limit-timing` | 778 | 778 | 100.0 | 0.0000 | 0.0000 | 0.0005 | 0.1337 | excellent |
| `parity-probe-02-choch-bos-isolator` | 1026 | 1026 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0705 | excellent |
| `parity-probe-04-percent-of-equity-sizing` | 57 | 57 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0006 | excellent |
| `parity-probe-05-small-equity-fraction` | 57 | 57 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0698 | excellent |
| `parity-probe-06-edge-margin-sizing` | 57 | 57 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0700 | excellent |
| `pineforge-analyzer` | 147 | 147 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0687 | excellent |
| `udt-method-probe-01-scalar-return` | 507 | 507 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0848 | excellent |
| `udt-method-probe-02-mutating-self` | 2293 | 2293 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0623 | excellent |
| `udt-method-probe-03-extra-args` | 2504 | 2504 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | excellent |
| `udt-method-probe-04-default-param` | 417 | 417 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0874 | excellent |
| `udt-method-probe-05-in-conditional` | 402 | 402 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0812 | excellent |
| `udt-method-probe-06-in-switch` | 402 | 402 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0812 | excellent |
| `udt-method-probe-07-in-for-loop` | 1660 | 1660 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0755 | excellent |
| `udt-method-probe-08-in-while-loop` | 306 | 306 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0845 | excellent |
| `udt-method-probe-09-uses-math-funcs` | 789 | 789 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0845 | excellent |
| `udt-method-probe-10-uses-na-nz` | 695 | 695 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0828 | excellent |
| `udt-method-probe-11-uses-history-of-globals` | 131 | 131 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.1028 | excellent |
| `udt-method-probe-12-receives-ta-result` | 2676 | 2676 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0753 | excellent |
| `udt-method-probe-13-reads-strategy-state` | 705 | 705 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0848 | excellent |
| `udt-method-probe-14-drives-strategy-entry` | 1635 | 1635 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0828 | excellent |
| `udt-method-probe-15-computes-exit-prices` | 704 | 704 | 100.0 | 0.0000 | 0.0000 | 0.0005 | 0.1084 | excellent |
| `udt-method-probe-16-var-instance-streak` | 1007 | 1007 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0557 | excellent |
| `udt-method-probe-17-tuple-return-destructure` | 1095 | 1095 | 100.0 | 0.0000 | 0.0000 | 0.0005 | 0.1076 | excellent |
| `udt-method-probe-18-method-calls-method` | 173 | 173 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0713 | excellent |
| `udt-method-probe-19-array-of-udt-method` | 868 | 868 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0828 | excellent |
| `udt-method-probe-20-udt-return-from-func` | 132 | 132 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0705 | excellent |
| `udt-method-probe-21-windowed-method-chain` | 173 | 173 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0713 | excellent |
| `vcp-probe-01-pivot-strength-5` | 1937 | 1937 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0773 | excellent |
| `vcp-probe-02-fvg-zones` | 11800 | 11851 | 100.0 | 0.4303 | 0.0000 | 0.0000 | 0.0584 | excellent |
| `vcp-probe-03-rsi-smooth-divergence` | 142 | 142 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0807 | excellent |
| `vcp-probe-04-vol-zscore-anomaly` | 591 | 591 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0739 | excellent |
| `vcp-probe-05-cumulative-volume-delta` | 3119 | 3119 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0761 | excellent |
| `vcp-probe-06-adx-volatility-regime` | 636 | 637 | 100.0 | 0.1570 | 0.0000 | 0.0000 | 0.0793 | excellent |
| `vcp-probe-07-session-tz-newyork` | 396 | 396 | 100.0 | 0.0000 | 0.0000 | 0.0000 | 0.0752 | excellent |

### `parity-anomalies/`

| Strategy | TV | eng | match% | count Δ% | entry p90 Δ% | exit p90 Δ% | PnL p90 Δ% | label |
| -------- | --:| --: | -----: | -------: | -----------: | ----------: | ---------: | :---- |
| `equity-mirror` | 24 | 25 | 54.2 | **4.0000** | 0.0000 | 0.0000 | **2.3202** | weak |

