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
    std::vector<double> _req_sec_lower_tf_0{};
    std::unordered_map<std::string, Series<double>> _security_helper_series_;
    ta::EMA _ta_ema_1;
    ta::EMA _ta_ema_2;
    ta::Crossover _ta_crossover_3;
    ta::Crossunder _ta_crossunder_4;
    std::vector<double> ltfCloses;
    double ltfSum = 0.0;
    double drift = 0.0;
    double emaFast = 0.0;
    double emaSlow = 0.0;
    bool entryCond = false;
    bool exitCond = false;
    bool _ta_initialized_ = false;

    explicit GeneratedStrategy() : _ta_ema_1(9), _ta_ema_2(21) {
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

    void configure_security_evaluators() override {
        security_eval_states_.clear();
        register_security_lower_tf_eval(0, "1", input_tf_);
    }

    void on_bar(const Bar& bar) override {
        ltfCloses = _req_sec_lower_tf_0;
        ltfSum = 0.0;
        if (((double)ltfCloses.size() > 0)) {
            for (int i = 0; i <= ((double)ltfCloses.size() - 1); i += 1) {
                ltfSum = (ltfSum + ltfCloses[i]);
            }
        }
        drift = (ltfSum - (15.0 * current_bar_.close));
        emaFast = (is_first_tick_ ? _ta_ema_1.compute(current_bar_.close) : _ta_ema_1.recompute(current_bar_.close));
        emaSlow = (is_first_tick_ ? _ta_ema_2.compute(current_bar_.close) : _ta_ema_2.recompute(current_bar_.close));
        entryCond = ((is_first_tick_ ? _ta_crossover_3.compute(emaFast, emaSlow) : _ta_crossover_3.recompute(emaFast, emaSlow)) && (std::abs(drift) > (current_bar_.close * 0.001)));
        exitCond = (is_first_tick_ ? _ta_crossunder_4.compute(emaFast, emaSlow) : _ta_crossunder_4.recompute(emaFast, emaSlow));
        if ((entryCond && (signed_position_size() == 0))) {
            strategy_entry(std::string("L"), true, na<double>(), na<double>(), 1, std::string("ltf-entry"), "", 0, -1);
        }
        if ((exitCond && (signed_position_size() > 0))) {
            strategy_close(std::string("L"), std::string("ltf-exit"), na<double>(), na<double>(), false);
        }
    }

    void _eval_security_0(const Bar& bar, bool is_complete) {
        if (security_lower_tf_sub_bar_index(0) == 0) _req_sec_lower_tf_0.clear();
        _req_sec_lower_tf_0.push_back(bar.close);
    }

    void evaluate_security(int sec_id, const Bar& bar, bool is_complete) override {
        switch (sec_id) {
            case 0: _eval_security_0(bar, is_complete); break;
        }
    }
    void clear_security(int sec_id) override {
        switch (sec_id) {
            case 0:
                _req_sec_lower_tf_0.clear();
                break;
        }
    }
};

extern "C" {
    void* strategy_create(const char* params_json) {
        return new GeneratedStrategy();
    }
    void run_backtest(void* s, Bar* bars, int n, ReportC* out) {
        auto* strat = static_cast<GeneratedStrategy*>(s);
        strat->run(bars, n, "", "", false, 4, MagnifierDistribution::ENDPOINTS);
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
        strat->run(bars, n, itf, stf, bar_magnifier != 0, magnifier_samples,
                   static_cast<MagnifierDistribution>(magnifier_dist));
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
