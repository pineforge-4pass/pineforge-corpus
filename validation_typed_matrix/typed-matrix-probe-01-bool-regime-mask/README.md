# typed-matrix-probe-01-bool-regime-mask

## Gap
Cross-confirmed P0 (X7): typed matrix dispatch — `matrix.new<bool>` bool-proxy storage, `set/get/transpose` round-trip, bool-matrix reductions flowing into entry conditions. Zero corpus coverage today.

## Expected behavior
- Bool matrix should round-trip through `matrix.transpose` losslessly.
- `hotCount` accumulates monotonically across the corpus window, gating entries once it crosses 6.
- >=10 closed long trades on the 15m ETH-USDT-USDT window.

## TV capture notes
- 15m chart, ETH-USDT-USDT, same window as `data/ohlcv_ETH-USDT-USDT_15m.csv`.
- Manual TradingView export → `tv_trades.csv` in this folder.
- Watch for any first-bar mask init drift between Pine and PineForge.
