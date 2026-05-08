# Corpus parity validation report

- **Generated**: `2026-05-08T16:18:00Z`
- **Host**: Darwin arm64 (16 cores), Python 3.14.3
- **pineforge-engine**: `7f9bce9` (dirty — refactor in progress: equity-mirror probe moved to parity-anomalies/)
- **pineforge-codegen**: `0a28aec` (dirty)
- **Pipeline**: codegen regen → cmake build (-j32) → harness run (xargs -P32) → verify_corpus.py --all --include-anomalies
- **Run wall-clock**: 65s for 168 strategies through the harness

## Headline (167 reference strategies)

This is the canonical figure. The headline counts cover the three reference-tier categories: `basic/`, `community/`, `validation/`. The `parity-anomalies/` category is excluded by design — see the next section.


| Tier      | Count   | Canonical (README) | Δ   |
| --------- | ------- | ------------------ | --- |
| excellent | 165     | 165                | —   |
| strong    | 2       | 2                  | —   |
| moderate  | 0       | 0                  | —   |
| weak      | 0       | 0                  | —   |
| minimal   | 0       | 0                  | —   |
| missing   | 0       | 0                  | —   |
| **total** | **167** | **167**            | —   |


Reproduce with:

```bash
python scripts/verify_corpus.py --all
```

## Anomaly probes (excluded from headline)

Probes that *deliberately* surface TV-side non-determinism. The engine is correct; the divergence is documented. Excluded from the headline so they don't mask as regressions. See `[corpus/parity-anomalies/README.md](parity-anomalies/README.md)` for the local index, and `[pineforge-utils/parity-anomalies/](https://github.com/fullpass-4pass/pineforge-utils/tree/main/parity-anomalies)` for each probe's full write-up.

### `parity-anomalies/equity-mirror` → **weak** *(documented anomaly, expected)*

```
  Profile:       strict
  TV trades:     24  (raw 24)
  Engine trades: 25  (raw 25)
  Matched:       13 (54.2% of TV)
  Count delta:             4.0000%  (X)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   2.3202%  (X)
  -> weak
```

Reproduce with:

```bash
python scripts/verify_corpus.py --category parity-anomalies
# or fold into the full sweep:
python scripts/verify_corpus.py --all --include-anomalies
```

## `strong` tier in the reference sweep

Trades match TV trade-by-trade; only the count delta exceeds the 1.0% strict threshold. Entry, exit, and PnL all pass strict on every matched trade.

### `community/VCP` → **strong**

```
  Profile:       strict
  TV trades:     71  (raw 71)
  Engine trades: 72  (raw 72)
  Matched:       71 (100.0% of TV)
  Count delta:             1.3889%  (X)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0005%  (OK)
  PnL         p90 delta:   0.1026%  (OK)
  -> strong
```

### `community/scalping-wunder-bots` → **strong**

```
  Profile:       strict
  TV trades:     419  (raw 419)
  Engine trades: 424  (raw 424)
  Matched:       417 (99.5% of TV)
  Count delta:             1.1792%  (X)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0005%  (OK)
  PnL         p90 delta:   0.1235%  (OK)
  -> strong
```

## Methodology

Strict-profile thresholds applied uniformly to every strategy (matches `pineforge-utils/validator/validate.py::DEFAULT_PARITY_STRICT`):

- trade-count delta < **1.0%**
- entry-price p90 delta < **0.01%**
- exit-price p90 delta < **0.01%**
- per-trade P&L p90 delta < **1.0%**

Trades are matched on direction + entry time (1-hour window) with a $3 entry-price gate to defend against same-bar duplicates. Common-window edge trimming drops bootstrap/terminal trades outside the mutually matched window. See `scripts/verify_corpus.py` for the canonical implementation.

## Full per-strategy output

Output of `python scripts/verify_corpus.py --all --include-anomalies` (168 strategies including the anomaly probe):

Click to expand all 168 strategy reports (1853 lines)

