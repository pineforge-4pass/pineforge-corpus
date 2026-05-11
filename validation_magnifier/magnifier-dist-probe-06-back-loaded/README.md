# Magnifier dist probe 06-back-loaded — BACK_LOADED

**Gap targeted**: X3 (Pine #22 + C++ #11) — magnifier 6 distributions (11/12 codepaths cold).

**Expected behavior**: Parity-anomaly (expected). TV cannot select BACK_LOADED; the harness override switches MagnifierDistribution::BACK_LOADED for the same Pine source. Compare against the ENDPOINTS dir trades.csv to quantify tick-density divergence.

**TV capture notes**:
- No TV capture for this dir (TV only emits ENDPOINTS).
- Run via harness with magnifier_dist=BACK_LOADED.
- Diff fills against magnifier-dist-probe-01-endpoints/trades.csv.
