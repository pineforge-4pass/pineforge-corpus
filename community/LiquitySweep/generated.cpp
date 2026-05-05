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
    ta::Highest _ta_highest_1;
    ta::Lowest _ta_lowest_2;
    ta::Highest _ta_highest_3;
    ta::Lowest _ta_lowest_4;
    ta::ATR _ta_atr_5;
    Series<double> _s_high;
    Series<double> _s_low;
    Series<double> structureHigh;
    Series<double> structureLow;
    Series<bool> sweepHigh;
    Series<bool> sweepLow;
    int lookback = 0;
    int structureLen = 0;
    int atrLen = 0;
    double atrMult = 0.0;
    double rr = 0.0;
    double recentHigh = 0.0;
    double recentLow = 0.0;
    bool bullStructureBreak = false;
    bool bearStructureBreak = false;
    bool longCondition = false;
    bool shortCondition = false;
    double atrValue = 0.0;
    double longStop = 0.0;
    double longTake = 0.0;
    double shortStop = 0.0;
    double shortTake = 0.0;
    bool _ta_initialized_ = false;

    explicit GeneratedStrategy() : _ta_highest_1(20), _ta_lowest_2(20), _ta_highest_3(5), _ta_lowest_4(5), _ta_atr_5(14) {
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
        if (is_first_tick_) _s_high.push(current_bar_.high);
        else _s_high.update(current_bar_.high);
        if (is_first_tick_) _s_low.push(current_bar_.low);
        else _s_low.update(current_bar_.low);
        if (!_ta_initialized_) {
            _ta_highest_1 = ta::Highest(get_input_int("Liquidity Lookback", 20));
            _ta_lowest_2 = ta::Lowest(get_input_int("Liquidity Lookback", 20));
            _ta_highest_3 = ta::Highest(get_input_int("Structure Length", 5));
            _ta_lowest_4 = ta::Lowest(get_input_int("Structure Length", 5));
            _ta_atr_5 = ta::ATR(get_input_int("ATR Length", 14));
            _ta_initialized_ = true;
        }
        lookback = get_input_int("Liquidity Lookback", 20);
        structureLen = get_input_int("Structure Length", 5);
        atrLen = get_input_int("ATR Length", 14);
        atrMult = get_input_double("Stop ATR Multiplier", 1.5);
        rr = get_input_double("Risk Reward", 2.0);
        recentHigh = (is_first_tick_ ? _ta_highest_1.compute(_s_high[1]) : _ta_highest_1.recompute(_s_high[1]));
        recentLow = (is_first_tick_ ? _ta_lowest_2.compute(_s_low[1]) : _ta_lowest_2.recompute(_s_low[1]));
        sweepHigh.push(((current_bar_.high > recentHigh) && (current_bar_.close < recentHigh)));
        sweepLow.push(((current_bar_.low < recentLow) && (current_bar_.close > recentLow)));
        structureHigh.push((is_first_tick_ ? _ta_highest_3.compute(current_bar_.high) : _ta_highest_3.recompute(current_bar_.high)));
        structureLow.push((is_first_tick_ ? _ta_lowest_4.compute(current_bar_.low) : _ta_lowest_4.recompute(current_bar_.low)));
        bullStructureBreak = (current_bar_.close > structureHigh[1]);
        bearStructureBreak = (current_bar_.close < structureLow[1]);
        longCondition = (sweepLow[1] && bullStructureBreak);
        shortCondition = (sweepHigh[1] && bearStructureBreak);
        atrValue = (is_first_tick_ ? _ta_atr_5.compute(current_bar_.high, current_bar_.low, current_bar_.close) : _ta_atr_5.recompute(current_bar_.high, current_bar_.low, current_bar_.close));
        if ((longCondition && (signed_position_size() == 0))) {
            strategy_entry(std::string("Long"), true, na<double>(), na<double>(), na<double>(), "");
        }
        if ((shortCondition && (signed_position_size() == 0))) {
            strategy_entry(std::string("Short"), false, na<double>(), na<double>(), na<double>(), "");
        }
        longStop = (position_entry_price_ - (atrValue * atrMult));
        longTake = (position_entry_price_ + ((atrValue * atrMult) * rr));
        shortStop = (position_entry_price_ + (atrValue * atrMult));
        shortTake = (position_entry_price_ - ((atrValue * atrMult) * rr));
        strategy_exit(std::string("Exit Long"), std::string("Long"), longTake, longStop, na<double>(), na<double>(), na<double>(), 100.0, "");
        strategy_exit(std::string("Exit Short"), std::string("Short"), shortTake, shortStop, na<double>(), na<double>(), na<double>(), 100.0, "");
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