```
basic/greedy
  Profile:       strict
  TV trades:     13  (raw 13)
  Engine trades: 13  (raw 13)
  Matched:       13 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0000%  (OK)
  -> excellent

basic/inside-bar
  Profile:       strict
  TV trades:     3332  (raw 3332)
  Engine trades: 3332  (raw 3332)
  Matched:       3332 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0785%  (OK)
  -> excellent

basic/keltner
  Profile:       strict
  TV trades:     314  (raw 314)
  Engine trades: 314  (raw 314)
  Matched:       313 (99.7% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0004%  (OK)
  Exit-price  p90 delta:   0.0004%  (OK)
  PnL         p90 delta:   0.1123%  (OK)
  -> excellent

basic/ma-cros
  Profile:       strict
  TV trades:     2315  (raw 2315)
  Engine trades: 2315  (raw 2315)
  Matched:       2315 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0799%  (OK)
  -> excellent

basic/parabolic-asr
  Profile:       strict
  TV trades:     3082  (raw 3082)
  Engine trades: 3082  (raw 3082)
  Matched:       3081 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0824%  (OK)
  -> excellent

basic/pivot-ext
  Profile:       strict
  TV trades:     4890  (raw 4890)
  Engine trades: 4890  (raw 4890)
  Matched:       4890 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0775%  (OK)
  -> excellent

basic/stochastic-slow
  Profile:       strict
  TV trades:     690  (raw 690)
  Engine trades: 690  (raw 690)
  Matched:       690 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0816%  (OK)
  -> excellent

basic/supertrend
  Profile:       strict
  TV trades:     760  (raw 761)
  Engine trades: 760  (raw 760)
  Matched:       760 (99.9% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0776%  (OK)
  -> excellent

basic/volty-expan
  Profile:       strict
  TV trades:     7235  (raw 7235)
  Engine trades: 7299  (raw 7299)
  Matched:       7132 (98.6% of TV)
  Count delta:             0.8768%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.1160%  (OK)
  -> excellent

community/4ema_rsi
  Profile:       strict
  TV trades:     809  (raw 809)
  Engine trades: 809  (raw 809)
  Matched:       809 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0921%  (OK)
  -> excellent

community/BOS_curv
  Profile:       strict
  TV trades:     262  (raw 272)
  Engine trades: 262  (raw 267)
  Matched:       262 (96.3% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0770%  (OK)
  -> excellent

community/IES
  Profile:       strict
  TV trades:     2668  (raw 2668)
  Engine trades: 2643  (raw 2643)
  Matched:       2618 (98.1% of TV)
  Count delta:             0.9370%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.7809%  (OK)
  -> excellent

community/KKB
  Profile:       strict
  TV trades:     149  (raw 150)
  Engine trades: 149  (raw 150)
  Matched:       149 (99.3% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0787%  (OK)
  -> excellent

community/LiquitySweep
  Profile:       strict
  TV trades:     93  (raw 93)
  Engine trades: 93  (raw 93)
  Matched:       93 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0005%  (OK)
  PnL         p90 delta:   0.0966%  (OK)
  -> excellent

community/MarketShift
  Profile:       strict
  TV trades:     1152  (raw 1152)
  Engine trades: 1152  (raw 1152)
  Matched:       1148 (99.7% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0771%  (OK)
  -> excellent

community/VCP
  Profile:       strict
  TV trades:     71  (raw 71)
  Engine trades: 72  (raw 72)
  Matched:       71 (100.0% of TV)
  Count delta:             1.3889%  (X)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0005%  (OK)
  PnL         p90 delta:   0.1026%  (OK)
  -> strong

community/kanuck
  Profile:       strict
  TV trades:     875  (raw 875)
  Engine trades: 875  (raw 875)
  Matched:       875 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0845%  (OK)
  -> excellent

community/scalping-strategy
  Profile:       strict
  TV trades:     468  (raw 468)
  Engine trades: 468  (raw 468)
  Matched:       468 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0000%  (OK)
  -> excellent

community/scalping-wunder-bots
  Profile:       strict
  TV trades:     419  (raw 419)
  Engine trades: 424  (raw 424)
  Matched:       417 (99.5% of TV)
  Count delta:             1.1792%  (X)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0005%  (OK)
  PnL         p90 delta:   0.1235%  (OK)
  -> strong

community/trendmaster
  Profile:       strict
  TV trades:     123  (raw 123)
  Engine trades: 123  (raw 123)
  Matched:       123 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0005%  (OK)
  PnL         p90 delta:   0.1368%  (OK)
  -> excellent

validation/01-macd-histogram
  Profile:       strict
  TV trades:     2813  (raw 2814)
  Engine trades: 2813  (raw 2813)
  Matched:       2813 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0884%  (OK)
  -> excellent

validation/02-bb-squeeze
  Profile:       strict
  TV trades:     813  (raw 814)
  Engine trades: 813  (raw 813)
  Matched:       813 (99.9% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0863%  (OK)
  -> excellent

validation/03-dmi-adx-trend
  Profile:       strict
  TV trades:     2747  (raw 2747)
  Engine trades: 2743  (raw 2743)
  Matched:       2741 (99.8% of TV)
  Count delta:             0.1456%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0779%  (OK)
  -> excellent

validation/04-stoch-rsi
  Profile:       strict
  TV trades:     1337  (raw 1337)
  Engine trades: 1337  (raw 1337)
  Matched:       1305 (97.6% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.1152%  (OK)
  -> excellent

validation/05-hma-cross
  Profile:       strict
  TV trades:     4713  (raw 4713)
  Engine trades: 4713  (raw 4713)
  Matched:       4713 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0821%  (OK)
  -> excellent

validation/06-cci-momentum
  Profile:       strict
  TV trades:     2462  (raw 2462)
  Engine trades: 2462  (raw 2462)
  Matched:       2462 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0719%  (OK)
  -> excellent

validation/07-tsi-signal
  Profile:       strict
  TV trades:     846  (raw 846)
  Engine trades: 846  (raw 846)
  Matched:       846 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0823%  (OK)
  -> excellent

validation/08-linreg-channel
  Profile:       strict
  TV trades:     248  (raw 248)
  Engine trades: 248  (raw 248)
  Matched:       248 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0942%  (OK)
  -> excellent

validation/09-aroon-oscillator
  Profile:       strict
  TV trades:     1585  (raw 1585)
  Engine trades: 1585  (raw 1585)
  Matched:       1585 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0834%  (OK)
  -> excellent

validation/10-donchian-breakout
  Profile:       strict
  TV trades:     1002  (raw 1002)
  Engine trades: 1002  (raw 1002)
  Matched:       1002 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0793%  (OK)
  -> excellent

validation/11-elder-ray
  Profile:       strict
  TV trades:     2483  (raw 2483)
  Engine trades: 2483  (raw 2483)
  Matched:       2483 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0786%  (OK)
  -> excellent

validation/12-chandelier-exit
  Profile:       strict
  TV trades:     1603  (raw 1604)
  Engine trades: 1603  (raw 1604)
  Matched:       1603 (99.9% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0827%  (OK)
  -> excellent

validation/13-atr-trailing-stop
  Profile:       strict
  TV trades:     5073  (raw 5073)
  Engine trades: 5072  (raw 5072)
  Matched:       5072 (100.0% of TV)
  Count delta:             0.0197%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0786%  (OK)
  -> excellent

validation/14-vwma-divergence
  Profile:       strict
  TV trades:     2574  (raw 2574)
  Engine trades: 2574  (raw 2574)
  Matched:       2574 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0800%  (OK)
  -> excellent

validation/15-momentum-roc
  Profile:       strict
  TV trades:     5690  (raw 5690)
  Engine trades: 5690  (raw 5690)
  Matched:       5690 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0782%  (OK)
  -> excellent

validation/16-mean-reversion-bb
  Profile:       strict
  TV trades:     495  (raw 495)
  Engine trades: 495  (raw 495)
  Matched:       495 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0819%  (OK)
  -> excellent

validation/17-dual-ma-switch
  Profile:       strict
  TV trades:     1238  (raw 1239)
  Engine trades: 1238  (raw 1238)
  Matched:       1238 (99.9% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0888%  (OK)
  -> excellent

validation/18-ema-ribbon-loop
  Profile:       strict
  TV trades:     626  (raw 628)
  Engine trades: 626  (raw 627)
  Matched:       626 (99.7% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0796%  (OK)
  -> excellent

validation/19-pivot-array-breakout
  Profile:       strict
  TV trades:     829  (raw 829)
  Engine trades: 829  (raw 829)
  Matched:       829 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0893%  (OK)
  -> excellent

validation/20-range-filter-while
  Profile:       strict
  TV trades:     401  (raw 402)
  Engine trades: 401  (raw 402)
  Matched:       401 (99.8% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0788%  (OK)
  -> excellent

validation/21-adaptive-ma-func
  Profile:       strict
  TV trades:     4598  (raw 4599)
  Engine trades: 4600  (raw 4608)
  Matched:       4598 (100.0% of TV)
  Count delta:             0.0435%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0772%  (OK)
  -> excellent

validation/22-candle-pattern
  Profile:       strict
  TV trades:     826  (raw 826)
  Engine trades: 826  (raw 826)
  Matched:       825 (99.9% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0791%  (OK)
  -> excellent

validation/23-dual-thrust
  Profile:       strict
  TV trades:     2870  (raw 2870)
  Engine trades: 2870  (raw 2870)
  Matched:       2870 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0784%  (OK)
  -> excellent

validation/25-percent-rank-revert
  Profile:       strict
  TV trades:     363  (raw 363)
  Engine trades: 362  (raw 362)
  Matched:       362 (99.7% of TV)
  Count delta:             0.2755%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0714%  (OK)
  -> excellent

validation/26-volume-breakout
  Profile:       strict
  TV trades:     1778  (raw 1778)
  Engine trades: 1778  (raw 1778)
  Matched:       1778 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0829%  (OK)
  -> excellent

validation/27-ma-stack-array
  Profile:       strict
  TV trades:     1406  (raw 1407)
  Engine trades: 1406  (raw 1406)
  Matched:       1406 (99.9% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0796%  (OK)
  -> excellent

validation/28-swing-pivot-atr
  Profile:       strict
  TV trades:     1618  (raw 1618)
  Engine trades: 1619  (raw 1619)
  Matched:       1618 (100.0% of TV)
  Count delta:             0.0618%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0005%  (OK)
  PnL         p90 delta:   0.1264%  (OK)
  -> excellent

validation/29-median-cross
  Profile:       strict
  TV trades:     2837  (raw 2837)
  Engine trades: 2837  (raw 2837)
  Matched:       2837 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0826%  (OK)
  -> excellent

validation/30-multi-indicator-score
  Profile:       strict
  TV trades:     3910  (raw 3910)
  Engine trades: 3911  (raw 3911)
  Matched:       3910 (100.0% of TV)
  Count delta:             0.0256%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0830%  (OK)
  -> excellent

validation/31-rsi-bands
  Profile:       strict
  TV trades:     350  (raw 350)
  Engine trades: 350  (raw 350)
  Matched:       350 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0802%  (OK)
  -> excellent

validation/32-supertrend-adx-filter
  Profile:       strict
  TV trades:     455  (raw 455)
  Engine trades: 455  (raw 455)
  Matched:       455 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0768%  (OK)
  -> excellent

validation/33-matrix-eigen-pca
  Profile:       strict
  TV trades:     2850  (raw 2850)
  Engine trades: 2850  (raw 2850)
  Matched:       2850 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0778%  (OK)
  -> excellent

validation/34-map-regime-tracker
  Profile:       strict
  TV trades:     801  (raw 802)
  Engine trades: 801  (raw 804)
  Matched:       801 (99.9% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0783%  (OK)
  -> excellent

validation/35-advanced-trade-metrics
  Profile:       strict
  TV trades:     751  (raw 751)
  Engine trades: 751  (raw 751)
  Matched:       751 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.1214%  (OK)
  -> excellent

validation/36-mtf-array-stats
  Profile:       strict
  TV trades:     362  (raw 362)
  Engine trades: 362  (raw 362)
  Matched:       362 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0961%  (OK)
  -> excellent

validation/37-regex-string-filter
  Profile:       strict
  TV trades:     1777  (raw 1780)
  Engine trades: 1777  (raw 1778)
  Matched:       1777 (99.8% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0812%  (OK)
  -> excellent

validation/38-udt-regime-stack
  Profile:       strict
  TV trades:     3910  (raw 3910)
  Engine trades: 3911  (raw 3911)
  Matched:       3910 (100.0% of TV)
  Count delta:             0.0256%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0830%  (OK)
  -> excellent

validation/39-request-security-parity
  Profile:       strict
  TV trades:     8775  (raw 8775)
  Engine trades: 8775  (raw 8775)
  Matched:       8775 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0672%  (OK)
  -> excellent

validation/40-bracket-exit-tp-sl
  Profile:       strict
  TV trades:     366  (raw 366)
  Engine trades: 366  (raw 366)
  Matched:       366 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0005%  (OK)
  PnL         p90 delta:   0.1255%  (OK)
  -> excellent

validation/41-partial-exit-qty-percent
  Profile:       strict
  TV trades:     725  (raw 725)
  Engine trades: 725  (raw 725)
  Matched:       725 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0004%  (OK)
  PnL         p90 delta:   0.1321%  (OK)
  -> excellent

validation/42-close-immediate-vs-next-bar
  Profile:       strict
  TV trades:     732  (raw 732)
  Engine trades: 732  (raw 732)
  Matched:       732 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0794%  (OK)
  -> excellent

validation/43-close-all-cancel-all
  Profile:       strict
  TV trades:     732  (raw 732)
  Engine trades: 732  (raw 732)
  Matched:       732 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0005%  (OK)
  PnL         p90 delta:   0.4543%  (OK)
  -> excellent

validation/44-oca-reduce-orders
  Profile:       strict
  TV trades:     366  (raw 366)
  Engine trades: 366  (raw 366)
  Matched:       366 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0005%  (OK)
  PnL         p90 delta:   0.1287%  (OK)
  -> excellent

validation/45-commission-and-sizing
  Profile:       strict
  TV trades:     366  (raw 366)
  Engine trades: 366  (raw 366)
  Matched:       366 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0014%  (OK)
  -> excellent

validation/46-risk-gates
  Profile:       strict
  TV trades:     732  (raw 732)
  Engine trades: 732  (raw 732)
  Matched:       732 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0726%  (OK)
  -> excellent

validation/47-request-security-gaps-on
  Profile:       strict
  TV trades:     2247  (raw 2247)
  Engine trades: 2247  (raw 2247)
  Matched:       2247 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0736%  (OK)
  -> excellent

validation/48-session-time-filter
  Profile:       strict
  TV trades:     366  (raw 366)
  Engine trades: 366  (raw 366)
  Matched:       366 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0698%  (OK)
  -> excellent

validation/49-na-nz-history-chain
  Profile:       strict
  TV trades:     3093  (raw 3094)
  Engine trades: 3093  (raw 3093)
  Matched:       3093 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0756%  (OK)
  -> excellent

validation/50-exit-stop-limit-trail-same-bar
  Profile:       strict
  TV trades:     732  (raw 732)
  Engine trades: 732  (raw 732)
  Matched:       732 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0934%  (OK)
  -> excellent

validation/51-trail-points-without-offset
  Profile:       strict
  TV trades:     710  (raw 710)
  Engine trades: 710  (raw 710)
  Matched:       710 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0934%  (OK)
  -> excellent

validation/52-stop-entry-cancel-opposite
  Profile:       strict
  TV trades:     1738  (raw 1739)
  Engine trades: 1738  (raw 1741)
  Matched:       1738 (99.9% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0843%  (OK)
  -> excellent

validation/53-stop-entry-touch-boundary
  Profile:       strict
  TV trades:     548  (raw 548)
  Engine trades: 548  (raw 548)
  Matched:       548 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0004%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.1220%  (OK)
  -> excellent

validation/54-mtf-roll-state-minimal
  Profile:       strict
  TV trades:     2938  (raw 2938)
  Engine trades: 2948  (raw 2948)
  Matched:       2938 (100.0% of TV)
  Count delta:             0.3392%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0676%  (OK)
  -> excellent

validation/55-entry-reversal-implicit-exit
  Profile:       strict
  TV trades:     1098  (raw 1098)
  Engine trades: 1098  (raw 1098)
  Matched:       1098 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0766%  (OK)
  -> excellent

validation/56-trailing-offset-activation-path
  Profile:       strict
  TV trades:     671  (raw 671)
  Engine trades: 671  (raw 671)
  Matched:       671 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0918%  (OK)
  -> excellent

validation/57-stop-entry-reversal-grouping
  Profile:       strict
  TV trades:     1464  (raw 1464)
  Engine trades: 1464  (raw 1464)
  Matched:       1464 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0748%  (OK)
  -> excellent

validation/58-same-id-stop-modification
  Profile:       strict
  TV trades:     689  (raw 689)
  Engine trades: 689  (raw 689)
  Matched:       689 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0004%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.1438%  (OK)
  -> excellent

validation/59-market-close-fill-basis
  Profile:       strict
  TV trades:     732  (raw 732)
  Engine trades: 732  (raw 732)
  Matched:       732 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0773%  (OK)
  -> excellent

validation/60-pyramiding-close-id-grouping
  Profile:       strict
  TV trades:     2196  (raw 2196)
  Engine trades: 2196  (raw 2196)
  Matched:       2196 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0804%  (OK)
  -> excellent

validation/61-same-id-stop-raise-only
  Profile:       strict
  TV trades:     666  (raw 666)
  Engine trades: 666  (raw 666)
  Matched:       666 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0004%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.1471%  (OK)
  -> excellent

validation/62-same-id-stop-cross-before-modify
  Profile:       strict
  TV trades:     732  (raw 732)
  Engine trades: 732  (raw 732)
  Matched:       732 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0849%  (OK)
  -> excellent

validation/63-dual-stop-cancel-rotation
  Profile:       strict
  TV trades:     792  (raw 792)
  Engine trades: 792  (raw 792)
  Matched:       792 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0004%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.1258%  (OK)
  -> excellent

validation/64-same-id-market-entry-repeat
  Profile:       strict
  TV trades:     732  (raw 732)
  Engine trades: 732  (raw 732)
  Matched:       732 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0782%  (OK)
  -> excellent

validation/65-same-id-entry-close-same-bar
  Profile:       strict
  TV trades:     732  (raw 732)
  Engine trades: 732  (raw 732)
  Matched:       732 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0656%  (OK)
  -> excellent

validation/67-same-id-exit-replace
  Profile:       strict
  TV trades:     366  (raw 366)
  Engine trades: 366  (raw 366)
  Matched:       366 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0004%  (OK)
  PnL         p90 delta:   0.1609%  (OK)
  -> excellent

validation/68-cross-entry-close-same-pass
  Profile:       strict
  TV trades:     732  (raw 732)
  Engine trades: 732  (raw 732)
  Matched:       732 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0828%  (OK)
  -> excellent

validation/69-cross-entry-cancel-same-pass
  Profile:       strict
  TV trades:     495  (raw 495)
  Engine trades: 495  (raw 495)
  Matched:       495 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0004%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.1485%  (OK)
  -> excellent

validation/70-cross-exit-close-same-pass
  Profile:       strict
  TV trades:     732  (raw 732)
  Engine trades: 732  (raw 732)
  Matched:       732 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0675%  (OK)
  -> excellent

validation/71-cross-entry-exit-same-pass
  Profile:       strict
  TV trades:     728  (raw 728)
  Engine trades: 728  (raw 728)
  Matched:       728 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0005%  (OK)
  PnL         p90 delta:   0.1248%  (OK)
  -> excellent

validation/72-cross-opposite-entry-close
  Profile:       strict
  TV trades:     843  (raw 843)
  Engine trades: 843  (raw 843)
  Matched:       843 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0004%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.1132%  (OK)
  -> excellent

validation/73-same-id-stop-minute-zero-only
  Profile:       strict
  TV trades:     578  (raw 578)
  Engine trades: 578  (raw 578)
  Matched:       578 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0004%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.1488%  (OK)
  -> excellent

validation/74-same-id-stop-window-four-bars
  Profile:       strict
  TV trades:     349  (raw 349)
  Engine trades: 349  (raw 349)
  Matched:       349 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0004%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.1318%  (OK)
  -> excellent

validation/75-same-id-stop-after-flat
  Profile:       strict
  TV trades:     703  (raw 703)
  Engine trades: 703  (raw 703)
  Matched:       703 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0004%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.1479%  (OK)
  -> excellent

validation/76-dual-side-same-id-stop-window-no-cancel
  Profile:       strict
  TV trades:     689  (raw 689)
  Engine trades: 689  (raw 689)
  Matched:       689 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0004%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.1438%  (OK)
  -> excellent

validation/77-dual-side-stop-window-with-close-no-cancel
  Profile:       strict
  TV trades:     572  (raw 572)
  Engine trades: 572  (raw 572)
  Matched:       572 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0004%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.1498%  (OK)
  -> excellent

validation/78-dual-side-four-bar-stop-window-no-close
  Profile:       strict
  TV trades:     366  (raw 366)
  Engine trades: 366  (raw 366)
  Matched:       366 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0004%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.1057%  (OK)
  -> excellent

validation/79-one-side-four-bar-with-far-opposite
  Profile:       strict
  TV trades:     349  (raw 349)
  Engine trades: 349  (raw 349)
  Matched:       349 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0004%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.1318%  (OK)
  -> excellent

validation/80-dual-stop-both-touch-priority
  Profile:       strict
  TV trades:     757  (raw 757)
  Engine trades: 758  (raw 758)
  Matched:       757 (100.0% of TV)
  Count delta:             0.1319%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0953%  (OK)
  -> excellent

validation/81-dual-stop-near-only
  Profile:       strict
  TV trades:     365  (raw 365)
  Engine trades: 366  (raw 366)
  Matched:       365 (100.0% of TV)
  Count delta:             0.2732%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.1100%  (OK)
  -> excellent

validation/82-dual-stop-far-only
  Profile:       strict
  TV trades:     364  (raw 364)
  Engine trades: 365  (raw 365)
  Matched:       364 (100.0% of TV)
  Count delta:             0.2740%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0951%  (OK)
  -> excellent

validation/83-dual-stop-open-tie
  Profile:       strict
  TV trades:     732  (raw 732)
  Engine trades: 732  (raw 732)
  Matched:       732 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0000%  (OK)
  -> excellent

validation/84-dual-stop-source-order-long-first
  Profile:       strict
  TV trades:     365  (raw 365)
  Engine trades: 366  (raw 366)
  Matched:       365 (100.0% of TV)
  Count delta:             0.2732%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.1100%  (OK)
  -> excellent

validation/85-dual-stop-source-order-short-first
  Profile:       strict
  TV trades:     365  (raw 365)
  Engine trades: 366  (raw 366)
  Matched:       365 (100.0% of TV)
  Count delta:             0.2732%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0871%  (OK)
  -> excellent

validation/86-dual-stop-open-proximity-low-first
  Profile:       strict
  TV trades:     188  (raw 189)
  Engine trades: 189  (raw 189)
  Matched:       188 (99.5% of TV)
  Count delta:             0.5291%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.1113%  (OK)
  -> excellent

validation/87-dual-stop-open-proximity-high-first
  Profile:       strict
  TV trades:     216  (raw 216)
  Engine trades: 217  (raw 217)
  Matched:       216 (100.0% of TV)
  Count delta:             0.4608%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.1142%  (OK)
  -> excellent

validation/88-exit-three-way-set-once-at-entry
  Profile:       strict
  TV trades:     792  (raw 792)
  Engine trades: 792  (raw 792)
  Matched:       792 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0000%  (OK)
  -> excellent

validation/89-trail-points-only-no-offset-explicit
  Profile:       strict
  TV trades:     782  (raw 782)
  Engine trades: 782  (raw 782)
  Matched:       782 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0000%  (OK)
  -> excellent

validation/90-narrow-stop-limit-with-trail
  Profile:       strict
  TV trades:     792  (raw 792)
  Engine trades: 792  (raw 792)
  Matched:       792 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0000%  (OK)
  -> excellent

validation/91-atr-trail-fixed-entry
  Profile:       strict
  TV trades:     792  (raw 792)
  Engine trades: 792  (raw 792)
  Matched:       792 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0000%  (OK)
  -> excellent

validation/92-flip-stop-no-paired-close
  Profile:       strict
  TV trades:     724  (raw 724)
  Engine trades: 724  (raw 724)
  Matched:       724 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0004%  (OK)
  Exit-price  p90 delta:   0.0004%  (OK)
  PnL         p90 delta:   0.2511%  (OK)
  -> excellent

validation/93-flip-stop-pyramiding-2
  Profile:       strict
  TV trades:     843  (raw 843)
  Engine trades: 843  (raw 843)
  Matched:       843 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0004%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.1132%  (OK)
  -> excellent

validation/94-stop-entry-cancel-no-regime-close
  Profile:       strict
  TV trades:     1610  (raw 1611)
  Engine trades: 1610  (raw 1613)
  Matched:       1610 (99.9% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0938%  (OK)
  -> excellent

validation/95-multi-cycle-open-guaranteed-stops
  Profile:       strict
  TV trades:     792  (raw 792)
  Engine trades: 792  (raw 792)
  Matched:       792 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0779%  (OK)
  -> excellent

validation/96-multi-cycle-pooc-cross-bar
  Profile:       strict
  TV trades:     791  (raw 792)
  Engine trades: 791  (raw 791)
  Matched:       791 (99.9% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0005%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.1441%  (OK)
  -> excellent

validation/ies-probe-01-adx-regime-classify
  Profile:       strict
  TV trades:     682  (raw 682)
  Engine trades: 683  (raw 683)
  Matched:       682 (100.0% of TV)
  Count delta:             0.1464%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0769%  (OK)
  -> excellent

validation/ies-probe-02-three-ema-bias
  Profile:       strict
  TV trades:     717  (raw 720)
  Engine trades: 717  (raw 723)
  Matched:       716 (99.4% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0771%  (OK)
  -> excellent

validation/ies-probe-03-rsi-macd-momentum
  Profile:       strict
  TV trades:     4798  (raw 4799)
  Engine trades: 4798  (raw 4799)
  Matched:       4798 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0821%  (OK)
  -> excellent

validation/ies-probe-04-pressure-gauge
  Profile:       strict
  TV trades:     2207  (raw 2207)
  Engine trades: 2207  (raw 2207)
  Matched:       2207 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0786%  (OK)
  -> excellent

validation/ies-probe-05-bb-kc-squeeze
  Profile:       strict
  TV trades:     902  (raw 903)
  Engine trades: 902  (raw 902)
  Matched:       902 (99.9% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0799%  (OK)
  -> excellent

validation/ies-probe-06-swing-pivot-sweep
  Profile:       strict
  TV trades:     706  (raw 706)
  Engine trades: 706  (raw 706)
  Matched:       706 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0786%  (OK)
  -> excellent

validation/ies-probe-07-bars-since-trade-cooldown
  Profile:       strict
  TV trades:     727  (raw 727)
  Engine trades: 727  (raw 727)
  Matched:       727 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0838%  (OK)
  -> excellent

validation/ies-probe-08-equity-feedback-sizing
  Profile:       strict
  TV trades:     79  (raw 79)
  Engine trades: 79  (raw 79)
  Matched:       79 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0005%  (OK)
  PnL         p90 delta:   0.0925%  (OK)
  -> excellent

validation/mtf-probe-01-htf-close-roll
  Profile:       strict
  TV trades:     8775  (raw 8775)
  Engine trades: 8775  (raw 8775)
  Matched:       8775 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0653%  (OK)
  -> excellent

validation/mtf-probe-02-dual-tf-closes
  Profile:       strict
  TV trades:     736  (raw 736)
  Engine trades: 736  (raw 736)
  Matched:       736 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0652%  (OK)
  -> excellent

validation/mtf-probe-03-htf-sma
  Profile:       strict
  TV trades:     540  (raw 540)
  Engine trades: 540  (raw 540)
  Matched:       540 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0767%  (OK)
  -> excellent

validation/mtf-probe-04-daily-prev-high
  Profile:       strict
  TV trades:     147  (raw 147)
  Engine trades: 147  (raw 147)
  Matched:       147 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0687%  (OK)
  -> excellent

validation/mtf-probe-05-htf-rsi
  Profile:       strict
  TV trades:     496  (raw 496)
  Engine trades: 496  (raw 496)
  Matched:       496 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0778%  (OK)
  -> excellent

validation/mtf-probe-06-htf-volume-sum
  Profile:       strict
  TV trades:     584  (raw 584)
  Engine trades: 584  (raw 584)
  Matched:       584 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0885%  (OK)
  -> excellent

validation/mtf-probe-07-triple-tf-close-confluence
  Profile:       strict
  TV trades:     48  (raw 48)
  Engine trades: 48  (raw 48)
  Matched:       48 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0910%  (OK)
  -> excellent

validation/mtf-probe-08-triple-tf-macd-hist-confluence
  Profile:       strict
  TV trades:     24  (raw 24)
  Engine trades: 24  (raw 24)
  Matched:       23 (95.8% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.1662%  (OK)
  -> excellent

validation/mtf-probe-09-htf-daily-ema-warmup
  Profile:       strict
  TV trades:     31  (raw 31)
  Engine trades: 31  (raw 31)
  Matched:       31 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0883%  (OK)
  -> excellent

validation/mtf-probe-10-htf-bracket-static
  Profile:       strict
  TV trades:     283  (raw 283)
  Engine trades: 283  (raw 283)
  Matched:       283 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0005%  (OK)
  PnL         p90 delta:   0.0919%  (OK)
  -> excellent

validation/mtf-probe-11-htf-bracket-manual-trail
  Profile:       strict
  TV trades:     345  (raw 345)
  Engine trades: 345  (raw 345)
  Matched:       345 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0005%  (OK)
  PnL         p90 delta:   0.1579%  (OK)
  -> excellent

validation/parity-probe-01-stop-limit-timing
  Profile:       strict
  TV trades:     778  (raw 778)
  Engine trades: 778  (raw 778)
  Matched:       778 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0005%  (OK)
  PnL         p90 delta:   0.1337%  (OK)
  -> excellent

validation/parity-probe-02-choch-bos-isolator
  Profile:       strict
  TV trades:     1026  (raw 1027)
  Engine trades: 1026  (raw 1026)
  Matched:       1026 (99.9% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0705%  (OK)
  -> excellent

validation/parity-probe-04-percent-of-equity-sizing
  Profile:       strict
  TV trades:     57  (raw 57)
  Engine trades: 57  (raw 57)
  Matched:       57 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0006%  (OK)
  -> excellent

validation/parity-probe-05-small-equity-fraction
  Profile:       strict
  TV trades:     57  (raw 57)
  Engine trades: 57  (raw 57)
  Matched:       57 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0698%  (OK)
  -> excellent

validation/parity-probe-06-edge-margin-sizing
  Profile:       strict
  TV trades:     57  (raw 57)
  Engine trades: 57  (raw 57)
  Matched:       57 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0700%  (OK)
  -> excellent

validation/pineforge-analyzer
  Profile:       strict
  TV trades:     147  (raw 147)
  Engine trades: 147  (raw 147)
  Matched:       147 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0687%  (OK)
  -> excellent

validation/udt-method-probe-01-scalar-return
  Profile:       strict
  TV trades:     507  (raw 507)
  Engine trades: 507  (raw 507)
  Matched:       507 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0848%  (OK)
  -> excellent

validation/udt-method-probe-02-mutating-self
  Profile:       strict
  TV trades:     2293  (raw 2293)
  Engine trades: 2293  (raw 2293)
  Matched:       2293 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0623%  (OK)
  -> excellent

validation/udt-method-probe-03-extra-args
  Profile:       strict
  TV trades:     2504  (raw 2504)
  Engine trades: 2504  (raw 2504)
  Matched:       2504 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0000%  (OK)
  -> excellent

validation/udt-method-probe-04-default-param
  Profile:       strict
  TV trades:     417  (raw 417)
  Engine trades: 417  (raw 417)
  Matched:       417 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0874%  (OK)
  -> excellent

validation/udt-method-probe-05-in-conditional
  Profile:       strict
  TV trades:     402  (raw 402)
  Engine trades: 402  (raw 402)
  Matched:       402 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0812%  (OK)
  -> excellent

validation/udt-method-probe-06-in-switch
  Profile:       strict
  TV trades:     402  (raw 402)
  Engine trades: 402  (raw 402)
  Matched:       402 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0812%  (OK)
  -> excellent

validation/udt-method-probe-07-in-for-loop
  Profile:       strict
  TV trades:     1660  (raw 1660)
  Engine trades: 1660  (raw 1660)
  Matched:       1660 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0755%  (OK)
  -> excellent

validation/udt-method-probe-08-in-while-loop
  Profile:       strict
  TV trades:     306  (raw 306)
  Engine trades: 306  (raw 306)
  Matched:       306 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0845%  (OK)
  -> excellent

validation/udt-method-probe-09-uses-math-funcs
  Profile:       strict
  TV trades:     789  (raw 789)
  Engine trades: 789  (raw 789)
  Matched:       789 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0845%  (OK)
  -> excellent

validation/udt-method-probe-10-uses-na-nz
  Profile:       strict
  TV trades:     695  (raw 695)
  Engine trades: 695  (raw 695)
  Matched:       695 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0828%  (OK)
  -> excellent

validation/udt-method-probe-11-uses-history-of-globals
  Profile:       strict
  TV trades:     131  (raw 131)
  Engine trades: 131  (raw 131)
  Matched:       131 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.1028%  (OK)
  -> excellent

validation/udt-method-probe-12-receives-ta-result
  Profile:       strict
  TV trades:     2676  (raw 2676)
  Engine trades: 2676  (raw 2676)
  Matched:       2676 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0753%  (OK)
  -> excellent

validation/udt-method-probe-13-reads-strategy-state
  Profile:       strict
  TV trades:     705  (raw 705)
  Engine trades: 705  (raw 705)
  Matched:       705 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0848%  (OK)
  -> excellent

validation/udt-method-probe-14-drives-strategy-entry
  Profile:       strict
  TV trades:     1635  (raw 1635)
  Engine trades: 1635  (raw 1635)
  Matched:       1635 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0828%  (OK)
  -> excellent

validation/udt-method-probe-15-computes-exit-prices
  Profile:       strict
  TV trades:     704  (raw 704)
  Engine trades: 704  (raw 704)
  Matched:       704 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0005%  (OK)
  PnL         p90 delta:   0.1084%  (OK)
  -> excellent

validation/udt-method-probe-16-var-instance-streak
  Profile:       strict
  TV trades:     1007  (raw 1007)
  Engine trades: 1007  (raw 1007)
  Matched:       1007 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0557%  (OK)
  -> excellent

validation/udt-method-probe-17-tuple-return-destructure
  Profile:       strict
  TV trades:     1095  (raw 1095)
  Engine trades: 1095  (raw 1095)
  Matched:       1095 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0005%  (OK)
  PnL         p90 delta:   0.1076%  (OK)
  -> excellent

validation/udt-method-probe-18-method-calls-method
  Profile:       strict
  TV trades:     173  (raw 173)
  Engine trades: 173  (raw 173)
  Matched:       173 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0713%  (OK)
  -> excellent

validation/udt-method-probe-19-array-of-udt-method
  Profile:       strict
  TV trades:     868  (raw 868)
  Engine trades: 868  (raw 868)
  Matched:       868 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0828%  (OK)
  -> excellent

validation/udt-method-probe-20-udt-return-from-func
  Profile:       strict
  TV trades:     132  (raw 132)
  Engine trades: 132  (raw 132)
  Matched:       132 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0705%  (OK)
  -> excellent

validation/udt-method-probe-21-windowed-method-chain
  Profile:       strict
  TV trades:     173  (raw 173)
  Engine trades: 173  (raw 173)
  Matched:       173 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0713%  (OK)
  -> excellent

validation/vcp-probe-01-pivot-strength-5
  Profile:       strict
  TV trades:     1937  (raw 1937)
  Engine trades: 1937  (raw 1937)
  Matched:       1937 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0773%  (OK)
  -> excellent

validation/vcp-probe-02-fvg-zones
  Profile:       strict
  TV trades:     11800  (raw 11800)
  Engine trades: 11851  (raw 11851)
  Matched:       11797 (100.0% of TV)
  Count delta:             0.4303%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0584%  (OK)
  -> excellent

validation/vcp-probe-03-rsi-smooth-divergence
  Profile:       strict
  TV trades:     142  (raw 142)
  Engine trades: 142  (raw 142)
  Matched:       142 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0807%  (OK)
  -> excellent

validation/vcp-probe-04-vol-zscore-anomaly
  Profile:       strict
  TV trades:     591  (raw 591)
  Engine trades: 591  (raw 591)
  Matched:       591 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0739%  (OK)
  -> excellent

validation/vcp-probe-05-cumulative-volume-delta
  Profile:       strict
  TV trades:     3119  (raw 3119)
  Engine trades: 3119  (raw 3119)
  Matched:       3119 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0761%  (OK)
  -> excellent

validation/vcp-probe-06-adx-volatility-regime
  Profile:       strict
  TV trades:     636  (raw 636)
  Engine trades: 637  (raw 637)
  Matched:       636 (100.0% of TV)
  Count delta:             0.1570%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0793%  (OK)
  -> excellent

validation/vcp-probe-07-session-tz-newyork
  Profile:       strict
  TV trades:     396  (raw 396)
  Engine trades: 396  (raw 396)
  Matched:       396 (100.0% of TV)
  Count delta:             0.0000%  (OK)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   0.0752%  (OK)
  -> excellent

parity-anomalies/equity-mirror
  Profile:       strict
  TV trades:     24  (raw 24)
  Engine trades: 25  (raw 25)
  Matched:       13 (54.2% of TV)
  Count delta:             4.0000%  (X)
  Entry-price p90 delta:   0.0000%  (OK)
  Exit-price  p90 delta:   0.0000%  (OK)
  PnL         p90 delta:   2.3202%  (X)
  -> weak


Verified 168 strategies — excellent=165, strong=2, moderate=0, weak=1, minimal=0, missing=0

Below strong tier:
  parity-anomalies/equity-mirror
```

