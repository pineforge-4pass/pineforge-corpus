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
    ta::ATR _ta_atr_2;
    ta::Crossover _ta_crossover_3;
    ta::Crossunder _ta_crossunder_4;
    Series<double> x;
    Series<double> p;
    double i_q = 0.0;
    double i_r = 0.0;
    int i_band_len = 0;
    int i_atr_len = 0;
    double i_atr_mult = 0.0;
    double x_pred = 0.0;
    double p_pred = 0.0;
    double k_gain = 0.0;
    bool kalman_bull = false;
    bool kalman_bear = false;
    double band = 0.0;
    double vol = 0.0;
    double upper = 0.0;
    double lower = 0.0;
    bool break_up = false;
    bool break_down = false;
    bool go_long = false;
    bool go_short = false;
    bool _var_initialized = false;
    bool _ta_initialized_ = false;

    explicit GeneratedStrategy() : _ta_ema_1(20), _ta_atr_2(14) {
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
            x.push(na<double>());
            p.push(1);
            _var_initialized = true;
        } else {
            if (is_first_tick_) x.push(x[0]);
            if (is_first_tick_) p.push(p[0]);
        }
        if (!_ta_initialized_) {
            _ta_ema_1 = ta::EMA(get_input_int("Band EMA length", 20));
            _ta_atr_2 = ta::ATR(get_input_int("ATR length", 14));
            _ta_initialized_ = true;
        }
        i_q = get_input_double("Process variance Q", 0.001);
        i_r = get_input_double("Measurement variance R", 0.1);
        i_band_len = get_input_int("Band EMA length", 20);
        i_atr_len = get_input_int("ATR length", 14);
        i_atr_mult = get_input_double("ATR multiplier", 1.5);
        x_pred = (is_na(x[1]) ? current_bar_.close : x[1]);
        p_pred = ((is_na(p[1]) ? 1.0 : p[1]) + i_q);
        k_gain = ((double)(p_pred) / (double)((p_pred + i_r)));
        x.update((x_pred + (k_gain * (current_bar_.close - x_pred))));
        p.update(((1 - k_gain) * p_pred));
        kalman_bull = (x[0] > (is_na(x[1]) ? x[0] : x[1]));
        kalman_bear = (x[0] < (is_na(x[1]) ? x[0] : x[1]));
        band = (is_first_tick_ ? _ta_ema_1.compute(current_bar_.close) : _ta_ema_1.recompute(current_bar_.close));
        vol = (is_first_tick_ ? _ta_atr_2.compute(current_bar_.high, current_bar_.low, current_bar_.close) : _ta_atr_2.recompute(current_bar_.high, current_bar_.low, current_bar_.close));
        upper = (band + (vol * i_atr_mult));
        lower = (band - (vol * i_atr_mult));
        break_up = (is_first_tick_ ? _ta_crossover_3.compute(current_bar_.close, upper) : _ta_crossover_3.recompute(current_bar_.close, upper));
        break_down = (is_first_tick_ ? _ta_crossunder_4.compute(current_bar_.close, lower) : _ta_crossunder_4.recompute(current_bar_.close, lower));
        go_long = (kalman_bull && break_up);
        go_short = (kalman_bear && break_down);
        if ((go_long && (signed_position_size() <= 0))) {
            strategy_entry(std::string("L"), true, na<double>(), na<double>(), 1, std::string("integ long"), "", 0, -1);
        }
        if ((go_short && (signed_position_size() >= 0))) {
            strategy_entry(std::string("S"), false, na<double>(), na<double>(), 1, std::string("integ short"), "", 0, -1);
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
