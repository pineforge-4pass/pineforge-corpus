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
#include <mutex>
#include <unordered_map>
#include <unordered_map>
#include <pineforge/color.hpp>
#include <pineforge/log.hpp>
#include <pineforge/str_utils.hpp>
#include <pineforge/session_time.hpp>

using namespace pineforge;

class GeneratedStrategy : public BacktestEngine {
public:
    ta::EMA _ta_ema_1;
    ta::EMA _ta_ema_2;
    ta::EMA _ta_ema_3;
    ta::EMA _ta_ema_4;
    ta::RSI _ta_rsi_5;
    ta::Crossover _ta_crossover_6;
    ta::Crossunder _ta_crossunder_7;
    ta::Crossunder _ta_crossunder_8;
    ta::Crossover _ta_crossover_9;
    int len7 = 0;
    int len21 = 0;
    int len50 = 0;
    int len200 = 0;
    int rsi_len = 0;
    int rsi_ob = 0;
    int rsi_os = 0;
    int rsi_mid = 0;
    double ema7 = 0.0;
    double ema21 = 0.0;
    double ema50 = 0.0;
    double ema200 = 0.0;
    double rsi_val = 0.0;
    bool bullish_macro = false;
    bool bearish_macro = false;
    bool rsi_long_ok = false;
    bool rsi_short_ok = false;
    bool long_trigger = false;
    bool short_trigger = false;
    bool long_exit = false;
    bool short_exit = false;
    bool perfect_bull = false;
    bool perfect_bear = false;
    bool _ta_initialized_ = false;

    explicit GeneratedStrategy() : _ta_ema_1(7), _ta_ema_2(21), _ta_ema_3(50), _ta_ema_4(200), _ta_rsi_5(14) {
        initial_capital_ = 1000000.0;
        default_qty_type_ = QtyType::FIXED;
        default_qty_value_ = 1.0;
        pyramiding_ = 1;
        commission_type_ = CommissionType::PERCENT;
        commission_value_ = 0.0;
        slippage_ = 0;
        script_has_strategy_close_ = true;
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

    void on_bar(const Bar& bar) override {
        if (!_ta_initialized_) {
            _ta_ema_1 = ta::EMA(get_input_int("EMA 7 (Fast Trigger)", 7));
            _ta_ema_2 = ta::EMA(get_input_int("EMA 21 (Slow Trigger)", 21));
            _ta_ema_3 = ta::EMA(get_input_int("EMA 50 (Trend & Dynamic Stop)", 50));
            _ta_ema_4 = ta::EMA(get_input_int("EMA 200 (Macro Trend)", 200));
            _ta_rsi_5 = ta::RSI(get_input_int("RSI Length", 14));
            _ta_initialized_ = true;
        }
        len7 = get_input_int("EMA 7 (Fast Trigger)", 7);
        len21 = get_input_int("EMA 21 (Slow Trigger)", 21);
        len50 = get_input_int("EMA 50 (Trend & Dynamic Stop)", 50);
        len200 = get_input_int("EMA 200 (Macro Trend)", 200);
        rsi_len = get_input_int("RSI Length", 14);
        rsi_ob = get_input_int("RSI Overbought (Max for Longs)", 70);
        rsi_os = get_input_int("RSI Oversold (Min for Shorts)", 30);
        rsi_mid = get_input_int("RSI Centerline (Momentum Shift)", 50);
        ema7 = (is_first_tick_ ? _ta_ema_1.compute(current_bar_.close) : _ta_ema_1.recompute(current_bar_.close));
        ema21 = (is_first_tick_ ? _ta_ema_2.compute(current_bar_.close) : _ta_ema_2.recompute(current_bar_.close));
        ema50 = (is_first_tick_ ? _ta_ema_3.compute(current_bar_.close) : _ta_ema_3.recompute(current_bar_.close));
        ema200 = (is_first_tick_ ? _ta_ema_4.compute(current_bar_.close) : _ta_ema_4.recompute(current_bar_.close));
        rsi_val = (is_first_tick_ ? _ta_rsi_5.compute(current_bar_.close) : _ta_rsi_5.recompute(current_bar_.close));
        bullish_macro = (ema50 > ema200);
        bearish_macro = (ema50 < ema200);
        rsi_long_ok = ((rsi_val > rsi_mid) && (rsi_val < rsi_ob));
        rsi_short_ok = ((rsi_val < rsi_mid) && (rsi_val > rsi_os));
        long_trigger = ((is_first_tick_ ? _ta_crossover_6.compute(ema7, ema21) : _ta_crossover_6.recompute(ema7, ema21)) && rsi_long_ok);
        short_trigger = ((is_first_tick_ ? _ta_crossunder_7.compute(ema7, ema21) : _ta_crossunder_7.recompute(ema7, ema21)) && rsi_short_ok);
        long_exit = ((is_first_tick_ ? _ta_crossunder_8.compute(ema7, ema21) : _ta_crossunder_8.recompute(ema7, ema21)) || (current_bar_.close < ema50));
        short_exit = ((is_first_tick_ ? _ta_crossover_9.compute(ema7, ema21) : _ta_crossover_9.recompute(ema7, ema21)) || (current_bar_.close > ema50));
        if ((bullish_macro && long_trigger)) {
            strategy_entry(std::string("Long"), true, na<double>(), na<double>(), na<double>(), std::string("Buy"));
        }
        if (((signed_position_size() > 0) && long_exit)) {
            strategy_close(std::string("Long"), std::string("Exit Long"), na<double>(), na<double>(), false);
        }
        if ((bearish_macro && short_trigger)) {
            strategy_entry(std::string("Short"), false, na<double>(), na<double>(), na<double>(), std::string("Sell"));
        }
        if (((signed_position_size() < 0) && short_exit)) {
            strategy_close(std::string("Short"), std::string("Exit Short"), na<double>(), na<double>(), false);
        }
        perfect_bull = (((ema7 > ema21) && (ema21 > ema50)) && (ema50 > ema200));
        perfect_bear = (((ema7 < ema21) && (ema21 < ema50)) && (ema50 < ema200));
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
