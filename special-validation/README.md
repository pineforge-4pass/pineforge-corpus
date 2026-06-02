# special-validation — probes outside the corpus sweep

Probes here use **non-default instruments** (licensed OHLCV that cannot be
committed — exchange ToS) or **document an engine limitation**, so they are kept
out of `scripts/run_corpus.sh` (the `corpus/*/*/` glob and `verify_corpus.py
--all` never descend into the categorized `<category>/<probe>/` layout). Build +
run them on demand with `bash special-validation/build_specials.sh` after
regenerating each probe's local data per its README.

| Category | Probe | Symbol | Status |
|---|---|---|---|
| us-equity | `symbol-equity-rth-gaps-aapl-01` | BATS:AAPL 5m | excellent (RTH + overnight gaps + DST) |
| futures | `symbol-futures-pointvalue-es-01` | CME_MINI:ES1! 5m | excellent (point value 50, 0.25 tick) |
| forex | `symbol-fx-5dp-eurusd-01` | OANDA:EURUSD 15m | strong (TV-exact fills; sub-cent FX PnL artifact) |
| crypto-htf | `mtf-htf-monthly-ema-cross-01` | ETHUSDT.P 15m | engine path enabled; TV parity needs years of monthly history (warmup-depth) |
| crypto-leverage | `leverage-margin-call-perp-5x-01` | ETHUSDT.P 15m | diverges — engine has no in-position forced liquidation |

All licensed OHLCV / TV / engine trade lists are git-ignored (see `.gitignore`);
only `strategy.pine` / `generated.cpp` / `inputs.json` / `README` are tracked.

---

# us-equity-exchange-tz-intraday-cap  (SPECIAL — in corpus, outside the sweep)

A standalone TV-parity probe that validates **intraday-cap day rollover on a
real US-equity session viewed on a non-UTC chart**. It lives in the corpus at
`corpus/special-validation/` — one level deep, so the `corpus/*/*/` glob in
`run_corpus.sh` (and the corpus CMake build, and `verify_corpus.py --all`)
does **not** pick it up. Different instrument, its own OHLCV feed, different
topic.

## ⚠️ Data is not committed (S&P/CBOE licensing)

Unlike the crypto probes, this uses **SPX** (S&P 500 index) data, which is
licensed market data that must not be redistributed in the public corpus. So
`ohlcv_SPX_5m.csv` and `tv_trades.csv` are **git-ignored** — you reproduce
them yourself from TradingView (steps below). Tracked here: `strategy.pine`,
`generated.cpp`, `inputs.json`, `build.sh`, `make_feed.py`, this README.

## Result (when reproduced)
`-> excellent`, **920 / 920 trades matched, 0% entry/exit/PnL delta** vs the
TradingView export. (TV raw 921 includes one trailing open position at the data
edge; 920 closed trades match.)

## What it proves
- Symbol `SP:SPX`, 5m, RTH `0930-1600` America/New_York, charted in **UTC+8**.
- In UTC+8 the RTH session spans ~21:30→04:00, **crossing chart-local midnight
  (00:00 Asia/Taipei = 16:00 UTC) mid-session** — exactly where the intraday-cap
  day-rollover timezone choice matters.
- `strategy.risk.max_intraday_filled_orders(5)` resets on the **chart** timezone
  (`engine_chart_timezone = Asia/Taipei`), and the engine matches TV bit-for-bit.
- => TradingView rolls the intraday cap on the **chart timezone**, not the
  exchange trading day — generalizing the probe-97 (crypto/UTC+8) finding to a
  real US-equity session. Confirms the deliberate decision to keep the intraday
  gates on `chart_timezone_` (NOT `syminfo_.timezone`); an exchange-tz gate would
  have diverged here. Resolves the exchange-tz-validation item of engine #26.

---

## Reproduce on SPX (step by step)

### 1. Export the OHLCV bars from TradingView
TradingView caps a chart-data export at ~20k bars, so a year of 5m needs two
overlapping pulls.

1. Open a chart, symbol **`SP:SPX`**, timeframe **5m**.
2. Set the chart timezone (bottom-right clock) to **UTC+8** (e.g.
   `Asia/Taipei`). This must match `engine_chart_timezone` / `tv_trades_csv_tz`
   below.
3. Scroll left to load as much history as TV allows (~20k bars), then
   **menu → Export chart data…** → CSV. Save to `~/Downloads/`.
4. Scroll left again to load an **earlier** window (overlap is fine — dups are
   removed), export a second CSV.
   - Each export has columns `time,open,high,low,close,Volume`, `time` in unix
     **seconds**.

### 2. Build the engine feed
From this directory:
```bash
python make_feed.py ohlcv_SPX_5m.csv \
    ~/Downloads/"SP_SPX, 5_<a>.csv" ~/Downloads/"SP_SPX, 5_<b>.csv"
```
This merges → de-dupes by timestamp → sorts → converts seconds→ms and writes
`ohlcv_SPX_5m.csv` in engine format (`timestamp,open,high,low,close,volume`).
Reference feed used for the validated result: 20,365 unique bars,
2025-05-19 → 2026-05-29 UTC.

### 3. Export the trades from TradingView
1. Pine editor → paste `strategy.pine` → **Add to chart** (same SP:SPX 5m chart,
   same UTC+8 timezone).
2. **Strategy Tester → List of Trades → export icon → CSV** → save as
   `tv_trades.csv` in this directory.
3. Make sure every trade falls inside the OHLCV window from step 2 (scroll the
   chart so the strategy only trades over bars you exported).

### 4. Build, run, verify
```bash
# engine must be built first (build/lib/libpineforge.a):
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DPINEFORGE_BUILD_TESTS=ON && cmake --build build -j

./corpus/special-validation/build.sh
python scripts/run_strategy.py corpus/special-validation --so-name strategy.dylib
python scripts/verify_corpus.py corpus/special-validation
# expect: -> excellent
```

`inputs.json` already pins the tz wiring: `ohlcv_csv = ohlcv_SPX_5m.csv`,
`engine_chart_timezone = Asia/Taipei`, `tv_trades_csv_tz = utc_plus_8`,
syminfo session `0930-1600:23456` / timezone `America/New_York`. If you export
on a different chart tz, update all three to match.

### Regenerating generated.cpp (private codegen)
`generated.cpp` is committed so the probe builds without the transpiler. To
re-emit after a `strategy.pine` edit, run the private codegen's
`scripts/regen_corpus_cpp.py` (or `transpile(strategy.pine)`).
