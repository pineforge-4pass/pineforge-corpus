# Magnifier dist probe 04-triangle — TRIANGLE

**Gap targeted**: X3 (Pine #22 + C++ #11) — magnifier 6 distributions (11/12 codepaths cold).

**Expected behavior**: Parity-anomaly (expected). TV cannot select TRIANGLE; the harness override switches MagnifierDistribution::TRIANGLE for the same Pine source. Compare against the ENDPOINTS dir trades.csv to quantify tick-density divergence.

**TV capture notes**:
- No TV capture for this dir (TV only emits ENDPOINTS).
- Run via harness with magnifier_dist=TRIANGLE.
- Diff fills against magnifier-dist-probe-01-endpoints/trades.csv.
