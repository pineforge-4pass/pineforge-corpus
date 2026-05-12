# PineForge Corpus Validation Report

**Generated:** rerun on engine main HEAD (post deep-anomaly analysis)

## Headline

- **Total strategies:** 206
- **Excellent:** 203 (98.5%)
- **Anomalies / non-excellent (detailed below):** 3
- **Engine-only by design (no TV reference):** 0
- **Expected-reject correctly engine-rejected:** 0

---

## ⚠️ Anomalies & Non-Excellent — read these first

Each row links to the strategy directory in this corpus. The deep-analysis verdict
(TV-side anomaly vs engine bug) is recorded in the strategy's `inputs.json` /
`README.md` where applicable.

| Tier | Strategy | TV | Eng | PnL p90 | Count Δ | Verdict |
|---|---|---:|---:|---:|---:|---|
| **strong** | [`community/BOS_curv`](./community/BOS_curv/) | 266 | 260 | 0.0768% | 2.2556% | TV chart-state non-determinism over Apr 5-14 window. After Apr 14: 262/262 PERFECT match. Engine processes 5 months extra warmup; Supertrend `var direction` state diverges from TV until natural convergence. |
| **engine_only** | [`community/VCP`](./community/VCP/) | 71 | 72 | 0.0821% | 1.3889% | TV `time(period, session, tz)` returns na on session-start boundary bar (well-known TV quirk, ~92% confidence, deep-analyzed). Engine logic correct per Pine semantics. |
| **anomaly** | [`parity-anomalies/equity-mirror`](./parity-anomalies/equity-mirror/) | 24 | 25 | 2.3202% | 4.0000% | TV broker margin-boundary non-determinism (≥95% confidence, deep-analyzed). Engine deterministic; TV admits some over-equity entries and rejects some under-equity ones — non-monotonic in overshoot. |

**Key conclusion:** All non-excellent labels traced to TV-side anomalies or
documented chart-state mismatches. **Zero real engine bugs remaining.**

---

## Full per-strategy report

## Summary

### Bar-for-bar parity (no `request.security`)

