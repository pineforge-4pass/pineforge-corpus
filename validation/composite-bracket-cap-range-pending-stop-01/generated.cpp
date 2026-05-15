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
    ta::ATR _ta_atr_1;
    Series<double> _s_close;
    Series<double> _s_high;
    Series<double> _s_low;
    Series<double> _s_open;
    Series<double> atr_now;
    int i_tp_ticks = 0;
    int i_sl_ticks = 0;
    int i_max_fills = 0;
    int i_atr_len = 0;
    double i_atr_mult = 0.0;
    double prev_range = 0.0;
    double atr_prev = 0.0;
    bool expansion = false;
    bool prev_up = false;
    bool prev_down = false;
    bool arm_long = false;
    bool arm_short = false;
    double pos_qty = 0.0;
    int pos_dir = 0;
    double entry_px = 0.0;
    double tp_px = 0.0;
    double sl_px = 0.0;
    bool in_position = false;
    double exit_dir = 0.0;
    bool _ta_initialized_ = false;

    explicit GeneratedStrategy() : _ta_atr_1(14) {
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
        if (is_first_tick_) _s_high.push(current_bar_.high);
        else _s_high.update(current_bar_.high);
        if (is_first_tick_) _s_low.push(current_bar_.low);
        else _s_low.update(current_bar_.low);
        if (is_first_tick_) _s_open.push(current_bar_.open);
        else _s_open.update(current_bar_.open);
        if (!_ta_initialized_) {
            _ta_atr_1 = ta::ATR(get_input_int("ATR length", 14));
            _ta_initialized_ = true;
        }
        i_tp_ticks = get_input_int("Take profit (ticks)", 10);
        i_sl_ticks = get_input_int("Stop loss (ticks)", 10);
        i_max_fills = get_input_int("Max intraday filled orders", 5);
        i_atr_len = get_input_int("ATR length", 14);
        i_atr_mult = get_input_double("Range/ATR threshold", 1.0);
        max_intraday_filled_orders_ = (int)(i_max_fills);
        prev_range = (_s_high[1] - _s_low[1]);
        atr_now.push((is_first_tick_ ? _ta_atr_1.compute(current_bar_.high, current_bar_.low, current_bar_.close) : _ta_atr_1.recompute(current_bar_.high, current_bar_.low, current_bar_.close)));
        atr_prev = atr_now[1];
        expansion = (!(is_na(atr_prev)) && (prev_range > (atr_prev * i_atr_mult)));
        prev_up = (_s_close[1] > _s_open[1]);
        prev_down = (_s_close[1] < _s_open[1]);
        arm_long = (expansion && prev_up);
        arm_short = (expansion && prev_down);
        if (arm_long) {
            strategy_entry(std::string("LongOnGap"), true, na<double>(), _s_high[1], na<double>(), "", "", 0, -1);
        } else {
            strategy_cancel(std::string("LongOnGap"));
        }
        if (arm_short) {
            strategy_entry(std::string("ShortOnGap"), false, na<double>(), _s_low[1], na<double>(), "", "", 0, -1);
        } else {
            strategy_cancel(std::string("ShortOnGap"));
        }
        pos_qty = std::abs(signed_position_size());
        pos_dir = (((signed_position_size() > 0)) ? (1) : ((((signed_position_size() < 0)) ? ((-1)) : (0))));
        entry_px = position_entry_price_;
        tp_px = (entry_px + ((pos_dir * i_tp_ticks) * syminfo_.mintick));
        sl_px = (entry_px - ((pos_dir * i_sl_ticks) * syminfo_.mintick));
        in_position = (pos_qty > 0);
        exit_dir = (((pos_dir > 0)) ? (false) : (true));
        if (in_position) {
            strategy_order(std::string("BracketTP"), exit_dir, pos_qty, tp_px, na<double>(), std::string("bracket97"), 2);
            strategy_order(std::string("BracketSL"), exit_dir, pos_qty, na<double>(), sl_px, std::string("bracket97"), 2);
        } else {
            strategy_cancel(std::string("BracketTP"));
            strategy_cancel(std::string("BracketSL"));
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
