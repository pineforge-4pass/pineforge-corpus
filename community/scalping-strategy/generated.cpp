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
    ta::RSI _ta_rsi_3;
    ta::DMI _ta_dmi_4;
    ta::ATR _ta_atr_5;
    ta::Crossover _ta_crossover_6;
    ta::Crossunder _ta_crossunder_7;
    int emaFast = 0;
    int emaSlow = 0;
    int rsiLen = 0;
    int rsiOB = 0;
    int rsiOS = 0;
    int adxLen = 0;
    int adxLevel = 0;
    int atrLen = 0;
    double atrMult = 0.0;
    double ema50 = 0.0;
    double ema200 = 0.0;
    double rsi = 0.0;
    double atr = 0.0;
    double body = 0.0;
    double candleRange = 0.0;
    bool strongBull = false;
    bool strongBear = false;
    bool trendLong = false;
    bool trendShort = false;
    bool rsiLong = false;
    bool rsiShort = false;
    bool trendStrength = false;
    bool longCondition = false;
    bool shortCondition = false;
    double longStop = 0.0;
    double shortStop = 0.0;
    double longTP = 0.0;
    double shortTP = 0.0;
    bool _ta_initialized_ = false;

    explicit GeneratedStrategy() : _ta_ema_1(50), _ta_ema_2(200), _ta_rsi_3(3), _ta_dmi_4(5, 5), _ta_atr_5(14) {
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
            _ta_ema_1 = ta::EMA(get_input_int("emaFast", 50));
            _ta_ema_2 = ta::EMA(get_input_int("emaSlow", 200));
            _ta_rsi_3 = ta::RSI(get_input_int("rsiLen", 3));
            _ta_dmi_4 = ta::DMI(get_input_int("adxLen", 5), get_input_int("adxLen", 5));
            _ta_atr_5 = ta::ATR(get_input_int("atrLen", 14));
            _ta_initialized_ = true;
        }
        emaFast = get_input_int("emaFast", 50);
        emaSlow = get_input_int("emaSlow", 200);
        rsiLen = get_input_int("rsiLen", 3);
        rsiOB = get_input_int("rsiOB", 80);
        rsiOS = get_input_int("rsiOS", 20);
        adxLen = get_input_int("adxLen", 5);
        adxLevel = get_input_int("adxLevel", 20);
        atrLen = get_input_int("atrLen", 14);
        atrMult = get_input_double("atrMult", 1.2);
        ema50 = (is_first_tick_ ? _ta_ema_1.compute(current_bar_.close) : _ta_ema_1.recompute(current_bar_.close));
        ema200 = (is_first_tick_ ? _ta_ema_2.compute(current_bar_.close) : _ta_ema_2.recompute(current_bar_.close));
        rsi = (is_first_tick_ ? _ta_rsi_3.compute(current_bar_.close) : _ta_rsi_3.recompute(current_bar_.close));
        auto _result__ta_dmi_4 = (is_first_tick_ ? _ta_dmi_4.compute(current_bar_.high, current_bar_.low, current_bar_.close) : _ta_dmi_4.recompute(current_bar_.high, current_bar_.low, current_bar_.close));
        double diplus = _result__ta_dmi_4.diplus;
        double diminus = _result__ta_dmi_4.diminus;
        double adx = _result__ta_dmi_4.adx;
        atr = (is_first_tick_ ? _ta_atr_5.compute(current_bar_.high, current_bar_.low, current_bar_.close) : _ta_atr_5.recompute(current_bar_.high, current_bar_.low, current_bar_.close));
        body = std::abs((current_bar_.close - current_bar_.open));
        candleRange = (current_bar_.high - current_bar_.low);
        strongBull = ((current_bar_.close > current_bar_.open) && (body > (candleRange * 0.5)));
        strongBear = ((current_bar_.close < current_bar_.open) && (body > (candleRange * 0.5)));
        trendLong = ((current_bar_.close > ema50) && (ema50 > ema200));
        trendShort = ((current_bar_.close < ema50) && (ema50 < ema200));
        rsiLong = (is_first_tick_ ? _ta_crossover_6.compute(rsi, rsiOS) : _ta_crossover_6.recompute(rsi, rsiOS));
        rsiShort = (is_first_tick_ ? _ta_crossunder_7.compute(rsi, rsiOB) : _ta_crossunder_7.recompute(rsi, rsiOB));
        trendStrength = (adx > adxLevel);
        longCondition = (((trendLong && rsiLong) && trendStrength) && strongBull);
        shortCondition = (((trendShort && rsiShort) && trendStrength) && strongBear);
        longStop = (current_bar_.close - (atr * atrMult));
        shortStop = (current_bar_.close + (atr * atrMult));
        longTP = (current_bar_.close + ((current_bar_.close - longStop) * 2));
        shortTP = (current_bar_.close - ((shortStop - current_bar_.close) * 2));
        if (longCondition) {
            strategy_entry(std::string("BUY"), true, na<double>(), na<double>(), na<double>(), "");
            strategy_exit(std::string("TP/SL BUY"), std::string("BUY"), longTP, longStop, atr, na<double>(), na<double>(), 100.0, "", na<double>(), "");
        }
        if (shortCondition) {
            strategy_entry(std::string("SELL"), false, na<double>(), na<double>(), na<double>(), "");
            strategy_exit(std::string("TP/SL SELL"), std::string("SELL"), shortTP, shortStop, atr, na<double>(), na<double>(), 100.0, "", na<double>(), "");
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