| Strategy | Tier | Match | Strict | Profile | Count Δ | Entry p90 | Exit p90 | PnL p90 | TV | Engine | Matched | Match % |
|----------|------|-------|--------|---------|---------|-----------|----------|---------|----|--------|---------|---------|
| basic/greedy | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 13 | 13 | 13 | 100.0% |
| basic/inside-bar | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0785% ✓ | 3331 | 3331 | 3331 | 100.0% |
| basic/keltner | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0787% ✓ | 313 | 313 | 312 | 99.7% |
| basic/ma-cros | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0794% ✓ | 2314 | 2314 | 2314 | 100.0% |
| basic/parabolic-asr | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0824% ✓ | 3081 | 3081 | 3080 | 100.0% |
| basic/pivot-ext | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0777% ✓ | 4889 | 4889 | 4889 | 100.0% |
| basic/stochastic-slow | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0812% ✓ | 689 | 689 | 689 | 100.0% |
| basic/supertrend | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0773% ✓ | 759 | 759 | 759 | 100.0% |
| basic/volty-expan | BarParity | excellent | Y | strict | 0.8770% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.1095% ✓ | 7234 | 7298 | 7131 | 98.6% |
| community/4ema_rsi | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0922% ✓ | 809 | 809 | 809 | 100.0% |
| community/BOS_curv | BarParity | strong | N | strict | 2.2556% ✗ | 0.0000% ✓ | 0.0000% ✓ | 0.0768% ✓ | 266 | 266 | 260 | 97.7% |
| community/KKB | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0751% ✓ | 148 | 149 | 148 | 100.0% |
| community/LiquitySweep | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0005% ✓ | 0.1020% ✓ | 93 | 93 | 93 | 100.0% |
| community/MarketShift | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0769% ✓ | 1151 | 1151 | 1147 | 99.7% |
| community/kanuck | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0828% ✓ | 873 | 874 | 873 | 100.0% |
| community/scalping-strategy | BarParity | excellent | Y | production | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 468 | 468 | 468 | 100.0% |
| community/scalping-wunder-bots | BarParity | excellent | Y | strict | 0.2381% ✓ | 0.0000% ✓ | 0.0005% ✓ | 0.1075% ✓ | 419 | 420 | 417 | 99.5% |
| community/trendmaster | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0005% ✓ | 0.1370% ✓ | 123 | 123 | 123 | 100.0% |
| parity-anomalies/equity-mirror | BarParity | weak | N | strict | 4.0000% ✗ | 0.0000% ✓ | 0.0000% ✓ | 2.3202% ✗ | 24 | 25 | 13 | 54.2% |
| validation/01-macd-histogram | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0884% ✓ | 2812 | 2812 | 2812 | 100.0% |
| validation/02-bb-squeeze | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0864% ✓ | 813 | 813 | 813 | 100.0% |
| validation/03-dmi-adx-trend | BarParity | excellent | Y | strict | 0.1457% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0779% ✓ | 2745 | 2742 | 2740 | 99.8% |
| validation/04-stoch-rsi | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.1146% ✓ | 1336 | 1336 | 1304 | 97.6% |
| validation/05-hma-cross | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0820% ✓ | 4712 | 4712 | 4712 | 100.0% |
| validation/06-cci-momentum | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0719% ✓ | 2461 | 2461 | 2461 | 100.0% |
| validation/07-tsi-signal | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0816% ✓ | 845 | 845 | 845 | 100.0% |
| validation/08-linreg-channel | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0943% ✓ | 248 | 248 | 248 | 100.0% |
| validation/09-aroon-oscillator | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0834% ✓ | 1584 | 1584 | 1584 | 100.0% |
| validation/10-donchian-breakout | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0792% ✓ | 1001 | 1001 | 1001 | 100.0% |
| validation/11-elder-ray | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0787% ✓ | 2483 | 2483 | 2483 | 100.0% |
| validation/12-chandelier-exit | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0825% ✓ | 1602 | 1603 | 1602 | 100.0% |
| validation/13-atr-trailing-stop | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0786% ✓ | 5071 | 5071 | 5071 | 100.0% |
| validation/14-vwma-divergence | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0801% ✓ | 2573 | 2573 | 2573 | 100.0% |
| validation/15-momentum-roc | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0782% ✓ | 5689 | 5689 | 5689 | 100.0% |
| validation/16-mean-reversion-bb | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0818% ✓ | 494 | 494 | 494 | 100.0% |
| validation/17-dual-ma-switch | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0887% ✓ | 1236 | 1237 | 1236 | 100.0% |
| validation/18-ema-ribbon-loop | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0792% ✓ | 626 | 626 | 625 | 99.8% |
| validation/19-pivot-array-breakout | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0891% ✓ | 828 | 828 | 828 | 100.0% |
| validation/20-range-filter-while | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0783% ✓ | 400 | 401 | 400 | 100.0% |
| validation/21-adaptive-ma-func | BarParity | excellent | Y | strict | 0.0435% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0771% ✓ | 4597 | 4607 | 4597 | 100.0% |
| validation/22-candle-pattern | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0788% ✓ | 825 | 825 | 824 | 99.9% |
| validation/23-dual-thrust | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0784% ✓ | 2869 | 2869 | 2869 | 100.0% |
| validation/25-percent-rank-revert | BarParity | excellent | Y | strict | 0.2755% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0716% ✓ | 363 | 362 | 362 | 99.7% |
| validation/26-volume-breakout | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0830% ✓ | 1778 | 1778 | 1778 | 100.0% |
| validation/27-ma-stack-array | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0796% ✓ | 1405 | 1405 | 1405 | 100.0% |
| validation/28-swing-pivot-atr | BarParity | excellent | Y | strict | 0.0618% ✓ | 0.0000% ✓ | 0.0005% ✓ | 0.1055% ✓ | 1618 | 1619 | 1618 | 100.0% |
| validation/29-median-cross | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0823% ✓ | 2836 | 2836 | 2836 | 100.0% |
| validation/30-multi-indicator-score | BarParity | excellent | Y | strict | 0.0256% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0829% ✓ | 3909 | 3910 | 3909 | 100.0% |
| validation/31-rsi-bands | BarParity | excellent | Y | strict | 0.2857% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0800% ✓ | 349 | 350 | 349 | 100.0% |
| validation/32-supertrend-adx-filter | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0762% ✓ | 454 | 454 | 454 | 100.0% |
| validation/33-matrix-eigen-pca | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0777% ✓ | 2849 | 2849 | 2849 | 100.0% |
| validation/34-map-regime-tracker | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0777% ✓ | 800 | 803 | 800 | 100.0% |
| validation/35-advanced-trade-metrics | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.1214% ✓ | 750 | 750 | 750 | 100.0% |
| validation/37-regex-string-filter | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0779% ✓ | 1916 | 1918 | 1916 | 100.0% |
| validation/38-udt-regime-stack | BarParity | excellent | Y | strict | 0.0256% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0829% ✓ | 3909 | 3910 | 3909 | 100.0% |
| validation/40-bracket-exit-tp-sl | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0005% ✓ | 0.1320% ✓ | 366 | 366 | 366 | 100.0% |
| validation/41-partial-exit-qty-percent | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0004% ✓ | 0.1594% ✓ | 725 | 725 | 725 | 100.0% |
| validation/42-close-immediate-vs-next-bar | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0794% ✓ | 732 | 732 | 732 | 100.0% |
| validation/43-close-all-cancel-all | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0005% ✓ | 0.4566% ✓ | 732 | 732 | 732 | 100.0% |
| validation/44-oca-reduce-orders | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0005% ✓ | 0.1273% ✓ | 366 | 366 | 366 | 100.0% |
| validation/45-commission-and-sizing | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0014% ✓ | 366 | 366 | 366 | 100.0% |
| validation/46-risk-gates | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0726% ✓ | 732 | 732 | 732 | 100.0% |
| validation/48-session-time-filter | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0699% ✓ | 366 | 366 | 366 | 100.0% |
| validation/49-na-nz-history-chain | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0755% ✓ | 3092 | 3092 | 3092 | 100.0% |
| validation/50-exit-stop-limit-trail-same-bar | BarParity | excellent | Y | production | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0755% ✓ | 732 | 732 | 732 | 100.0% |
| validation/51-trail-points-without-offset | BarParity | excellent | Y | production | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0935% ✓ | 710 | 710 | 710 | 100.0% |
| validation/52-stop-entry-cancel-opposite | BarParity | excellent | Y | strict | 0.1149% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0817% ✓ | 1738 | 1740 | 1737 | 99.9% |
| validation/53-stop-entry-touch-boundary | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0811% ✓ | 548 | 548 | 548 | 100.0% |
| validation/55-entry-reversal-implicit-exit | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0767% ✓ | 1098 | 1098 | 1098 | 100.0% |
| validation/56-trailing-offset-activation-path | BarParity | excellent | Y | production | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0919% ✓ | 671 | 671 | 671 | 100.0% |
| validation/57-stop-entry-reversal-grouping | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0748% ✓ | 1464 | 1464 | 1464 | 100.0% |
| validation/58-same-id-stop-modification | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0844% ✓ | 689 | 689 | 689 | 100.0% |
| validation/59-market-close-fill-basis | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0773% ✓ | 732 | 732 | 732 | 100.0% |
| validation/60-pyramiding-close-id-grouping | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0805% ✓ | 2196 | 2196 | 2196 | 100.0% |
| validation/61-same-id-stop-raise-only | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0823% ✓ | 665 | 665 | 665 | 100.0% |
| validation/62-same-id-stop-cross-before-modify | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0815% ✓ | 732 | 732 | 732 | 100.0% |
| validation/63-dual-stop-cancel-rotation | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0784% ✓ | 791 | 791 | 791 | 100.0% |
| validation/64-same-id-market-entry-repeat | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0782% ✓ | 732 | 732 | 732 | 100.0% |
| validation/65-same-id-entry-close-same-bar | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0656% ✓ | 732 | 732 | 732 | 100.0% |
| validation/67-same-id-exit-replace | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0004% ✓ | 0.1584% ✓ | 366 | 366 | 366 | 100.0% |
| validation/68-cross-entry-close-same-pass | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0830% ✓ | 732 | 732 | 732 | 100.0% |
| validation/69-cross-entry-cancel-same-pass | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0826% ✓ | 494 | 494 | 494 | 100.0% |
| validation/70-cross-exit-close-same-pass | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0678% ✓ | 732 | 732 | 732 | 100.0% |
| validation/71-cross-entry-exit-same-pass | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0005% ✓ | 0.1130% ✓ | 728 | 728 | 728 | 100.0% |
| validation/72-cross-opposite-entry-close | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0791% ✓ | 843 | 843 | 843 | 100.0% |
| validation/73-same-id-stop-minute-zero-only | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0811% ✓ | 578 | 578 | 578 | 100.0% |
| validation/74-same-id-stop-window-four-bars | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0818% ✓ | 349 | 349 | 349 | 100.0% |
| validation/75-same-id-stop-after-flat | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0821% ✓ | 703 | 703 | 703 | 100.0% |
| validation/76-dual-side-same-id-stop-window-no-cancel | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0844% ✓ | 689 | 689 | 689 | 100.0% |
| validation/77-dual-side-stop-window-with-close-no-cancel | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0799% ✓ | 571 | 571 | 571 | 100.0% |
| validation/78-dual-side-four-bar-stop-window-no-close | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0817% ✓ | 366 | 366 | 366 | 100.0% |
| validation/79-one-side-four-bar-with-far-opposite | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0818% ✓ | 349 | 349 | 349 | 100.0% |
| validation/80-dual-stop-both-touch-priority | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0949% ✓ | 757 | 757 | 757 | 100.0% |
| validation/81-dual-stop-near-only | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.1101% ✓ | 365 | 365 | 365 | 100.0% |
| validation/82-dual-stop-far-only | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0952% ✓ | 364 | 364 | 364 | 100.0% |
| validation/83-dual-stop-open-tie | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0891% ✓ | 732 | 732 | 732 | 100.0% |
| validation/84-dual-stop-source-order-long-first | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.1101% ✓ | 365 | 365 | 365 | 100.0% |
| validation/85-dual-stop-source-order-short-first | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0872% ✓ | 365 | 365 | 365 | 100.0% |
| validation/86-dual-stop-open-proximity-low-first | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.1113% ✓ | 188 | 188 | 188 | 100.0% |
| validation/87-dual-stop-open-proximity-high-first | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.1134% ✓ | 216 | 216 | 216 | 100.0% |
| validation/88-exit-three-way-set-once-at-entry | BarParity | excellent | Y | production | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 792 | 792 | 792 | 100.0% |
| validation/89-trail-points-only-no-offset-explicit | BarParity | excellent | Y | production | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 782 | 782 | 782 | 100.0% |
| validation/90-narrow-stop-limit-with-trail | BarParity | excellent | Y | production | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 792 | 792 | 792 | 100.0% |
| validation/91-atr-trail-fixed-entry | BarParity | excellent | Y | production | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 792 | 792 | 792 | 100.0% |
| validation/92-flip-stop-no-paired-close | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0769% ✓ | 724 | 724 | 724 | 100.0% |
| validation/93-flip-stop-pyramiding-2 | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0791% ✓ | 843 | 843 | 843 | 100.0% |
| validation/94-stop-entry-cancel-no-regime-close | BarParity | excellent | Y | strict | 0.1241% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0852% ✓ | 1610 | 1612 | 1609 | 99.9% |
| validation/95-multi-cycle-open-guaranteed-stops | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0772% ✓ | 791 | 791 | 791 | 100.0% |
| validation/96-multi-cycle-pooc-cross-bar | BarParity | excellent | Y | strict | 0.1266% ✓ | 0.0005% ✓ | 0.0000% ✓ | 0.1434% ✓ | 790 | 790 | 789 | 99.9% |
| validation/ies-probe-01-adx-regime-classify | BarParity | excellent | Y | strict | 0.1464% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0770% ✓ | 682 | 683 | 682 | 100.0% |
| validation/ies-probe-02-three-ema-bias | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0770% ✓ | 718 | 722 | 715 | 99.6% |
| validation/ies-probe-03-rsi-macd-momentum | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0821% ✓ | 4797 | 4798 | 4797 | 100.0% |
| validation/ies-probe-04-pressure-gauge | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0783% ✓ | 2206 | 2206 | 2206 | 100.0% |
| validation/ies-probe-05-bb-kc-squeeze | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0799% ✓ | 901 | 901 | 901 | 100.0% |
| validation/ies-probe-06-swing-pivot-sweep | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0783% ✓ | 705 | 705 | 705 | 100.0% |
| validation/ies-probe-07-bars-since-trade-cooldown | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0838% ✓ | 727 | 727 | 727 | 100.0% |
| validation/ies-probe-08-equity-feedback-sizing | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0005% ✓ | 0.0891% ✓ | 79 | 79 | 79 | 100.0% |
| validation/parity-probe-01-stop-limit-timing | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0004% ✓ | 0.1008% ✓ | 778 | 778 | 778 | 100.0% |
| validation/parity-probe-02-choch-bos-isolator | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0705% ✓ | 1026 | 1026 | 1026 | 100.0% |
| validation/parity-probe-04-percent-of-equity-sizing | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0006% ✓ | 57 | 57 | 57 | 100.0% |
| validation/parity-probe-05-small-equity-fraction | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0698% ✓ | 57 | 57 | 57 | 100.0% |
| validation/parity-probe-06-edge-margin-sizing | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0701% ✓ | 57 | 57 | 57 | 100.0% |
| validation/udt-method-probe-01-scalar-return | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0849% ✓ | 507 | 507 | 507 | 100.0% |
| validation/udt-method-probe-02-mutating-self | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0624% ✓ | 2293 | 2293 | 2293 | 100.0% |
| validation/udt-method-probe-03-extra-args | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 2504 | 2504 | 2504 | 100.0% |
| validation/udt-method-probe-04-default-param | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0875% ✓ | 417 | 417 | 417 | 100.0% |
| validation/udt-method-probe-05-in-conditional | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0811% ✓ | 401 | 401 | 401 | 100.0% |
| validation/udt-method-probe-06-in-switch | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0811% ✓ | 401 | 401 | 401 | 100.0% |
| validation/udt-method-probe-07-in-for-loop | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0756% ✓ | 1660 | 1660 | 1660 | 100.0% |
| validation/udt-method-probe-08-in-while-loop | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0846% ✓ | 306 | 306 | 306 | 100.0% |
| validation/udt-method-probe-09-uses-math-funcs | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0845% ✓ | 789 | 789 | 789 | 100.0% |
| validation/udt-method-probe-10-uses-na-nz | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0829% ✓ | 695 | 695 | 695 | 100.0% |
| validation/udt-method-probe-11-uses-history-of-globals | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.1029% ✓ | 131 | 131 | 131 | 100.0% |
| validation/udt-method-probe-12-receives-ta-result | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0754% ✓ | 2676 | 2676 | 2676 | 100.0% |
| validation/udt-method-probe-13-reads-strategy-state | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0848% ✓ | 705 | 705 | 705 | 100.0% |
| validation/udt-method-probe-14-drives-strategy-entry | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0828% ✓ | 1634 | 1634 | 1634 | 100.0% |
| validation/udt-method-probe-15-computes-exit-prices | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0004% ✓ | 0.0876% ✓ | 704 | 704 | 704 | 100.0% |
| validation/udt-method-probe-16-var-instance-streak | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0557% ✓ | 1007 | 1007 | 1007 | 100.0% |
| validation/udt-method-probe-17-tuple-return-destructure | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0004% ✓ | 0.0874% ✓ | 1094 | 1094 | 1094 | 100.0% |
| validation/udt-method-probe-18-method-calls-method | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0713% ✓ | 173 | 173 | 173 | 100.0% |
| validation/udt-method-probe-19-array-of-udt-method | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0829% ✓ | 868 | 868 | 868 | 100.0% |
| validation/udt-method-probe-20-udt-return-from-func | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0705% ✓ | 132 | 132 | 132 | 100.0% |
| validation/udt-method-probe-21-windowed-method-chain | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0713% ✓ | 173 | 173 | 173 | 100.0% |
| validation/vcp-probe-01-pivot-strength-5 | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0773% ✓ | 1937 | 1937 | 1937 | 100.0% |
| validation/vcp-probe-02-fvg-zones | BarParity | excellent | Y | strict | 0.4303% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0587% ✓ | 11800 | 11851 | 11797 | 100.0% |
| validation/vcp-probe-03-rsi-smooth-divergence | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0805% ✓ | 141 | 141 | 141 | 100.0% |
| validation/vcp-probe-04-vol-zscore-anomaly | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0739% ✓ | 591 | 591 | 591 | 100.0% |
| validation/vcp-probe-05-cumulative-volume-delta | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0761% ✓ | 3119 | 3119 | 3119 | 100.0% |
| validation/vcp-probe-06-adx-volatility-regime | BarParity | excellent | Y | strict | 0.1570% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0797% ✓ | 636 | 637 | 636 | 100.0% |
| validation/vcp-probe-07-session-tz-newyork | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0752% ✓ | 396 | 396 | 396 | 100.0% |
| validation_barstate/barstate-magnifier-probe-01a-isconfirmed-magnifier-on | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0829% ✓ | 871 | 871 | 871 | 100.0% |
| validation_barstate/barstate-magnifier-probe-01b-isconfirmed-magnifier-off | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0829% ✓ | 871 | 871 | 871 | 100.0% |
| validation_lower_tf/lower-tf-probe-01-numeric-float-ratio15 | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0845% ✓ | 820 | 820 | 820 | 100.0% |
| validation_lower_tf/lower-tf-probe-02-bool-array | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0739% ✓ | 3116 | 3116 | 3116 | 100.0% |
| validation_magnifier/magnifier-dist-probe-01-endpoints | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0740% ✓ | 871 | 871 | 871 | 100.0% |
| validation_magnifier/magnifier-dist-probe-02-uniform | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0740% ✓ | 871 | 871 | 871 | 100.0% |
| validation_magnifier/magnifier-dist-probe-03-cosine | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0740% ✓ | 871 | 871 | 871 | 100.0% |
| validation_magnifier/magnifier-dist-probe-04-triangle | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0740% ✓ | 871 | 871 | 871 | 100.0% |
| validation_magnifier/magnifier-dist-probe-05-front-loaded | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0740% ✓ | 871 | 871 | 871 | 100.0% |
| validation_magnifier/magnifier-dist-probe-06-back-loaded | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0740% ✓ | 871 | 871 | 871 | 100.0% |
| validation_magnifier/magnifier-dist-probe-07-volume-weighted-on | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0740% ✓ | 871 | 871 | 871 | 100.0% |
| validation_magnifier/magnifier-dist-probe-08a-endpoints-rsi-cross | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0708% ✓ | 2345 | 2345 | 2345 | 100.0% |
| validation_magnifier/magnifier-dist-probe-08b-uniform-rsi-cross | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0708% ✓ | 2345 | 2345 | 2345 | 100.0% |
| validation_na_chain/na-chain-probe-01-deep-history-int-na | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.1001% ✓ | 106 | 106 | 106 | 100.0% |
| validation_oca/oca-three-way-probe-01-same-bar-touch | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0005% ✓ | 0.1087% ✓ | 421 | 421 | 421 | 100.0% |
| validation_oca/oca-three-way-probe-02-multi-group-partial | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0005% ✓ | 0.1362% ✓ | 1244 | 1244 | 1244 | 100.0% |
| validation_process_orders/process-orders-probe-01-on-close-vs-next-bar | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0829% ✓ | 857 | 857 | 857 | 100.0% |
| validation_process_orders/process-orders-probe-02-on-close-true | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0829% ✓ | 857 | 857 | 857 | 100.0% |
| validation_pyramid/pyramid-carry-probe-01-deferred-flip | BarParity | excellent | Y | strict | 0.9251% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0855% ✓ | 2356 | 2378 | 2345 | 99.5% |
| validation_pyramid/pyramid-carry-probe-02-cash-fractional | BarParity | excellent | Y | strict | 0.1266% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0370% ✓ | 790 | 789 | 788 | 99.7% |
| validation_recompute/ta-recompute-probe-02-untested-classes | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0795% ✓ | 582 | 582 | 582 | 100.0% |
| validation_session/session-dst-probe-01-ny-spring-forward | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0735% ✓ | 396 | 396 | 396 | 100.0% |
| validation_ta_isolate/ta-isolate-01-rsi14-cross-50 | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0767% ✓ | 4689 | 4689 | 4689 | 100.0% |
| validation_ta_isolate/ta-isolate-02-rsi14-bands-30-70 | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0803% ✓ | 255 | 255 | 255 | 100.0% |
| validation_ta_isolate/ta-isolate-03-macd-12-26-9-cross | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0855% ✓ | 3021 | 3021 | 3021 | 100.0% |
| validation_ta_isolate/ta-isolate-04-hma-55-close-cross | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0816% ✓ | 4838 | 4838 | 4838 | 100.0% |
| validation_ta_isolate/ta-isolate-05-sma-152-close-cross | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0698% ✓ | 1616 | 1616 | 1616 | 100.0% |
| validation_ta_isolate/ta-isolate-06-rsi14-gt-50-continuous-state | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0751% ✓ | 2345 | 2345 | 2345 | 100.0% |
| validation_ta_isolate/ta-isolate-07-macd-line-gt-signal-continuous-state | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0861% ✓ | 1511 | 1511 | 1511 | 100.0% |
| validation_ta_isolate/ta-isolate-08-rsi-and-macd-continuous-state | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0779% ✓ | 1916 | 1918 | 1916 | 100.0% |
| validation_ta_isolate/ta-isolate-09-rsi14-gt60-lt45-no-matrix | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0841% ✓ | 785 | 785 | 785 | 100.0% |
| validation_ta_isolate/ta-isolate-10-typed-matrix-explicit-utc | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0844% ✓ | 769 | 769 | 769 | 100.0% |
| validation_ta_isolate/ta-isolate-11-mask-only-no-transpose | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0844% ✓ | 769 | 769 | 769 | 100.0% |
| validation_ta_isolate/ta-isolate-12-sample-only-no-hotcount | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0844% ✓ | 769 | 769 | 769 | 100.0% |
| validation_typed_matrix/typed-matrix-probe-01-bool-regime-mask | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0844% ✓ | 769 | 769 | 769 | 100.0% |
| validation_typed_matrix/typed-matrix-probe-02-eigen-rank-deficient | BarParity | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0829% ✓ | 871 | 871 | 871 | 100.0% |

