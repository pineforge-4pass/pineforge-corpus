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
    Series<bool> bearish_bias;
    Series<bool> bullish_bias;
    int i_ma_fast = 0;
    int i_ma_slow = 0;
    int i_ma_trend = 0;
    double i_bias_thresh = 0.0;
    double ma_fast = 0.0;
    double ma_slow = 0.0;
    double ma_trend = 0.0;
    bool ma_bullish = false;
    bool ma_bearish = false;
    bool price_above_structure = false;
    bool price_below_structure = false;
    double bias_score = 0.0;
    double bear_bias_score = 0.0;
    bool long_entry = false;
    bool short_entry = false;
    bool _ta_initialized_ = false;

    explicit GeneratedStrategy() : _ta_ema_1(21), _ta_ema_2(55), _ta_ema_3(200) {
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
            _ta_ema_1 = ta::EMA(get_input_int("Fast MA", 21));
            _ta_ema_2 = ta::EMA(get_input_int("Slow MA", 55));
            _ta_ema_3 = ta::EMA(get_input_int("Trend MA", 200));
            _ta_initialized_ = true;
        }
        i_ma_fast = get_input_int("Fast MA", 21);
        i_ma_slow = get_input_int("Slow MA", 55);
        i_ma_trend = get_input_int("Trend MA", 200);
        i_bias_thresh = get_input_double("Bias Threshold", 30);
        ma_fast = (is_first_tick_ ? _ta_ema_1.compute(current_bar_.close) : _ta_ema_1.recompute(current_bar_.close));
        ma_slow = (is_first_tick_ ? _ta_ema_2.compute(current_bar_.close) : _ta_ema_2.recompute(current_bar_.close));
        ma_trend = (is_first_tick_ ? _ta_ema_3.compute(current_bar_.close) : _ta_ema_3.recompute(current_bar_.close));
        ma_bullish = ((ma_fast > ma_slow) && (ma_slow > ma_trend));
        ma_bearish = ((ma_fast < ma_slow) && (ma_slow < ma_trend));
        price_above_structure = ((current_bar_.close > ma_fast) && (current_bar_.close > ma_slow));
        price_below_structure = ((current_bar_.close < ma_fast) && (current_bar_.close < ma_slow));
        bias_score = 0.0;
        if (ma_bullish) {
            bias_score += 30;
        }
        if (price_above_structure) {
            bias_score += 20;
        }
        if ((current_bar_.close > ma_trend)) {
            bias_score += 20;
        }
        bear_bias_score = 0.0;
        if (ma_bearish) {
            bear_bias_score += 30;
        }
        if (price_below_structure) {
            bear_bias_score += 20;
        }
        if ((current_bar_.close < ma_trend)) {
            bear_bias_score += 20;
        }
        bullish_bias.push((bias_score >= i_bias_thresh));
        bearish_bias.push((bear_bias_score >= i_bias_thresh));
        long_entry = ((bullish_bias[0] && !(bullish_bias[1])) && (signed_position_size() <= 0));
        short_entry = ((bearish_bias[0] && !(bearish_bias[1])) && (signed_position_size() >= 0));
        if (long_entry) {
            if ((signed_position_size() < 0)) {
                strategy_close(std::string("S"), std::string("flip"), na<double>(), na<double>(), false);
            }
            strategy_entry(std::string("L"), true, na<double>(), na<double>(), 1, std::string("bias rising long"), "", 0, -1);
        }
        if (short_entry) {
            if ((signed_position_size() > 0)) {
                strategy_close(std::string("L"), std::string("flip"), na<double>(), na<double>(), false);
            }
            strategy_entry(std::string("S"), false, na<double>(), na<double>(), 1, std::string("bias rising short"), "", 0, -1);
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
