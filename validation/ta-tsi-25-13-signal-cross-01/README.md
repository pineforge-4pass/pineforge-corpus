# ta-tsi-25-13-signal-cross-01

Isolates `ta.tsi(close, 25, 13)` crossed against `ta.ema(tsi, 13)`. One of the
volume/state TA built-ins that had zero TradingView-parity coverage; verified
`excellent` (trade-for-trade) on BINANCE:ETHUSDT.P 15m.

## Reproduce
Symbol `BINANCE:ETHUSDT.P`, 15m, chart tz Asia/Taipei (UTC+8). Apply
`strategy.pine` → Strategy Tester → List of Trades → Export CSV → `tv_trades.csv`.
Then `scripts/verify_corpus.py <this dir>`.
