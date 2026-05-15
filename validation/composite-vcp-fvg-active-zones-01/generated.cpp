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
    std::vector<double> fvgTops;
    std::vector<double> fvgBottoms;
    std::vector<bool> fvgBullish;
    double atrVal = 0.0;
    bool bullFVG = false;
    bool bearFVG = false;
    double minFVGSize = 0.0;
    double fvgTop = 0.0;
    double fvgBottom = 0.0;
    bool inBullFVG = false;
    bool inBearFVG = false;
    bool _var_initialized = false;
    bool _ta_initialized_ = false;

    explicit GeneratedStrategy() : _ta_atr_1(14) {
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
        if (is_first_tick_) _s_open.push(current_bar_.open);
        else _s_open.update(current_bar_.open);
        if (!_var_initialized) {
            fvgTops = std::vector<double>();
            fvgBottoms = std::vector<double>();
            fvgBullish = std::vector<bool>();
            _var_initialized = true;
        } else {
        }
        atrVal = (is_first_tick_ ? _ta_atr_1.compute(current_bar_.high, current_bar_.low, current_bar_.close) : _ta_atr_1.recompute(current_bar_.high, current_bar_.low, current_bar_.close));
        bullFVG = ((current_bar_.low > _s_high[2]) && (_s_close[1] > _s_open[1]));
        bearFVG = ((current_bar_.high < _s_low[2]) && (_s_close[1] < _s_open[1]));
        minFVGSize = (atrVal * 0.3);
        fvgTop = na<double>();
        fvgBottom = na<double>();
        if ((bullFVG && ((current_bar_.low - _s_high[2]) >= minFVGSize))) {
            fvgTop = current_bar_.low;
            fvgBottom = _s_high[2];
            fvgTops.push_back(fvgTop);
            fvgBottoms.push_back(fvgBottom);
            fvgBullish.push_back(true);
        }
        if ((bearFVG && ((_s_low[2] - current_bar_.high) >= minFVGSize))) {
            fvgTop = _s_low[2];
            fvgBottom = current_bar_.high;
            fvgTops.push_back(fvgTop);
            fvgBottoms.push_back(fvgBottom);
            fvgBullish.push_back(false);
        }
        while (((double)fvgTops.size() > 30)) {
            [&](){ auto v=fvgTops.front(); fvgTops.erase(fvgTops.begin()); return v; }();
            [&](){ auto v=fvgBottoms.front(); fvgBottoms.erase(fvgBottoms.begin()); return v; }();
            [&](){ auto v=fvgBullish.front(); fvgBullish.erase(fvgBullish.begin()); return v; }();
        }
        inBullFVG = false;
        inBearFVG = false;
        if (((double)fvgTops.size() > 0)) {
            for (int i = 0; i <= ((double)fvgTops.size() - 1); i += 1) {
                double fTop = fvgTops[i];
                double fBottom = fvgBottoms[i];
                bool isBull = fvgBullish[i];
                if (((current_bar_.low <= fTop) && (current_bar_.high >= fBottom))) {
                    if (isBull) {
                        inBullFVG = true;
                    } else {
                        inBearFVG = true;
                    }
                }
            }
        }
        if ((inBullFVG && (signed_position_size() == 0))) {
            strategy_entry(std::string("L"), true, na<double>(), na<double>(), 1, std::string("bull-fvg touch"), "", 0, -1);
        }
        if ((inBearFVG && (signed_position_size() > 0))) {
            strategy_close(std::string("L"), std::string("bear-fvg touch exit"), na<double>(), na<double>(), false);
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