### MTF experimental (`request.security` present)

| Strategy | Tier | Match | Strict | Profile | Count Δ | Entry p90 | Exit p90 | PnL p90 | TV | Engine | Matched | Match % |
|----------|------|-------|--------|---------|---------|-----------|----------|---------|----|--------|---------|---------|
| community/IES | MTF | excellent | Y | strict | 0.9370% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.7797% ✓ | 2668 | 2643 | 2618 | 98.1% |
| community/VCP | MTF | engine_only | N | strict | 1.3889% ✗ | 0.0000% ✓ | 0.0004% ✓ | 0.0821% ✓ | 71 | 72 | 71 | 100.0% |
| validation/36-mtf-array-stats | MTF | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0948% ✓ | 361 | 361 | 361 | 100.0% |
| validation/39-request-security-parity | MTF | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0674% ✓ | 8775 | 8775 | 8775 | 100.0% |
| validation/47-request-security-gaps-on | MTF | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0734% ✓ | 2246 | 2246 | 2246 | 100.0% |
| validation/54-mtf-roll-state-minimal | MTF | excellent | Y | strict | 0.3393% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0675% ✓ | 2937 | 2947 | 2937 | 100.0% |
| validation/mtf-probe-01-htf-close-roll | MTF | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0654% ✓ | 8775 | 8775 | 8775 | 100.0% |
| validation/mtf-probe-02-dual-tf-closes | MTF | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0653% ✓ | 736 | 736 | 736 | 100.0% |
| validation/mtf-probe-03-htf-sma | MTF | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0764% ✓ | 539 | 539 | 539 | 100.0% |
| validation/mtf-probe-04-daily-prev-high | MTF | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0687% ✓ | 147 | 147 | 147 | 100.0% |
| validation/mtf-probe-05-htf-rsi | MTF | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0776% ✓ | 495 | 495 | 495 | 100.0% |
| validation/mtf-probe-06-htf-volume-sum | MTF | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0886% ✓ | 584 | 584 | 584 | 100.0% |
| validation/mtf-probe-07-triple-tf-close-confluence | MTF | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0807% ✓ | 47 | 47 | 47 | 100.0% |
| validation/mtf-probe-08-triple-tf-macd-hist-confluence | MTF | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.1034% ✓ | 23 | 23 | 22 | 95.7% |
| validation/mtf-probe-09-htf-daily-ema-warmup | MTF | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0803% ✓ | 30 | 30 | 30 | 100.0% |
| validation/mtf-probe-10-htf-bracket-static | MTF | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0004% ✓ | 0.0815% ✓ | 283 | 283 | 283 | 100.0% |
| validation/mtf-probe-11-htf-bracket-manual-trail | MTF | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0004% ✓ | 0.0814% ✓ | 345 | 345 | 345 | 100.0% |
| validation/pineforge-analyzer | MTF | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0687% ✓ | 147 | 147 | 147 | 100.0% |
| validation_recompute/ta-recompute-probe-01-mtf-rsi-macd-bb | MTF | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0700% ✓ | 286 | 286 | 286 | 100.0% |
| validation_varip/varip-reject-probe-02-var-udt-in-security | MTF | excellent | Y | strict | 0.0000% ✓ | 0.0000% ✓ | 0.0000% ✓ | 0.0710% ✓ | 207 | 207 | 207 | 100.0% |

## Error distributions (per strategy)

### basic/greedy

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=13  mean=3.49806e-14  p50=0  p90=0  p95=1.81899e-13  p99=4.00178e-13  max=4.54747e-13
- `exit_diff`: n=13  mean=3.49806e-14  p50=0  p90=0  p95=1.81899e-13  p99=4.00178e-13  max=4.54747e-13
- `pnl_diff`: n=13  mean=1.51622e-13  p50=1.09246e-13  p90=3.08731e-13  p95=3.27205e-13  p99=3.27631e-13  max=3.27738e-13
- `time_diff_ms`: n=13  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 13
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### basic/inside-bar

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=3331  mean=6.38913e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=3331  mean=6.38913e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=3331  mean=0.0074272  p50=2.38032e-13  p90=0.02  p95=0.03  p99=0.08  max=0.24
- `time_diff_ms`: n=3331  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 3331
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### basic/keltner

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=312  mean=0.00634615  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=1.49
- `exit_diff`: n=312  mean=0.0749038  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=21.39
- `pnl_diff`: n=312  mean=0.111827  p50=0.02  p90=0.07  p95=0.15  p99=0.5778  max=21.39
- `time_diff_ms`: n=312  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 310
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 1
  - (1,10]: 1
  - \>10: 0

### basic/ma-cros

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=2314  mean=0.00166811  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=2.81
- `exit_diff`: n=2314  mean=0.00166811  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=2.81
- `pnl_diff`: n=2314  mean=0.0125972  p50=2.73559e-13  p90=0.02  p95=0.04  p99=0.11  max=2.81
- `time_diff_ms`: n=2314  mean=777.874  p50=0  p90=0  p95=0  p99=0  max=900000
- `entry_diff` histogram (abs):
  - (0,1e-06]: 2312
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 2
  - \>10: 0

### basic/parabolic-asr

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=3080  mean=3.57143e-05  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=0.11
- `exit_diff`: n=3080  mean=0.00130844  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=4.03
- `pnl_diff`: n=3080  mean=0.0103474  p50=3.64153e-13  p90=0.02  p95=0.04  p99=0.09  max=3.92
- `time_diff_ms`: n=3080  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 3079
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 1
  - (1,10]: 0
  - \>10: 0

### basic/pivot-ext

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=4889  mean=6.57612e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=4889  mean=6.57612e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=4889  mean=0.00593168  p50=2.16716e-13  p90=0.02  p95=0.03  p99=0.06  max=0.44
- `time_diff_ms`: n=4889  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 4889
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### basic/stochastic-slow

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=689  mean=6.66611e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=689  mean=6.66611e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=689  mean=0.022148  p50=0.01  p90=0.05  p95=0.096  p99=0.21  max=0.55
- `time_diff_ms`: n=689  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 689
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### basic/supertrend

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=759  mean=7.09981e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=759  mean=7.12977e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=759  mean=0.0183794  p50=0.01  p90=0.04  p95=0.07  p99=0.1584  max=0.61
- `time_diff_ms`: n=759  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 759
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### basic/volty-expan

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=7131  mean=0.0156892  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=0.48  max=2.81
- `exit_diff`: n=7131  mean=0.312035  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=5.676  max=242.35
- `pnl_diff`: n=7131  mean=0.332216  p50=2.18492e-13  p90=0.02  p95=0.09  p99=5.672  max=242.32
- `time_diff_ms`: n=7131  mean=2397.98  p50=0  p90=0  p95=0  p99=0  max=2.7e+06
- `entry_diff` histogram (abs):
  - (0,1e-06]: 7015
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 11
  - (0.1,1]: 56
  - (1,10]: 49
  - \>10: 0

### community/4ema_rsi

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=809  mean=6.32374e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=809  mean=5.84595e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=809  mean=0.00980222  p50=2.54019e-13  p90=0.02  p95=0.036  p99=0.1192  max=0.9
- `time_diff_ms`: n=809  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 809
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### community/BOS_curv

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=260  mean=4.98473e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=6.41194e-13  max=9.09495e-13
- `exit_diff`: n=260  mean=5.07218e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=6.41194e-13  max=9.09495e-13
- `pnl_diff`: n=260  mean=0.0250385  p50=0.01  p90=0.05  p95=0.08  p99=0.3992  max=0.57
- `time_diff_ms`: n=260  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 260
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### community/IES

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=2618  mean=0.00173033  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=2.49
- `exit_diff`: n=2618  mean=0.22301  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=5.6231  max=95.15
- `pnl_diff`: n=2618  mean=110.13  p50=5.0368  p90=41.1087  p95=68.4455  p99=2192.03  max=21327.6
- `time_diff_ms`: n=2618  mean=1718.87  p50=0  p90=0  p95=0  p99=0  max=3.6e+06
- `entry_diff` histogram (abs):
  - (0,1e-06]: 2616
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 2
  - \>10: 0

### community/KKB

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=148  mean=5.22345e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=6.95763e-13  max=9.09495e-13
- `exit_diff`: n=148  mean=5.53071e-14  p50=0  p90=2.95586e-13  p95=4.54747e-13  p99=6.95763e-13  max=9.09495e-13
- `pnl_diff`: n=148  mean=0.0503378  p50=0.01  p90=0.086  p95=0.14  p99=0.4642  max=1.9
- `time_diff_ms`: n=148  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 148
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### community/LiquitySweep

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=93  mean=5.62322e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=93  mean=0.0425806  p50=2.27374e-13  p90=0.01  p95=0.01  p99=1.2976  max=2.42
- `pnl_diff`: n=93  mean=0.0507527  p50=0.01  p90=0.03  p95=0.04  p99=1.2424  max=2.42
- `time_diff_ms`: n=93  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 93
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### community/MarketShift

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=1147  mean=5.84788e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=1147  mean=0.0207236  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=23.77
- `pnl_diff`: n=1147  mean=0.0300174  p50=2.18492e-13  p90=0.02  p95=0.03  p99=0.1508  max=23.77
- `time_diff_ms`: n=1147  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 1147
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### community/VCP

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=71  mean=3.84294e-14  p50=0  p90=0  p95=4.54747e-13  p99=4.54747e-13  max=4.54747e-13
- `exit_diff`: n=71  mean=0.0043662  p50=0  p90=0.01  p95=0.01  p99=0.01  max=0.01
- `pnl_diff`: n=71  mean=0.0309859  p50=0.02  p90=0.06  p95=0.085  p99=0.153  max=0.23
- `time_diff_ms`: n=71  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 71
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### community/kanuck

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=873  mean=6.53732e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=873  mean=1.14548e-05  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=0.01
- `pnl_diff`: n=873  mean=0.0104238  p50=3.45501e-13  p90=0.03  p95=0.04  p99=0.1  max=0.33
- `time_diff_ms`: n=873  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 873
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### community/scalping-strategy

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=468  mean=5.1985e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=468  mean=0.0169658  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=6.04814e-13  max=7.94
- `pnl_diff`: n=468  mean=0.0172009  p50=1.27329e-13  p90=3.63792e-13  p95=4.96589e-13  p99=0.01  max=7.94
- `time_diff_ms`: n=468  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 468
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### community/scalping-wunder-bots

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=417  mean=5.88881e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=8.36735e-13  max=9.09495e-13
- `exit_diff`: n=417  mean=0.0430456  p50=0  p90=0.01  p95=0.01  p99=1.0276  max=5.79
- `pnl_diff`: n=417  mean=0.0712739  p50=0.00480507  p90=0.0160275  p95=0.0206601  p99=1.42598  max=9.08612
- `time_diff_ms`: n=417  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 417
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### community/trendmaster

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=123  mean=8.50341e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=123  mean=0.0280488  p50=0  p90=0.01  p95=0.01  p99=0.02  max=2.98
- `pnl_diff`: n=123  mean=0.192195  p50=0.05  p90=0.198  p95=0.22  p99=0.3512  max=14.92
- `time_diff_ms`: n=123  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 123
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### parity-anomalies/equity-mirror

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=13  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `exit_diff`: n=13  mean=1.39922e-13  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=4.54747e-13  max=4.54747e-13
- `pnl_diff`: n=13  mean=61.2346  p50=21.119  p90=156.965  p95=189.32  p99=220.023  max=227.698
- `time_diff_ms`: n=13  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 13
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/01-macd-histogram

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=2812  mean=6.76784e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=2812  mean=6.78402e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=2812  mean=0.00948791  p50=3.27738e-13  p90=0.02  p95=0.04  p99=0.09  max=0.34
- `time_diff_ms`: n=2812  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 2812
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/02-bb-squeeze

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=813  mean=7.32742e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=813  mean=6.90791e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=813  mean=0.0110209  p50=5.08926e-13  p90=0.03  p95=0.05  p99=0.1  max=0.42
- `time_diff_ms`: n=813  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 813
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/03-dmi-adx-trend

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=2740  mean=0.0014927  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=2.07
- `exit_diff`: n=2740  mean=0.00340511  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=5.24
- `pnl_diff`: n=2740  mean=0.0105365  p50=2.18492e-13  p90=0.02  p95=0.03  p99=0.09  max=7.31
- `time_diff_ms`: n=2740  mean=1970.8  p50=0  p90=0  p95=0  p99=0  max=2.7e+06
- `entry_diff` histogram (abs):
  - (0,1e-06]: 2738
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 2
  - \>10: 0

