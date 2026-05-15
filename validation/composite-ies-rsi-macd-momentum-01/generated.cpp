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
    ta::RSI _ta_rsi_1;
    ta::EMA _ta_ema_2;
    ta::EMA _ta_ema_3;
    ta::EMA _ta_ema_4;
    Series<double> macd_hist;
    Series<int> momentum_bear_score;
    Series<int> momentum_bull_score;
    Series<double> rsi;
    int i_rsi_len = 0;
    double i_rsi_bull = 0.0;
    double i_rsi_bear = 0.0;
    int i_macd_fast = 0;
    int i_macd_slow = 0;
    int i_macd_sig = 0;
    bool rsi_bullish = false;
    bool rsi_bearish = false;
    bool rsi_momentum_up = false;
    bool rsi_momentum_dn = false;
    double macd_line = 0.0;
    double macd_signal = 0.0;
    bool macd_bullish = false;
    bool macd_bearish = false;
    bool long_entry = false;
    bool short_entry = false;
    bool _ta_initialized_ = false;

    explicit GeneratedStrategy() : _ta_rsi_1(14), _ta_ema_2(12), _ta_ema_3(26), _ta_ema_4(9) {
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
            _ta_rsi_1 = ta::RSI(get_input_int("RSI Period", 14));
            _ta_ema_2 = ta::EMA(get_input_int("MACD Fast", 12));
            _ta_ema_3 = ta::EMA(get_input_int("MACD Slow", 26));
            _ta_ema_4 = ta::EMA(get_input_int("MACD Signal", 9));
            _ta_initialized_ = true;
        }
        i_rsi_len = get_input_int("RSI Period", 14);
        i_rsi_bull = get_input_double("RSI Bullish Level", 55);
        i_rsi_bear = get_input_double("RSI Bearish Level", 45);
        i_macd_fast = get_input_int("MACD Fast", 12);
        i_macd_slow = get_input_int("MACD Slow", 26);
        i_macd_sig = get_input_int("MACD Signal", 9);
        rsi.push((is_first_tick_ ? _ta_rsi_1.compute(current_bar_.close) : _ta_rsi_1.recompute(current_bar_.close)));
        rsi_bullish = (rsi[0] > i_rsi_bull);
        rsi_bearish = (rsi[0] < i_rsi_bear);
        rsi_momentum_up = (rsi[0] > rsi[3]);
        rsi_momentum_dn = (rsi[0] < rsi[3]);
        macd_line = ((is_first_tick_ ? _ta_ema_2.compute(current_bar_.close) : _ta_ema_2.recompute(current_bar_.close)) - (is_first_tick_ ? _ta_ema_3.compute(current_bar_.close) : _ta_ema_3.recompute(current_bar_.close)));
        macd_signal = (is_first_tick_ ? _ta_ema_4.compute(macd_line) : _ta_ema_4.recompute(macd_line));
        macd_hist.push((macd_line - macd_signal));
        macd_bullish = ((macd_hist[0] > 0) && (macd_hist[0] > macd_hist[1]));
        macd_bearish = ((macd_hist[0] < 0) && (macd_hist[0] < macd_hist[1]));
        momentum_bull_score.push(0);
        if (rsi_bullish) {
            momentum_bull_score.update(momentum_bull_score[0] + 1);
        }
        if (rsi_momentum_up) {
            momentum_bull_score.update(momentum_bull_score[0] + 1);
        }
        if (macd_bullish) {
            momentum_bull_score.update(momentum_bull_score[0] + 1);
        }
        momentum_bear_score.push(0);
        if (rsi_bearish) {
            momentum_bear_score.update(momentum_bear_score[0] + 1);
        }
        if (rsi_momentum_dn) {
            momentum_bear_score.update(momentum_bear_score[0] + 1);
        }
        if (macd_bearish) {
            momentum_bear_score.update(momentum_bear_score[0] + 1);
        }
        long_entry = (((momentum_bull_score[0] >= 2) && (momentum_bull_score[1] < 2)) && (signed_position_size() <= 0));
        short_entry = (((momentum_bear_score[0] >= 2) && (momentum_bear_score[1] < 2)) && (signed_position_size() >= 0));
        if (long_entry) {
            if ((signed_position_size() < 0)) {
                strategy_close(std::string("S"), std::string("flip"), na<double>(), na<double>(), false);
            }
            strategy_entry(std::string("L"), true, na<double>(), na<double>(), 1, std::string("mom 2of3 long"), "", 0, -1);
        }
        if (short_entry) {
            if ((signed_position_size() > 0)) {
                strategy_close(std::string("L"), std::string("flip"), na<double>(), na<double>(), false);
            }
            strategy_entry(std::string("S"), false, na<double>(), na<double>(), 1, std::string("mom 2of3 short"), "", 0, -1);
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
