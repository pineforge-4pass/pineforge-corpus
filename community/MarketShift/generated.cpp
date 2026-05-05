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
    ta::HMA _ta_hma_1;
    ta::Crossover _ta_crossover_2;
    ta::Crossunder _ta_crossunder_3;
    ta::SMA _ta_sma_4;
    Series<double> _s_close;
    Series<double> _s_high;
    Series<double> _s_low;
    Series<double> _s_volume;
    double level;
    Series<bool> is_green_bar;
    Series<bool> is_red_bar;
    bool is_below_sma;
    bool is_above_sma;
    Series<double> hma1;
    Series<double> sma152;
    int length = 0;
    bool lbl_data = false;
    int color_shiftUp = 0;
    int color_shiftDn = 0;
    int ma_period = 0;
    bool allow_overnight = false;
    int close_hour = 0;
    int close_minute = 0;
    bool able_to_trade = false;
    double hma2 = 0.0;
    double vol_sum = 0.0;
    int shift_col = 0;
    bool is_close_time = false;
    bool long_entry = false;
    bool short_entry = false;
    bool _var_initialized = false;
    bool _ta_initialized_ = false;

    explicit GeneratedStrategy() : _ta_hma_1(55), _ta_sma_4(152), is_below_sma(false), is_above_sma(false) {
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
        if (is_first_tick_) _s_close.push(current_bar_.close);
        else _s_close.update(current_bar_.close);
        if (is_first_tick_) _s_high.push(current_bar_.high);
        else _s_high.update(current_bar_.high);
        if (is_first_tick_) _s_low.push(current_bar_.low);
        else _s_low.update(current_bar_.low);
        if (is_first_tick_) _s_volume.push(current_bar_.volume);
        else _s_volume.update(current_bar_.volume);
        if (!_var_initialized) {
            is_green_bar.push(false);
            is_red_bar.push(false);
            _var_initialized = true;
        } else {
            if (is_first_tick_) is_green_bar.push(is_green_bar[0]);
            if (is_first_tick_) is_red_bar.push(is_red_bar[0]);
        }
        if (!_ta_initialized_) {
            _ta_hma_1 = ta::HMA(get_input_int("Market Shift Length", 55));
            _ta_initialized_ = true;
        }
        length = get_input_int("Market Shift Length", 55);
        lbl_data = (get_input_string("Label Data: ", std::string("Volume")) == std::string("Volume"));
        color_shiftUp = get_input_int("Shift Up Color", pine_color::new_color(((int64_t)(36) << 16 | (int64_t)(213) << 8 | (int64_t)(128)), 0));
        color_shiftDn = get_input_int("Shift Down Color", pine_color::new_color(((int64_t)(225) << 16 | (int64_t)(43) << 8 | (int64_t)(43)), 0));
        ma_period = 152;
        allow_overnight = get_input_bool("Allow Overnight Position", false);
        close_hour = 4;
        close_minute = 55;
        able_to_trade = true;
        hma1.push((is_first_tick_ ? _ta_hma_1.compute(current_bar_.close) : _ta_hma_1.recompute(current_bar_.close)));
        hma2 = hma1[5];
        vol_sum = ((_s_volume[2] + _s_volume[1]) + _s_volume[0]);
        if ((is_first_tick_ ? _ta_crossover_2.compute(hma1[0], hma2) : _ta_crossover_2.recompute(hma1[0], hma2))) {
            level = current_bar_.low;
        }
        if ((is_first_tick_ ? _ta_crossunder_3.compute(hma1[0], hma2) : _ta_crossunder_3.recompute(hma1[0], hma2))) {
            level = current_bar_.high;
        }
        sma152.push((is_first_tick_ ? _ta_sma_4.compute(current_bar_.close) : _ta_sma_4.recompute(current_bar_.close)));
        is_red_bar.update((current_bar_.close < level));
        is_green_bar.update(!(is_red_bar[0]));
        is_below_sma = (current_bar_.close < sma152[0]);
        is_above_sma = !(is_below_sma);
        shift_col = ((is_red_bar[0]) ? (color_shiftDn) : (color_shiftUp));
        is_close_time = false;
        if (!(allow_overnight)) {
            is_close_time = ((_bar_hour() == 4) && (_bar_minute() >= 55));
        }
        if ((is_close_time && (signed_position_size() != 0))) {
            if ((signed_position_size() > 0)) {
                strategy_close(std::string("Long"), std::string("Daily Close"), na<double>(), na<double>(), false);
            } else
            if ((signed_position_size() < 0)) {
                strategy_close(std::string("Short"), std::string("Daily Close"), na<double>(), na<double>(), false);
            }
        }
        if ((((signed_position_size() > 0) && is_red_bar[0]) && (current_bar_.close < position_entry_price_))) {
            strategy_close(std::string("Long"), std::string("紅棒且低於入場價"), na<double>(), na<double>(), false);
        }
        if ((((signed_position_size() > 0) && is_red_bar[0]) && is_below_sma)) {
            strategy_close(std::string("Long"), std::string("紅棒且低於SMA152"), na<double>(), na<double>(), false);
        }
        if ((signed_position_size() > 0)) {
            if (((is_green_bar[1] && is_green_bar[2]) && (_s_close[1] < sma152[1]))) {
                if (is_red_bar[0]) {
                    strategy_close(std::string("Long"), std::string("連續綠棒且低於SMA152，且紅棒"), na<double>(), na<double>(), false);
                }
            }
        }
        if ((((signed_position_size() < 0) && is_green_bar[0]) && (current_bar_.close > position_entry_price_))) {
            strategy_close(std::string("Short"), std::string("綠棒且高於入場價"), na<double>(), na<double>(), false);
        }
        if ((((signed_position_size() < 0) && is_green_bar[0]) && is_above_sma)) {
            strategy_close(std::string("Short"), std::string("綠棒且高於SMA152"), na<double>(), na<double>(), false);
        }
        if ((signed_position_size() < 0)) {
            if (((is_red_bar[0] && is_red_bar[1]) && (_s_close[1] > sma152[1]))) {
                if (is_green_bar[0]) {
                    strategy_close(std::string("Short"), std::string("連續紅棒且高於SMA152，且綠棒"), na<double>(), na<double>(), false);
                }
            }
        }
        long_entry = (is_green_bar[0] && is_above_sma);
        if ((long_entry && (signed_position_size() <= 0))) {
            if ((signed_position_size() < 0)) {
                strategy_close(std::string("Short"), "", na<double>(), na<double>(), false);
            }
            strategy_entry(std::string("Long"), true, na<double>(), na<double>(), na<double>(), "");
        }
        short_entry = (is_red_bar[0] && is_below_sma);
        if ((short_entry && (signed_position_size() >= 0))) {
            if ((signed_position_size() > 0)) {
                strategy_close(std::string("Long"), "", na<double>(), na<double>(), false);
            }
            strategy_entry(std::string("Short"), false, na<double>(), na<double>(), na<double>(), "");
        }
        if ((((_s_high[2] < level) && (current_bar_.high < level)) && (_s_high[1] > level))) {
        }
        if ((((_s_low[2] > level) && (_s_low[1] < level)) && (current_bar_.low > level))) {
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
