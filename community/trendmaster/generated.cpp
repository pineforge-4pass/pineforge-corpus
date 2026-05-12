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

const int SLOption_Percent = 0;
const int SLOption_MACross = 1;
const int SLOption_Band = 2;
static const std::string SLOption_str_values[] = {std::string("%"), std::string("MA Cross"), std::string("Band")};

const int TSOption_Percent = 0;
const int TSOption_Ticks = 1;
static const std::string TSOption_str_values[] = {std::string("%"), std::string("Ticks")};

class GeneratedStrategy : public BacktestEngine {
public:
    ta::ATR _ta_atr_1;
    ta::SMA _ta_sma_2;
    ta::EMA _ta_ema_3;
    ta::SMA _ta_sma_4;
    ta::EMA _ta_ema_5;
    ta::SMA _ta_sma_6;
    ta::SMA _ta_sma_2_cs1;
    ta::Crossover _ta_crossover_7;
    ta::Crossunder _ta_crossunder_8;
    ta::Change _ta_change_9;
    ta::Change _ta_change_10;
    ta::Cross _ta_cross_11;
    ta::Cross _ta_cross_12;
    ta::EMA _ta_ema_13;
    ta::SMA _ta_sma_14;
    ta::SMA _ta_sma_2_cs2;
    ta::SMA _ta_sma_15;
    ta::StdDev _ta_stdev_16;
    ta::ATR _ta_atr_17;
    ta::RSI _ta_rsi_18;
    ta::MACD _ta_macd_19;
    ta::Stoch _ta_stoch_20;
    ta::SMA _ta_sma_21;
    ta::SMA _ta_sma_22;
    ta::DMI _ta_dmi_23;
    ta::EMA _ta_ema_24;
    ta::SMA _ta_sma_25;
    ta::SMA _ta_sma_2_cs3;
    ta::EMA _ta_ema_26;
    ta::SMA _ta_sma_27;
    ta::SMA _ta_sma_2_cs4;
    double maCrossValue;
    double basis;
    double upperBand;
    double lowerBand;
    std::string tpslGroup;
    double longQty;
    double longEntry;
    double longTP;
    double longSL;
    int longTPTriggered;
    int longSLTriggered;
    double shortQty;
    double shortEntry;
    double shortTP;
    double shortSL;
    int shortTPTriggered;
    int shortSLTriggered;
    double entryPrice;
    int tradesPerSessionCounterAsia;
    int tradesPerSessionCounterLondon;
    int tradesPerSessionCounterNYAM;
    int tradesPerSessionCounterNYPM;
    Series<bool> inAsiaSession;
    Series<bool> inLondonSession;
    Series<bool> inNYAMSession;
    Series<bool> inNYPMSession;
    Series<double> smma;
    Series<double> _strat_position_size;
    std::string maType = std::string("");
    bool enableTrendFilter = false;
    std::string trendMaType = std::string("");
    int trendMaLength = 0;
    int shortMALength = 0;
    int longMALength = 0;
    int atrLength = 0;
    bool useAtrMultiplier = false;
    double atrMultiplierSL = 0.0;
    bool useVolatilityFilter = false;
    bool useTrendFilter = false;
    int bbLength = 0;
    double bbMultiplier = 0.0;
    bool useRSI = false;
    int rsiLength = 0;
    int rsiLongThreshold = 0;
    int rsiShortThreshold = 0;
    bool useMACD = false;
    int macdFastLength = 0;
    int macdSlowLength = 0;
    int macdSignalSmoothing = 0;
    bool useStochastic = false;
    int stochLength = 0;
    int stochSmoothing = 0;
    int stochOverbought = 0;
    int stochOversold = 0;
    bool useADX = false;
    int adxLength = 0;
    int adxThreshold = 0;
    double riskRewardRatio = 0.0;
    int slTicksOffset = 0;
    int bandLength = 0;
    int bandType = 0;
    double atr = 0.0;
    double stopLossOffset = 0.0;
    double shortMA = 0.0;
    double longMA = 0.0;
    bool buyCondition = false;
    bool sellCondition = false;
    double trendMA = 0.0;
    bool trendFilterLong = false;
    bool trendFilterShort = false;
    double dev = 0.0;
    double atrValue = 0.0;
    bool volatilityFilter = false;
    bool bbTrendFilterLong = false;
    bool bbTrendFilterShort = false;
    double rsi = 0.0;
    bool rsiFilterLong = false;
    bool rsiFilterShort = false;
    bool macdFilterLong = false;
    bool macdFilterShort = false;
    double k = 0.0;
    double d = 0.0;
    bool stochFilterLong = false;
    bool stochFilterShort = false;
    bool adxFilter = false;
    bool buyConditionFiltered = false;
    bool sellConditionFiltered = false;
    double maHigh = 0.0;
    double maLow = 0.0;
    bool asiaEnabled = false;
    std::string sessionAsiaTime = std::string("");
    int colorAsia = 0;
    bool londonEnabled = false;
    std::string sessionLondonTime = std::string("");
    int colorLondon = 0;
    bool nyAmEnabled = false;
    std::string sessionNYAMTime = std::string("");
    int colorNYAM = 0;
    bool nyPmEnabled = false;
    std::string sessionNYPMTime = std::string("");
    int colorNYPM = 0;
    bool showMon = false;
    bool showTue = false;
    bool showWed = false;
    bool showThu = false;
    bool showFri = false;
    std::string sessionAsia = std::string("");
    std::string sessionLondon = std::string("");
    std::string sessionNYAM = std::string("");
    std::string sessionNYPM = std::string("");
    int maxTradesPerSessionAsia = 0;
    int maxTradesPerSessionLondon = 0;
    int maxTradesPerSessionNYAM = 0;
    int maxTradesPerSessionNYPM = 0;
    int marketCloseHour = 0;
    int marketCloseMinute = 0;
    bool useAutoRisk = false;
    int autoRiskAmountMin = 0;
    int autoRiskAmountMax = 0;
    double fixedContactSize = 0.0;
    int tpReachedLimit = 0;
    int slReachedLimit = 0;
    double rrRatio = 0.0;
    bool useSL = false;
    double slOption = 0.0;
    double slPercent = 0.0;
    bool useTP = false;
    bool isInTradingSession = false;
    bool longEntrySignal = false;
    bool shortEntrySignal = false;
    bool longExitSignal = false;
    bool shortExitSignal = false;
    double slPlot1 = 0.0;
    double entryPlot1 = 0.0;
    double tpPlot1 = 0.0;
    double slPlot2 = 0.0;
    double entryPlot2 = 0.0;
    double tpPlot2 = 0.0;
    bool alertEnabled = false;
    std::string alertMessageBuy = std::string("");
    std::string alertMessageSell = std::string("");
    Series<double> smma_cs1;
    Series<double> smma_cs2;
    Series<double> smma_cs3;
    Series<double> smma_cs4;
    bool _var_initialized = false;
    bool _ta_initialized_ = false;

