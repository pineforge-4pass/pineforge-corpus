# TradingView reference capture

The reference is the complete trade-report CSV of the original `strategy.pine` from corpus commit `8425ca820c4b7272db24d659a64d88e8e05b6407`, captured for `BINANCE:ETHUSDT.P` on the 15-minute timeframe over 2020-01-01 through 2026-04-01. The report was captured at 2026-11-16T06:15:53.969Z and the CSV contains 152 trade pairs, 304 rows, including the terminal range-end mark. TradingView's native performance counters report 151 closed trades and one open position. Requested and returned backtest bounds agree. The CSV uses UTC+8.

File identities:

| Artifact | SHA256 |
| --- | --- |
| `strategy.pine` | `743182d8ed677a5b3a3a9dc456aa20282ba0e11a5bc02a332185fea37cdda849` |
| `tv_trades.csv` | `b99833bb6349d89a3ef9273a38675375e84a3e585b666c40ac32963c4277b6e9` |

The report's short trade 130 enters at Unix epoch 1748044800000 (2025-05-24 00:00 UTC), price 2525.38 and quantity 1, and exits at epoch 1751328000000, price 2484.30. This supplies the original-source reference for [engine issue #883](https://github.com/pineforge-4pass/pineforge-engine/issues/883).

`metrics.json` records the observed range, source and artifact identities using public metadata. The report establishes the returned trade identities and backtest range. Engine parity is measured separately. The corpus grade is determined by the engine's unchanged corpus grader on its actual full-window run.
