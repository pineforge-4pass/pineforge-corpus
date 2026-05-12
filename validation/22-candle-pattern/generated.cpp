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
    Series<double> _s_close;
    Series<double> _s_open;
    Series<double> bodySize;
    int trendLen = 0;
    double trendMA = 0.0;
    bool upTrend = false;
    bool dnTrend = false;
    double upperWick = 0.0;
    double lowerWick = 0.0;
    double totalRange = 0.0;
    bool bullEngulf = false;
    bool bearEngulf = false;
    bool hammer = false;
    bool shootStar = false;
    bool doji = false;
    bool morningStar = false;
    bool bullSignal = false;
    bool bearSignal = false;
    bool _ta_initialized_ = false;

    explicit GeneratedStrategy() : _ta_sma_1(20) {
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
        if (is_first_tick_) _s_open.push(current_bar_.open);
        else _s_open.update(current_bar_.open);
        if (!_ta_initialized_) {
            _ta_sma_1 = ta::SMA(get_input_int("Trend MA Length", 20));
            _ta_initialized_ = true;
        }
        trendLen = get_input_int("Trend MA Length", 20);
        trendMA = (is_first_tick_ ? _ta_sma_1.compute(current_bar_.close) : _ta_sma_1.recompute(current_bar_.close));
        upTrend = (current_bar_.close > trendMA);
        dnTrend = (current_bar_.close < trendMA);
        bodySize.push(std::abs((current_bar_.close - current_bar_.open)));
        upperWick = (current_bar_.high - std::max((double)(current_bar_.open), (double)(current_bar_.close)));
        lowerWick = (std::min((double)(current_bar_.open), (double)(current_bar_.close)) - current_bar_.low);
        totalRange = (current_bar_.high - current_bar_.low);
        bullEngulf = ((((current_bar_.close > current_bar_.open) && (_s_close[1] < _s_open[1])) && (current_bar_.close > _s_open[1])) && (current_bar_.open < _s_close[1]));
        bearEngulf = ((((current_bar_.close < current_bar_.open) && (_s_close[1] > _s_open[1])) && (current_bar_.close < _s_open[1])) && (current_bar_.open > _s_close[1]));
        hammer = (((lowerWick > (bodySize[0] * 2)) && (upperWick < (bodySize[0] * 0.5))) && (bodySize[0] > 0));
        shootStar = (((upperWick > (bodySize[0] * 2)) && (lowerWick < (bodySize[0] * 0.5))) && (bodySize[0] > 0));
        doji = ((bodySize[0] < (totalRange * 0.1)) && (totalRange > 0));
        morningStar = ((((_s_close[2] < _s_open[2]) && (bodySize[1] < (bodySize[2] * 0.3))) && (current_bar_.close > current_bar_.open)) && (current_bar_.close > ((double)((_s_open[2] + _s_close[2])) / (double)(2))));
        bullSignal = (((bullEngulf || hammer) || morningStar) && dnTrend);
        bearSignal = ((bearEngulf || shootStar) && upTrend);
        if (bullSignal) {
            strategy_entry(std::string("Long"), true, na<double>(), na<double>(), na<double>(), "");
        }
        if (bearSignal) {
            strategy_entry(std::string("Short"), false, na<double>(), na<double>(), na<double>(), "");
        }
        if (((signed_position_size() > 0) && bearSignal)) {
            strategy_close(std::string("Long"), "", na<double>(), na<double>(), false);
        }
        if (((signed_position_size() < 0) && bullSignal)) {
            strategy_close(std::string("Short"), "", na<double>(), na<double>(), false);
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
