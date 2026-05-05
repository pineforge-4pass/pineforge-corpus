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

struct Stat {
    double last = na<double>();
    double xMean = na<double>();
    double xSd = na<double>();
    int n = 0;
    static Stat create() { return Stat{}; }
};

class GeneratedStrategy : public BacktestEngine {
public:
    ta::SMA _ta_sma_1;
    ta::StdDev _ta_stdev_2;
    ta::EMA _ta_ema_3;
    ta::EMA _ta_ema_4;
    ta::Crossover _ta_crossover_5;
    ta::Crossunder _ta_crossunder_6;
    Stat stat;
    int statLen = 0;
    double smaVal = 0.0;
    double sdVal = 0.0;
    double z = 0.0;
    double emaFast = 0.0;
    double emaSlow = 0.0;
    bool _var_initialized = false;
    bool _ta_initialized_ = false;

    explicit GeneratedStrategy() : _ta_sma_1(50), _ta_stdev_2(50), _ta_ema_3(9), _ta_ema_4(21) {
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

    int _udt_Stat_feed(Stat& self, double v, double m, double s) {
        self.last = v;
        self.xMean = m;
        self.xSd = s;
        self.n = (self.n + 1);
        return self.n;
    }

    double _udt_Stat_mean(Stat& self) {
        return self.xMean;
    }

    double _udt_Stat_variance(Stat& self) {
        double _m = _udt_Stat_mean(self);
        return (self.xSd * self.xSd);
    }

    double _udt_Stat_zscore(Stat& self) {
        double m = _udt_Stat_mean(self);
        double v2 = _udt_Stat_variance(self);
        double sd = std::sqrt(v2);
        return (((is_na(sd) || (sd == 0.0))) ? (0.0) : (((double)((self.last - m)) / (double)(sd))));
    }

    double _udt_Stat_refreshAndScore(Stat& self, double v, double m, double s) {
        int _ = _udt_Stat_feed(self, v, m, s);
        return _udt_Stat_zscore(self);
    }

    void on_bar(const Bar& bar) override {
        if (!_var_initialized) {
            stat = Stat{.last = na<double>(), .xMean = na<double>(), .xSd = na<double>(), .n = 0};
            _var_initialized = true;
        } else {
        }
        statLen = 50;
        smaVal = (is_first_tick_ ? _ta_sma_1.compute(current_bar_.close) : _ta_sma_1.recompute(current_bar_.close));
        sdVal = (is_first_tick_ ? _ta_stdev_2.compute(current_bar_.close) : _ta_stdev_2.recompute(current_bar_.close));
        z = _udt_Stat_refreshAndScore(stat, current_bar_.close, smaVal, sdVal);
        emaFast = (is_first_tick_ ? _ta_ema_3.compute(current_bar_.close) : _ta_ema_3.recompute(current_bar_.close));
        emaSlow = (is_first_tick_ ? _ta_ema_4.compute(current_bar_.close) : _ta_ema_4.recompute(current_bar_.close));
        if ((((is_first_tick_ ? _ta_crossover_5.compute(emaFast, emaSlow) : _ta_crossover_5.recompute(emaFast, emaSlow)) && (z > 1.0)) && (signed_position_size() == 0))) {
            strategy_entry(std::string("L"), true, na<double>(), na<double>(), 1, std::string("entry long"), "", 0, -1);
        }
        if (((is_first_tick_ ? _ta_crossunder_6.compute(emaFast, emaSlow) : _ta_crossunder_6.recompute(emaFast, emaSlow)) && (signed_position_size() > 0))) {
            strategy_close(std::string("L"), std::string("exit long"), na<double>(), na<double>(), false);
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
