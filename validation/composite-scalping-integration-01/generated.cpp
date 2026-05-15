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
    ta::Crossover _ta_crossover_3;
    ta::Crossunder _ta_crossunder_4;
    int i_fast = 0;
    int i_slow = 0;
    int i_tp_tk = 0;
    int i_sl_tk = 0;
    double ema_fast = 0.0;
    double ema_slow = 0.0;
    bool long_signal = false;
    bool short_signal = false;
    double entry_px = 0.0;
    double tp_px = 0.0;
    double sl_px = 0.0;
    bool _ta_initialized_ = false;

    explicit GeneratedStrategy() : _ta_ema_1(5), _ta_ema_2(13) {
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
            _ta_ema_1 = ta::EMA(get_input_int("Fast EMA", 5));
            _ta_ema_2 = ta::EMA(get_input_int("Slow EMA", 13));
            _ta_initialized_ = true;
        }
        i_fast = get_input_int("Fast EMA", 5);
        i_slow = get_input_int("Slow EMA", 13);
        i_tp_tk = get_input_int("Take profit (ticks)", 15);
        i_sl_tk = get_input_int("Stop loss (ticks)", 7);
        ema_fast = (is_first_tick_ ? _ta_ema_1.compute(current_bar_.close) : _ta_ema_1.recompute(current_bar_.close));
        ema_slow = (is_first_tick_ ? _ta_ema_2.compute(current_bar_.close) : _ta_ema_2.recompute(current_bar_.close));
        long_signal = (is_first_tick_ ? _ta_crossover_3.compute(ema_fast, ema_slow) : _ta_crossover_3.recompute(ema_fast, ema_slow));
        short_signal = (is_first_tick_ ? _ta_crossunder_4.compute(ema_fast, ema_slow) : _ta_crossunder_4.recompute(ema_fast, ema_slow));
        if (long_signal) {
            strategy_entry(std::string("L"), true, na<double>(), na<double>(), 1, std::string("integ scalp long"), "", 0, -1);
        }
        if (short_signal) {
            strategy_entry(std::string("S"), false, na<double>(), na<double>(), 1, std::string("integ scalp short"), "", 0, -1);
        }
        entry_px = position_entry_price_;
        if ((signed_position_size() > 0)) {
            tp_px = (entry_px + (i_tp_tk * syminfo_.mintick));
            sl_px = (entry_px - (i_sl_tk * syminfo_.mintick));
            strategy_exit(std::string("Brk"), std::string("L"), tp_px, sl_px, na<double>(), na<double>(), na<double>(), 100.0, "", na<double>(), "");
        }
        if ((signed_position_size() < 0)) {
            tp_px = (entry_px - (i_tp_tk * syminfo_.mintick));
            sl_px = (entry_px + (i_sl_tk * syminfo_.mintick));
            strategy_exit(std::string("Brk"), std::string("S"), tp_px, sl_px, na<double>(), na<double>(), na<double>(), 100.0, "", na<double>(), "");
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
