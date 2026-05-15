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
    ta::Highest _ta_highest_1;
    ta::Lowest _ta_lowest_2;
    ta::Highest _ta_highest_3;
    ta::Lowest _ta_lowest_4;
    Series<double> entryLower;
    Series<double> entryUpper;
    Series<double> exitLower;
    Series<double> exitUpper;
    int entryLen = 0;
    int exitLen = 0;
    double entryMid = 0.0;
    bool longCond = false;
    bool shortCond = false;
    bool _ta_initialized_ = false;

    explicit GeneratedStrategy() : _ta_highest_1(20), _ta_lowest_2(20), _ta_highest_3(10), _ta_lowest_4(10) {
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
            _ta_highest_1 = ta::Highest(get_input_int("Entry Channel Length", 20));
            _ta_lowest_2 = ta::Lowest(get_input_int("Entry Channel Length", 20));
            _ta_highest_3 = ta::Highest(get_input_int("Exit Channel Length", 10));
            _ta_lowest_4 = ta::Lowest(get_input_int("Exit Channel Length", 10));
            _ta_initialized_ = true;
        }
        entryLen = get_input_int("Entry Channel Length", 20);
        exitLen = get_input_int("Exit Channel Length", 10);
        entryUpper.push((is_first_tick_ ? _ta_highest_1.compute(current_bar_.high) : _ta_highest_1.recompute(current_bar_.high)));
        entryLower.push((is_first_tick_ ? _ta_lowest_2.compute(current_bar_.low) : _ta_lowest_2.recompute(current_bar_.low)));
        entryMid = ((double)((entryUpper[0] + entryLower[0])) / (double)(2));
        exitUpper.push((is_first_tick_ ? _ta_highest_3.compute(current_bar_.high) : _ta_highest_3.recompute(current_bar_.high)));
        exitLower.push((is_first_tick_ ? _ta_lowest_4.compute(current_bar_.low) : _ta_lowest_4.recompute(current_bar_.low)));
        longCond = (current_bar_.close > entryUpper[1]);
        shortCond = (current_bar_.close < entryLower[1]);
        if (longCond) {
            strategy_entry(std::string("Long"), true, na<double>(), na<double>(), na<double>(), "");
        }
        if (shortCond) {
            strategy_entry(std::string("Short"), false, na<double>(), na<double>(), na<double>(), "");
        }
        if (((signed_position_size() > 0) && (current_bar_.close < exitLower[1]))) {
            strategy_close(std::string("Long"), "", na<double>(), na<double>(), false);
        }
        if (((signed_position_size() < 0) && (current_bar_.close > exitUpper[1]))) {
            strategy_close(std::string("Short"), "", na<double>(), na<double>(), false);
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
