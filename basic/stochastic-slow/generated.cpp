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
    ta::Stoch _ta_stoch_1;
    ta::SMA _ta_sma_2;
    ta::SMA _ta_sma_3;
    ta::Crossover _ta_crossover_4;
    ta::Crossunder _ta_crossunder_5;
    int length = 0;
    double OverBought = 0.0;
    double OverSold = 0.0;
    int smoothK = 0;
    int smoothD = 0;
    double k = 0.0;
    double d = 0.0;
    bool co = false;
    bool cu = false;
    bool _ta_initialized_ = false;

    explicit GeneratedStrategy() : _ta_stoch_1(14), _ta_sma_2(3), _ta_sma_3(3) {
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
        if (!_ta_initialized_) {
            _ta_stoch_1 = ta::Stoch(get_input_int("Length", 14));
            _ta_initialized_ = true;
        }
        length = get_input_int("Length", 14);
        OverBought = get_input_double("Overbought", 80);
        OverSold = get_input_double("Oversold", 20);
        smoothK = 3;
        smoothD = 3;
        k = (is_first_tick_ ? _ta_sma_2.compute((is_first_tick_ ? _ta_stoch_1.compute(current_bar_.close, current_bar_.high, current_bar_.low) : _ta_stoch_1.recompute(current_bar_.close, current_bar_.high, current_bar_.low))) : _ta_sma_2.recompute((is_first_tick_ ? _ta_stoch_1.compute(current_bar_.close, current_bar_.high, current_bar_.low) : _ta_stoch_1.recompute(current_bar_.close, current_bar_.high, current_bar_.low))));
        d = (is_first_tick_ ? _ta_sma_3.compute(k) : _ta_sma_3.recompute(k));
        co = (is_first_tick_ ? _ta_crossover_4.compute(k, d) : _ta_crossover_4.recompute(k, d));
        cu = (is_first_tick_ ? _ta_crossunder_5.compute(k, d) : _ta_crossunder_5.recompute(k, d));
        if ((!(is_na(k)) && !(is_na(d)))) {
            if ((co && (k < OverSold))) {
                strategy_entry(std::string("StochLE"), true, na<double>(), na<double>(), na<double>(), std::string("StochLE"));
            }
            if ((cu && (k > OverBought))) {
                strategy_entry(std::string("StochSE"), false, na<double>(), na<double>(), na<double>(), std::string("StochSE"));
            }
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
