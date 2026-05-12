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
    ta::SMA _ta_sma_1;
    ta::Crossover _ta_crossover_2;
    ta::Crossunder _ta_crossunder_3;
    double x_p;
    double x_v;
    double p00;
    double p01;
    double p10;
    double p11;
    double processNoisePos = 0.0;
    double processNoiseVel = 0.0;
    double measurementNoise = 0.0;
    int bandLookback = 0;
    double bandMultiplier = 0.0;
    double m = 0.0;
    double pPrime = 0.0;
    double vPrime = 0.0;
    double a00 = 0.0;
    double a01 = 0.0;
    double a10 = 0.0;
    double a11 = 0.0;
    double p00_ = 0.0;
    double p01_ = 0.0;
    double p10_ = 0.0;
    double p11_ = 0.0;
    double z = 0.0;
    double y = 0.0;
    double S = 0.0;
    double K0 = 0.0;
    double K1 = 0.0;
    double x_p_upd = 0.0;
    double x_v_upd = 0.0;
    double i00 = 0.0;
    double i01 = 0.0;
    double i10 = 0.0;
    double i11 = 0.0;
    double pp00 = 0.0;
    double pp01 = 0.0;
    double pp10 = 0.0;
    double pp11 = 0.0;
    double kalmanPrice = 0.0;
    double absDiff = 0.0;
    double mae = 0.0;
    double upperBand = 0.0;
    double lowerBand = 0.0;
    bool bullSignal = false;
    bool bearSignal = false;
    bool _var_initialized = false;
    bool _ta_initialized_ = false;

    explicit GeneratedStrategy() : _ta_sma_1(200), x_p(na<double>()), x_v(na<double>()), p00(na<double>()), p01(na<double>()), p10(na<double>()), p11(na<double>()) {
        initial_capital_ = 1000000.0;
        default_qty_type_ = QtyType::FIXED;
        default_qty_value_ = 1.0;
        pyramiding_ = 1;
        commission_type_ = CommissionType::PERCENT;
        commission_value_ = 0.0;
        slippage_ = 0;
        margin_long_ = 100.0;
        margin_short_ = 100.0;
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
            _ta_sma_1 = ta::SMA(get_input_int("Band Lookback for Abs Error", 200));
            _ta_initialized_ = true;
        }
        processNoisePos = get_input_double("Base Process Noise (Position)", 0.05);
        processNoiseVel = get_input_double("Base Process Noise (Velocity)", 0.0001);
        measurementNoise = get_input_double("Base Measurement Noise (R)", 250);
        bandLookback = get_input_int("Band Lookback for Abs Error", 200);
        bandMultiplier = get_input_double("Band Multiplier", 2.6);
        m = get_input_double("ATR Trailing Multiplier (Ref)", 7.88);
        if ((bar_index_ == 0)) {
            x_p = current_bar_.close;
            x_v = 0.0;
            p00 = 1.0;
            p01 = 0.0;
            p10 = 0.0;
            p11 = 1.0;
        }
        pPrime = (x_p + x_v);
        vPrime = x_v;
        a00 = ((1 * p00) + (1 * p10));
        a01 = ((1 * p01) + (1 * p11));
        a10 = ((0 * p00) + (1 * p10));
        a11 = ((0 * p01) + (1 * p11));
        p00_ = ((a00 * 1) + (a01 * 1));
        p01_ = ((a00 * 0) + (a01 * 1));
        p10_ = ((a10 * 1) + (a11 * 1));
        p11_ = ((a10 * 0) + (a11 * 1));
        p00_ += processNoisePos;
        p11_ += processNoiseVel;
        z = current_bar_.close;
        y = (z - pPrime);
        S = (p00_ + measurementNoise);
        K0 = ((double)(p00_) / (double)(S));
        K1 = ((double)(p10_) / (double)(S));
        x_p_upd = (pPrime + (K0 * y));
        x_v_upd = (vPrime + (K1 * y));
        i00 = (1 - K0);
        i01 = 0.0;
        i10 = (-K1);
        i11 = 1.0;
        pp00 = ((i00 * p00_) + (0.0 * p10_));
        pp01 = ((i00 * p01_) + (0.0 * p11_));
        pp10 = ((i10 * p00_) + (1.0 * p10_));
        pp11 = ((i10 * p01_) + (1.0 * p11_));
        x_p = x_p_upd;
        x_v = x_v_upd;
        p00 = pp00;
        p01 = pp01;
        p10 = pp10;
        p11 = pp11;
        kalmanPrice = x_p;
        absDiff = std::abs((current_bar_.close - kalmanPrice));
        mae = (is_first_tick_ ? _ta_sma_1.compute(absDiff) : _ta_sma_1.recompute(absDiff));
        upperBand = (kalmanPrice + (bandMultiplier * mae));
        lowerBand = (kalmanPrice - (bandMultiplier * mae));
        bullSignal = (is_first_tick_ ? _ta_crossover_2.compute(current_bar_.close, upperBand) : _ta_crossover_2.recompute(current_bar_.close, upperBand));
        bearSignal = (is_first_tick_ ? _ta_crossunder_3.compute(current_bar_.close, lowerBand) : _ta_crossunder_3.recompute(current_bar_.close, lowerBand));
        if (bullSignal) {
            strategy_entry(std::string("Long"), true, na<double>(), na<double>(), na<double>(), "");
        }
        if (bearSignal) {
            strategy_entry(std::string("Short"), false, na<double>(), na<double>(), na<double>(), "");
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
