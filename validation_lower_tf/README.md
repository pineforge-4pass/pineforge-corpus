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
| `lower-tf-probe-03-ratio-mismatch-reject` | rejection (runtime) | codegen passes; engine `validate_security_timeframes` throws `request.security_lower_tf: requested timeframe '4' is not an integer divisor of input '15'`. Validator captures via `strategy_get_last_error`. |
| `lower-tf-probe-04-tuple-element-reject` | rejection (codegen) | codegen rejects UDT element type with: `request.security_lower_tf element type 'PineType.VOID' is not yet supported. Supported element types: float, int, bool.` |

## TV capture instructions (acceptance probes only)

1. Open TradingView, set chart to **ETH-USDT-USDT, 15m**.
2. Paste `strategy.pine` into Pine editor; Add to chart.
3. Open the **Strategy Tester → List of Trades** tab.
4. Export to CSV; save as `tv_trades.csv` next to `strategy.pine`.
5. Window must align with `data/ohlcv_ETH-USDT-USDT_15m.csv` (~36k bars).

Rejection probes (`-03`, `-04`) intentionally have no `tv_trades.csv` —
they assert the codegen / runtime error path. Capture the
diagnostic in the validation report instead.
