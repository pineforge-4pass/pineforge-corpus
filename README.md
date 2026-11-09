# PineForge validation corpus

The corpus is PineForge's reproducibility kit for the parity claim in the
[engine README](https://github.com/pineforge-4pass/pineforge-engine#readme). Every
probe is a hand-written, clean-room PineScript v6 strategy paired with
TradingView's exported trade list and PineForge's own trade list, so a third party
can diff the two CSVs and confirm engine behaviour matches TradingView on the same
bar feed.

## Headline parity

Measured by the engine's parity gate
([`scripts/check_corpus_parity.sh`](https://github.com/pineforge-4pass/pineforge-engine/blob/main/scripts/check_corpus_parity.sh)),
which builds every probe's `generated.cpp`, re-runs it, and grades the result
against `tv_trades.csv` with `scripts/verify_corpus.py`. At engine `35db01c`
(2026-09-29; engine v1.0.0, released 2026-09-30, includes that commit):

- **312** probes under `validation/`, in 33 categories.
- **311** excellent (exact trade-count parity and every other gated dimension
  within the resolved thresholds, and, where TradingView shows two or more distinct entry Signals at one exact time, price and direction, the engine shows as many distinct entries there (distinct-entry identity); see [Parity thresholds](#parity-thresholds)).
  The gate pins its headline verbatim:
  `Verified 312 strategies — excellent=311, strong=0, moderate=0, weak=0, minimal=0, anomaly=1, engine_only=0, missing=0`.
- **1** declared anomaly — `anomaly-equity-mirror-strategy-equity-01` declares
  `expected_tier: anomaly` in its `inputs.json`, so the gate reports it as
  `anomaly`, not as a failure. Its `notes` field records the original diagnosis
  (TradingView broker behaviour at the exact 1× equity margin boundary); the
  engine README on `main` reports the probe as matching on the maintainers'
  baseline, so the diagnosis is under review.
- **0** strong / moderate / weak / minimal / missing.
- **431,244** TradingView trades in the committed `tv_trades.csv` files (311
  probes; `analyzer-self-test-multi-mode-01` keeps its four per-mode TradingView
  tapes in `trades-*.csv`).

**The committed tapes are the ones `main` carries, and the engine reproduces them.** All 312 `engine_trades.csv` tapes have the 12-column header that ends `Engine range-end`. 311 of them are unchanged since `e7a0f8f` (2026-10-02); the tape of `composite-4emarsi-quad-ema-stack-01` was rewritten by `adef1c6` (2026-11-03), after that probe's series origin changed. The `generated.cpp` files are what codegen 1.7.0 (`89b48cb6e2fe`) emits, the files that the engine pins. The engine writes the committed tapes from them: 311 tapes match the sha256 that the engine's `scripts/corpus_parity_baseline.txt` pins at `e1d29cb95` (the gate hashes the file's text with its CRLF line endings read as LF, so `shasum -a 256` on the file gives another value), and the tape of the quad-ema probe is the file that the engine's `scripts/run_strategy.py` writes on this tree. `run_manifest.json` records the sha256 of the 312 tapes, the engine commit that reproduces them (`e1d29cb95`), the sha256 of the grading rubric `scripts/verify_corpus.py` at that commit, and when the digests were written.

`validation_report.md` is the report of the 2026-11-03 run (engine `d129cc03`, corpus `7ad36e2`); it gives the headline above, and so does `scripts/regen_validation_report.py` over the committed tapes. `validation_report.html` and `validation_report.pdf` were last rendered on 2026-10-02 (`a35c7c4`); the header of the HTML names engine `5718c5dc` and corpus `d53f949`. The engine's gate re-runs every probe and compares each fresh tape's sha256 with the baseline, not with the committed file. The engine README states the same headline: 311 excellent and 1 declared anomaly.

Each statement can be checked from an engine checkout whose `corpus/` is this repository (`<engine>` is that checkout; `adef1c6` is the head of `main` at the merge):

```sh
cd <engine>
# no tape differs from main's; 311 are unchanged since e7a0f8f (one file changed: the quad-ema tape)
git -C corpus diff --stat adef1c6 HEAD -- 'validation/*/engine_trades.csv'
git -C corpus diff --stat e7a0f8f adef1c6 -- 'validation/*/engine_trades.csv'
# one header line, 312 tapes
head -qn1 corpus/validation/*/engine_trades.csv | tr -d '\r' | sort | uniq -c
# 311 of 312 tapes match the baseline that the engine pins at e1d29cb95 (run it in a checkout of that commit)
python3 -c "import hashlib,pathlib;b=[l.split() for l in open('scripts/corpus_parity_baseline.txt') if l.strip() and l[0]!='#'];print(sum(hashlib.sha256(pathlib.Path('corpus',p).read_text().encode()).hexdigest()==d for d,p in b),'of',len(b))"
# the recorded digests, engine commit and file digests are consistent: ends with RESULT valid
python3 scripts/validate_corpus_sha.py --corpus corpus --engine . --records-only
# the recorded rubric digest is the sha256 of scripts/verify_corpus.py at the recorded engine commit: both lines show the same 64 hex digits
git show e1d29cb95:scripts/verify_corpus.py | shasum -a 256
grep rubricSha256 corpus/run_manifest.json
# the report of 2026-11-03, the renderings of 2026-10-02, and the headline over the committed tapes
sed -n 3p corpus/validation_report.md
git -C corpus log -1 --format='%h %as' -- validation_report.html validation_report.pdf
grep -a -A1 'Generated' corpus/validation_report.html | head -n 3
python3 scripts/regen_validation_report.py --output regenerated_report.md
# the quad-ema tape is what the engine writes: build the probe, run it, and the tracked file does not change
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DPINEFORGE_BUILD_TESTS=OFF -DPINEFORGE_BUILD_TUTORIAL=OFF -DPINEFORGE_BUILD_CORPUS_STRATEGIES=ON
cmake --build build --target strategy_validation_composite_4emarsi_quad_ema_stack_01
PINEFORGE_REPORT_SINK=0 python3 scripts/run_strategy.py corpus/validation/composite-4emarsi-quad-ema-stack-01 --so-name strategy.so
git -C corpus status --short
```

Beyond this public corpus, the engine is graded on a much larger private set of
community-shared TradingView scripts (private under TradingView's Terms of Service
— not redistributable). **Measured (UTC promotion date: <!-- pf:scoreboard.date -->2026-10-06<!-- /pf -->)** on main engine <!-- pf:scoreboard.engineCommit|short-code -->`59082e69`<!-- /pf --> with codegen-oss <!-- pf:scoreboard.codegenCommit|short-code -->`1e6eb833`<!-- /pf --> (baseline <!-- pf:scoreboard.id|code -->`pineforge-parity-baseline-20261006-codegen-1e6eb833`<!-- /pf -->, snapshot <!-- pf:scoreboard.snapshotSha256|short-code -->`8db963b7`<!-- /pf -->) covers <!-- pf:scoreboard.population|int -->8,006<!-- /pf --> probes: <!-- pf:scoreboard.graded|int -->7,989<!-- /pf --> are graded — **<!-- pf:scoreboard.excellent|int -->7,983<!-- /pf --> excellent (<!-- pf:scoreboard.excellentPct|pct -->99.92<!-- /pf --> %)
and <!-- pf:scoreboard.strong|int -->6<!-- /pf --> strong (<!-- pf:scoreboard.strongPct|pct -->0.08<!-- /pf --> %), <!-- pf:scoreboard.belowStrong|int -->0<!-- /pf --> below strong** —
and <!-- pf:scoreboard.anomaliesExcluded|int -->17<!-- /pf --> are excluded as TradingView-side defects. <!-- pf:scoreboard.corpusProbes|int -->309<!-- /pf --> of
this corpus's 312 probes belong to that population, <!-- pf:scoreboard.scopes.corpus.excellent|int -->309<!-- /pf --> excellent there
(the three left out are `analyzer-self-test-multi-mode-01`,
`bracket-rivet-calc-on-fill-01` and `order-switchback-all-in-reversal-01`).

Release **1.3.0 grades <!-- pf:releases[1.3.0].scoreboard.excellent|int -->7,982<!-- /pf --> excellent / <!-- pf:releases[1.3.0].scoreboard.strong|int -->7<!-- /pf --> strong until the next release**, on <!-- pf:releases[1.3.0].scoreboard.graded|int -->7,989<!-- /pf --> probes (baseline <!-- pf:releases[1.3.0].scoreboard.id|code -->`pineforge-parity-baseline-20261006-engine-7a1f01c0`<!-- /pf -->, <!-- pf:releases[1.3.0].scoreboard.date -->2026-10-06<!-- /pf -->). A main scoreboard advance does not change release results.

The quantities above render from the public [facts tokens](https://github.com/pineforge-4pass/pineforge-release/blob/main/facts/facts.json). Maintain them with `lab facts render --repo . --facts <local facts file or pinned raw URL>`; `lab facts check` with the same inputs reports drift. Grades are registry-derived; the authored-script and closed-trade inventory is explicitly sourced to a historical public README for the identical population, not to registry row or slug totals.

The 60 PineForge-owned additions introduced across the 282- and 312-probe
expansions are all independently authored, source-bound to actual
private-editor TradingView exports, and graded excellent by the native Corpus
verifier. No non-excellent candidate from either expansion is present in this
public tree. Their ownership, coverage tags, artifact hashes, and native
results are bound in [`owned_strategies.json`](owned_strategies.json); the engine's
corpus validator checks every recorded hash against the committed file.

[`validation_report.md`](validation_report.md) (rendered as
`validation_report.html` and `validation_report.pdf`) is the per-probe disposition
table the engine's sweep emits. The `.md` copy committed here is the report of the
2026-11-03 run; the `.html` and `.pdf` renderings are the 2026-10-02 ones (see above).

## Artifact tuple

Each probe directory ships this core artifact tuple in git. 122 of the 312 probes
also carry an `inputs.json` (probe metadata: the timezone of the TradingView
export, timeframes and input overrides, and, for the declared anomaly, its
`expected_tier`); the other 190 have none.

| File                | Source                       | Role                                                       |
| ------------------- | ---------------------------- | ---------------------------------------------------------- |
| `strategy.pine`     | hand-written                 | PineScript v6 source                                       |
| `generated.cpp`     | pineforge-codegen transpiler | C++ output of the transpiler over `strategy.pine`          |
| `tv_trades.csv`     | TradingView export           | TV broker emulator's trade list for `strategy.pine`        |
| `engine_trades.csv` | PineForge                    | Engine's trade list for the same script (TV-format CSV)    |

The one exception is `analyzer-self-test-multi-mode-01`: it has four TradingView
tapes, `trades-<mode>.csv`, and its `inputs.json` names the one to grade
(`tv_trades_csv`).

`generated.cpp` is the transpiler output of our own clean-room
PineScript and ships under the same Apache-2.0 license as
`strategy.pine`. It is included in-tree so public users can rebuild
without needing access to the separate, source-available `pineforge-codegen`
transpiler — in the engine checkout, configured with
`-DPINEFORGE_BUILD_CORPUS_STRATEGIES=ON` and this repository mounted as `corpus/`,
`cmake --build build --target corpus_strategies` compiles
each `generated.cpp` into a per-strategy shared library. The compiled
`strategy.dylib` / `.so` / `.dll` are platform-specific build artefacts
and remain ignored.

## Reference OHLCV

The corpus ships exactly **one** feed (stored via Git LFS, so install `git-lfs`
before cloning or run `git lfs pull`):

- `data/ohlcv_ETH-USDT-USDT_1m.csv` — Binance ETH-USDT-USDT perp
  1-minute bars spanning more than six years from 2020-01-01 00:00 UTC
  through 2026-05-04 15:14 UTC. Its SHA-256 is
  `db8c1332da093008cfbd063e05db0b33fe8f7fd35d78cf058a366519eb9f6cc5`.
  The pinned file contains 32 forward gaps totalling 175 missing minutes;
  it has no duplicate or non-monotonic timestamps and is not described as
  complete or gap-free. The deep history
  matches the depth TradingView's own chart computes warmup over, so
  TA, MTF, pivot, and equity-feedback state starts where TV's does.

Every other feed the harnesses consume is derived deterministically
from it into `data/derived/` (gitignored) by the engine repo's
`scripts/derive_corpus_feeds.py` (invoked automatically by
`scripts/run_corpus.sh` and `scripts/run_strategy.py`):

- `data/derived/ohlcv_ETH-USDT-USDT_15m.csv` — 900s resample
  (open=first, high=max, low=min, close=last, volume=sum), the default
  15m chart feed.
- `data/derived/ohlcv_ETH-USDT-USDT_15m_window.csv` — comparison-window
  slice of the above, used by cold-start probes and as the harness's
  window-bounds reference.

`ltf-*` probes consume the committed 1m feed directly (engine-side
aggregation to the 15m chart); `magnifier-*` probes synthesize intrabar
ticks from chart bars and need no extra feed.

## Layout

Paths are relative to this repository's root, which the engine checks out as its
`corpus/` submodule.

```
.
├── validation/                312 probes — surface-driven probe family
│   ├── ta-*                    73 probes — TA built-in math (rsi, macd, sma, ...)
│   ├── composite-*             53 probes — multi-surface integration (community-style)
│   ├── order-*                 48 probes — entry/exit/cancel placement
│   ├── udt-*                   28 probes — user-defined types + methods
│   ├── mtf-*                   19 probes — request.security regular HTF
│   ├── bracket-*               17 probes — TP/SL via strategy.exit / strategy.order
│   ├── matrix-*                 9 probes — matrix<T> typed/generic
│   ├── analyzer-*               8 probes — engine analyzer / parity isolation
│   ├── drawing-*                6 probes — drawing objects as data
│   ├── pyramid-*                5 probes — pyramiding=N
│   ├── session-*                4 probes — session() / TZ / DST
│   ├── barstate-*               3 probes — barstate.* checks
│   ├── cap-*                    3 probes — intraday risk caps
│   ├── magnifier-*              3 probes — bar_magnifier sub-bar walks
│   ├── na-*                     3 probes — na propagation
│   ├── oca-*                    3 probes — OCA group cancel/reduce/none
│   ├── array-*                  2 probes — array lifecycle and iteration
│   ├── input-*                  2 probes — input.source runtime override / subscript
│   ├── ltf-*                    2 probes — request.security_lower_tf arrays
│   ├── map-*                    2 probes — map collection behavior
│   ├── math-*                   2 probes — explicit math and loop behavior
│   ├── recompute-*              2 probes — calc_on_every_tick / TA recompute
│   ├── stats-*                  2 probes — performance stats / reporting
│   ├── syntax-*                 2 probes — scalar and switch semantics
│   ├── timeframe-*              2 probes — chart/input timeframe handling
│   ├── vwap-*                   2 probes — VWAP band pricing / fills
│   ├── bands-*                  1 probe  — statistical-band behavior
│   ├── calendar-*               1 probe  — calendar filters
│   ├── control-*                1 probe  — explicit control-flow behavior
│   ├── enum-*                   1 probe  — enum type and input selection
│   ├── risk-*                   1 probe  — risk gates / limits
│   ├── volume-*                 1 probe  — volume-flow behavior
│   └── anomaly-*                1 probe  — declared anomaly (expected_tier: anomaly)
├── draft-probes/               staged probes pending TV capture (excluded from sweep; own README)
├── special-validation/         separate-instrument probes (feeds git-ignored; own README + build_specials.sh)
├── data/                       reference OHLCV (committed 1m feed; 15m derived locally into data/derived/)
├── LICENSE                     Apache-2.0
├── NOTICE                      attribution
├── LEGAL.md                    provenance / trademarks
├── README.md                   this file
├── CLAUDE.md                   guardrails for AI agents working in this repository
├── CMakeLists.txt              per-strategy .so build glob (used from the engine checkout)
├── .gitignore                  ignores compiled strategy libs, data/derived/, .omc/
├── .gitattributes              Git LFS for data/ohlcv_*.csv; no line-ending conversion for the tapes
├── owned_strategies.json       ownership, coverage, hashes, and Excellent evidence for 60 additions
├── run_manifest.json           sha256 of the 312 committed engine_trades.csv tapes, the engine commit that reproduces them
│                               (e1d29cb95), the sha256 of its grading rubric scripts/verify_corpus.py, and when the digests were written
├── validation_report.md        per-probe parity disposition: the report of the 2026-11-03 run (engine d129cc03, corpus 7ad36e2)
└── validation_report.{html,pdf}   renderings of the 2026-10-02 report (engine 5718c5dc, corpus d53f949)
```

Total: **312** probes.

## Naming convention

Every probe directory follows:

```
<category>-<descriptive-slug>-NN[a-z]?
```

- **`<category>`** — one of the 33 surface categories below. The category
  is the engine surface or PineScript feature the probe is built to
  exercise.
- **`<descriptive-slug>`** — kebab-case description of the specific
  behaviour under test (e.g. `atr-trail-series-int-points`,
  `kalman-filter-1d`, `bb-kc-squeeze-release`).
- **`NN`** — two-digit sequence number, used to disambiguate when more
  than one probe lands on the same `(category, slug)` pair.
- **`[a-z]?`** — optional letter suffix, used **only** for documented
  A/B variant pairs that share the same numeric slot (e.g.
  `barstate-isconfirmed-magnifier-on-01a` vs
  `…-magnifier-off-01b`).

The 33 categories (with probe counts):

| Category    | Count | Surface exercised                                          |
| ----------- | ----: | ---------------------------------------------------------- |
| `ta`        |    73 | TA built-in math (rsi, macd, sma, hma, …)                  |
| `composite` |    53 | Multi-surface integration probes (community-style scripts) |
| `order`     |    48 | Entry/exit/cancel order placement                          |
| `udt`       |    28 | User-defined types + methods                               |
| `mtf`       |    19 | `request.security` regular HTF                             |
| `bracket`   |    17 | TP/SL via `strategy.exit` / `strategy.order`               |
| `matrix`    |     9 | `matrix<T>` typed and generic                              |
| `analyzer`  |     8 | Engine analyzer / parity isolation                         |
| `drawing`   |     6 | Drawing objects as data (`line`, `box`, `chart.point`)      |
| `pyramid`   |     5 | `pyramiding=N`                                             |
| `session`   |     4 | `session()` / TZ / DST                                     |
| `barstate`  |     3 | `barstate.*` checks                                        |
| `cap`       |     3 | Intraday risk caps                                         |
| `oca`       |     3 | OCA group cancel / reduce / none                           |
| `magnifier` |     3 | `bar_magnifier` sub-bar walks                              |
| `na`        |     3 | `na` propagation                                           |
| `array`     |     2 | Array lifecycle and iteration                              |
| `ltf`       |     2 | `request.security_lower_tf` arrays                         |
| `input`     |     2 | `input.source` runtime override / subscript                |
| `map`       |     2 | Map collection behavior                                    |
| `math`      |     2 | Explicit math and loop behavior                            |
| `recompute` |     2 | `calc_on_every_tick` / TA recompute                        |
| `stats`     |     2 | Performance stats / reporting                              |
| `syntax`    |     2 | Scalar and switch semantics                                |
| `timeframe` |     2 | Chart/input timeframe handling                             |
| `vwap`      |     2 | VWAP band pricing / fills                                  |
| `bands`     |     1 | Statistical-band behavior                                  |
| `calendar`  |     1 | Calendar filters                                           |
| `control`   |     1 | Explicit control-flow behavior                             |
| `enum`      |     1 | Enum type and input selection                              |
| `risk`      |     1 | risk gates / limits                                        |
| `volume`    |     1 | Volume-flow behavior                                       |
| `anomaly`   |     1 | `strategy.equity` mirror at 1× equity (declared anomaly)   |

## Where the numbers come from

The engine's parity gate runs the whole pipeline (build + run + grade across the
tree), and its stages are also available on their own:

```bash
JOBS=8 scripts/run_corpus.sh              # from the engine checkout: build, run, verify, write the report
```

That script:

1. Configures CMake with `-DPINEFORGE_BUILD_CORPUS_STRATEGIES=ON`.
2. Builds `libpineforge.a` plus one `strategy.so` per probe via
   `cmake --build build --target corpus_strategies`.
3. Loads each `strategy.so` through `scripts/run_strategy.py`, runs it
   against the 15m chart feed derived from
   `corpus/data/ohlcv_ETH-USDT-USDT_1m.csv`, and writes
   `engine_trades.csv` next to the probe.
4. Runs `scripts/verify_corpus.py --all` to produce the report.

`scripts/check_corpus_parity.sh` wraps it and adds the byte-identity check against
the pinned baseline; the engine's CI runs it nightly.

## Reproducing parity locally

No transpiler access required — `generated.cpp` ships in-tree.

```bash
# 1. Clone the engine and pull this corpus submodule (needs git-lfs, CMake and a C++17 compiler)
git clone https://github.com/pineforge-4pass/pineforge-engine.git
cd pineforge-engine
git lfs install
git submodule update --init corpus

# 2. Build all per-strategy .so files, run them, and check parity against the pinned baseline
JOBS=8 ./scripts/check_corpus_parity.sh            # all 312 probes
JOBS=8 ./scripts/check_corpus_parity.sh --subset   # 54 probes, a few minutes
```

The gate prints `TradingView parity holds` and exits 0 when every fresh tape hashes
to its pinned value and the verifier headline matches the one quoted above.

You need the engine repo, this corpus, and a C++17 compiler. The engine
is deterministic given a fixed bar feed, the shipped `generated.cpp`,
and a fixed runtime build. The submodule checkout is the corpus commit the engine
pins. A fresh run is judged against the engine's baseline
(`scripts/corpus_parity_baseline.txt`), which pins the tapes that the engine writes
from the committed `generated.cpp` files; the committed tapes are those tapes for
311 of the 312 probes (see above). If a fresh run disagrees with that baseline,
that is a bug — please open an issue.

## CSV format

`tv_trades.csv` and `engine_trades.csv` use TradingView's row layout:

- **Two rows per trade**, sharing the same `Trade #`. The exit row comes
  before the entry row (TV convention; PineForge mirrors it for direct diff).
- **Order by trade number differs**: `tv_trades.csv` is oldest first, and
  `engine_trades.csv` is newest first (reverse-chronological).
- **Time format**: `YYYY-MM-DD HH:MM`. Engine CSVs are UTC. TradingView
  exports use the chart's wall-clock timezone; this corpus defaults to
  UTC+8 unless a probe `inputs.json` overrides `tv_trades_csv_tz`.

`tv_trades.csv` (TradingView's actual export; the first column is headed
`Trade number` and the first line starts with a byte-order mark):

```
Trade number,Type,Date and time,Signal,Price USDT,Size (qty),Size (value),Net PnL USDT,Return %,Commission USDT,Favorable excursion USDT,...,Duration (bars)
1,Exit long,2025-04-03 06:00,Anvil Costed Bracket,1837.51,3.9396,7493.946516,-266.67853,-3.56,11.78640073,213.67694,...,21
1,Entry long,2025-04-03 00:45,Anvil Long,1902.21,3.9396,7493.946516,-266.67853,-3.56,11.78640073,213.67694,...,21
```

That 17-column header, ending with `Duration (bars)`, is the one in 305 of the 311
tapes; the six `drawing-*` tapes have 15 columns and end with `Cumulative PnL %`.

`engine_trades.csv` (PineForge's mirrored format, fewer columns —
PineForge does not currently emit TV's "Signal" tag or percent-of-position
excursions):

```
Trade #,Type,Date and time,Price,Qty,Net PnL,Net PnL %,Favorable excursion USD,Adverse excursion USD,Cumulative PnL,Engine entry incarnation,Engine range-end
325,Exit long,2026-05-04 00:00,2316.970000,3.13888382,-23.605034,-0.3240,107.077924,-29.902966,-2817.536822,,open
325,Entry long,2026-05-03 11:45,2320.780000,3.13888382,-23.605034,-0.3240,107.077924,-29.902966,-2817.536822,1756,
```

All 312 tapes committed here have those 12 columns. `Engine range-end` reads
`open` on the exit row of a position still open at the end of the range, and is
empty on every other row; 131 of the 312 tapes have such a row, always their
newest trade, like trade 325 of `analyzer-anvil-percent-costs-01` above. `Qty` is
printed to eight decimal places with trailing zeros trimmed.

`Net PnL` and `Net PnL %` are per-trade. `Cumulative PnL` is the
engine-side running total. The excursion columns use TV's names and sign
convention: favorable excursion is a non-negative total-USD run-up,
adverse excursion is a **negative** total-USD drawdown
(`(price diff) × qty`, summed over pyramid entries). Note this is the
export convention only — Pine's `strategy.*trades.max_drawdown`
accessors stay positive per the Pine v6 spec.

## Parity thresholds

The verifier (`scripts/verify_corpus.py` in the engine repo) applies one of two
threshold profiles per probe and emits a tier label:

### Profiles

| Dimension                  | STRICT  | PRODUCTION |
| -------------------------- | ------: | ---------: |
| Absolute trade-count parity | Δ == 0 |     Δ == 0 |
| Coverage (matched / all closed TV trades) | ≥99% | ≥99% |
| Entry-price p90 delta      |   0.01% |      0.01% |
| Exit-price p90 delta       |   0.01% |      0.05% |
| Per-trade P&L p90 delta    |   1.0%  |       100% |

PRODUCTION relaxes the exit-price tolerance (5×) and the per-trade P&L
tolerance (~100× — its 100% gate catches only catastrophic divergence)
to absorb sub-bar broker-side fill drift on trailing exits. The verifier
auto-selects PRODUCTION for probes whose `strategy.pine` uses any
`trail_*` parameter on `strategy.exit` (plain stop/limit exits stay
STRICT); an `inputs.json` `parity_profile` override wins over
auto-detection.

Excursion metrics (MAE/MFE) are **report-only diagnostics**, not gates:
they pin TV's excursion conventions (sign, total-USD scaling, exit-fill
inclusion) and flag regressions loudly (a sign error reads ~200%), but
same-bar stop/limit round-trips carry TV-side excursions sourced from
intrabar data that chart-TF OHLC cannot reproduce (the engine correctly
emits 0), so excursions can never be a pass/fail dimension.

A trade is "matched" when engine and TV agree on direction and entry/
exit times fall within a 1-hour gating window (plus a $3 entry-price
gate to defend against same-bar duplicates). The PnL p90 calc applies a
near-zero filter (`|tv_pnl| > $0.01`) to avoid div-by-near-zero blow-up
on TV's magnifier zero-PnL trades.

### Tier labels

| Tier          | Meaning |
| ------------- | ------- |
| `excellent`   | All gated dimensions (absolute count parity, coverage, entry, exit, P&L) pass the resolved profile, and, where TradingView shows two or more distinct entry Signals at one exact time, price and direction, the engine shows as many distinct entries there (distinct-entry identity). Bit-for-bit or within strict-profile thresholds. |
| `strong`      | Dimensions pass a relaxed envelope (count <6%, entry 10×, exit 50×, P&L 100%, coverage ≥95%) — close but not excellent. Used as a pass-with-caveat tier. |
| `moderate`    | Some dimensions exceed the strong envelope but trades still align meaningfully. Investigate. |
| `weak`        | Significant divergence. Real bug or probe-design issue. |
| `minimal`     | Probe produces zero engine trades or zero TV trades — nothing to compare. |
| `anomaly`     | Declared per probe via `inputs.json` `expected_tier`; applied only when the measured result is below excellent. The probe's `notes` field records the diagnosis it was declared on. Excluded from headline excellent count. Currently 1 probe (`anomaly-equity-mirror-strategy-equity-01`). |
| `engine_only` | Engine produces correct trades that intentionally diverge from TV (e.g., engine fires a bar TV's broker emulator silently drops). Documented per-probe via `inputs.json::validation_overrides::expect_tv_match: false` plus an `expect_tv_match_reason` write-up. Currently 0 probes. |
| `missing`     | Required artefact (TV CSV or engine CSV) absent. Should never appear in committed state. |

The `anomaly` and `engine_only` overrides only fire when the computed
tier would be below `excellent` — a future engine fix that lifts a
declared divergence to bit-for-bit match still reports as `excellent`,
not silently masked.

## Publishing posture

The corpus is published under **Apache-2.0**, matching the engine. Every
`strategy.pine` is a clean-room PineForge original — no third-party
PineScript is redistributed. TradingView trade-list CSVs are factual
records of running each script on TV's broker emulator, included only for
parity verification. OHLCV is public market data from Binance USDT-M
futures. See [`LEGAL.md`](LEGAL.md) for the full provenance and trademark
notes.

## File digest manifest

`file_digests.json` records the SHA-256 of every tracked corpus file except itself.
It records what the corpus held when it was written and detects any tracked file
change made without rewriting the manifest; each deliberate change is visible
in the manifest diff. It is not a signature and cannot prevent a deliberate rewrite.
Every change to a tracked file needs the manifest rewritten on a clean checkout:
run `python3 <engine checkout>/scripts/write_corpus_file_digests.py --corpus .`
after committing the changed files, then commit the manifest with the update.
The corpus workflow fails until the manifest matches every tracked file.
