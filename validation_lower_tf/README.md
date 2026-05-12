# validation_lower_tf

Probes for `request.security_lower_tf` element-type and ratio synthesis.
Addresses gap **X1** in `/tmp/probe_design_spec.md` §1 (cross-confirmed
P0; zero corpus uses today; runtime claims support but no fixture
exercises it). One bug here breaks every future LTF user.

## Probes

| Probe | Kind | Expected |
|---|---|---|
| `lower-tf-probe-01-numeric-float-ratio15` | acceptance | clean transpile + ≥10 trades on 36k 15m ETH bars; sum-of-LTF parity vs `15*close` exposes remainder-on-last-sub-bar drift |
| `lower-tf-probe-02-bool-array` | acceptance | clean transpile + ≥10 trades; bool element storage / count-true reduce path |

> **Removed probes**: `lower-tf-probe-03-ratio-mismatch-reject` (runtime rejection) and
> `lower-tf-probe-04-tuple-element-reject` (codegen rejection) were moved to the
> codegen pytest scope where reject-path assertions are maintained.

## TV capture instructions (acceptance probes only)

1. Open TradingView, set chart to **ETH-USDT-USDT, 15m**.
2. Paste `strategy.pine` into Pine editor; Add to chart.
3. Open the **Strategy Tester → List of Trades** tab.
4. Export to CSV; save as `tv_trades.csv` next to `strategy.pine`.
5. Window must align with `data/ohlcv_ETH-USDT-USDT_15m.csv` (~36k bars).

Both remaining probes are acceptance probes. Capture TV trades and verify
via the standard validator pipeline.
