# symbol-futures-pointvalue-es-01

## Purpose

Isolate the **per-instrument PnL multiplier (point value)** and the
**0.25-tick fill snap** on a real CME futures contract.

`CME_MINI:ES1!` (E-mini S&P 500 future) has:

- `syminfo.pointvalue = 50` — USD per index point per contract
- `syminfo.mintick = 0.25` — minimum price increment (tick size 0.25)

Compared with the engine's default crypto instrument (`pointvalue = 1`,
`mintick = 0.01`), this probe verifies that:

1. Each closed 1-contract trade's gross PnL equals
   `(exit_px - entry_px) * 50`.
2. Fills snap to 0.25 price boundaries (slippage=1 tick = 0.25, applied
   directionally — buy up, sell down).
3. `cash_per_contract` commission of $2.10 is charged per contract on
   both entry and exit.

The signal is a plain `SMA(9)` / `SMA(21)` crossover so the trade set is
fully deterministic and the point-value scaling is the only variable.

## Strategy

- Entry: `ta.crossover(SMA(9), SMA(21))` → `strategy.entry("L", long)`
- Exit:  `ta.crossunder(SMA(9), SMA(21))` → `strategy.close("L")`
- `default_qty_type=fixed`, `default_qty_value=1`, `pyramiding=1`
- `commission_type=cash_per_contract`, `commission_value=2.10`
- `slippage=1`, `process_orders_on_close=false`

## Status — BLOCKED on per-symbol data + syminfo injection

This probe needs:

1. **ES 5m OHLCV** (`data/ohlcv_CME_MINI_ES1_5m.csv`). The bundled corpus
   data is ETH-USDT-USDT only.
2. **ES syminfo** (`pointvalue=50`, `mintick=0.25`). The current
   `scripts/run_strategy.py` harness can inject tz/session and fundamental
   `syminfo_metadata`, but has **no path to override `syminfo.pointvalue`
   or `syminfo.mintick`** — those are only set via the `SymInfo` struct in
   `BacktestEngine::run`, not exposed through the C ABI used by the loader.

Until the harness (or pineforge-data) supplies ES OHLCV + ES syminfo,
compile/run are skipped (`compiled=false`, `ran=false`). The probe lives
here so the Pine and its intent are version-controlled, mirroring
`corpus/validation/symbol-specified/*`.

## TradingView export recipe

1. Open chart for symbol **`CME_MINI:ES1!`**.
2. Set timeframe to **5m**.
3. Set **chart timezone to `America/New_York`** (Exchange time;
   right-click price scale → or chart settings → Timezone).
4. Apply `strategy.pine` to the chart (Pine Editor → Add to chart).
5. In `strategy()` keep all settings as declared — do **not** change
   commission/slippage in the Strategy Tester Properties (they are pinned
   in the script: cash_per_contract $2.10, slippage 1 tick).
6. Open **Strategy Tester → List of Trades**.
7. Click **Export** → save as `tv_trades.csv` in this directory.
8. Provide the matching `CME_MINI:ES1!` 5m OHLCV as
   `data/ohlcv_CME_MINI_ES1_5m.csv` (or update `inputs.json::ohlcv_csv`).
9. Move this directory into `corpus/validation/` (or, until the
   data/syminfo plumbing lands, under
   `corpus/validation/symbol-specified/ES/`) and validate by passing its
   full path explicitly to the sweep.
