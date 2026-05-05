#include <pineforge/engine.hpp>
#include <pineforge/ta.hpp>
#include <pineforge/math.hpp>
#include <pineforge/series.hpp>
#include <pineforge/na.hpp>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <numeric>
#include <string>
#include <vector>
#include <tuple>
#include <memory>
#include <unordered_map>
#include <unordered_map>
#include <pineforge/color.hpp>
#include <pineforge/log.hpp>
#include <pineforge/str_utils.hpp>
#include <pineforge/session_time.hpp>

using namespace pineforge;

class GeneratedStrategy : public BacktestEngine {
public:
    ta::PivotHigh _ta_pivothigh_1;
    ta::PivotLow _ta_pivotlow_2;
    ta::SMA _ta_sma_3;
    ta::StdDev _ta_stdev_4;
    ta::EMA _ta_ema_5;
    ta::SMA _ta_sma_6;
    ta::WMA _ta_wma_7;
    ta::EMA _ta_ema_5_cs1;
    ta::SMA _ta_sma_6_cs1;
    ta::WMA _ta_wma_7_cs1;
    ta::Crossunder _ta_crossunder_8;
    ta::Crossover _ta_crossover_9;
    ta::SMA _ta_sma_10;
    ta::ATR _ta_atr_11;
    ta::SMA _ta_sma_12;
    double resistance_level;
    double support_level;
    int trade_counter;
    double daily_pnl;
    int last_trade_day;
    int bars_since_entry;
    double dashboard;
    Series<int> _strat_closedtrades;
    int capital = 0;
    double risk_per_trade = 0.0;
    double riskReward = 0.0;
    int use_leverage = 0;
    double max_daily_loss = 0.0;
    int max_positions = 0;
    int sr_lookback = 0;
    double sr_tolerance = 0.0;
    int bb_length = 0;
    double bb_mult = 0.0;
    double bb_src = 0.0;
    int fast_ma_len = 0;
    int slow_ma_len = 0;
    std::string ma_type = std::string("");
    bool use_volume_filter = false;
    double vol_mult = 0.0;
    bool use_atr_filter = false;
    double atr_min_mult = 0.0;
    int min_bars_between = 0;
    int testStartYear = 0;
    int testStartMonth = 0;
    int testStartDay = 0;
    int testPeriodStart = 0;
    double pivot_high = 0.0;
    double pivot_low = 0.0;
    double sr_zone = 0.0;
    bool near_resistance = false;
    bool near_support = false;
    double bb_basis = 0.0;
    double bb_dev = 0.0;
    double bb_upper = 0.0;
    double bb_lower = 0.0;
    double bb_pct = 0.0;
    bool bb_long_signal = false;
    bool bb_short_signal = false;
    double fast_ma = 0.0;
    double slow_ma = 0.0;
    bool ma_bearish_cross = false;
    bool ma_bullish_cross = false;
    double vol_avg = 0.0;
    bool vol_ok = false;
    double atr_val = 0.0;
    double atr_avg = 0.0;
    bool atr_ok = false;
    double long_score = 0.0;
    double short_score = 0.0;
    bool reverse_long_condition = false;
    bool reverse_short_condition = false;
    double current_day = 0.0;
    bool daily_loss_ok = false;
    double open_trades = 0.0;
    bool can_open_new = false;
    double atr_stop_distance = 0.0;
    double entry_price = 0.0;
    double stop_val = 0.0;
    double take_val = 0.0;
    double risk_amount = 0.0;
    double stop_dist_pct = 0.0;
    double qty_contracts = 0.0;
    std::string trade_id = std::string("");
    double last_pnl = 0.0;
    double p_bb_upper = 0.0;
    double p_bb_lower = 0.0;
    double p_bb_basis = 0.0;
    std::string pos_text = std::string("");
    bool _var_initialized = false;
    bool _ta_initialized_ = false;

