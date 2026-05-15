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
    ta::PivotHigh _ta_pivothigh_1;
    ta::PivotLow _ta_pivotlow_2;
    ta::Highest _ta_highest_3;
    ta::Lowest _ta_lowest_4;
    ta::Crossover _ta_crossover_5;
    ta::Crossunder _ta_crossunder_6;
    double last_ph;
    double last_pl;
    int market_state;
    int prev_state;
    Series<double> roll_hi;
    Series<double> roll_lo;
    int i_pivot = 0;
    int i_window = 0;
    double ph = 0.0;
    double pl = 0.0;
    double ref_hi = 0.0;
    double ref_lo = 0.0;
    bool extreme_up = false;
    bool extreme_down = false;
    bool pivot_break_up = false;
    bool pivot_break_down = false;
    bool state_long_edge = false;
    bool state_short_edge = false;
    bool go_long = false;
    bool go_short = false;
    bool _var_initialized = false;
    bool _ta_initialized_ = false;

    explicit GeneratedStrategy() : _ta_pivothigh_1(5, 5), _ta_pivotlow_2(5, 5), _ta_highest_3(50), _ta_lowest_4(50), last_ph(na<double>()), last_pl(na<double>()), market_state(0), prev_state(0) {
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
            _ta_pivothigh_1 = ta::PivotHigh(get_input_int("Pivot strength", 5), get_input_int("Pivot strength", 5));
            _ta_pivotlow_2 = ta::PivotLow(get_input_int("Pivot strength", 5), get_input_int("Pivot strength", 5));
            _ta_highest_3 = ta::Highest(get_input_int("Rolling window length", 50));
            _ta_lowest_4 = ta::Lowest(get_input_int("Rolling window length", 50));
            _ta_initialized_ = true;
        }
        i_pivot = get_input_int("Pivot strength", 5);
        i_window = get_input_int("Rolling window length", 50);
        ph = (is_first_tick_ ? _ta_pivothigh_1.compute(current_bar_.high) : _ta_pivothigh_1.recompute(current_bar_.high));
        pl = (is_first_tick_ ? _ta_pivotlow_2.compute(current_bar_.low) : _ta_pivotlow_2.recompute(current_bar_.low));
        if (!(is_na(ph))) {
            last_ph = ph;
        }
        if (!(is_na(pl))) {
            last_pl = pl;
        }
        roll_hi.push((is_first_tick_ ? _ta_highest_3.compute(current_bar_.high) : _ta_highest_3.recompute(current_bar_.high)));
        roll_lo.push((is_first_tick_ ? _ta_lowest_4.compute(current_bar_.low) : _ta_lowest_4.recompute(current_bar_.low)));
        ref_hi = roll_hi[1];
        ref_lo = roll_lo[1];
        extreme_up = (is_first_tick_ ? _ta_crossover_5.compute(current_bar_.close, ref_lo) : _ta_crossover_5.recompute(current_bar_.close, ref_lo));
        extreme_down = (is_first_tick_ ? _ta_crossunder_6.compute(current_bar_.close, ref_hi) : _ta_crossunder_6.recompute(current_bar_.close, ref_hi));
        pivot_break_up = (!(is_na(last_ph)) && (current_bar_.close > last_ph));
        pivot_break_down = (!(is_na(last_pl)) && (current_bar_.close < last_pl));
        if (pivot_break_up) {
            market_state = 1;
        } else
        if (pivot_break_down) {
            market_state = (-1);
        }
        state_long_edge = ((market_state == 1) && (prev_state != 1));
        state_short_edge = ((market_state == (-1)) && (prev_state != (-1)));
        go_long = (state_long_edge || ((market_state == 1) && extreme_up));
        go_short = (state_short_edge || ((market_state == (-1)) && extreme_down));
        if ((go_long && (signed_position_size() <= 0))) {
            strategy_entry(std::string("L"), true, na<double>(), na<double>(), 1, std::string("integ bull"), "", 0, -1);
        }
        if ((go_short && (signed_position_size() >= 0))) {
            strategy_entry(std::string("S"), false, na<double>(), na<double>(), 1, std::string("integ bear"), "", 0, -1);
        }
        prev_state = market_state;
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
