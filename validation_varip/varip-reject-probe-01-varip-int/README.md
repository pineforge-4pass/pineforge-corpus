# varip-reject-probe-01-varip-int

## Gap
P1 (X10, Pine#1, C++#15): `varip` intra-bar mutation is unsupported by PineForge. Silent acceptance corrupts every downstream order. This is a REJECTION probe.

## Expected behavior
- `transpile(...)` MUST raise an exception.
- Expected error message references `varip` declaration (e.g. "unsupported declaration: varip", "varip not supported", or similar checker rejection text).
- If the transpile pipeline silently accepts this script, the rejection probe has FAILED and the support_checker has drifted.

## TV capture notes
- Not applicable — build-time rejection probe. No `tv_trades.csv` will be captured.
