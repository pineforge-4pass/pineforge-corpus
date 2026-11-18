# TradingView reference capture

This reference is the complete trade-report CSV of the original `strategy.pine` from corpus commit `8425ca820c4b7272db24d659a64d88e8e05b6407`, captured for `BINANCE:ETHUSDT.P` on the 15-minute timeframe over 2025-02-04 through 2026-04-08. The report was captured at 2026-11-17T19:48:46.321Z. It contains 1,574 exported trade pairs and 3,148 CSV rows, including the terminal range-end mark. TradingView's native performance counters report 1,573 closed trades and one open position. Requested and returned backtest bounds agree. The CSV renders times in UTC+8. The captured CSV is copied without filtering, renumbering or other edits.

| Artifact | SHA256 |
| --- | --- |
| `strategy.pine` | `e865936219d2c79a18afc90af6738ecf819b7762535e517c175ab1603a8e6988` |
| Previous `tv_trades.csv` | `44c9763b7588f98210cdac2b3c2ab0dab64ef5ac1303b1d387e4e179c7c86fa4` |
| New `tv_trades.csv` | `a8560526c3f1737b3d9c9f9fad9f2e35f8bca3625d5e72110e9369903951de98` |
| Capture custody record | `09d4384a14f3bd13931f81cb8db62605a2df5f797dae067e8b2d330aae84748a` |
| Engine versus new reference comparison | `a4cf44543b0694e6026aff389a512b945e6a1b62bc4eeccfc1244b3028ce06d6` |
| Engine versus previous reference comparison | `744ebf0e9dbd611baac3cc979b747f5cd13538d62a322efe7fb4b53c788945a9` |

For [engine issue #731](https://github.com/pineforge-4pass/pineforge-engine/issues/731), the engine trade list `cc11a5e2a416465a4c173b56d8cd8a72f28b53220e03f13fcc817f47b36fad5d` matches all 1,338 compared closed trades in this capture exactly in direction, entry/exit times, prices and quantity. Against the previous reference, the same comparison reports 1,252 exact matches, 42 differing exits, and 44 unmatched records on each side. The release acceptance decision on 2026-11-18 classified the old-reference grade change as reference aging and authorized this replacement. No grading threshold or strategy source is changed.

The engine comparison window is 2025-04-05 10:15 UTC through 2026-04-06 00:00 UTC. Captured trade **227 is a WINDOW EDGE**: its long entry is 2025-04-04 12:00 UTC at 1783.15 and its exit is at the window's first data bar, 2025-04-05 10:15 UTC at 1819.40, quantity 1. It is retained verbatim and is not an engine mismatch. Its CSV wall-clock entry/exit are 2025-04-04 20:00 and 2025-04-05 18:15 respectively. The engine's terminal short record and the capture's later range-end record are open-position marks; they are not compared as closed fills. Context trades outside the engine window remain in the full CSV.

`metrics.json` records public source, range, reference and comparison identities. Capture custody passed a 41-file content scan with zero hits and a successful exit. The report establishes returned trade identities and backtest range. It does not establish identical seed history or a full-window grade; full-window corpus grading remains the engine's unchanged grader's responsibility. The previous reference and the terminal-row limitation remain part of the historical comparison evidence.
