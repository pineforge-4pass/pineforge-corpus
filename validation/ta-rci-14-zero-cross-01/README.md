# ta-rci-14-zero-cross-01

## Purpose
Isolate `ta.rci(close, 14)` — the Rank Correlation Index: the Spearman rank
correlation between bar time-order (0..n-1) and the price ranks within the
rolling window, scaled `x100`. Entries are driven purely off the sign of RCI
(long when RCI crosses over 0, short when it crosses under 0), so any
RCI-computation divergence surfaces directly as trade-count / fill drift.

**Suspect under test:** the engine computes Spearman rho via the d²-shortcut
formula `rho = 1 - 6*Σd² / (n*(n²-1))` (see `src/ta_oscillators.cpp` `RCI::compute`).
This shortcut is only exact when the window contains **no tied prices**.
On ties the engine assigns average ranks but still feeds them through the
d²-shortcut, whereas TradingView may compute Pearson correlation directly on
the average-ranks (the tie-correct Spearman form). With 15m ETH closes, exact
price ties inside a 14-bar window are rare but not impossible (round-number
clustering, low-volatility runs), and near-tie windows push RCI very close to
the ±0 threshold where the two formulas diverge by enough to flip a crossover.
Only a TV export can adjudicate which form TradingView uses.

## Setup + export
See `../README.md`. Quick reference:
- **Symbol / TF:** `ETH-USDT-USDT` (the corpus default feed), **15m** timeframe.
- **Chart timezone:** match the corpus default (`Etc/UTC`); leave Strategy
  Tester / chart TZ at UTC so exported timestamps align with the engine feed.
- **strategy() settings:** none to change — defaults baked into `strategy.pine`
  (initial_capital=1000000, commission 0, slippage 0, qty=1 fixed, pyramiding=1,
  `process_orders_on_close=false`).
- Apply `strategy.pine` → **Strategy Tester** → **List of Trades** → **Export**
  → save the CSV as `tv_trades.csv` next to this README.
- Then move this directory into `corpus/validation/` to enroll it in the sweep.

## Diagnostic interpretation
| Result | Conclusion |
|---|---|
| `excellent` | Engine RCI is bit-parity with TV across this feed — the d²-shortcut and TV's form agree because no (or only inconsequential) ties occurred. RCI path is clean. |
| `strong/moderate`, a few flipped trades | Tie / near-tie windows where the d²-shortcut and Pearson-on-average-ranks disagree by enough to flip a zero crossover. Localize the divergent bars (compare RCI series at the flip timestamps) to confirm ties are the trigger. |
| `weak`, broad drift | Algorithmic mismatch beyond ties — e.g. different rank convention, different window-fill / warmup (`n < length` na handling), or a scaling difference. Re-derive `RCI::compute` against TV: switch to Pearson correlation on average-ranks (`rho = cov(t, rank) / (σ_t · σ_rank)`) which is the tie-correct Spearman and reduces to the d²-shortcut when ties are absent. |
