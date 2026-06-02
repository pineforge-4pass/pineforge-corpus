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