    explicit GeneratedStrategy() : _ta_atr_1(14), _ta_sma_2(9), _ta_ema_3(9), _ta_sma_4(9), _ta_ema_5(21), _ta_sma_6(21), _ta_sma_2_cs1(21), _ta_ema_13(50), _ta_sma_14(50), _ta_sma_2_cs2(50), _ta_sma_15(20), _ta_stdev_16(20), _ta_atr_17(20), _ta_rsi_18(14), _ta_macd_19(12, 26, 9), _ta_stoch_20(14), _ta_sma_21(3), _ta_sma_22(3), _ta_dmi_23(14, 14), _ta_ema_24(20), _ta_sma_25(20), _ta_sma_2_cs3(20), _ta_ema_26(20), _ta_sma_27(20), _ta_sma_2_cs4(20), maCrossValue(na<double>()), basis(na<double>()), upperBand(na<double>()), lowerBand(na<double>()), longQty(na<double>()), longEntry(na<double>()), longTP(na<double>()), longSL(na<double>()), longTPTriggered(0), longSLTriggered(0), shortQty(na<double>()), shortEntry(na<double>()), shortTP(na<double>()), shortSL(na<double>()), shortTPTriggered(0), shortSLTriggered(0), entryPrice(na<double>()), tradesPerSessionCounterAsia(0), tradesPerSessionCounterLondon(0), tradesPerSessionCounterNYAM(0), tradesPerSessionCounterNYPM(0) {
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

    double _fn_smma_cs0(double src, int len) {
        smma.push(0.0);
        smma.update(((is_na(smma[1])) ? ((is_first_tick_ ? _ta_sma_2.compute(src) : _ta_sma_2.recompute(src))) : (((double)(((smma[1] * (len - 1)) + src)) / (double)(len)))));
        return smma[0];
    }

    double _fn_smma_cs1(double src, int len) {
        smma_cs1.push(0.0);
        smma_cs1.update(((is_na(smma_cs1[1])) ? ((is_first_tick_ ? _ta_sma_2_cs1.compute(src) : _ta_sma_2_cs1.recompute(src))) : (((double)(((smma_cs1[1] * (len - 1)) + src)) / (double)(len)))));
        return smma_cs1[0];
    }

    double _fn_smma_cs2(double src, int len) {
        smma_cs2.push(0.0);
        smma_cs2.update(((is_na(smma_cs2[1])) ? ((is_first_tick_ ? _ta_sma_2_cs2.compute(src) : _ta_sma_2_cs2.recompute(src))) : (((double)(((smma_cs2[1] * (len - 1)) + src)) / (double)(len)))));
        return smma_cs2[0];
    }

    double _fn_smma_cs3(double src, int len) {
        smma_cs3.push(0.0);
        smma_cs3.update(((is_na(smma_cs3[1])) ? ((is_first_tick_ ? _ta_sma_2_cs3.compute(src) : _ta_sma_2_cs3.recompute(src))) : (((double)(((smma_cs3[1] * (len - 1)) + src)) / (double)(len)))));
        return smma_cs3[0];
    }

    double _fn_smma_cs4(double src, int len) {
        smma_cs4.push(0.0);
        smma_cs4.update(((is_na(smma_cs4[1])) ? ((is_first_tick_ ? _ta_sma_2_cs4.compute(src) : _ta_sma_2_cs4.recompute(src))) : (((double)(((smma_cs4[1] * (len - 1)) + src)) / (double)(len)))));
        return smma_cs4[0];
    }

    std::tuple<double, double> _getSlopes_cs0(double source1, double source2) {
        double m1 = (is_first_tick_ ? _ta_change_9.compute(source1) : _ta_change_9.recompute(source1));
        double m2 = (is_first_tick_ ? _ta_change_10.compute(source2) : _ta_change_10.recompute(source2));
        return std::make_tuple(m1, m2);
    }

    double _commonScalingFactor(double source1, double source2, double m1, double m2) {
        return ((double)((source1 - source2)) / (double)((m1 - m2)));
    }

    double _crossingValue_cs0(double source1, double source2) {
        double out = na<double>();
        auto [m1, m2] = _getSlopes_cs0(source1, source2);
        if ((is_first_tick_ ? _ta_cross_11.compute(source1, source2) : _ta_cross_11.recompute(source1, source2))) {
            out = (source1 - (m1 * _commonScalingFactor(source1, source2, m1, m2)));
        }
        return out;
    }

    std::string _addSessionWeekdays(std::string sessionString) {
        std::string out = sessionString;
        bool hasAnySelected = ((((showMon || showTue) || showWed) || showThu) || showFri);
        if (hasAnySelected) {
            out = (sessionString + std::string(":"));
            if (showMon) {
                out += std::string("2");
            }
            if (showTue) {
                out += std::string("3");
            }
            if (showWed) {
                out += std::string("4");
            }
            if (showThu) {
                out += std::string("5");
            }
            if (showFri) {
                out += std::string("6");
            }
        }
        return out;
    }

    bool isInSession(std::string sess, std::string res) {
        bool out = !(is_na(pine_time(current_bar_.timestamp, res, sess, std::string(""), script_tf_)));
        return out;
    }

    double _bracketPercentage(double price, double sign, double value) {
        return (price * (1 + ((double)((sign * value)) / (double)(100))));
    }

    bool canGoLong() {
        return ((((true && isInTradingSession) && ((((false || (inAsiaSession[0] && (((int)trades_.size()) < (tradesPerSessionCounterAsia + maxTradesPerSessionAsia)))) || (inLondonSession[0] && (((int)trades_.size()) < (tradesPerSessionCounterLondon + maxTradesPerSessionLondon)))) || (inNYAMSession[0] && (((int)trades_.size()) < (tradesPerSessionCounterNYAM + maxTradesPerSessionNYAM)))) || (inNYPMSession[0] && (((int)trades_.size()) < (tradesPerSessionCounterNYPM + maxTradesPerSessionNYPM))))) && (longSLTriggered < slReachedLimit)) && (longTPTriggered < tpReachedLimit));
    }

    double _sl(double p, int s) {
        int sign = (((s) > 0) - ((s) < 0));
        double _func_ret = 0.0;
        auto __switch_val_0 = slOption;
        if (__switch_val_0 == SLOption_MACross) {
            _func_ret = maCrossValue;
        }
        else if (__switch_val_0 == SLOption_Band) {
            _func_ret = (((sign == 1)) ? (maLow) : ((((sign == (-1))) ? (maHigh) : (na<double>()))));
        }
        else {
            _func_ret = _bracketPercentage(p, (-sign), slPercent);
        }
        return _func_ret;
    }

    double _calcEntryQty(double sl) {
        double slInTicks = ((double)(sl) / (double)(syminfo_.mintick));
        double slInMoney = (slInTicks * syminfo_.pointvalue);
        double contracts = (std::round(((double)(autoRiskAmountMax) / (double)(slInMoney)) * std::pow(10.0, 0)) / std::pow(10.0, 0));
        double riskPerContracts = (contracts * slInMoney);
        double out = ((((autoRiskAmountMin <= riskPerContracts) && (riskPerContracts <= autoRiskAmountMax))) ? (contracts) : (0));
        return out;
    }

    double _tp(double p, double s, double sl, double rrr) {
        return (p + (((((s) > 0) - ((s) < 0)) * rrr) * std::abs((p - sl))));
    }

    bool canGoShort() {
        return ((((true && isInTradingSession) && ((((false || (inAsiaSession[0] && (((int)trades_.size()) < (tradesPerSessionCounterAsia + maxTradesPerSessionAsia)))) || (inLondonSession[0] && (((int)trades_.size()) < (tradesPerSessionCounterLondon + maxTradesPerSessionLondon)))) || (inNYAMSession[0] && (((int)trades_.size()) < (tradesPerSessionCounterNYAM + maxTradesPerSessionNYAM)))) || (inNYPMSession[0] && (((int)trades_.size()) < (tradesPerSessionCounterNYPM + maxTradesPerSessionNYPM))))) && (shortSLTriggered < slReachedLimit)) && (shortTPTriggered < tpReachedLimit));
    }

    void on_bar(const Bar& bar) override {
        _strat_position_size.push(signed_position_size());
        if (!_var_initialized) {
            _var_initialized = true;
        } else {
        }
        if (!_ta_initialized_) {
            _ta_sma_2 = ta::SMA(get_input_int("Short-Term MA Length", 9));
            _ta_ema_3 = ta::EMA(get_input_int("Short-Term MA Length", 9));
            _ta_sma_4 = ta::SMA(get_input_int("Short-Term MA Length", 9));
            _ta_ema_5 = ta::EMA(get_input_int("Long-Term MA Length", 21));
            _ta_sma_6 = ta::SMA(get_input_int("Long-Term MA Length", 21));
            _ta_sma_2_cs1 = ta::SMA(get_input_int("Long-Term MA Length", 21));
            _ta_ema_13 = ta::EMA(get_input_int("Trend MA Length", 50));
            _ta_sma_14 = ta::SMA(get_input_int("Trend MA Length", 50));
            _ta_sma_2_cs2 = ta::SMA(get_input_int("Trend MA Length", 50));
            _ta_ema_24 = ta::EMA(get_input_int("Band Length", 20));
            _ta_sma_25 = ta::SMA(get_input_int("Band Length", 20));
            _ta_sma_2_cs3 = ta::SMA(get_input_int("Band Length", 20));
            _ta_ema_26 = ta::EMA(get_input_int("Band Length", 20));
            _ta_sma_27 = ta::SMA(get_input_int("Band Length", 20));
            _ta_sma_2_cs4 = ta::SMA(get_input_int("Band Length", 20));
            _ta_initialized_ = true;
        }
        maType = get_input_string("MA Type", std::string("SMA"));
        enableTrendFilter = get_input_bool("Enable Higher Timeframe Trend", false);
        trendMaType = get_input_string("Trend MA Type", std::string("EMA"));
        trendMaLength = get_input_int("Trend MA Length", 50);
        shortMALength = get_input_int("Short-Term MA Length", 9);
        longMALength = get_input_int("Long-Term MA Length", 21);
        atrLength = 14;
        useAtrMultiplier = true;
        atrMultiplierSL = 2.0;
        useVolatilityFilter = true;
        useTrendFilter = true;
        bbLength = 20;
        bbMultiplier = 2.0;
        useRSI = true;
        rsiLength = 14;
        rsiLongThreshold = 55;
        rsiShortThreshold = 45;
        useMACD = true;
        macdFastLength = 12;
        macdSlowLength = 26;
        macdSignalSmoothing = 9;
        useStochastic = true;
        stochLength = 14;
        stochSmoothing = 3;
        stochOverbought = 80;
        stochOversold = 20;
        useADX = true;
        adxLength = 14;
        adxThreshold = 25;
        riskRewardRatio = 2.0;
        slTicksOffset = 5;
        bandLength = get_input_int("Band Length", 20);
        bandType = get_input_int("Band Type", 2);
        atr = (is_first_tick_ ? _ta_atr_1.compute(current_bar_.high, current_bar_.low, current_bar_.close) : _ta_atr_1.recompute(current_bar_.high, current_bar_.low, current_bar_.close));
        stopLossOffset = ((true) ? ((atr * 2.0)) : (atr));
        shortMA = (((maType == std::string("EMA"))) ? ((is_first_tick_ ? _ta_ema_3.compute(current_bar_.close) : _ta_ema_3.recompute(current_bar_.close))) : ((((maType == std::string("SMA"))) ? ((is_first_tick_ ? _ta_sma_4.compute(current_bar_.close) : _ta_sma_4.recompute(current_bar_.close))) : (_fn_smma_cs0(current_bar_.close, shortMALength)))));
        longMA = (((maType == std::string("EMA"))) ? ((is_first_tick_ ? _ta_ema_5.compute(current_bar_.close) : _ta_ema_5.recompute(current_bar_.close))) : ((((maType == std::string("SMA"))) ? ((is_first_tick_ ? _ta_sma_6.compute(current_bar_.close) : _ta_sma_6.recompute(current_bar_.close))) : (_fn_smma_cs1(current_bar_.close, longMALength)))));
        buyCondition = (is_first_tick_ ? _ta_crossover_7.compute(shortMA, longMA) : _ta_crossover_7.recompute(shortMA, longMA));
        sellCondition = (is_first_tick_ ? _ta_crossunder_8.compute(shortMA, longMA) : _ta_crossunder_8.recompute(shortMA, longMA));
        if ((is_first_tick_ ? _ta_cross_12.compute(shortMA, longMA) : _ta_cross_12.recompute(shortMA, longMA))) {
            maCrossValue = _crossingValue_cs0(shortMA, longMA);
        }
        trendMA = (((trendMaType == std::string("EMA"))) ? ((is_first_tick_ ? _ta_ema_13.compute(current_bar_.close) : _ta_ema_13.recompute(current_bar_.close))) : ((((trendMaType == std::string("SMA"))) ? ((is_first_tick_ ? _ta_sma_14.compute(current_bar_.close) : _ta_sma_14.recompute(current_bar_.close))) : (_fn_smma_cs2(current_bar_.close, trendMaLength)))));
        trendFilterLong = (!(enableTrendFilter) || (current_bar_.close > trendMA));
        trendFilterShort = (!(enableTrendFilter) || (current_bar_.close < trendMA));
        if ((true || true)) {
            basis = (is_first_tick_ ? _ta_sma_15.compute(current_bar_.close) : _ta_sma_15.recompute(current_bar_.close));
            dev = (is_first_tick_ ? _ta_stdev_16.compute(current_bar_.close) : _ta_stdev_16.recompute(current_bar_.close));
            upperBand = (basis + (2.0 * dev));
            lowerBand = (basis - (2.0 * dev));
        }
        atrValue = (is_first_tick_ ? _ta_atr_17.compute(current_bar_.high, current_bar_.low, current_bar_.close) : _ta_atr_17.recompute(current_bar_.high, current_bar_.low, current_bar_.close));
        volatilityFilter = (!(true) || ((upperBand - lowerBand) > (atrValue * 2.0)));
        bbTrendFilterLong = (!(true) || (current_bar_.close > basis));
        bbTrendFilterShort = (!(true) || (current_bar_.close < basis));
        rsi = (is_first_tick_ ? _ta_rsi_18.compute(current_bar_.close) : _ta_rsi_18.recompute(current_bar_.close));
        rsiFilterLong = (!(true) || (rsi > 55));
        rsiFilterShort = (!(true) || (rsi < 45));
        auto _result__ta_macd_19 = (is_first_tick_ ? _ta_macd_19.compute(current_bar_.close) : _ta_macd_19.recompute(current_bar_.close));
        double macdLine = _result__ta_macd_19.macd_line;
        double signalLine = _result__ta_macd_19.signal_line;
        macdFilterLong = (!(true) || (macdLine > signalLine));
        macdFilterShort = (!(true) || (macdLine < signalLine));
        k = (is_first_tick_ ? _ta_sma_21.compute((is_first_tick_ ? _ta_stoch_20.compute(current_bar_.close, current_bar_.high, current_bar_.low) : _ta_stoch_20.recompute(current_bar_.close, current_bar_.high, current_bar_.low))) : _ta_sma_21.recompute((is_first_tick_ ? _ta_stoch_20.compute(current_bar_.close, current_bar_.high, current_bar_.low) : _ta_stoch_20.recompute(current_bar_.close, current_bar_.high, current_bar_.low))));
        d = (is_first_tick_ ? _ta_sma_22.compute(k) : _ta_sma_22.recompute(k));
        stochFilterLong = (!(true) || ((k > 20) && (k > d)));
        stochFilterShort = (!(true) || ((k < 80) && (k < d)));
        auto _result__ta_dmi_23 = (is_first_tick_ ? _ta_dmi_23.compute(current_bar_.high, current_bar_.low, current_bar_.close) : _ta_dmi_23.recompute(current_bar_.high, current_bar_.low, current_bar_.close));
        double plusDI = _result__ta_dmi_23.diplus;
        double minusDI = _result__ta_dmi_23.diminus;
        double adx = _result__ta_dmi_23.adx;
        adxFilter = (!(true) || (adx > 25));
        buyConditionFiltered = (((((((buyCondition && volatilityFilter) && bbTrendFilterLong) && rsiFilterLong) && macdFilterLong) && stochFilterLong) && adxFilter) && trendFilterLong);
        sellConditionFiltered = (((((((sellCondition && volatilityFilter) && bbTrendFilterShort) && rsiFilterShort) && macdFilterShort) && stochFilterShort) && adxFilter) && trendFilterShort);
        if ((bandType == 1)) {
            maHigh = (is_first_tick_ ? _ta_ema_24.compute(current_bar_.high) : _ta_ema_24.recompute(current_bar_.high));
        } else
        if ((bandType == 2)) {
            maHigh = (is_first_tick_ ? _ta_sma_25.compute(current_bar_.high) : _ta_sma_25.recompute(current_bar_.high));
        } else {
            maHigh = _fn_smma_cs3(current_bar_.high, bandLength);
        }
        if ((bandType == 1)) {
            maLow = (is_first_tick_ ? _ta_ema_26.compute(current_bar_.low) : _ta_ema_26.recompute(current_bar_.low));
        } else
        if ((bandType == 2)) {
            maLow = (is_first_tick_ ? _ta_sma_27.compute(current_bar_.low) : _ta_sma_27.recompute(current_bar_.low));
        } else {
            maLow = _fn_smma_cs4(current_bar_.low, bandLength);
        }
        asiaEnabled = get_input_bool("Asia  ", true);
        sessionAsiaTime = get_input_string("", std::string("1800-0000"));
        colorAsia = get_input_int("", pine_color::new_color(pine_color::yellow, (int)(90)));
        londonEnabled = get_input_bool("London ", true);
        sessionLondonTime = get_input_string("", std::string("0000-0800"));
        colorLondon = get_input_int("", pine_color::new_color(pine_color::blue, (int)(90)));
        nyAmEnabled = get_input_bool("NY AM ", true);
        sessionNYAMTime = get_input_string("", std::string("0800-1300"));
        colorNYAM = get_input_int("", pine_color::new_color(pine_color::orange, (int)(90)));
        nyPmEnabled = get_input_bool("NY PM ", true);
        sessionNYPMTime = get_input_string("", std::string("1300-1800"));
        colorNYPM = get_input_int("", pine_color::new_color(pine_color::purple, (int)(90)));
        showMon = get_input_bool("Mon", true);
        showTue = get_input_bool("Tue", true);
        showWed = get_input_bool("Wed", true);
        showThu = get_input_bool("Thur", true);
        showFri = get_input_bool("Fri", true);
        sessionAsia = _addSessionWeekdays(sessionAsiaTime);
        sessionLondon = _addSessionWeekdays(sessionLondonTime);
        sessionNYAM = _addSessionWeekdays(sessionNYAMTime);
        sessionNYPM = _addSessionWeekdays(sessionNYPMTime);
        inAsiaSession.push((asiaEnabled && isInSession(sessionAsia, script_tf_)));
        inLondonSession.push((londonEnabled && isInSession(sessionLondon, script_tf_)));
        inNYAMSession.push((nyAmEnabled && isInSession(sessionNYAM, script_tf_)));
        inNYPMSession.push((nyPmEnabled && isInSession(sessionNYPM, script_tf_)));
        maxTradesPerSessionAsia = get_input_int("Asia: Max Trades Per Session", 5);
        maxTradesPerSessionLondon = get_input_int("London: Max Trades Per Session", 5);
        maxTradesPerSessionNYAM = get_input_int("NY AM: Max Trades Per Session", 5);
        maxTradesPerSessionNYPM = get_input_int("NY PM: Max Trades Per Session", 5);
        marketCloseHour = get_input_int("Market Close Hour/Minute", 18);
        marketCloseMinute = get_input_int("", 0);
        useAutoRisk = get_input_bool("Auto Risk, Min/Max $:", false);
        autoRiskAmountMin = get_input_int("", 150);
        autoRiskAmountMax = get_input_int("", 250);
        fixedContactSize = get_input_double("Fixed Contact Size", 5);
        tpReachedLimit = get_input_int("TP Reached Limit", 3);
        slReachedLimit = get_input_int("SL Reached Limit", 3);
        rrRatio = get_input_double("Risk/Reward Ratio (1 : *)", 2);
        useSL = get_input_bool("SL Option | %", true);
        slOption = get_input_int("", SLOption_Percent);
        slPercent = get_input_double("", 1);
        useTP = get_input_bool("TP", true);
        isInTradingSession = ((((false || inAsiaSession[0]) || inLondonSession[0]) || inNYAMSession[0]) || inNYPMSession[0]);
        if ((inAsiaSession[0] && !(inAsiaSession[1]))) {
            tradesPerSessionCounterAsia = ((int)trades_.size());
        }
        if ((inLondonSession[0] && !(inLondonSession[1]))) {
            tradesPerSessionCounterLondon = ((int)trades_.size());
        }
        if ((inNYAMSession[0] && !(inNYAMSession[1]))) {
            tradesPerSessionCounterNYAM = ((int)trades_.size());
        }
        if ((inNYPMSession[0] && !(inNYPMSession[1]))) {
            tradesPerSessionCounterNYPM = ((int)trades_.size());
        }
        longEntrySignal = buyConditionFiltered;
        shortEntrySignal = sellConditionFiltered;
        longExitSignal = shortEntrySignal;
        shortExitSignal = longEntrySignal;
        if (((_bar_hour() == marketCloseHour) && (_bar_minute() == marketCloseMinute))) {
            longSLTriggered = 0;
            longTPTriggered = 0;
            shortSLTriggered = 0;
            shortTPTriggered = 0;
            strategy_cancel_all();
            strategy_close_all();
        }
        if (((((signed_position_size() == 0) || (signed_position_size() < 0)) && longEntrySignal) && canGoLong())) {
            if ((signed_position_size() < 0)) {
                strategy_close(std::string("Short Entry"), std::string("Close Position Short"), na<double>(), na<double>(), true);
            }
            entryPrice = current_bar_.close;
            longSL = ((useSL) ? (_sl(entryPrice, 1)) : (na<double>()));
            longQty = ((useAutoRisk) ? (_calcEntryQty(longSL)) : (fixedContactSize));
            if ((longQty > 0)) {
                strategy_entry(std::string("Long Entry"), true, na<double>(), na<double>(), longQty, std::string("Long"), "", 0, -1);
            }
        }
        if (((_strat_position_size[1] <= 0) && (signed_position_size() > 0))) {
            longEntry = entryPrice;
            longTP = ((useTP) ? (_tp(position_entry_price_, signed_position_size(), longSL, rrRatio)) : (na<double>()));
            strategy_exit(std::string("Long Exit"), std::string("Long Entry"), ((useTP) ? (longTP) : (na<double>())), ((useSL) ? (longSL) : (na<double>())), na<double>(), na<double>(), na<double>(), 100.0, "", na<double>(), "");
        }
        if (((((signed_position_size() == 0) || (signed_position_size() > 0)) && shortEntrySignal) && canGoShort())) {
            if ((signed_position_size() > 0)) {
                strategy_close(std::string("Long Entry"), std::string("Close Position Long"), na<double>(), na<double>(), true);
            }
            entryPrice = current_bar_.close;
            shortSL = ((useSL) ? (_sl(entryPrice, (-1))) : (na<double>()));
            shortQty = ((useAutoRisk) ? (_calcEntryQty(shortSL)) : (fixedContactSize));
            if ((shortQty > 0)) {
                strategy_entry(std::string("Short Entry"), false, na<double>(), na<double>(), shortQty, std::string("Short"), "", 0, -1);
            }
        }
        if (((_strat_position_size[1] >= 0) && (signed_position_size() < 0))) {
            shortEntry = entryPrice;
            shortTP = ((useTP) ? (_tp(position_entry_price_, signed_position_size(), shortSL, rrRatio)) : (na<double>()));
            strategy_exit(std::string("Short Exit"), std::string("Short Entry"), ((useTP) ? (shortTP) : (na<double>())), ((useSL) ? (shortSL) : (na<double>())), na<double>(), na<double>(), na<double>(), 100.0, "", na<double>(), "");
        }
        if (((signed_position_size() > 0) && longExitSignal)) {
            strategy_close(std::string("Long Entry"), std::string("Close Position Long"), na<double>(), na<double>(), true);
        }
        if (((signed_position_size() < 0) && shortExitSignal)) {
            strategy_close(std::string("Short Entry"), std::string("Close Position Short"), na<double>(), na<double>(), true);
        }
        if (((_strat_position_size[1] > 0) && (signed_position_size() <= 0))) {
            longEntry = na<double>();
            longQty = na<double>();
            longTP = na<double>();
            longSL = na<double>();
            if ((current_bar_.low <= longSL)) {
                longSLTriggered += 1;
            } else
            if ((current_bar_.high >= longTP)) {
                longTPTriggered += 1;
            }
        }
        if (((_strat_position_size[1] < 0) && (signed_position_size() >= 0))) {
            shortEntry = na<double>();
            shortQty = na<double>();
            shortTP = na<double>();
            shortSL = na<double>();
            if ((current_bar_.high >= shortSL)) {
                shortSLTriggered += 1;
            } else
            if ((current_bar_.low <= shortTP)) {
                shortTPTriggered += 1;
            }
        }
        alertEnabled = get_input_bool("Enable Alerts", true);
        alertMessageBuy = get_input_string("Buy Alert Message", std::string("Buy Signal"));
        alertMessageSell = get_input_string("Sell Alert Message", std::string("Sell Signal"));
        if ((alertEnabled && buyConditionFiltered)) {
        }
        if ((alertEnabled && sellConditionFiltered)) {
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