    explicit GeneratedStrategy() : _ta_pivothigh_1(20, 20), _ta_pivotlow_2(20, 20), _ta_sma_3(20), _ta_stdev_4(20), _ta_ema_5(9), _ta_sma_6(9), _ta_wma_7(9), _ta_ema_5_cs1(21), _ta_sma_6_cs1(21), _ta_wma_7_cs1(21), _ta_sma_10(20), _ta_atr_11(14), _ta_sma_12(50), resistance_level(na<double>()), support_level(na<double>()), trade_counter(0), daily_pnl(0), last_trade_day(0), bars_since_entry(100) {
        initial_capital_ = 1000000.0;
        default_qty_type_ = QtyType::FIXED;
        default_qty_value_ = 1.0;
        pyramiding_ = 1;
        commission_type_ = CommissionType::PERCENT;
        commission_value_ = 0.0;
        slippage_ = 0;
        margin_long_ = 0.0;
        margin_short_ = 0.0;
    }

    void set_strategy_override(const std::string& key, const std::string& value) {
        if (key == "initial_capital") { initial_capital_ = std::stod(value); return; }
        if (key == "commission_value") { commission_value_ = std::stod(value); return; }
        if (key == "default_qty_value") { default_qty_value_ = std::stod(value); return; }
        if (key == "pyramiding") { pyramiding_ = std::stoi(value); return; }
        if (key == "slippage") { slippage_ = std::stoi(value); return; }
        if (key == "process_orders_on_close") { process_orders_on_close_ = (value == "true" || value == "1"); return; }
        if (key == "close_entries_rule") { close_entries_rule_any_ = (value == "ANY" || value == "any" || value == "1"); return; }
        if (key == "default_qty_type") {
            if (value == "fixed" || value == "strategy.fixed" || value == "0") default_qty_type_ = QtyType::FIXED;
            else if (value == "percent_of_equity" || value == "strategy.percent_of_equity" || value == "1") default_qty_type_ = QtyType::PERCENT_OF_EQUITY;
            else if (value == "cash" || value == "strategy.cash" || value == "2") default_qty_type_ = QtyType::CASH;
            return;
        }
        if (key == "commission_type") {
            if (value == "percent" || value == "strategy.commission.percent" || value == "0") commission_type_ = CommissionType::PERCENT;
            else if (value == "cash_per_order" || value == "strategy.commission.cash_per_order" || value == "1") commission_type_ = CommissionType::CASH_PER_ORDER;
            else if (value == "cash_per_contract" || value == "strategy.commission.cash_per_contract" || value == "2") commission_type_ = CommissionType::CASH_PER_CONTRACT;
            return;
        }
    }

    double get_ma_cs0(double src, int length) {
        double _func_ret = 0.0;
        auto __switch_val_0 = ma_type;
        if (__switch_val_0 == std::string("EMA")) {
            _func_ret = (is_first_tick_ ? _ta_ema_5.compute(src) : _ta_ema_5.recompute(src));
        }
        else if (__switch_val_0 == std::string("SMA")) {
            _func_ret = (is_first_tick_ ? _ta_sma_6.compute(src) : _ta_sma_6.recompute(src));
        }
        else if (__switch_val_0 == std::string("WMA")) {
            _func_ret = (is_first_tick_ ? _ta_wma_7.compute(src) : _ta_wma_7.recompute(src));
        }
        return _func_ret;
    }

    double get_ma_cs1(double src, int length) {
        double _func_ret = 0.0;
        auto __switch_val_1 = ma_type;
        if (__switch_val_1 == std::string("EMA")) {
            _func_ret = (is_first_tick_ ? _ta_ema_5_cs1.compute(src) : _ta_ema_5_cs1.recompute(src));
        }
        else if (__switch_val_1 == std::string("SMA")) {
            _func_ret = (is_first_tick_ ? _ta_sma_6_cs1.compute(src) : _ta_sma_6_cs1.recompute(src));
        }
        else if (__switch_val_1 == std::string("WMA")) {
            _func_ret = (is_first_tick_ ? _ta_wma_7_cs1.compute(src) : _ta_wma_7_cs1.recompute(src));
        }
        return _func_ret;
    }

