# Legal — PineForge Validation Corpus

This corpus is licensed under the Apache License, Version 2.0
(see `LICENSE`). Same license as the PineForge engine runtime.

## What This Tree Contains

- **PineScript v6 sources** (`strategy.pine`) — clean-room PineForge
  originals. Each carries an Apache-2.0 SPDX header, a clean-room
  marker, and a rationale block describing the engine surface under
  test. None are derived from third-party PineScript on TradingView's
  public library or community scripts; the prior `basic/` (TradingView
  built-in templates) and `community/` (mixed MPL-2.0 + unlicensed
  third-party) trees were deleted and replaced by these clean-room
  rewrites.

- **TradingView "List of Trades" CSV exports** (`tv_trades.csv`) —
  emitted by TradingView's broker emulator running our own
  `strategy.pine` files on the reference OHLCV feed. We hold the right
  to redistribute these as artefacts of our own scripts.

- **Engine trade lists** (`engine_trades.csv`) — produced by PineForge
  against the same OHLCV in TradingView's row-and-column format for
  direct line-by-line parity comparison.

- **OHLCV market data** (`data/ohlcv_ETH-USDT-USDT_15m.csv` and
  `data/ohlcv_ETH-USDT-USDT_15m_warmup6m.csv`, plus the 1m variants) —
  Binance USDT-M Perpetual ETH/USDT-USDT. Public market price/volume
  series; redistribution is factual-data territory.

`generated.cpp` (transpiler output) and `strategy.dylib` /
`strategy.so` / `strategy.dll` (compiled artefacts) are NOT shipped
publicly. The `pineforge-codegen` transpiler is closed-source;
reproducers regenerate `generated.cpp` locally via
`scripts/regen_corpus_cpp.py` and compile the per-probe `.so` via
`cmake --build build --target corpus_strategies`. See `corpus/.gitignore`.

## Trademarks

TradingView and PineScript are trademarks of their respective owners.
This corpus is not affiliated with or endorsed by TradingView.

## License Field

SPDX: `Apache-2.0`.

## Reproducibility

The headline parity figure documented in `validation_report.md` is
reproducible from the artefacts in this tree plus the public engine
runtime. See the project root `README.md` for the full reproducer
instructions.
