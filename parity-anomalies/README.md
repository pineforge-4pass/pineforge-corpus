# corpus/parity-anomalies

> Probes that *deliberately* surface TradingView-side non-determinism.
> Engine output is correct (or below noise); the divergence comes from
> TV behaviour we cannot reverse-engineer from publicly observable
> Pine state. Excluded from the headline parity sweep so they don't
> mask as regressions.

These probes still ship the standard four-file shape — `strategy.pine`,
`tv_trades.csv`, `generated.cpp`, `engine_trades.csv` — and still build
into a `strategy.so` like every other corpus directory. The only
differences:

1. They are **not** included in `python scripts/verify_corpus.py --all`
   by default. The default `--all` covers `basic/`, `community/`, and
   `validation/` only (167 reference strategies). Run with
   `--include-anomalies` (or `--category parity-anomalies`) to inspect
   them explicitly.
2. Each one has a corresponding **anomaly write-up** in
   [`pineforge-utils/parity-anomalies/`](https://github.com/fullpass-4pass/pineforge-utils/tree/main/parity-anomalies)
   explaining what the probe does, what we observed, what we ruled out
   on the engine side, and the evidence that points at TV.
3. They are kept around because a probe that **reliably** trips a
   TV-side anomaly is itself a useful diagnostic — if a future engine
   change perturbs the trip pattern (e.g., the engine starts agreeing
   with TV on a bar where it previously disagreed), that's signal
   worth inspecting.

## Index

| Probe | Anomaly | Write-up |
|---|---|---|
| [`equity-mirror/`](equity-mirror/) | TV broker emulator's margin check is non-deterministic at the exact 1× equity boundary | [`tv-margin-boundary.md`](https://github.com/fullpass-4pass/pineforge-utils/blob/main/parity-anomalies/tv-margin-boundary.md) |

## Why segregate these?

Including them in the headline parity score would force one of two
unsatisfying choices:

- Mark them as regressions every run (false alarm — engine is correct).
- Slacken the strict thresholds globally to avoid the false alarm
  (loses signal on actual engine regressions).

Pulling them into a separate category lets `--all` keep its strict
thresholds and clean tier counts, while these probes remain runnable
and inspectable on demand.

## Running

```bash
# Inspect a single anomaly probe (verbose, ignores the --all gate)
python scripts/verify_corpus.py corpus/parity-anomalies/equity-mirror

# Run every anomaly probe in this category
python scripts/verify_corpus.py --category parity-anomalies

# Or fold them into the full sweep (will degrade the headline tier counts)
python scripts/verify_corpus.py --all --include-anomalies
```

## Adding a new anomaly probe

1. Reproduce the anomaly with a self-contained `strategy.pine` that
   touches the corner deterministically (no random inputs, no
   live-data drift).
2. Capture `tv_trades.csv` from a fresh TradingView export.
3. Build + run via `bash scripts/run_corpus.sh` to produce
   `engine_trades.csv` and `generated.cpp`.
4. Write the anomaly explainer in
   `pineforge-utils/parity-anomalies/<topic>.md` following the
   five-section template (`What you'll see` / `What we tried` /
   `What we observed` / `Why it doesn't matter in practice` /
   `What would change our mind`).
5. Add a row to the **Index** table above and to the index in the
   utils repo.
