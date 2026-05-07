# PineForge validation corpus

> **Open-source checkout:** this tree is **omitted** in public `git clone`
> unless you have access to the private submodule. Maintainers run
> `git submodule update --init corpus` after cloning. See the root
> `[CONTRIBUTING.md](../CONTRIBUTING.md)`.

**Legal (private repo):** see `[LEGAL.md](LEGAL.md)`.

A reproducibility kit for the parity claim in the project README.
For each of **162 reference strategies**, this directory ships:


| File                | Source               | Role                                                                                                                                                           |
| ------------------- | -------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `strategy.pine`     | hand-written         | The PineScript v6 source the strategy is defined by                                                                                                            |
| `tv_trades.csv`     | TradingView export   | The "List of Trades" CSV TradingView's broker emulator produced for `strategy.pine` running on the reference OHLCV                                             |
| `generated.cpp`     | PineForge transpiler | The raw C++ source the closed-source transpiler produced for `strategy.pine`. Compiles standalone against `libpineforge.a`.                                    |
| `engine_trades.csv` | PineForge            | The trade list PineForge's compiled `.so` produced for the same script + same OHLCV, in TradingView's row-and-column format for direct line-by-line comparison |


The comparison OHLCV — **ETH/USDT-USDT 15-minute, 36,361 bars** — lives at
`[data/ohlcv_ETH-USDT-USDT_15m.csv](data/ohlcv_ETH-USDT-USDT_15m.csv)`.
The default harness uses
`[data/ohlcv_ETH-USDT-USDT_15m_warmup6m.csv](data/ohlcv_ETH-USDT-USDT_15m_warmup6m.csv)`
when present, adding six months of Binance USDT-M futures warmup bars before
the comparison window so TA, pivot, MTF, and equity-feedback state starts
closer to TradingView's chart state.

## Layout

```
corpus/
├── data/
│   └── ohlcv_ETH-USDT-USDT_15m.csv   1.9 MB — the reference bar feed
├── basic/                             9 strategies  — small, focused tests
│   ├── greedy/
│   │   ├── strategy.pine              the original PineScript v6 source
│   │   ├── tv_trades.csv              TV "List of Trades" export
│   │   ├── generated.cpp              raw C++ from the PineForge transpiler
│   │   └── engine_trades.csv          PineForge's trade list (after compile + run)
│   ├── inside-bar/
│   ├── keltner/
│   └── ...
├── community/                         11 strategies — common community scripts
│   ├── 4ema_rsi/
│   ├── BOS_curv/
│   ├── IES/                           multi-timeframe, edge case
│   ├── KKB/
│   ├── LiquitySweep/
│   ├── MarketShift/
│   ├── VCP/                           multi-timeframe, edge case
│   ├── kanuck/
│   ├── scalping-strategy/
│   ├── scalping-wunder-bots/
│   └── trendmaster/                   multi-timeframe, edge case
└── validation/                        142 probes — narrowly scoped semantic tests
    ├── 01-macd-histogram/             ... 96-multi-cycle-pooc-cross-bar/
    ├── ies-probe-01-…/                8 probes shaking out IES regressions
    ├── mtf-probe-01-…/                11 probes for request.security() flows
    ├── udt-method-probe-01-…/         21 probes for UDT method semantics
    └── vcp-probe-01-…/                7 probes shaking out VCP regressions
```

## Where the numbers come from

The headline parity figure — **excellent=158, strong=4 across 162 strategies** — is
produced by comparing each `engine_trades.csv` against its `tv_trades.csv` under
the threshold profile documented in the parent project's
`reports/validation_detailed.md`:

- **Strict profile** (used by 154 of 162 strategies): trade-count delta < 1.0%, entry-price p90 delta < 0.01%, exit-price p90 delta < 0.01%, P&L p90 delta < 1.0%.
- **Production profile** (used by trail-heavy / path-dependent strategies): count delta < 2.0%, entry p90 < 0.01%, exit p90 < 0.05%, and P&L p90 < 100%.

A trade is counted as "matched" when the engine and TV agree on direction and
their entry and exit times fall within a 1-hour gating window (with a $3 entry-
price gate to defend against same-bar duplicates). The `excellent` label means
all four parity dimensions pass strict thresholds; `strong` means trades match
trade-by-trade but at least one dimension exceeds strict (still well below
non-strict tolerances).

## CSV format

Both `tv_trades.csv` and `engine_trades.csv` use TradingView's row layout:

- **Two rows per trade**, sharing the same `Trade #`. The exit row is emitted
before the entry row (TV convention, kept by PineForge for direct diff).
- **Reverse-chronological by trade number** (newest first).
- **Time format**: `YYYY-MM-DD HH:MM`. PineForge engine CSVs are UTC.
TradingView exports use the chart's wall-clock timezone; this corpus
defaults to UTC+8 unless a strategy `inputs.json` overrides
`tv_trades_csv_tz`.

