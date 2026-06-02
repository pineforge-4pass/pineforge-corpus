# ta-wpr-14-bands-01

## Purpose

Isolates `ta.wpr(length)` — Williams %R(14) — against TradingView. Williams %R is the
oscillator `(ta.highest(high, len) - close) / (ta.highest(high, len) - ta.lowest(low, len)) * -100`,
ranging from -100 (most oversold) to 0 (most overbought), with close/high/low taken as
implicit native sources. The strategy goes long when %R crosses up through the -80
oversold band and short when %R crosses down through the -20 overbought band, so a TV
export adjudicates the engine's %R sign, range, and the rolling highest/lowest(14) window
alignment in a single probe.

## TradingView export recipe

1. Open a TradingView chart for symbol **ETH-USDT-USDT** (the corpus default feed;
   use the BINANCE/MEXC ETHUSDT.P perpetual that matches your corpus OHLCV source).
2. Set the timeframe to **15m**.
3. Set the **chart timezone to UTC** (right-click the time axis -> Timezone -> UTC).
4. Add the script: open `strategy.pine` in the Pine Editor and click **Add to chart**.
   Leave every `strategy()` setting at the values declared in the script
   (initial_capital=1000000, commission 0, slippage 0, qty_type=fixed, qty_value=1,
   pyramiding=1, process_orders_on_close=false). Do not override anything in the
   Strategy Tester "Properties" tab.
5. Open the **Strategy Tester** panel -> **List of Trades** tab.
6. Click **Export** (download icon) -> save the CSV as **tv_trades.csv** in this
   directory (`proposed-probes/ta-wpr-14-bands-01/tv_trades.csv`).
7. Move this whole directory into `corpus/validation/` to register it in the parity sweep.

## Engine run

- Feed: ETH-USDT-USDT 15m, 6-month warmup (corpus default).
- Result: 1767 trades, net_profit ~= -3624.54 over 53929 bars.
- Reproduce:
  ```bash
  EIGEN=/opt/homebrew/include/eigen3
  clang++ -std=c++17 -O2 -fPIC -shared -I include -I "$EIGEN" \
    proposed-probes/ta-wpr-14-bands-01/generated.cpp \
    -Wl,-force_load,build/lib/libpineforge.a \
    -o proposed-probes/ta-wpr-14-bands-01/strategy.dylib
  python3 scripts/run_strategy.py proposed-probes/ta-wpr-14-bands-01 --so-name strategy.dylib
  ```