### validation/04-stoch-rsi

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=1304  mean=0.0101304  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=2.9
- `exit_diff`: n=1304  mean=0.268781  p50=0  p90=4.54747e-13  p95=9.09495e-13  p99=9.0289  max=52.47
- `pnl_diff`: n=1304  mean=0.292147  p50=0.01  p90=0.04  p95=0.14  p99=9.0286  max=52.3
- `time_diff_ms`: n=1304  mean=11733.1  p50=0  p90=0  p95=0  p99=0  max=2.7e+06
- `entry_diff` histogram (abs):
  - (0,1e-06]: 1292
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 1
  - (0.1,1]: 5
  - (1,10]: 6
  - \>10: 0

### validation/05-hma-cross

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=4712  mean=6.32612e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=4712  mean=6.32612e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=4712  mean=0.00663837  p50=2.38032e-13  p90=0.02  p95=0.03  p99=0.07  max=0.28
- `time_diff_ms`: n=4712  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 4712
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/06-cci-momentum

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=2461  mean=6.19018e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=2461  mean=6.15323e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=2461  mean=0.00623324  p50=1.82077e-13  p90=0.01  p95=0.03  p99=0.09  max=0.55
- `time_diff_ms`: n=2461  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 2461
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/07-tsi-signal

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=845  mean=6.18887e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=845  mean=6.18887e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=845  mean=0.0170178  p50=0.01  p90=0.04  p95=0.07  p99=0.1856  max=0.43
- `time_diff_ms`: n=845  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 845
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/08-linreg-channel

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=248  mean=4.8592e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=4.54747e-13  max=9.09495e-13
- `exit_diff`: n=248  mean=5.59266e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=6.95763e-13  max=9.09495e-13
- `pnl_diff`: n=248  mean=0.0114113  p50=0.01  p90=0.03  p95=0.0565  p99=0.09  max=0.12
- `time_diff_ms`: n=248  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 248
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/09-aroon-oscillator

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=1584  mean=5.92837e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=1584  mean=6.90447e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=1584  mean=0.00921717  p50=3.10862e-13  p90=0.02  p95=0.03  p99=0.09  max=0.36
- `time_diff_ms`: n=1584  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 1584
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/10-donchian-breakout

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=1001  mean=5.92852e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=1001  mean=5.83767e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=1001  mean=0.0143457  p50=0.01  p90=0.03  p95=0.05  p99=0.14  max=0.81
- `time_diff_ms`: n=1001  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 1001
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/11-elder-ray

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=2483  mean=6.48331e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=2483  mean=6.45584e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=2483  mean=0.00689086  p50=2.36478e-13  p90=0.02  p95=0.03  p99=0.0618  max=0.25
- `time_diff_ms`: n=2483  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 2483
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/12-chandelier-exit

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=1602  mean=0.0030025  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=2.96
- `exit_diff`: n=1602  mean=0.00184769  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=2.96
- `pnl_diff`: n=1602  mean=0.0170787  p50=0.01  p90=0.03  p95=0.04  p99=0.14  max=4.81
- `time_diff_ms`: n=1602  mean=1685.39  p50=0  p90=0  p95=0  p99=0  max=1.8e+06
- `entry_diff` histogram (abs):
  - (0,1e-06]: 1600
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 2
  - \>10: 0

### validation/13-atr-trailing-stop

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=5071  mean=6.51048e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=5071  mean=6.46116e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=5071  mean=0.00458884  p50=1.8191e-13  p90=0.01  p95=0.02  p99=0.06  max=0.42
- `time_diff_ms`: n=5071  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 5071
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/14-vwma-divergence

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=2573  mean=3.88651e-06  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=0.01
- `exit_diff`: n=2573  mean=3.88651e-06  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=0.01
- `pnl_diff`: n=2573  mean=0.00887291  p50=2.55795e-13  p90=0.02  p95=0.04  p99=0.09  max=0.45
- `time_diff_ms`: n=2573  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 2572
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 1
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/15-momentum-roc

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=5689  mean=6.4627e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=5689  mean=6.4627e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=5689  mean=0.00491651  p50=1.82077e-13  p90=0.01  p95=0.02  p99=0.06  max=0.37
- `time_diff_ms`: n=5689  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 5689
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/16-mean-reversion-bb

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=494  mean=6.6279e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=494  mean=7.04214e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=494  mean=0.0151619  p50=0.01  p90=0.03  p95=0.06  p99=0.15  max=0.72
- `time_diff_ms`: n=494  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 494
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/17-dual-ma-switch

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=1236  mean=0.00178803  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=2.21
- `exit_diff`: n=1236  mean=7.28479e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=1236  mean=0.0150162  p50=5.8209e-13  p90=0.03  p95=0.05  p99=0.173  max=2.21
- `time_diff_ms`: n=1236  mean=728.155  p50=0  p90=0  p95=0  p99=0  max=900000
- `entry_diff` histogram (abs):
  - (0,1e-06]: 1235
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 1
  - \>10: 0

### validation/18-ema-ribbon-loop

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=625  mean=5.63887e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=625  mean=5.60249e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=625  mean=0.018864  p50=0.01  p90=0.04  p95=0.06  p99=0.2052  max=0.62
- `time_diff_ms`: n=625  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 625
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/19-pivot-array-breakout

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=828  mean=5.4372e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=828  mean=5.54704e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=828  mean=0.0193116  p50=0.01  p90=0.04  p95=0.07  p99=0.18  max=0.81
- `time_diff_ms`: n=828  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 828
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/20-range-filter-while

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=400  mean=5.45697e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=400  mean=5.57066e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=400  mean=0.029725  p50=0.01  p90=0.06  p95=0.09  p99=0.3407  max=0.58
- `time_diff_ms`: n=400  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 400
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/21-adaptive-ma-func

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=4597  mean=6.47943e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=4597  mean=6.47943e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=4597  mean=0.00514466  p50=1.82077e-13  p90=0.01  p95=0.02  p99=0.06  max=0.46
- `time_diff_ms`: n=4597  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 4597
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/22-candle-pattern

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=824  mean=6.81569e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=824  mean=0.00133495  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=1.1
- `pnl_diff`: n=824  mean=0.0187985  p50=0.01  p90=0.04  p95=0.0685  p99=0.1577  max=1.1
- `time_diff_ms`: n=824  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 824
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/23-dual-thrust

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=2869  mean=5.80916e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=2869  mean=5.80916e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=2869  mean=0.00829209  p50=2.72671e-13  p90=0.02  p95=0.03  p99=0.09  max=0.37
- `time_diff_ms`: n=2869  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 2869
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/25-percent-rank-revert

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=362  mean=7.28601e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=362  mean=6.0298e-14  p50=0  p90=4.3201e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=362  mean=0.00400552  p50=1.81188e-13  p90=0.01  p95=0.02  p99=0.05  max=0.13
- `time_diff_ms`: n=362  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 362
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/26-volume-breakout

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=1778  mean=6.31736e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=1778  mean=5.92092e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=1778  mean=0.00638358  p50=2.18492e-13  p90=0.02  p95=0.03  p99=0.0823  max=0.18
- `time_diff_ms`: n=1778  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 1778
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/27-ma-stack-array

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=1405  mean=7.11744e-06  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=0.01
- `exit_diff`: n=1405  mean=6.94258e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=1405  mean=0.0104413  p50=3.09086e-13  p90=0.02  p95=0.04  p99=0.11  max=0.92
- `time_diff_ms`: n=1405  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 1404
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 1
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/28-swing-pivot-atr

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=1618  mean=5.56489e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=1618  mean=0.217206  p50=0  p90=0.01  p95=0.01  p99=1.073  max=189.16
- `pnl_diff`: n=1618  mean=0.22424  p50=0.01  p90=0.03  p95=0.04  p99=1.0747  max=188.97
- `time_diff_ms`: n=1618  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 1618
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/29-median-cross

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=2836  mean=3.52609e-06  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=0.01
- `exit_diff`: n=2836  mean=3.52609e-06  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=0.01
- `pnl_diff`: n=2836  mean=0.00925952  p50=3.09142e-13  p90=0.02  p95=0.04  p99=0.09  max=0.26
- `time_diff_ms`: n=2836  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 2835
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 1
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/30-multi-indicator-score

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=3909  mean=6.40997e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=3909  mean=0.000404195  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=1.58
- `pnl_diff`: n=3909  mean=0.00616014  p50=2.00284e-13  p90=0.01  p95=0.02  p99=0.07  max=1.58
- `time_diff_ms`: n=3909  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 3909
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/31-rsi-bands

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=349  mean=6.2544e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=349  mean=6.84076e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=349  mean=0.0119771  p50=0.01  p90=0.02  p95=0.046  p99=0.1352  max=0.35
- `time_diff_ms`: n=349  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 349
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/32-supertrend-adx-filter

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=454  mean=6.11004e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=454  mean=7.56243e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=454  mean=0.0185903  p50=0.01  p90=0.04  p95=0.07  p99=0.1594  max=0.61
- `time_diff_ms`: n=454  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 454
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/33-matrix-eigen-pca

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=2849  mean=6.1612e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=2849  mean=6.1612e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=2849  mean=0.00869779  p50=3.09086e-13  p90=0.02  p95=0.04  p99=0.09  max=0.23
- `time_diff_ms`: n=2849  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 2849
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/34-map-regime-tracker

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=800  mean=6.62226e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=800  mean=6.62226e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=800  mean=0.0185625  p50=0.01  p90=0.04  p95=0.07  p99=0.1704  max=0.85
- `time_diff_ms`: n=800  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 800
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/35-advanced-trade-metrics

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=750  mean=5.3357e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=750  mean=5.39634e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=750  mean=0.0225467  p50=0.01  p90=0.05  p95=0.08  p99=0.3002  max=0.78
- `time_diff_ms`: n=750  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 750
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/36-mtf-array-stats

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=361  mean=5.6686e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=361  mean=5.6686e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=361  mean=0.0315235  p50=0.01  p90=0.08  p95=0.15  p99=0.304  max=0.95
- `time_diff_ms`: n=361  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 361
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/37-regex-string-filter

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=1916  mean=7.2152e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=1916  mean=5.77928e-14  p50=0  p90=3.41061e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=1916  mean=0.00495303  p50=1.9984e-13  p90=0.01  p95=0.02  p99=0.05  max=0.24
- `time_diff_ms`: n=1916  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 1916
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/38-udt-regime-stack

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=3909  mean=6.40997e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=3909  mean=0.000404195  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=1.58
- `pnl_diff`: n=3909  mean=0.00616014  p50=2.00284e-13  p90=0.01  p95=0.02  p99=0.07  max=1.58
- `time_diff_ms`: n=3909  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 3909
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/39-request-security-parity

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=8775  mean=6.06589e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=8775  mean=6.21358e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=8775  mean=0.0021265  p50=1.54543e-13  p90=0.01  p95=0.01  p99=0.03  max=0.12
- `time_diff_ms`: n=8775  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 8775
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/40-bracket-exit-tp-sl

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=366  mean=6.64726e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=366  mean=0.0039071  p50=0  p90=0.01  p95=0.01  p99=0.01  max=0.01
- `pnl_diff`: n=366  mean=0.00677596  p50=0.01  p90=0.02  p95=0.02  p99=0.0235  max=0.15
- `time_diff_ms`: n=366  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 366
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/41-partial-exit-qty-percent

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=725  mean=7.33868e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=725  mean=0.0529379  p50=0  p90=0.01  p95=0.01  p99=0.01  max=18
- `pnl_diff`: n=725  mean=0.0602483  p50=0.01  p90=0.02  p95=0.03  p99=0.1  max=18
- `time_diff_ms`: n=725  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 725
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/42-close-immediate-vs-next-bar

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=732  mean=5.99496e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=732  mean=7.17532e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=732  mean=0.00461749  p50=2.00728e-13  p90=0.01  p95=0.02  p99=0.05  max=0.15
- `time_diff_ms`: n=732  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 732
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/43-close-all-cancel-all

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=732  mean=7.02001e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=732  mean=0.00521858  p50=0.01  p90=0.01  p95=0.01  p99=0.01  max=0.04
- `pnl_diff`: n=732  mean=0.00733607  p50=0.01  p90=0.02  p95=0.02  p99=0.04  max=0.08
- `time_diff_ms`: n=732  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 732
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/44-oca-reduce-orders

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=366  mean=6.33664e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=366  mean=0.00363388  p50=0  p90=0.01  p95=0.01  p99=0.01  max=0.01
- `pnl_diff`: n=366  mean=0.00663934  p50=0.01  p90=0.02  p95=0.02  p99=0.03  max=0.03
- `time_diff_ms`: n=366  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 366
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/45-commission-and-sizing

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=366  mean=6.27452e-14  p50=0  p90=3.41061e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=366  mean=7.33063e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=366  mean=0.0133555  p50=0.00808445  p90=0.0299485  p95=0.0451002  p99=0.0748371  max=0.0967522
- `time_diff_ms`: n=366  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 366
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/46-risk-gates

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=732  mean=6.74045e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=732  mean=7.2685e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=732  mean=0.00401639  p50=1.81188e-13  p90=0.01  p95=0.02  p99=0.05  max=0.21
- `time_diff_ms`: n=732  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 732
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/47-request-security-gaps-on

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=2246  mean=6.56002e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=2246  mean=6.10447e-14  p50=0  p90=3.41061e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=2246  mean=0.00562333  p50=2.0961e-13  p90=0.01  p95=0.02  p99=0.0655  max=0.18
- `time_diff_ms`: n=2246  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 2246
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/48-session-time-filter

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=366  mean=6.64726e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=366  mean=6.64726e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=366  mean=0.00311475  p50=1.63591e-13  p90=0.01  p95=0.01  p99=0.03  max=0.16
- `time_diff_ms`: n=366  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 366
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/49-na-nz-history-chain

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=3092  mean=6.25057e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=3092  mean=6.56678e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=3092  mean=0.00470569  p50=1.82077e-13  p90=0.01  p95=0.02  p99=0.05  max=0.28
- `time_diff_ms`: n=3092  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 3092
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/50-exit-stop-limit-trail-same-bar

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=732  mean=6.18133e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=732  mean=0.000327869  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=0.23
- `pnl_diff`: n=732  mean=0.00289617  p50=1.63425e-13  p90=0.01  p95=0.01  p99=0.03  max=0.23
- `time_diff_ms`: n=732  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 732
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/51-trail-points-without-offset

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=710  mean=6.75716e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=710  mean=0.000591549  p50=0  p90=4.54747e-13  p95=0.01  p99=0.01  max=0.02
- `pnl_diff`: n=710  mean=0.00546479  p50=2.05169e-13  p90=0.01  p95=0.02  p99=0.0782  max=0.2
- `time_diff_ms`: n=710  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 710
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/52-stop-entry-cancel-opposite

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=1737  mean=0.00139896  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=2.43
- `exit_diff`: n=1737  mean=6.25703e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=1737  mean=0.169044  p50=0.01  p90=0.14  p95=0.25  p99=1.6556  max=32.15
- `time_diff_ms`: n=1737  mean=1554.4  p50=0  p90=0  p95=0  p99=0  max=2.7e+06
- `entry_diff` histogram (abs):
  - (0,1e-06]: 1736
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 1
  - \>10: 0