    bool testPeriod() {
        return (current_bar_.timestamp >= testPeriodStart);
    }

    void on_bar(const Bar& bar) override {
        _strat_closedtrades.push(((int)trades_.size()));
        if (!_var_initialized) {
            _var_initialized = true;
        } else {
        }
        if (!_ta_initialized_) {
            _ta_pivothigh_1 = ta::PivotHigh(get_input_int("S/R Lookback Period", 20), get_input_int("S/R Lookback Period", 20));
            _ta_pivotlow_2 = ta::PivotLow(get_input_int("S/R Lookback Period", 20), get_input_int("S/R Lookback Period", 20));
            _ta_sma_3 = ta::SMA(get_input_int("BB Length", 20));
            _ta_stdev_4 = ta::StdDev(get_input_int("BB Length", 20));
            _ta_ema_5 = ta::EMA(get_input_int("Fast MA Length", 9));
            _ta_sma_6 = ta::SMA(get_input_int("Fast MA Length", 9));
            _ta_wma_7 = ta::WMA(get_input_int("Fast MA Length", 9));
            _ta_ema_5_cs1 = ta::EMA(get_input_int("Slow MA Length", 21));
            _ta_sma_6_cs1 = ta::SMA(get_input_int("Slow MA Length", 21));
            _ta_wma_7_cs1 = ta::WMA(get_input_int("Slow MA Length", 21));
            _ta_initialized_ = true;
        }
        capital = get_input_int("Capital ($)", 1000);
        risk_per_trade = get_input_double("Risk per Trade (%)", 1.0);
        riskReward = get_input_double("Risk:Reward Ratio", 2.0);
        use_leverage = get_input_int("Leverage", 5);
        max_daily_loss = get_input_double("Max Daily Loss (%)", 3.0);
        max_positions = get_input_int("Max Open Positions", 5);
        sr_lookback = get_input_int("S/R Lookback Period", 20);
        sr_tolerance = get_input_double("S/R Zone Width (%)", 0.3);
        bb_length = get_input_int("BB Length", 20);
        bb_mult = get_input_double("BB Multiplier", 2.0);
        bb_src = get_input_double("BB Source", current_bar_.close);
        fast_ma_len = get_input_int("Fast MA Length", 9);
        slow_ma_len = get_input_int("Slow MA Length", 21);
        ma_type = get_input_string("MA Type", std::string("EMA"));
        use_volume_filter = get_input_bool("Use Volume Filter", true);
        vol_mult = get_input_double("Min Volume Multiplier", 1.2);
        use_atr_filter = get_input_bool("Use ATR Volatility Filter", true);
        atr_min_mult = get_input_double("Min ATR Multiplier", 0.5);
        min_bars_between = get_input_int("Min Bars Between Entries", 3);
        testStartYear = get_input_int("Backtest Start Year", 2025);
        testStartMonth = get_input_int("Backtest Start Month", 1);
        testStartDay = get_input_int("Backtest Start Day", 1);
        testPeriodStart = [&](){ struct tm t = {}; t.tm_year = (int)(testStartYear) - 1900; t.tm_mon = (int)(testStartMonth) - 1; t.tm_mday = (int)(testStartDay); t.tm_hour = (int)(0); t.tm_min = (int)(0); return (int64_t)timegm(&t) * 1000; }();
        pivot_high = (is_first_tick_ ? _ta_pivothigh_1.compute(current_bar_.high) : _ta_pivothigh_1.recompute(current_bar_.high));
        pivot_low = (is_first_tick_ ? _ta_pivotlow_2.compute(current_bar_.low) : _ta_pivotlow_2.recompute(current_bar_.low));
        if (!(is_na(pivot_high))) {
            resistance_level = pivot_high;
        }
        if (!(is_na(pivot_low))) {
            support_level = pivot_low;
        }
        sr_zone = (current_bar_.close * ((double)(sr_tolerance) / (double)(100)));
        near_resistance = (!(is_na(resistance_level)) && (std::abs((current_bar_.close - resistance_level)) <= sr_zone));
        near_support = (!(is_na(support_level)) && (std::abs((current_bar_.close - support_level)) <= sr_zone));
        bb_basis = (is_first_tick_ ? _ta_sma_3.compute(bb_src) : _ta_sma_3.recompute(bb_src));
        bb_dev = (bb_mult * (is_first_tick_ ? _ta_stdev_4.compute(bb_src) : _ta_stdev_4.recompute(bb_src)));
        bb_upper = (bb_basis + bb_dev);
        bb_lower = (bb_basis - bb_dev);
        bb_pct = ((double)((current_bar_.close - bb_lower)) / (double)((bb_upper - bb_lower)));
        bb_long_signal = (current_bar_.close > bb_upper);
        bb_short_signal = (current_bar_.close < bb_lower);
        fast_ma = get_ma_cs0(current_bar_.close, fast_ma_len);
        slow_ma = get_ma_cs1(current_bar_.close, slow_ma_len);
        ma_bearish_cross = (is_first_tick_ ? _ta_crossunder_8.compute(fast_ma, slow_ma) : _ta_crossunder_8.recompute(fast_ma, slow_ma));
        ma_bullish_cross = (is_first_tick_ ? _ta_crossover_9.compute(fast_ma, slow_ma) : _ta_crossover_9.recompute(fast_ma, slow_ma));
        vol_avg = (is_first_tick_ ? _ta_sma_10.compute(current_bar_.volume) : _ta_sma_10.recompute(current_bar_.volume));
        vol_ok = (!(use_volume_filter) || (current_bar_.volume >= (vol_avg * vol_mult)));
        atr_val = (is_first_tick_ ? _ta_atr_11.compute(current_bar_.high, current_bar_.low, current_bar_.close) : _ta_atr_11.recompute(current_bar_.high, current_bar_.low, current_bar_.close));
        atr_avg = (is_first_tick_ ? _ta_sma_12.compute(atr_val) : _ta_sma_12.recompute(atr_val));
        atr_ok = (!(use_atr_filter) || (atr_val >= (atr_avg * atr_min_mult)));
        long_score = ((((near_resistance) ? (1) : (0)) + ((bb_long_signal) ? (1) : (0))) + ((ma_bearish_cross) ? (1) : (0)));
        short_score = ((((near_support) ? (1) : (0)) + ((bb_short_signal) ? (1) : (0))) + ((ma_bullish_cross) ? (1) : (0)));
        reverse_long_condition = (((long_score >= 2) && vol_ok) && atr_ok);
        reverse_short_condition = (((short_score >= 2) && vol_ok) && atr_ok);
        bars_since_entry += 1;
        current_day = _bar_dayofmonth();
        if ((current_day != last_trade_day)) {
            daily_pnl = 0.0;
            last_trade_day = current_day;
        }
        daily_loss_ok = (daily_pnl > (-((double)((capital * max_daily_loss)) / (double)(100))));
        open_trades = ((int)pyramid_entries_.size());
        can_open_new = ((open_trades < max_positions) && (bars_since_entry >= min_bars_between));
        atr_stop_distance = (atr_val * 1.5);
        if (((testPeriod() && daily_loss_ok) && can_open_new)) {
            if (reverse_long_condition) {
                trade_counter += 1;
                entry_price = current_bar_.close;
                stop_val = (current_bar_.close - atr_stop_distance);
                take_val = (current_bar_.close + (atr_stop_distance * riskReward));
                risk_amount = (capital * ((double)(risk_per_trade) / (double)(100)));
                stop_dist_pct = ((double)(std::abs((entry_price - stop_val))) / (double)(entry_price));
                qty_contracts = (((stop_dist_pct > 0)) ? (((double)(std::round(((double)(risk_amount) / (double)(stop_dist_pct)))) / (double)(current_bar_.close))) : (0.0));
                trade_id = (std::string("Long_") + std::to_string(trade_counter));
                if ((qty_contracts > 0)) {
                    strategy_entry(trade_id, true, na<double>(), na<double>(), qty_contracts, "", "", 0, -1);
                    strategy_exit((std::string("Exit_") + std::to_string(trade_counter)), trade_id, take_val, stop_val, na<double>(), na<double>(), na<double>(), 100.0, "");
                    bars_since_entry = 0;
                }
            }
            if (reverse_short_condition) {
                trade_counter += 1;
                entry_price = current_bar_.close;
                stop_val = (current_bar_.close + atr_stop_distance);
                take_val = (current_bar_.close - (atr_stop_distance * riskReward));
                risk_amount = (capital * ((double)(risk_per_trade) / (double)(100)));
                stop_dist_pct = ((double)(std::abs((stop_val - entry_price))) / (double)(entry_price));
                qty_contracts = (((stop_dist_pct > 0)) ? (((double)(std::round(((double)(risk_amount) / (double)(stop_dist_pct)))) / (double)(current_bar_.close))) : (0.0));
                trade_id = (std::string("Short_") + std::to_string(trade_counter));
                if ((qty_contracts > 0)) {
                    strategy_entry(trade_id, false, na<double>(), na<double>(), qty_contracts, "", "", 0, -1);
                    strategy_exit((std::string("Exit_") + std::to_string(trade_counter)), trade_id, take_val, stop_val, na<double>(), na<double>(), na<double>(), 100.0, "");
                    bars_since_entry = 0;
                }
            }
        }
        if ((((int)trades_.size()) > 0)) {
            last_pnl = closed_trade_profit((((int)trades_.size()) - 1));
            if ((((int)trades_.size()) != _strat_closedtrades[1])) {
                daily_pnl += last_pnl;
            }
        }
        if (barstate_islast_) {
            pos_text = (((signed_position_size() > 0)) ? (std::string("NET LONG")) : ((((signed_position_size() < 0)) ? (std::string("NET SHORT")) : (std::string("FLAT")))));
        }
    }

};

