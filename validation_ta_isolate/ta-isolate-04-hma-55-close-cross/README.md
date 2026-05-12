# ta-isolate-04-hma-55-close-cross

## Purpose
Isolate `ta.hma(close, 55)`. HMA = `wma(2*wma(src, n/2) - wma(src, n), sqrt(n))`. Validates:
- WMA partial-window seed (n=27 first inner WMA, n=55 second inner WMA)
- Nested WMA composition with intermediate negative weights via `2*wma_half - wma_full`
- `sqrt(n)` rounding: `sqrt(55) = 7.416...` → Pine floors to **7**; verify engine matches

Targets gap probe `community/MarketShift` which uses HMA(close, 55).

## Chart setup (TV)
- Symbol: **ETH-USDT-USDT**
- Exchange: same feed as probes 01-03 (per first-bar OHLC check)
- Timeframe: **15m**
- Range: full shipped warmup6m window

## TV export
1. Apply `strategy.pine`.
2. `Strategy Tester → List of Trades → Export → CSV` → `tv_trades.csv`.

## Diagnostic interpretation
| Result | Conclusion |
|---|---|
| `excellent` | Engine HMA(55) is bit-exact. MarketShift HMA path not at fault — check its SMA(152) (probe 05). |
| `strong/moderate` 1-3 trade drift | Threshold-equality at `close == hma` ULP boundaries. Inspect engine `crossover` for boundary equality semantics. |
| `moderate/weak` significant drift | Engine HMA math drifts. Likely candidates: WMA seed bar index, `sqrt(n)` rounding (must match Pine `math.floor(math.sqrt(n))`), or nested-WMA weight chain. |