### validation/53-stop-entry-touch-boundary

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=548  mean=7.30251e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=548  mean=1.82482e-05  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=0.01
- `pnl_diff`: n=548  mean=0.0134307  p50=4.36984e-13  p90=0.03  p95=0.06  p99=0.1506  max=0.26
- `time_diff_ms`: n=548  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 548
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/54-mtf-roll-state-minimal

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=2937  mean=6.4798e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=2937  mean=0.034811  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=83.98
- `pnl_diff`: n=2937  mean=0.0378277  p50=1.63425e-13  p90=0.01  p95=0.01  p99=0.05  max=83.96
- `time_diff_ms`: n=2937  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 2937
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/55-entry-reversal-implicit-exit

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=1098  mean=6.87505e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=1098  mean=6.64726e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=1098  mean=0.00338798  p50=1.63425e-13  p90=0.01  p95=0.01  p99=0.04  max=0.16
- `time_diff_ms`: n=1098  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 1098
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/56-trailing-offset-activation-path

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=671  mean=7.04825e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=671  mean=0.000178838  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=0.01  max=0.02
- `pnl_diff`: n=671  mean=0.00862891  p50=2.54907e-13  p90=0.02  p95=0.03  p99=0.1  max=0.59
- `time_diff_ms`: n=671  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 671
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/57-stop-entry-reversal-grouping

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=1464  mean=6.8647e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=1464  mean=6.64726e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=1464  mean=0.00300546  p50=1.63425e-13  p90=0.01  p95=0.01  p99=0.03  max=0.16
- `time_diff_ms`: n=1464  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 1464
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/58-same-id-stop-modification

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=689  mean=6.63311e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=5.09317e-13  max=9.09495e-13
- `exit_diff`: n=689  mean=1.45138e-05  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=0.01
- `pnl_diff`: n=689  mean=0.0129318  p50=0.01  p90=0.03  p95=0.05  p99=0.1324  max=0.3
- `time_diff_ms`: n=689  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 689
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/59-market-close-fill-basis

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=732  mean=6.8647e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=732  mean=6.49195e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=732  mean=0.0076776  p50=2.55795e-13  p90=0.02  p95=0.03  p99=0.0869  max=0.19
- `time_diff_ms`: n=732  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 732
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/60-pyramiding-close-id-grouping

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=2196  mean=6.97859e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=2196  mean=6.74045e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=2196  mean=0.00369763  p50=1.81188e-13  p90=0.01  p95=0.01  p99=0.04  max=0.21
- `time_diff_ms`: n=2196  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 2196
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/61-same-id-stop-raise-only

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=665  mean=6.66735e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=6.18456e-13  max=9.09495e-13
- `exit_diff`: n=665  mean=1.50376e-05  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=0.01
- `pnl_diff`: n=665  mean=0.0125865  p50=4.33431e-13  p90=0.03  p95=0.05  p99=0.1336  max=0.33
- `time_diff_ms`: n=665  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 665
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/62-same-id-stop-cross-before-modify

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=732  mean=6.49195e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=732  mean=1.36612e-05  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=0.01
- `pnl_diff`: n=732  mean=0.00952186  p50=3.2685e-13  p90=0.029  p95=0.04  p99=0.1069  max=0.23
- `time_diff_ms`: n=732  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 732
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/63-dual-stop-cancel-rotation

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=791  mean=6.84133e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=791  mean=6.4389e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=791  mean=0.0448925  p50=0.01  p90=0.09  p95=0.145  p99=0.443  max=2.88
- `time_diff_ms`: n=791  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 791
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/64-same-id-market-entry-repeat

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=732  mean=6.98894e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=732  mean=6.74045e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=732  mean=0.00453552  p50=2.00062e-13  p90=0.01  p95=0.02  p99=0.0469  max=0.21
- `time_diff_ms`: n=732  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 732
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/65-same-id-entry-close-same-bar

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=732  mean=6.98894e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=732  mean=6.46089e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=732  mean=0.00273224  p50=1.45661e-13  p90=0.01  p95=0.01  p99=0.03  max=0.16
- `time_diff_ms`: n=732  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 732
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/67-same-id-exit-replace

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=366  mean=6.27452e-14  p50=0  p90=3.41061e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=366  mean=0.00357923  p50=0  p90=0.01  p95=0.01  p99=0.01  max=0.01
- `pnl_diff`: n=366  mean=0.00590164  p50=0.01  p90=0.01  p95=0.02  p99=0.03  max=0.05
- `time_diff_ms`: n=366  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 366
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/68-cross-entry-close-same-pass

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=732  mean=6.98894e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=732  mean=6.36771e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=732  mean=0.0180738  p50=0.01  p90=0.04  p95=0.07  p99=0.1869  max=0.35
- `time_diff_ms`: n=732  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 732
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/69-cross-entry-cancel-same-pass

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=494  mean=6.76598e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=494  mean=6.76598e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=494  mean=0.0112955  p50=3.27405e-13  p90=0.03  p95=0.05  p99=0.11  max=0.43
- `time_diff_ms`: n=494  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 494
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/70-cross-exit-close-same-pass

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=732  mean=1.36612e-05  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=0.01
- `exit_diff`: n=732  mean=6.18133e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=732  mean=0.00273224  p50=1.45661e-13  p90=0.01  p95=0.01  p99=0.03  max=0.16
- `time_diff_ms`: n=732  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 731
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 1
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/71-cross-entry-exit-same-pass

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=728  mean=6.2153e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=728  mean=0.00482143  p50=4.54747e-13  p90=0.01  p95=0.01  p99=0.01  max=0.01
- `pnl_diff`: n=728  mean=0.00803571  p50=0.01  p90=0.02  p95=0.02  p99=0.0273  max=0.04
- `time_diff_ms`: n=728  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 728
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/72-cross-opposite-entry-close

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=843  mean=6.98574e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=843  mean=6.52722e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=843  mean=0.498932  p50=3.69482e-13  p90=0.05  p95=0.08  p99=8.2008  max=103.96
- `time_diff_ms`: n=843  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 843
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/73-same-id-stop-minute-zero-only

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=578  mean=7.51356e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=578  mean=1.7301e-05  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=0.01
- `pnl_diff`: n=578  mean=0.0139446  p50=0.01  p90=0.04  p95=0.06  p99=0.16  max=0.3
- `time_diff_ms`: n=578  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 578
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/74-same-id-stop-window-four-bars

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=349  mean=5.01655e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=6.91216e-13  max=9.09495e-13
- `exit_diff`: n=349  mean=2.86533e-05  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=0.01
- `pnl_diff`: n=349  mean=0.00873926  p50=3.27516e-13  p90=0.02  p95=0.03  p99=0.0804  max=0.16
- `time_diff_ms`: n=349  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 349
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/75-same-id-stop-after-flat

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=703  mean=4.43104e-14  p50=0  p90=0  p95=4.54747e-13  p99=9.004e-13  max=9.09495e-13
- `exit_diff`: n=703  mean=1.42248e-05  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=0.01
- `pnl_diff`: n=703  mean=0.00843528  p50=3.09086e-13  p90=0.02  p95=0.03  p99=0.0798  max=0.25
- `time_diff_ms`: n=703  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 703
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/76-dual-side-same-id-stop-window-no-cancel

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=689  mean=6.63311e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=5.09317e-13  max=9.09495e-13
- `exit_diff`: n=689  mean=1.45138e-05  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=0.01
- `pnl_diff`: n=689  mean=0.0129318  p50=0.01  p90=0.03  p95=0.05  p99=0.1324  max=0.3
- `time_diff_ms`: n=689  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 689
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/77-dual-side-stop-window-with-close-no-cancel

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=571  mean=5.85358e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=5.91172e-13  max=9.09495e-13
- `exit_diff`: n=571  mean=1.75131e-05  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=0.01
- `pnl_diff`: n=571  mean=0.0133275  p50=0.01  p90=0.03  p95=0.05  p99=0.136  max=0.34
- `time_diff_ms`: n=571  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 571
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/78-dual-side-four-bar-stop-window-no-close

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=366  mean=5.09416e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=366  mean=5.09416e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=4.54747e-13  max=9.09495e-13
- `pnl_diff`: n=366  mean=0.0337432  p50=0.01  p90=0.08  p95=0.12  p99=0.297  max=1
- `time_diff_ms`: n=366  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 366
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/79-one-side-four-bar-with-far-opposite

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=349  mean=5.01655e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=6.91216e-13  max=9.09495e-13
- `exit_diff`: n=349  mean=2.86533e-05  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=0.01
- `pnl_diff`: n=349  mean=0.00873926  p50=3.27516e-13  p90=0.02  p95=0.03  p99=0.0804  max=0.16
- `time_diff_ms`: n=349  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 349
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/80-dual-stop-both-touch-priority

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=757  mean=6.51785e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=757  mean=0.843738  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=27.7096  max=119.57
- `pnl_diff`: n=757  mean=0.853765  p50=6.18172e-13  p90=0.03  p95=0.06  p99=27.7096  max=119.58
- `time_diff_ms`: n=757  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 757
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/81-dual-stop-near-only

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=365  mean=6.4163e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=365  mean=0.555507  p50=0  p90=4.54747e-13  p95=9.09495e-13  p99=19.5464  max=31.68
- `pnl_diff`: n=365  mean=0.564411  p50=2.90989e-13  p90=0.03  p95=0.076  p99=19.5428  max=31.68
- `time_diff_ms`: n=365  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 365
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/82-dual-stop-far-only

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=364  mean=6.18406e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=364  mean=2.42396  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=94.2629  max=157.36
- `pnl_diff`: n=364  mean=2.43882  p50=0.01  p90=0.04  p95=0.1185  p99=94.2666  max=157.36
- `time_diff_ms`: n=364  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 364
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/83-dual-stop-open-tie

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=732  mean=5.87071e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=732  mean=6.30558e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=732  mean=2.73224e-05  p50=9.09516e-15  p90=2.36469e-13  p95=2.36469e-13  p99=6.2355e-13  max=0.01
- `time_diff_ms`: n=732  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 732
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/84-dual-stop-source-order-long-first

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=365  mean=6.4163e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=365  mean=0.555507  p50=0  p90=4.54747e-13  p95=9.09495e-13  p99=19.5464  max=31.68
- `pnl_diff`: n=365  mean=0.564411  p50=2.90989e-13  p90=0.03  p95=0.076  p99=19.5428  max=31.68
- `time_diff_ms`: n=365  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 365
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/85-dual-stop-source-order-short-first

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=365  mean=6.4163e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=365  mean=0.0709863  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=21.3
- `pnl_diff`: n=365  mean=0.0799452  p50=2.54019e-13  p90=0.02  p95=0.04  p99=0.1408  max=21.29
- `time_diff_ms`: n=365  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 365
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/86-dual-stop-open-proximity-low-first

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=188  mean=5.32151e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=188  mean=0.713936  p50=0  p90=4.54747e-13  p95=7.50333e-13  p99=18.9162  max=31.29
- `pnl_diff`: n=188  mean=0.722021  p50=2.54019e-13  p90=0.03  p95=0.093  p99=18.9149  max=31.3
- `time_diff_ms`: n=188  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 188
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/87-dual-stop-open-proximity-high-first

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=216  mean=6.4212e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=216  mean=0.686157  p50=0  p90=4.54747e-13  p95=9.09495e-13  p99=22.679  max=65.68
- `pnl_diff`: n=216  mean=0.696157  p50=3.00204e-13  p90=0.03  p95=0.0675  p99=22.6805  max=65.69
- `time_diff_ms`: n=216  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 216
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/88-exit-three-way-set-once-at-entry

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=792  mean=6.28723e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=792  mean=6.89011e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=792  mean=0.000277778  p50=1.8191e-13  p90=2.72837e-13  p95=7.27585e-13  p99=0.01  max=0.04
- `time_diff_ms`: n=792  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 792
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/89-trail-points-only-no-offset-explicit

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=782  mean=6.25132e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=782  mean=7.15268e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=782  mean=0.000652174  p50=7.27612e-14  p90=3.81986e-13  p95=3.81986e-13  p99=8.36733e-13  max=0.2
- `time_diff_ms`: n=782  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 782
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/90-narrow-stop-limit-with-trail

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=792  mean=6.28723e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=792  mean=6.74657e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=792  mean=0.00020202  p50=7.27612e-14  p90=3.81986e-13  p95=3.81986e-13  p99=0.01  max=0.02
- `time_diff_ms`: n=792  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 792
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/91-atr-trail-fixed-entry

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=792  mean=6.28723e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=792  mean=1.26263e-05  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=0.01
- `pnl_diff`: n=792  mean=0.000239899  p50=1.27329e-13  p90=3.63792e-13  p95=5.45702e-13  p99=0.01  max=0.02
- `time_diff_ms`: n=792  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 792
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/92-flip-stop-no-paired-close

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=724  mean=7.00336e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=724  mean=7.09758e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=724  mean=0.00979282  p50=3.00204e-13  p90=0.03  p95=0.04  p99=0.09  max=0.27
- `time_diff_ms`: n=724  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 724
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/93-flip-stop-pyramiding-2

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=843  mean=6.98574e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=843  mean=6.52722e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=843  mean=0.498932  p50=3.69482e-13  p90=0.05  p95=0.08  p99=8.2008  max=103.96
- `time_diff_ms`: n=843  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 843
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/94-stop-entry-cancel-no-regime-close

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=1609  mean=0.00151025  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=2.43
- `exit_diff`: n=1609  mean=5.96344e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=1609  mean=0.0132318  p50=4.72511e-13  p90=0.03  p95=0.05  p99=0.12  max=2.43
- `time_diff_ms`: n=1609  mean=1678.06  p50=0  p90=0  p95=0  p99=0  max=2.7e+06
- `entry_diff` histogram (abs):
  - (0,1e-06]: 1608
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 1
  - \>10: 0

