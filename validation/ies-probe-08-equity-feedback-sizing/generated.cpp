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
    ta::DMI _ta_dmi_1;
    ta::ATR _ta_atr_2;
    double entryStop;
    double entryTP;
    double i_risk_pct = 0.0;
    double i_atr_stop_mult = 0.0;
    double i_atr_tp_mult = 0.0;
    double i_adx_trend = 0.0;
    double i_trend_mult = 0.0;
    double i_neutral_mult = 0.0;
    double i_quality_mult = 0.0;
    bool trending_regime = false;
    double regime_size_mult = 0.0;
    double atr_val = 0.0;
    double long_risk = 0.0;
    double account_risk = 0.0;
    double long_position_size = 0.0;
    bool long_entry = false;
    bool _var_initialized = false;
    bool _ta_initialized_ = false;

    explicit GeneratedStrategy() : _ta_dmi_1(14, 14), _ta_atr_2(14), entryStop(na<double>()), entryTP(na<double>()) {
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
        i_risk_pct = get_input_double("Risk Per Trade %", 1.0);
        i_atr_stop_mult = get_input_double("ATR Stop Multiplier", 2.0);
        i_atr_tp_mult = get_input_double("ATR Target Multiplier", 4.0);
        i_adx_trend = get_input_double("ADX Trend Threshold", 25);
        i_trend_mult = get_input_double("Trending Size Mult", 1.2);
        i_neutral_mult = get_input_double("Neutral Size Mult", 1.0);
        i_quality_mult = get_input_double("Quality Mult (fixed)", 1.15);
        auto _result__ta_dmi_1 = (is_first_tick_ ? _ta_dmi_1.compute(current_bar_.high, current_bar_.low, current_bar_.close) : _ta_dmi_1.recompute(current_bar_.high, current_bar_.low, current_bar_.close));
        double plus_di = _result__ta_dmi_1.diplus;
        double minus_di = _result__ta_dmi_1.diminus;
        double adx_val = _result__ta_dmi_1.adx;
        trending_regime = (adx_val >= i_adx_trend);
        regime_size_mult = ((trending_regime) ? (i_trend_mult) : (i_neutral_mult));
        atr_val = (is_first_tick_ ? _ta_atr_2.compute(current_bar_.high, current_bar_.low, current_bar_.close) : _ta_atr_2.recompute(current_bar_.high, current_bar_.low, current_bar_.close));
        long_risk = (atr_val * i_atr_stop_mult);
        account_risk = ((((current_equity() + open_profit(current_bar_.close)) * ((double)(i_risk_pct) / (double)(100.0))) * regime_size_mult) * i_quality_mult);
        long_position_size = (((long_risk > 0)) ? (((double)(account_risk) / (double)(long_risk))) : (0.0));
        long_entry = (((((_bar_hour() == 0) && (_bar_minute() == 15)) && (signed_position_size() == 0)) && !(is_na(atr_val))) && !(is_na(adx_val)));
        if (long_entry) {
            entryStop = (current_bar_.close - (atr_val * i_atr_stop_mult));
            entryTP = (current_bar_.close + (atr_val * i_atr_tp_mult));
            strategy_entry(std::string("L"), true, na<double>(), na<double>(), long_position_size, std::string("dyn qty long"), "", 0, -1);
            strategy_exit(std::string("LX"), std::string("L"), entryTP, entryStop, na<double>(), na<double>(), na<double>(), 100.0, std::string("bracket"));
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
