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
    ta::RSI _ta_rsi_1;
    ta::Crossover _ta_crossover_2;
    ta::Crossunder _ta_crossunder_3;
    bool long_armed;
    bool short_armed;
    int i_rsi_len = 0;
    double i_dip_lo = 0.0;
    double i_pop_lo = 0.0;
    double i_dip_hi = 0.0;
    double i_pop_hi = 0.0;
    double r = 0.0;
    bool long_fire = false;
    bool short_fire = false;
    bool _var_initialized = false;
    bool _ta_initialized_ = false;

    explicit GeneratedStrategy() : _ta_rsi_1(14), long_armed(false), short_armed(false) {
        initial_capital_ = 1000000.0;
        default_qty_type_ = QtyType::FIXED;
        default_qty_value_ = 1.0;
        pyramiding_ = 1;
        commission_type_ = CommissionType::PERCENT;
        commission_value_ = 0.0;
        slippage_ = 0;
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
        if (!_var_initialized) {
            _var_initialized = true;
        } else {
        }
        if (!_ta_initialized_) {
            _ta_rsi_1 = ta::RSI(get_input_int("RSI length", 14));
            _ta_initialized_ = true;
        }
        i_rsi_len = get_input_int("RSI length", 14);
        i_dip_lo = get_input_double("Long pullback band", 40);
        i_pop_lo = get_input_double("Long recovery line", 50);
        i_dip_hi = get_input_double("Short pullback band", 60);
        i_pop_hi = get_input_double("Short recovery line", 50);
        r = (is_first_tick_ ? _ta_rsi_1.compute(current_bar_.close) : _ta_rsi_1.recompute(current_bar_.close));
        if ((r < i_dip_lo)) {
            long_armed = true;
        }
        if ((r > i_dip_hi)) {
            short_armed = true;
        }
        long_fire = (long_armed && (is_first_tick_ ? _ta_crossover_2.compute(r, i_pop_lo) : _ta_crossover_2.recompute(r, i_pop_lo)));
        short_fire = (short_armed && (is_first_tick_ ? _ta_crossunder_3.compute(r, i_pop_hi) : _ta_crossunder_3.recompute(r, i_pop_hi)));
        if (long_fire) {
            long_armed = false;
        }
        if (short_fire) {
            short_armed = false;
        }
        if ((long_fire && (signed_position_size() <= 0))) {
            strategy_entry(std::string("L"), true, na<double>(), na<double>(), 1, std::string("rsi pullback long"), "", 0, -1);
        }
        if ((short_fire && (signed_position_size() >= 0))) {
            strategy_entry(std::string("S"), false, na<double>(), na<double>(), 1, std::string("rsi pullback short"), "", 0, -1);
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
        bool needs_full_run = (bar_magnifier != 0)
            || (!itf.empty() && !stf.empty() && itf != stf);
        if (!needs_full_run) {
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