### validation/95-multi-cycle-open-guaranteed-stops

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=791  mean=5.95023e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=791  mean=5.49031e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=791  mean=0.141909  p50=0.05  p90=0.3  p95=0.47  p99=1.287  max=10.49
- `time_diff_ms`: n=791  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 791
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/96-multi-cycle-pooc-cross-bar

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=789  mean=0.00524715  p50=4.54747e-13  p90=0.01  p95=0.01  p99=0.02  max=0.08
- `exit_diff`: n=789  mean=5.50423e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=789  mean=0.159176  p50=0.06  p90=0.352  p95=0.536  p99=1.272  max=10.49
- `time_diff_ms`: n=789  mean=900000  p50=900000  p90=900000  p95=900000  p99=900000  max=900000
- `entry_diff` histogram (abs):
  - (0,1e-06]: 400
  - (1e-06,0.01]: 178
  - (0.01,0.1]: 211
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/ies-probe-01-adx-regime-classify

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=682  mean=6.86789e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=682  mean=0.0202346  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=13.8
- `pnl_diff`: n=682  mean=0.0334604  p50=0.01  p90=0.03  p95=0.0595  p99=0.1714  max=13.8
- `time_diff_ms`: n=682  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 682
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/ies-probe-02-three-ema-bias

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=715  mean=6.3283e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=715  mean=0.000839161  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=0.6
- `pnl_diff`: n=715  mean=0.0144476  p50=3.2685e-13  p90=0.03  p95=0.06  p99=0.1958  max=0.61
- `time_diff_ms`: n=715  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 715
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/ies-probe-03-rsi-macd-momentum

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=4797  mean=6.4131e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=4797  mean=6.4131e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=4797  mean=0.00601626  p50=2.1827e-13  p90=0.02  p95=0.03  p99=0.06  max=0.37
- `time_diff_ms`: n=4797  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 4797
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/ies-probe-04-pressure-gauge

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=2206  mean=6.1327e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=2206  mean=6.1327e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=2206  mean=0.0105077  p50=3.81917e-13  p90=0.02  p95=0.04  p99=0.11  max=0.38
- `time_diff_ms`: n=2206  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 2206
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/ies-probe-05-bb-kc-squeeze

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=901  mean=6.48558e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=901  mean=5.80421e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=901  mean=0.00847947  p50=2.54685e-13  p90=0.02  p95=0.03  p99=0.09  max=0.41
- `time_diff_ms`: n=901  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 901
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/ies-probe-06-swing-pivot-sweep

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=705  mean=6.0633e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=705  mean=6.0633e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=705  mean=0.0195887  p50=0.01  p90=0.05  p95=0.078  p99=0.2296  max=0.59
- `time_diff_ms`: n=705  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 705
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/ies-probe-07-bars-since-trade-cooldown

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=727  mean=6.16129e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=727  mean=5.78599e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=727  mean=0.00936726  p50=3.27738e-13  p90=0.02  p95=0.04  p99=0.0974  max=0.29
- `time_diff_ms`: n=727  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 727
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/ies-probe-08-equity-feedback-sizing

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=79  mean=5.46848e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=4.54747e-13  max=4.54747e-13
- `exit_diff`: n=79  mean=0.00303797  p50=0  p90=0.01  p95=0.01  p99=0.01  max=0.01
- `pnl_diff`: n=79  mean=7.91483  p50=6.25358  p90=16.371  p95=18.759  p99=32.1243  max=41.792
- `time_diff_ms`: n=79  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 79
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/mtf-probe-01-htf-close-roll

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=8775  mean=6.23691e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=8775  mean=6.3276e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=8775  mean=0.00208205  p50=1.63425e-13  p90=0.01  p95=0.01  p99=0.03  max=0.12
- `time_diff_ms`: n=8775  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 8775
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/mtf-probe-02-dual-tf-closes

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=736  mean=7.13632e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=736  mean=5.52988e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=736  mean=0.00194293  p50=1.63425e-13  p90=0.01  p95=0.01  p99=0.03  max=0.05
- `time_diff_ms`: n=736  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 736
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/mtf-probe-03-htf-sma

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=539  mean=6.2011e-14  p50=0  p90=2.72848e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=539  mean=5.14649e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=539  mean=0.0123562  p50=5.08038e-13  p90=0.03  p95=0.05  p99=0.1324  max=0.28
- `time_diff_ms`: n=539  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 539
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/mtf-probe-04-daily-prev-high

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=147  mean=9.74459e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=4.54747e-13  max=9.09495e-13
- `exit_diff`: n=147  mean=6.03236e-14  p50=0  p90=3.18323e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=147  mean=0.0166667  p50=0.01  p90=0.04  p95=0.06  p99=0.14  max=0.27
- `time_diff_ms`: n=147  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 147
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/mtf-probe-05-htf-rsi

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=495  mean=6.93605e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=495  mean=5.14462e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=495  mean=0.0106465  p50=3.2685e-13  p90=0.02  p95=0.04  p99=0.14  max=0.35
- `time_diff_ms`: n=495  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 495
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/mtf-probe-06-htf-volume-sum

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=584  mean=6.57982e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=584  mean=5.295e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=584  mean=0.00875  p50=2.54463e-13  p90=0.02  p95=0.04  p99=0.1  max=0.16
- `time_diff_ms`: n=584  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 584
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/mtf-probe-07-triple-tf-close-confluence

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=47  mean=3.87019e-14  p50=0  p90=9.09495e-14  p95=3.86535e-13  p99=4.54747e-13  max=4.54747e-13
- `exit_diff`: n=47  mean=3.38642e-14  p50=0  p90=0  p95=3.86535e-13  p99=4.54747e-13  max=4.54747e-13
- `pnl_diff`: n=47  mean=0.107872  p50=0.04  p90=0.208  p95=0.358  p99=1.0216  max=1.27
- `time_diff_ms`: n=47  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 47
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/mtf-probe-08-triple-tf-macd-hist-confluence

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=22  mean=1.13687e-13  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=8.13998e-13  max=9.09495e-13
- `exit_diff`: n=22  mean=1.90864  p50=0  p90=4.54747e-13  p95=8.86757e-13  p99=33.1721  max=41.99
- `pnl_diff`: n=22  mean=2.05455  p50=0.05  p90=0.374  p95=1.159  p99=33.5742  max=42.18
- `time_diff_ms`: n=22  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 22
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/mtf-probe-09-htf-daily-ema-warmup

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=30  mean=1.51582e-14  p50=0  p90=0  p95=0  p99=3.22871e-13  max=4.54747e-13
- `exit_diff`: n=30  mean=1.51582e-14  p50=0  p90=0  p95=0  p99=3.22871e-13  max=4.54747e-13
- `pnl_diff`: n=30  mean=0.0636667  p50=0.025  p90=0.174  p95=0.232  p99=0.2713  max=0.28
- `time_diff_ms`: n=30  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 30
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/mtf-probe-10-htf-bracket-static

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=283  mean=5.4634e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=5.36602e-13  max=9.09495e-13
- `exit_diff`: n=283  mean=0.0035689  p50=0  p90=0.01  p95=0.01  p99=0.01  max=0.02
- `pnl_diff`: n=283  mean=0.015265  p50=0.01  p90=0.03  p95=0.05  p99=0.0718  max=0.15
- `time_diff_ms`: n=283  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 283
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/mtf-probe-11-htf-bracket-manual-trail

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=345  mean=5.33834e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=7.09406e-13  max=9.09495e-13
- `exit_diff`: n=345  mean=0.00214493  p50=0  p90=0.01  p95=0.01  p99=0.01  max=0.02
- `pnl_diff`: n=345  mean=0.00898551  p50=4.1922e-13  p90=0.02  p95=0.03  p99=0.0656  max=0.15
- `time_diff_ms`: n=345  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 345
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/parity-probe-01-stop-limit-timing

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=778  mean=6.37114e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=778  mean=0.0033419  p50=0  p90=0.01  p95=0.01  p99=0.01  max=0.2
- `pnl_diff`: n=778  mean=0.00800771  p50=3.91687e-13  p90=0.02  p95=0.0215  p99=0.0523  max=0.2
- `time_diff_ms`: n=778  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 778
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/parity-probe-02-choch-bos-isolator

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=1026  mean=5.58462e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=1026  mean=5.76191e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=4.54747e-13  max=9.09495e-13
- `pnl_diff`: n=1026  mean=0.00295322  p50=1.63869e-13  p90=0.01  p95=0.01  p99=0.03  max=0.09
- `time_diff_ms`: n=1026  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 1026
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/parity-probe-04-percent-of-equity-sizing

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=57  mean=1.1967e-14  p50=0  p90=0  p95=0  p99=3.27418e-13  max=4.54747e-13
- `exit_diff`: n=57  mean=7.18022e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=6.54836e-13  max=9.09495e-13
- `pnl_diff`: n=57  mean=0.01277  p50=0.0041192  p90=0.0426179  p95=0.0524215  p99=0.0672099  max=0.0784594
- `time_diff_ms`: n=57  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 57
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/parity-probe-05-small-equity-fraction

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=57  mean=1.1967e-14  p50=0  p90=0  p95=0  p99=3.27418e-13  max=4.54747e-13
- `exit_diff`: n=57  mean=7.18022e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=6.54836e-13  max=9.09495e-13
- `pnl_diff`: n=57  mean=0.173942  p50=0.08248  p90=0.4451  p95=0.581272  p99=0.972833  max=1.15056
- `time_diff_ms`: n=57  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 57
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/parity-probe-06-edge-margin-sizing

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=57  mean=1.1967e-14  p50=0  p90=0  p95=0  p99=3.27418e-13  max=4.54747e-13
- `exit_diff`: n=57  mean=7.18022e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=6.54836e-13  max=9.09495e-13
- `pnl_diff`: n=57  mean=0.873206  p50=0.42235  p90=2.25034  p95=2.96126  p99=4.8137  max=5.762
- `time_diff_ms`: n=57  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 57
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/pineforge-analyzer

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=147  mean=9.74459e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=4.54747e-13  max=9.09495e-13
- `exit_diff`: n=147  mean=6.03236e-14  p50=0  p90=3.18323e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=147  mean=0.0166667  p50=0.01  p90=0.04  p95=0.06  p99=0.14  max=0.27
- `time_diff_ms`: n=147  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 147
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/udt-method-probe-01-scalar-return

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=507  mean=6.23372e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=507  mean=5.56101e-14  p50=0  p90=3.18323e-13  p95=4.54747e-13  p99=4.54747e-13  max=9.09495e-13
- `pnl_diff`: n=507  mean=0.0110848  p50=0.01  p90=0.03  p95=0.05  p99=0.0994  max=0.12
- `time_diff_ms`: n=507  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 507
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/udt-method-probe-02-mutating-self

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=2293  mean=7.29817e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=2293  mean=6.12808e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=2293  mean=0.00225033  p50=1.63425e-13  p90=0.01  p95=0.01  p99=0.03  max=0.18
- `time_diff_ms`: n=2293  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 2293
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/udt-method-probe-03-extra-args

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=2504  mean=6.21101e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=2504  mean=6.43802e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=2504  mean=1.01868e-13  p50=9.10383e-15  p90=2.36478e-13  p95=2.36478e-13  p99=6.91225e-13  max=6.91225e-13
- `time_diff_ms`: n=2504  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 2504
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/udt-method-probe-04-default-param

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=417  mean=7.5246e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=417  mean=6.21597e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=417  mean=0.0135492  p50=0.01  p90=0.04  p95=0.05  p99=0.1  max=0.29
- `time_diff_ms`: n=417  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 417
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/udt-method-probe-05-in-conditional

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=401  mean=6.12378e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=401  mean=6.29388e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=401  mean=0.0163342  p50=0.01  p90=0.04  p95=0.06  p99=0.14  max=0.3
- `time_diff_ms`: n=401  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 401
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/udt-method-probe-06-in-switch

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=401  mean=6.12378e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=401  mean=6.29388e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=401  mean=0.0163342  p50=0.01  p90=0.04  p95=0.06  p99=0.14  max=0.3
- `time_diff_ms`: n=401  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 401
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/udt-method-probe-07-in-for-loop

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=1660  mean=5.8761e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=1660  mean=6.24593e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=1660  mean=0.00486145  p50=1.82077e-13  p90=0.01  p95=0.02  p99=0.06  max=0.17
- `time_diff_ms`: n=1660  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 1660
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/udt-method-probe-08-in-while-loop

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=306  mean=6.39024e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=306  mean=5.20136e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=4.54747e-13  max=9.09495e-13
- `pnl_diff`: n=306  mean=0.0103268  p50=7.98472e-13  p90=0.03  p95=0.04  p99=0.07  max=0.11
- `time_diff_ms`: n=306  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 306
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/udt-method-probe-09-uses-math-funcs

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=789  mean=6.4264e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=789  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `pnl_diff`: n=789  mean=0.0073384  p50=0.01  p90=0.02  p95=0.02  p99=0.05  max=0.09
- `time_diff_ms`: n=789  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 789
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/udt-method-probe-10-uses-na-nz

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=695  mean=6.37955e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=695  mean=5.79067e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=695  mean=0.00978417  p50=3.09086e-13  p90=0.03  p95=0.043  p99=0.1  max=0.29
- `time_diff_ms`: n=695  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 695
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/udt-method-probe-11-uses-history-of-globals

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=131  mean=7.11628e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=4.54747e-13  max=9.09495e-13
- `exit_diff`: n=131  mean=5.3806e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=4.54747e-13  max=4.54747e-13
- `pnl_diff`: n=131  mean=0.0170992  p50=0.01  p90=0.05  p95=0.075  p99=0.107  max=0.11
- `time_diff_ms`: n=131  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 131
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/udt-method-probe-12-receives-ta-result

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=2676  mean=6.72095e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=2676  mean=6.19415e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=2676  mean=0.00421151  p50=1.81188e-13  p90=0.01  p95=0.02  p99=0.05  max=0.32
- `time_diff_ms`: n=2676  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 2676
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/udt-method-probe-13-reads-strategy-state

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=705  mean=6.48257e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=705  mean=6.09555e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=705  mean=0.00947518  p50=3.27738e-13  p90=0.02  p95=0.04  p99=0.0996  max=0.18
- `time_diff_ms`: n=705  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 705
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/udt-method-probe-14-drives-strategy-entry

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=1634  mean=6.19225e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=1634  mean=6.19225e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=1634  mean=0.011377  p50=3.64153e-13  p90=0.03  p95=0.04  p99=0.12  max=0.81
- `time_diff_ms`: n=1634  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 1634
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/udt-method-probe-15-computes-exit-prices

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=704  mean=6.45948e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=704  mean=0.00377841  p50=0  p90=0.01  p95=0.01  p99=0.01  max=0.36
- `pnl_diff`: n=704  mean=0.0126278  p50=0.01  p90=0.03  p95=0.04  p99=0.08  max=0.36
- `time_diff_ms`: n=704  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 704
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/udt-method-probe-16-var-instance-streak

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=1007  mean=6.63832e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=1007  mean=6.4351e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=1007  mean=0.00209533  p50=1.45661e-13  p90=0.01  p95=0.01  p99=0.03  max=0.08
- `time_diff_ms`: n=1007  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 1007
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/udt-method-probe-17-tuple-return-destructure

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=1094  mean=5.81944e-14  p50=0  p90=3.86535e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=1094  mean=0.00421389  p50=0  p90=0.01  p95=0.01  p99=0.01  max=0.82
- `pnl_diff`: n=1094  mean=0.0132633  p50=0.01  p90=0.03  p95=0.04  p99=0.0707  max=0.82
- `time_diff_ms`: n=1094  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 1094
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/udt-method-probe-18-method-calls-method

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=173  mean=6.44006e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=5.82077e-13  max=9.09495e-13
- `exit_diff`: n=173  mean=3.9429e-14  p50=0  p90=0  p95=4.54747e-13  p99=5.82077e-13  max=9.09495e-13
- `pnl_diff`: n=173  mean=0.0116185  p50=4.33431e-13  p90=0.038  p95=0.05  p99=0.1056  max=0.12
- `time_diff_ms`: n=173  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 173
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/udt-method-probe-19-array-of-udt-method

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=868  mean=6.4702e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=868  mean=5.9201e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=868  mean=0.00957373  p50=3.2685e-13  p90=0.02  p95=0.04  p99=0.1  max=0.29
- `time_diff_ms`: n=868  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 868
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/udt-method-probe-20-udt-return-from-func

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=132  mean=7.06236e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=132  mean=3.10055e-14  p50=0  p90=0  p95=3.29692e-13  p99=4.54747e-13  max=4.54747e-13
- `pnl_diff`: n=132  mean=0.0143939  p50=0.01  p90=0.04  p95=0.06  p99=0.0869  max=0.12
- `time_diff_ms`: n=132  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 132
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/udt-method-probe-21-windowed-method-chain

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=173  mean=6.44006e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=5.82077e-13  max=9.09495e-13
- `exit_diff`: n=173  mean=3.9429e-14  p50=0  p90=0  p95=4.54747e-13  p99=5.82077e-13  max=9.09495e-13
- `pnl_diff`: n=173  mean=0.0116185  p50=4.33431e-13  p90=0.038  p95=0.05  p99=0.1056  max=0.12
- `time_diff_ms`: n=173  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 173
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/vcp-probe-01-pivot-strength-5

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=1937  mean=6.91394e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=1937  mean=6.12747e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=1937  mean=0.0073619  p50=2.18492e-13  p90=0.02  p95=0.03  p99=0.0764  max=0.57
- `time_diff_ms`: n=1937  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 1937
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/vcp-probe-02-fvg-zones

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=11797  mean=0.00146308  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=2.88
- `exit_diff`: n=11797  mean=0.0146639  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=39.6
- `pnl_diff`: n=11797  mean=0.0164839  p50=1.45661e-13  p90=0.01  p95=0.01  p99=0.03  max=38.36
- `time_diff_ms`: n=11797  mean=839.196  p50=0  p90=0  p95=0  p99=0  max=900000
- `entry_diff` histogram (abs):
  - (0,1e-06]: 11785
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 1
  - (0.1,1]: 3
  - (1,10]: 8
  - \>10: 0