extern "C" {
    void* strategy_create(const char* params_json) {
        return new GeneratedStrategy();
    }
    void run_backtest(void* s, Bar* bars, int n, ReportC* out) {
        auto* strat = static_cast<GeneratedStrategy*>(s);
        strat->run(bars, n);
        strat->fill_report(out);
    }
    void run_backtest_full(void* s, Bar* bars, int n,
                           const char* input_tf, const char* script_tf,
                           int bar_magnifier, int magnifier_samples,
                           int magnifier_dist,
                           ReportC* out) {
        auto* strat = static_cast<GeneratedStrategy*>(s);
        std::string itf = input_tf ? input_tf : "";
        std::string stf = script_tf ? script_tf : "";
        if (itf.empty() || stf.empty() || itf == stf) {
            strat->run(bars, n);
        } else {
            strat->run(bars, n, itf, stf, bar_magnifier != 0, magnifier_samples,
                       static_cast<MagnifierDistribution>(magnifier_dist));
        }
        strat->fill_report(out);
    }
    void strategy_free(void* s) {
        delete static_cast<GeneratedStrategy*>(s);
    }
    void report_free(ReportC* report) {
        BacktestEngine::free_report(report);
    }
    void strategy_set_input(void* s, const char* key, const char* value) {
        if (!s || !key || !value) return;
        static_cast<GeneratedStrategy*>(s)->set_input(key, value);
    }
    void strategy_set_override(void* s, const char* key, const char* value) {
        if (!s || !key || !value) return;
        static_cast<GeneratedStrategy*>(s)->set_strategy_override(key, value);
    }
    void strategy_set_magnifier_volume_weighted(void* s, int on) {
        if (!s) return;
        static_cast<GeneratedStrategy*>(s)->set_magnifier_volume_weighted(on != 0);
    }
}
