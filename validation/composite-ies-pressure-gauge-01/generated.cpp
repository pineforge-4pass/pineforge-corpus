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
    Series<bool> pressure_bear;
    Series<bool> pressure_bull;
    Series<double> pressure_smooth;
    int i_pressure_len = 0;
    int i_pressure_smooth = 0;
    int i_pressure_mom = 0;
    double i_pressure_high = 0.0;
    double i_pressure_low = 0.0;
    double i_pressure_thresh = 0.0;
    double range_val = 0.0;
    double raw_buy = 0.0;
    double pressure_ratio = 0.0;
    double pressure_momentum = 0.0;
    int pressure_state = 0;
    bool long_entry = false;
    bool short_entry = false;
    bool _ta_initialized_ = false;

    explicit GeneratedStrategy() : _ta_ema_1(14), _ta_ema_2(5) {
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
            _ta_ema_1 = ta::EMA(get_input_int("Pressure Period", 14));
            _ta_ema_2 = ta::EMA(get_input_int("Pressure Smoothing", 5));
            _ta_initialized_ = true;
        }
        i_pressure_len = get_input_int("Pressure Period", 14);
        i_pressure_smooth = get_input_int("Pressure Smoothing", 5);
        i_pressure_mom = get_input_int("Pressure Momentum", 10);
        i_pressure_high = get_input_double("Extreme Buy Level", 0.7);
        i_pressure_low = get_input_double("Extreme Sell Level", 0.3);
        i_pressure_thresh = get_input_double("Momentum Threshold", 0.05);
        range_val = (current_bar_.high - current_bar_.low);
        raw_buy = (((range_val > 0)) ? (((double)((current_bar_.close - current_bar_.low)) / (double)(range_val))) : (0.5));
        pressure_ratio = (is_first_tick_ ? _ta_ema_1.compute(raw_buy) : _ta_ema_1.recompute(raw_buy));
        pressure_smooth.push((is_first_tick_ ? _ta_ema_2.compute(pressure_ratio) : _ta_ema_2.recompute(pressure_ratio)));
        pressure_momentum = (pressure_smooth[0] - pressure_smooth[i_pressure_mom]);
        pressure_state = 0;
        if ((pressure_smooth[0] >= i_pressure_high)) {
            pressure_state = 2;
        } else
        if ((pressure_smooth[0] > (0.5 + i_pressure_thresh))) {
            pressure_state = 1;
        } else
        if ((pressure_smooth[0] <= i_pressure_low)) {
            pressure_state = (-2);
        } else
        if ((pressure_smooth[0] < (0.5 - i_pressure_thresh))) {
            pressure_state = (-1);
        }
        pressure_bull.push(((pressure_state >= 1) || (pressure_momentum > i_pressure_thresh)));
        pressure_bear.push(((pressure_state <= (-1)) || (pressure_momentum < (-i_pressure_thresh))));
        long_entry = ((pressure_bull[0] && !(pressure_bull[1])) && (signed_position_size() <= 0));
        short_entry = ((pressure_bear[0] && !(pressure_bear[1])) && (signed_position_size() >= 0));
        if (long_entry) {
            if ((signed_position_size() < 0)) {
                strategy_close(std::string("S"), std::string("flip"), na<double>(), na<double>(), false);
            }
            strategy_entry(std::string("L"), true, na<double>(), na<double>(), 1, std::string("pressure bull"), "", 0, -1);
        }
        if (short_entry) {
            if ((signed_position_size() > 0)) {
                strategy_close(std::string("L"), std::string("flip"), na<double>(), na<double>(), false);
            }
            strategy_entry(std::string("S"), false, na<double>(), na<double>(), 1, std::string("pressure bear"), "", 0, -1);
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