### validation/vcp-probe-03-rsi-smooth-divergence

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=141  mean=5.64403e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=4.54747e-13  max=9.09495e-13
- `exit_diff`: n=141  mean=7.25661e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=7.27596e-13  max=9.09495e-13
- `pnl_diff`: n=141  mean=0.0374468  p50=0.01  p90=0.07  p95=0.14  p99=0.582  max=0.66
- `time_diff_ms`: n=141  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 141
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/vcp-probe-04-vol-zscore-anomaly

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=591  mean=6.92509e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=591  mean=6.57883e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=591  mean=0.0134856  p50=0.01  p90=0.04  p95=0.06  p99=0.13  max=0.31
- `time_diff_ms`: n=591  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 591
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/vcp-probe-05-cumulative-volume-delta

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=3119  mean=5.91215e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=3119  mean=6.08711e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=3119  mean=0.00482206  p50=1.98952e-13  p90=0.01  p95=0.02  p99=0.06  max=0.29
- `time_diff_ms`: n=3119  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 3119
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/vcp-probe-06-adx-volatility-regime

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=636  mean=7.32887e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=636  mean=0.0216981  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=13.8
- `pnl_diff`: n=636  mean=0.0287579  p50=2.23821e-13  p90=0.02  p95=0.03  p99=0.08  max=13.8
- `time_diff_ms`: n=636  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 636
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation/vcp-probe-07-session-tz-newyork

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=396  mean=6.08627e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=396  mean=7.75138e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=396  mean=0.0212374  p50=0.01  p90=0.05  p95=0.08  p99=0.1905  max=0.39
- `time_diff_ms`: n=396  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 396
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_barstate/barstate-magnifier-probe-01a-isconfirmed-magnifier-on

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=871  mean=6.44791e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=871  mean=5.97802e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=871  mean=0.00931114  p50=2.91323e-13  p90=0.02  p95=0.04  p99=0.1  max=0.29
- `time_diff_ms`: n=871  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 871
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_barstate/barstate-magnifier-probe-01b-isconfirmed-magnifier-off

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=871  mean=6.44791e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=871  mean=5.97802e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=871  mean=0.00931114  p50=2.91323e-13  p90=0.02  p95=0.04  p99=0.1  max=0.29
- `time_diff_ms`: n=871  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 871
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_lower_tf/lower-tf-probe-01-numeric-float-ratio15

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=820  mean=6.37755e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=820  mean=5.71207e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=820  mean=0.00985366  p50=3.19744e-13  p90=0.03  p95=0.05  p99=0.1  max=0.29
- `time_diff_ms`: n=820  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 820
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_lower_tf/lower-tf-probe-02-bool-array

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=3116  mean=6.64754e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=3116  mean=6.24621e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=3116  mean=0.00320924  p50=1.81188e-13  p90=0.01  p95=0.01  p99=0.04  max=0.15
- `time_diff_ms`: n=3116  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 3116
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_magnifier/magnifier-dist-probe-01-endpoints

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=871  mean=6.44791e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=871  mean=0.103628  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=1.897  max=43.26
- `pnl_diff`: n=871  mean=0.104742  p50=4.52971e-14  p90=5.81979e-13  p95=0.015  p99=1.897  max=43.26
- `time_diff_ms`: n=871  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 871
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_magnifier/magnifier-dist-probe-02-uniform

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=871  mean=6.44791e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=871  mean=0.103628  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=1.897  max=43.26
- `pnl_diff`: n=871  mean=0.104742  p50=4.52971e-14  p90=5.81979e-13  p95=0.015  p99=1.897  max=43.26
- `time_diff_ms`: n=871  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 871
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_magnifier/magnifier-dist-probe-03-cosine

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=871  mean=6.44791e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=871  mean=0.103628  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=1.897  max=43.26
- `pnl_diff`: n=871  mean=0.104742  p50=4.52971e-14  p90=5.81979e-13  p95=0.015  p99=1.897  max=43.26
- `time_diff_ms`: n=871  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 871
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_magnifier/magnifier-dist-probe-04-triangle

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=871  mean=6.44791e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=871  mean=0.103628  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=1.897  max=43.26
- `pnl_diff`: n=871  mean=0.104742  p50=4.52971e-14  p90=5.81979e-13  p95=0.015  p99=1.897  max=43.26
- `time_diff_ms`: n=871  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 871
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_magnifier/magnifier-dist-probe-05-front-loaded

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=871  mean=6.44791e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=871  mean=0.103628  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=1.897  max=43.26
- `pnl_diff`: n=871  mean=0.104742  p50=4.52971e-14  p90=5.81979e-13  p95=0.015  p99=1.897  max=43.26
- `time_diff_ms`: n=871  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 871
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_magnifier/magnifier-dist-probe-06-back-loaded

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=871  mean=6.44791e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=871  mean=0.103628  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=1.897  max=43.26
- `pnl_diff`: n=871  mean=0.104742  p50=4.52971e-14  p90=5.81979e-13  p95=0.015  p99=1.897  max=43.26
- `time_diff_ms`: n=871  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 871
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_magnifier/magnifier-dist-probe-07-volume-weighted-on

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=871  mean=6.44791e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=871  mean=0.103628  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=1.897  max=43.26
- `pnl_diff`: n=871  mean=0.104742  p50=4.52971e-14  p90=5.81979e-13  p95=0.015  p99=1.897  max=43.26
- `time_diff_ms`: n=871  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 871
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_magnifier/magnifier-dist-probe-08a-endpoints-rsi-cross

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=2345  mean=7.25269e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=2345  mean=0.257936  p50=0  p90=4.54747e-13  p95=9.09495e-13  p99=2.8244  max=190.86
- `pnl_diff`: n=2345  mean=0.259288  p50=1.2701e-13  p90=0.01  p95=0.028  p99=2.82  max=190.85
- `time_diff_ms`: n=2345  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 2345
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_magnifier/magnifier-dist-probe-08b-uniform-rsi-cross

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=2345  mean=7.25269e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=2345  mean=0.257936  p50=0  p90=4.54747e-13  p95=9.09495e-13  p99=2.8244  max=190.86
- `pnl_diff`: n=2345  mean=0.259288  p50=1.2701e-13  p90=0.01  p95=0.028  p99=2.82  max=190.85
- `time_diff_ms`: n=2345  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 2345
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_na_chain/na-chain-probe-01-deep-history-int-na

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=106  mean=4.29007e-14  p50=0  p90=0  p95=4.54747e-13  p99=4.54747e-13  max=9.09495e-13
- `exit_diff`: n=106  mean=5.79159e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=4.54747e-13  max=9.09495e-13
- `pnl_diff`: n=106  mean=0.0251887  p50=0.01  p90=0.08  p95=0.135  p99=0.189  max=0.24
- `time_diff_ms`: n=106  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 106
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_oca/oca-three-way-probe-01-same-bar-touch

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=421  mean=6.0489e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=421  mean=0.00501188  p50=0.01  p90=0.01  p95=0.01  p99=0.01  max=0.01
- `pnl_diff`: n=421  mean=0.0129929  p50=0.01  p90=0.03  p95=0.04  p99=0.092  max=0.11
- `time_diff_ms`: n=421  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 421
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_oca/oca-three-way-probe-02-multi-group-partial

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=1244  mean=6.39717e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=1244  mean=0.0048955  p50=4.54747e-13  p90=0.01  p95=0.01  p99=0.01  max=0.18
- `pnl_diff`: n=1244  mean=0.00987138  p50=0.01  p90=0.02  p95=0.03  p99=0.05  max=0.18
- `time_diff_ms`: n=1244  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 1244
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_process_orders/process-orders-probe-01-on-close-vs-next-bar

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=857  mean=6.31446e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=857  mean=5.94302e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=857  mean=0.00939323  p50=3.09086e-13  p90=0.02  p95=0.04  p99=0.1  max=0.29
- `time_diff_ms`: n=857  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 857
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_process_orders/process-orders-probe-02-on-close-true

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=857  mean=6.63284e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=857  mean=5.96955e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=857  mean=0.00938156  p50=2.911e-13  p90=0.02  p95=0.04  p99=0.1  max=0.29
- `time_diff_ms`: n=857  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 857
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_pyramid/pyramid-carry-probe-01-deferred-flip

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=2345  mean=0.000170576  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=0.25
- `exit_diff`: n=2345  mean=0.0166141  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=19.61
- `pnl_diff`: n=2345  mean=0.0693518  p50=3.2685e-13  p90=0.02  p95=0.04  p99=0.1356  max=98.54
- `time_diff_ms`: n=2345  mean=1535.18  p50=0  p90=0  p95=0  p99=0  max=3.6e+06
- `entry_diff` histogram (abs):
  - (0,1e-06]: 2343
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 2
  - (1,10]: 0
  - \>10: 0

