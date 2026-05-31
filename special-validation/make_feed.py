#!/usr/bin/env python3
"""make_feed.py — build the engine OHLCV feed from raw TradingView chart exports.

TradingView's chart-data export ("Export chart data…") emits one CSV per pull
with columns:  time,open,high,low,close,Volume  where ``time`` is unix SECONDS.
TV caps a single export at ~20k bars, so a year of 5m SPX needs two overlapping
pulls. This merges any number of such CSVs into the engine feed format
``timestamp,open,high,low,close,volume`` (unix MILLISECONDS), de-duplicating by
timestamp and sorting ascending.

Usage:
    python make_feed.py OUT.csv RAW1.csv RAW2.csv [RAW3.csv ...]

Example (from this dir, after exporting the two SPX 5m pulls to ~/Downloads):
    python make_feed.py ohlcv_SPX_5m.csv \
        ~/Downloads/"SP_SPX, 5_ccb9a.csv" ~/Downloads/"SP_SPX, 5_d57de.csv"
"""
import csv
import sys


def main(argv: list[str]) -> int:
    if len(argv) < 3:
        sys.stderr.write(__doc__)
        return 2
    out_path, *src_paths = argv[1:]
    bars: dict[int, tuple[str, str, str, str, str]] = {}
    for src in src_paths:
        with open(src, encoding="utf-8-sig", newline="") as f:
            reader = csv.DictReader(f)
            for row in reader:
                ts_ms = int(float(row["time"])) * 1000  # seconds -> ms
                vol = row.get("Volume") or row.get("volume") or "0"
                bars[ts_ms] = (row["open"], row["high"], row["low"],
                               row["close"], vol)
    with open(out_path, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["timestamp", "open", "high", "low", "close", "volume"])
        for ts in sorted(bars):
            o, h, l, c, v = bars[ts]
            w.writerow([ts, o, h, l, c, v])
    print(f"wrote {out_path}: {len(bars)} unique bars from {len(src_paths)} export(s)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
