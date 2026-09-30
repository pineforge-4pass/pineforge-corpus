# special-validation — probes outside the corpus sweep

Probes here use **non-default instruments** (licensed OHLCV that cannot be
committed — exchange ToS) or **document an engine limitation**, so they are kept
out of `scripts/run_corpus.sh` (the `corpus/*/*/` glob and `verify_corpus.py
--all` never descend into the categorized `<category>/<probe>/` layout). Build +
run them on demand with `bash special-validation/build_specials.sh` after
regenerating each probe's local data per its README.

The engine's parity gate does not run these probes. Four have a TradingView
result, recorded when they were added: SPX on 2026-06-01 (corpus `1f7f6f9`), and
AAPL, ES and EURUSD on 2026-06-03 (corpus `07084e4`; engine `de127185` records
the AAPL and ES trade counts). The monthly and leverage probes have no
TradingView tape and were never graded. No later TradingView result is
recorded for any of them: their `generated.cpp` files were re-emitted since,
last on 2026-09-25 and 2026-09-26, and build against the engine, and engine
`0d76a099`, which re-pinned the corpus after the 2026-09-25 re-emission, notes
that no probe here has a TradingView tape to grade against. Except for the SPX probe's, the
probe READMEs were written before those first runs and before the engine
changes named below. For what the engine does today with the features these
probes exercise, the reference is the engine's
[`docs/coverage.md`](https://github.com/pineforge-4pass/pineforge-engine/blob/main/docs/coverage.md)
(checked at `main` `6b45f510`).

| Category | Probe | Symbol | Status |
|---|---|---|---|
| us-equity | `us-equity-exchange-tz-intraday-cap-01` | SP:SPX 5m | excellent on 2026-06-01 (920 / 920 trades; intraday-cap rollover on a non-UTC chart) |
| us-equity | `symbol-equity-rth-gaps-aapl-01` | BATS:AAPL 5m | excellent on 2026-06-03 (82 / 82 trades; RTH + overnight gaps + DST) |
| futures | `symbol-futures-pointvalue-es-01` | CME_MINI:ES1! 5m | excellent on 2026-06-03 (550 / 550 trades; point value 50, 0.25 tick) |
| forex | `symbol-fx-5dp-eurusd-01` | OANDA:EURUSD 15m | strong on 2026-06-03 (TV-exact fills; sub-cent FX PnL artifact) |
| crypto-htf | `mtf-htf-monthly-ema-cross-01` | ETHUSDT.P 15m | not graded: no TradingView tape; TV parity needs years of monthly history (warmup depth). The engine has accepted a monthly `request.security` timeframe since engine `de127185` (2026-06-03) and aggregates it by calendar month; the rejection the probe's README describes predates that |
| crypto-leverage | `leverage-margin-call-perp-5x-01` | ETHUSDT.P 15m | not graded: no TradingView tape. Added to document a gap: the engine then had no in-position forced liquidation. The engine has booked TradingView's margin calls since engine `848cce35` (2026-06-30); see `NativeMarginModel` in coverage.md, "Position-sizing and commission" |

All licensed OHLCV / TV / engine trade lists are git-ignored (see `.gitignore`);
only `strategy.pine` / `generated.cpp` / `inputs.json` / `README` are tracked.

---