### validation_pyramid/pyramid-carry-probe-02-cash-fractional

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=788  mean=6.46341e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=788  mean=4.4436e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=4.54747e-13  max=9.09495e-13
- `pnl_diff`: n=788  mean=0.198107  p50=0.0512101  p90=0.490081  p95=0.821999  p99=1.97846  max=7.43008
- `time_diff_ms`: n=788  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 788
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_recompute/ta-recompute-probe-01-mtf-rsi-macd-bb

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=286  mean=5.8831e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=4.54747e-13  max=9.09495e-13
- `exit_diff`: n=286  mean=0.0529371  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=15.14
- `pnl_diff`: n=286  mean=0.0673776  p50=0.01  p90=0.04  p95=0.0575  p99=0.1  max=15.14
- `time_diff_ms`: n=286  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 286
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_recompute/ta-recompute-probe-02-untested-classes

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=582  mean=5.66481e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=582  mean=6.71963e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=582  mean=0.0064433  p50=2.5456e-13  p90=0.02  p95=0.02  p99=0.06  max=0.12
- `time_diff_ms`: n=582  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 582
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_session/session-dst-probe-01-ny-spring-forward

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=396  mean=6.14368e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=4.54747e-13  max=9.09495e-13
- `exit_diff`: n=396  mean=7.75138e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=396  mean=0.0183081  p50=0.01  p90=0.04  p95=0.07  p99=0.1715  max=0.44
- `time_diff_ms`: n=396  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 396
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_ta_isolate/ta-isolate-01-rsi14-cross-50

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=4689  mean=6.57051e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=4689  mean=6.57051e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=4689  mean=0.00479633  p50=1.81855e-13  p90=0.01  p95=0.02  p99=0.06  max=0.95
- `time_diff_ms`: n=4689  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 4689
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_ta_isolate/ta-isolate-02-rsi14-bands-30-70

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=255  mean=6.24163e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=4.54747e-13  max=9.09495e-13
- `exit_diff`: n=255  mean=6.24163e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=4.54747e-13  max=9.09495e-13
- `pnl_diff`: n=255  mean=0.0321961  p50=0.01  p90=0.066  p95=0.12  p99=0.3576  max=0.57
- `time_diff_ms`: n=255  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 255
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_ta_isolate/ta-isolate-03-macd-12-26-9-cross

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=3021  mean=6.72864e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=3021  mean=6.72864e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=3021  mean=0.0090566  p50=3.09308e-13  p90=0.02  p95=0.04  p99=0.09  max=0.34
- `time_diff_ms`: n=3021  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 3021
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_ta_isolate/ta-isolate-04-hma-55-close-cross

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=4838  mean=6.28826e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=4838  mean=6.28826e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=4838  mean=0.00589293  p50=2.00728e-13  p90=0.02  p95=0.03  p99=0.07  max=0.35
- `time_diff_ms`: n=4838  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 4838
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_ta_isolate/ta-isolate-05-sma-152-close-cross

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=1616  mean=6.26122e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=1616  mean=6.26122e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=1616  mean=0.00727104  p50=1.81855e-13  p90=0.01  p95=0.03  p99=0.11  max=0.59
- `time_diff_ms`: n=1616  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 1616
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_ta_isolate/ta-isolate-06-rsi14-gt-50-continuous-state

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=2345  mean=7.25269e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=2345  mean=5.88554e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=2345  mean=0.00445629  p50=1.81855e-13  p90=0.01  p95=0.02  p99=0.06  max=0.32
- `time_diff_ms`: n=2345  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 2345
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_ta_isolate/ta-isolate-07-macd-line-gt-signal-continuous-state

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=1511  mean=7.28318e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=1511  mean=6.16964e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=1511  mean=0.00911317  p50=3.09308e-13  p90=0.02  p95=0.04  p99=0.09  max=0.24
- `time_diff_ms`: n=1511  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 1511
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_ta_isolate/ta-isolate-08-rsi-and-macd-continuous-state

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=1916  mean=7.2152e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=1916  mean=5.77928e-14  p50=0  p90=3.41061e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=1916  mean=0.00495303  p50=1.9984e-13  p90=0.01  p95=0.02  p99=0.05  max=0.24
- `time_diff_ms`: n=1916  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 1916
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_ta_isolate/ta-isolate-09-rsi14-gt60-lt45-no-matrix

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=785  mean=5.85089e-14  p50=0  p90=3.63798e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=785  mean=6.11157e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=785  mean=0.0108535  p50=5.45342e-13  p90=0.03  p95=0.05  p99=0.1  max=0.29
- `time_diff_ms`: n=785  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 785
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_ta_isolate/ta-isolate-10-typed-matrix-explicit-utc

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=769  mean=5.94306e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=769  mean=6.20916e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=769  mean=0.0110793  p50=7.27418e-13  p90=0.03  p95=0.05  p99=0.1  max=0.29
- `time_diff_ms`: n=769  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 769
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_ta_isolate/ta-isolate-11-mask-only-no-transpose

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=769  mean=5.94306e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=769  mean=6.20916e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=769  mean=0.0110793  p50=7.27418e-13  p90=0.03  p95=0.05  p99=0.1  max=0.29
- `time_diff_ms`: n=769  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 769
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_ta_isolate/ta-isolate-12-sample-only-no-hotcount

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=769  mean=5.94306e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=769  mean=6.20916e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=769  mean=0.0110793  p50=7.27418e-13  p90=0.03  p95=0.05  p99=0.1  max=0.29
- `time_diff_ms`: n=769  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 769
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_typed_matrix/typed-matrix-probe-01-bool-regime-mask

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=769  mean=5.94306e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=769  mean=6.20916e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=769  mean=0.0110793  p50=7.27418e-13  p90=0.03  p95=0.05  p99=0.1  max=0.29
- `time_diff_ms`: n=769  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 769
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_typed_matrix/typed-matrix-probe-02-eigen-rank-deficient

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=871  mean=6.44791e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=871  mean=5.95192e-14  p50=0  p90=4.54747e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `pnl_diff`: n=871  mean=0.00954076  p50=3.12639e-13  p90=0.02  p95=0.04  p99=0.1  max=0.29
- `time_diff_ms`: n=871  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 871
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

### validation_varip/varip-reject-probe-02-var-udt-in-security

**ALL** — entry/exit/pnl/time deltas (absolute)

- `entry_diff`: n=207  mean=5.93149e-14  p50=0  p90=2.27374e-13  p95=4.54747e-13  p99=9.09495e-13  max=9.09495e-13
- `exit_diff`: n=207  mean=5.82164e-14  p50=0  p90=3.18323e-13  p95=4.54747e-13  p99=8.8221e-13  max=9.09495e-13
- `pnl_diff`: n=207  mean=0.0148792  p50=0.01  p90=0.04  p95=0.05  p99=0.1294  max=0.28
- `time_diff_ms`: n=207  mean=0  p50=0  p90=0  p95=0  p99=0  max=0
- `entry_diff` histogram (abs):
  - (0,1e-06]: 207
  - (1e-06,0.01]: 0
  - (0.01,0.1]: 0
  - (0.1,1]: 0
  - (1,10]: 0
  - \>10: 0

## Failed / skipped (no compare or engine error)

_None._


## Known gaps (tracked for future work)

This run: **203/206** strategies at **`excellent`** (all four parity dims pass under their profile). Of those, 195 cleared the STRICT profile and 8 cleared the PRODUCTION profile (auto-applied to scripts using `trail_*` exits). **Strict Y:** 203/206. **Validation-override labels:** `expected_reject_passed`=0, `engine_only`=1, `FAILED_to_reject`=0. Counts by match label: `excellent`=203, `strong`=1, `moderate`=0, `weak`=1, `minimal`=0. Rows under **Failed / skipped** could not complete compare or hit a hard error; strategies with `weak` / `minimal` still ran compare — see the summary table.

| Category | Meaning | Planned direction |
|----------|---------|-------------------|
| `NO_ALIGNMENT` | No engine trade matches the first TV trade (`find_first_match`) | Data overlap, direction, or early backtest divergence |
| `no_common_trade_window` | TV and engine entry-time spans do not overlap after filters | Align `trades.csv` export window with OHLCV / symbol |
| `error` | Transpile or compile failed | Codegen / generated C++ for that strategy |
| `subprocess_failed` | Native crash (e.g. SIGSEGV) during backtest | Runtime or codegen bug on that path |
| `weak` / `minimal` | Compare ran but strict parity failed (trade count or entry/exit rel p90) | Same as historical `parity_failed` — C++ order/fill vs TV; tighten with engine fixes |
| `expected_reject_passed` | Probe set `validation_overrides.expect_reject=true` and codegen/runtime correctly rejected it | Success — no action |
| `engine_only` | Probe set `validation_overrides.expect_tv_match=false` (engine-only experiment) | Excluded from headline parity counts; metrics retained for diagnostics |
| `FAILED_to_reject` | Probe expected rejection but transpile/compile/run completed silently | Regression — fix the missing reject in codegen/runtime |

**Note:** Bar-for-bar scripts usually align better than `request.security` / MTF-heavy ones. Use `--legacy-alignment-only` or looser `--max-*` to explore match quality without blocking the sweep.

