# varip-reject-probe-02-var-udt-in-security

## Gap
Companion to probe 01 (X10, Pine#1, C++#15): confirms the `varip` rejection is targeted, not blanket. A `var` UDT field mutated inside a `request.security` callable should ACCEPT and produce trades.

## Expected behavior
- `transpile(...)` should succeed with no errors.
- `var Counter c` inside the `htfWork()` security callable persists across HTF bars.
- Each HTF EMA crossover increments `c.hits`; chart-side `htfFired` flags the bar where the counter advances.
- >=10 closed long trades on the 15m ETH-USDT-USDT window.

## TV capture notes
- 15m chart, ETH-USDT-USDT, same window as `data/ohlcv_ETH-USDT-USDT_15m.csv`.
- Manual TradingView export → `tv_trades.csv`.
- Any divergence in HTF crossover counts indicates security-callable state drift.