`tv_trades.csv` (TradingView's actual export):

```
Trade #,Type,Date and time,Signal,Price USDT,Position size (qty),Position size (value),Net P&L USD,Net P&L %,Favorable excursion USD,Favorable excursion %,Adverse excursion USD,Adverse excursion %,Cumulative P&L USD,Cumulative P&L %
14,Exit long,2026-04-27 20:30,TPSL,2291.52,1,2291.52,1.82,0.0795,2.25,0.0982,0.16,0.007,...
14,Entry long,2026-04-27 20:15,…,2289.7,1,2289.7,…
```

`engine_trades.csv` (PineForge's mirrored format, fewer columns —
PineForge does not currently track a TV-style "Signal" tag or
percent-of-position MFE/MAE):

```
Trade #,Type,Date and time,Price,Qty,Net PnL,Net PnL %,MFE,MAE,Cumulative PnL
14,Exit long,2026-04-27 20:30,2291.520000,1,1.820000,0.0795,2.250000,0.160000,-38.120000
14,Entry long,2026-04-27 20:15,2289.700000,1,1.820000,0.0795,2.250000,0.160000,-38.120000
```

`Net PnL` and `Net PnL %` are per-trade. `Cumulative PnL` is engine-side
running total (computed during CSV emission, not stored on the trade record).
`MFE` and `MAE` are dollars per unit qty, mirroring TV's "Favorable / Adverse
excursion USD" (PineForge does not currently emit the percent variant).

## Quick verification

Pick a strategy and diff the two CSVs visually — you should see TV's exit and
entry rows match PineForge's exit and entry rows by `Trade #`, side, time, and
price within the parity thresholds.

For programmatic inspection, see
`[scripts/verify_corpus.py](../scripts/verify_corpus.py)` in the repo root. It
reads both CSVs, aligns trades by entry time + direction, applies common-window
edge trimming, honors strategy metadata such as alternate `tv_trades_csv`
files, and reports the canonical five-tier labels.

```bash
python scripts/verify_corpus.py corpus/basic/greedy
```

Sample output (a perfect match):

```
basic/greedy
  TV trades:     14
  Engine trades: 14
  Matched:       14
  Entry-price max delta:  0.000000  (strict OK)
  Exit-price max delta:   0.000000  (strict OK)
  PnL max delta:          0.000000  (strict OK)
  -> excellent (Y/Y/Y/Y)
```

To run all 162 strategies:

```bash
python scripts/verify_corpus.py --all
```

## Reproducing the engine output

You can regenerate every `engine_trades.csv` from this repository alone —
no external tooling required. The closed-source transpiler is run *once*
upstream to produce each `generated.cpp`; everything downstream of that
file (compile, run, CSV emission, parity check) is open and shipped here.

The full pipeline is one shell command:

```bash
bash scripts/run_corpus.sh
```

That script:

1. Configures CMake with `-DPINEFORGE_BUILD_CORPUS_STRATEGIES=ON`.
2. Builds `libpineforge.a` plus 162 `strategy.so` files (one per
  `corpus/<cat>/<name>/generated.cpp`), in parallel via
   `cmake --build build --target corpus_strategies`.
3. Loads each `strategy.so` through `scripts/run_strategy.py`, runs it
  against `corpus/data/ohlcv_ETH-USDT-USDT_15m.csv`, and writes the
   resulting trade list to `corpus/<cat>/<name>/engine_trades.csv`.
4. Calls `scripts/verify_corpus.py --all --quiet` as a lightweight inspection
  summary. Drift from that helper is warned but does not fail the 162-strategy
   reproducibility run.

Subcommands for finer control:

```bash
# Build everything (no run, no verify)
cmake -B build -DPINEFORGE_BUILD_CORPUS_STRATEGIES=ON
cmake --build build --target corpus_strategies -j

# Run one strategy through the harness
python scripts/run_strategy.py corpus/basic/greedy

# Verify one strategy
python scripts/verify_corpus.py corpus/basic/greedy --show-diffs 5

# Verify everything
python scripts/verify_corpus.py --all
```

The `engine_trades.csv` files shipped in git are the canonical output of
running the same pipeline on the maintainer's machine. Re-running
`scripts/run_corpus.sh` should reproduce them byte-for-byte (the engine
is deterministic given a fixed bar feed, fixed `generated.cpp`, and a
fixed runtime build). If you find a strategy whose regenerated output
disagrees with the committed CSV, that is a bug — please open an issue.

## Provenance and licensing

- `**strategy.pine` files** are the hand-written contributions of the
PineForge maintainers and are distributed under Apache 2.0 along with
the rest of this repository. The `community/`* strategies are
re-implementations of common public PineScript patterns; if you
recognise yours and want attribution, please open an issue.
- `**tv_trades.csv` files** are exports of running the corresponding
`strategy.pine` on TradingView. The trade lists are derived from
the user's own backtest activity on the platform and are included
here as factual records for parity verification, not as redistributions
of TradingView intellectual property.
- `**generated.cpp` files** are mechanically produced from the
corresponding `strategy.pine` by the PineForge transpiler. They are
derivative works of the user-authored PineScripts in this repo and
are distributed under Apache 2.0.
- `**engine_trades.csv` files** are computed by PineForge from the
PineScripts and OHLCV in this repository. Apache 2.0.
- **OHLCV** is public market data from a USDT perpetual ETH futures
feed. Public market data is not copyrightable in the US/EU.

