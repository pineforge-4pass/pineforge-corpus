# Legal - PineForge Validation Corpus (Private)

This repository is maintained separately from the public PineForge engine. It
is not automatically licensed to the public: access is limited to people granted
read access here.

## What This Tree Contains

- PineScript sources (`strategy.pine`) and related artefacts used to validate
  the open-source runtime.
- TradingView "List of Trades" CSV exports (`tv_trades.csv`) and similar
  validation outputs.
- Transpiler-generated C++ (`generated.cpp`) produced by the closed-source
  PineForge compiler for those scripts.
- OHLCV market data under `data/` used only for replay testing.

## Redistribution And Confidentiality

1. TradingView exports: treat these as confidential validation artefacts. Do
   not publish them in a public repo, gist, or artifact store unless you have an
   independent right to do so. The public engine repository intentionally omits
   this directory via a private git submodule.
2. `generated.cpp`: derivative of both `strategy.pine` and the transpiler. Use
   is limited to internal parity testing and building `strategy.so` against
   `libpineforge.a` for that purpose. Redistribution outside your org requires
   your own legal review.
3. Market data: OHLCV files are factual price/volume series; rights depend on
   the vendor and your agreement. Do not assume they may be republished broadly.

## Trademarks

TradingView and PineScript are trademarks of their respective owners. This
corpus is not affiliated with or endorsed by TradingView.

## License Field

If you need a single SPDX line for internal tracking: `LicenseRef-Proprietary`
or `NONE` for the aggregate corpus as distributed here, unless your organisation
applies a specific internal or CLA-based license to contributions.

For questions about releasing any part of this tree publicly, contact counsel;
do not rely on this file as legal advice.
