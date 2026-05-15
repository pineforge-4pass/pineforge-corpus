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
    math::Sum _ta_sum_1;
    ta::Crossover _ta_crossover_2;
    ta::Crossunder _ta_crossunder_3;
    Series<double> _s_close;
    Series<double> kama;
    int i_kama_len = 0;
    int i_kama_fast = 0;
    int i_kama_slow = 0;
    double change_n = 0.0;
    double vol_sum = 0.0;
    double er = 0.0;
    double fast_sc = 0.0;
    double slow_sc = 0.0;
    double sc = 0.0;
    bool cross_up = false;
    bool cross_down = false;
    bool _var_initialized = false;
    bool _ta_initialized_ = false;

    explicit GeneratedStrategy() : _ta_sum_1(14) {
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
        if (is_first_tick_) _s_close.push(current_bar_.close);
        else _s_close.update(current_bar_.close);
        if (!_var_initialized) {
            kama.push(na<double>());
            _var_initialized = true;
        } else {
            if (is_first_tick_) kama.push(kama[0]);
        }
        if (!_ta_initialized_) {
            _ta_sum_1 = math::Sum(get_input_int("KAMA length", 14));
            _ta_initialized_ = true;
        }
        i_kama_len = get_input_int("KAMA length", 14);
        i_kama_fast = get_input_int("KAMA fast end", 2);
        i_kama_slow = get_input_int("KAMA slow end", 30);
        change_n = std::abs((current_bar_.close - _s_close[i_kama_len]));
        vol_sum = (is_first_tick_ ? _ta_sum_1.compute(std::abs((current_bar_.close - _s_close[1]))) : _ta_sum_1.recompute(std::abs((current_bar_.close - _s_close[1]))));
        er = (((vol_sum > 0)) ? (((double)(change_n) / (double)(vol_sum))) : (0.0));
        fast_sc = ((double)(2.0) / (double)((i_kama_fast + 1)));
        slow_sc = ((double)(2.0) / (double)((i_kama_slow + 1)));
        sc = std::pow(((er * (fast_sc - slow_sc)) + slow_sc), 2);
        kama.update(((is_na(kama[1])) ? (current_bar_.close) : ((kama[1] + (sc * (current_bar_.close - kama[1]))))));
        cross_up = (is_first_tick_ ? _ta_crossover_2.compute(current_bar_.close, kama[0]) : _ta_crossover_2.recompute(current_bar_.close, kama[0]));
        cross_down = (is_first_tick_ ? _ta_crossunder_3.compute(current_bar_.close, kama[0]) : _ta_crossunder_3.recompute(current_bar_.close, kama[0]));
        if ((cross_up && (signed_position_size() <= 0))) {
            strategy_entry(std::string("L"), true, na<double>(), na<double>(), 1, std::string("kama cross up"), "", 0, -1);
        }
        if ((cross_down && (signed_position_size() >= 0))) {
            strategy_entry(std::string("S"), false, na<double>(), na<double>(), 1, std::string("kama cross down"), "", 0, -1);
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
