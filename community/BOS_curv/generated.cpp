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
    ta::ATR _ta_atr_1;
    ta::PivotHigh _ta_pivothigh_2;
    ta::PivotLow _ta_pivotlow_3;
    ta::Highest _ta_highest_4;
    ta::Lowest _ta_lowest_5;
    ta::ATR _ta_atr_6;
    ta::ATR _ta_atr_7;
    ta::SMA _ta_sma_8;
    ta::Change _ta_change_9;
    ta::SMA _ta_sma_10;
    ta::ATR _ta_atr_11;
    Series<double> _s_close;
    double supertrend;
    int direction;
    double entryPrice;
    double exitPrice;
    bool positionActive;
    int currentDirection;
    std::vector<double> profitHistory;
    std::vector<double> pivotLevels;
    std::vector<int> pivotBars;
    double lastPivotHigh;
    double lastPivotLow;
    int pivotLength;
    double keyResistance;
    double keySupport;
    double nearbyResistance;
    double nearbySupport;
    double recentHigh;
    double recentLow;
    double anchorPrice;
    int anchorBar;
    double velocity;
    int barCount;
    double profit;
    double infoTable;
    double alertTable;
    double reminderWindow;
    double currentProfit;
    double last_alert_price;
    double accumulated_profit;
    int atrLength = 0;
    double atrMult = 0.0;
    double radiusStrength = 0.0;
    int smoothness = 0;
    int upColor = 0;
    int dnColor = 0;
    double atr = 0.0;
    double src = 0.0;
    double upperBand = 0.0;
    double lowerBand = 0.0;
    double prevSupertrend = 0.0;
    int prevDirection = 0;
    double pivotHigh = 0.0;
    double pivotLow = 0.0;
    int lookback = 0;
    double priceRange = 0.0;
    double currentPrice = 0.0;
    double finalResistance = 0.0;
    double finalSupport = 0.0;
    bool breakoutSignal = false;
    bool breakdownSignal = false;
    bool approachingResistance = false;
    bool approachingSupport = false;
    double warningDistance = 0.0;
    double distanceToResistance = 0.0;
    double distanceToSupport = 0.0;
    bool trendChanged = false;
    double curvedBand = 0.0;
    int trendColor = 0;
    double basePlot = 0.0;
    double bandPlot = 0.0;
    double outerBand = 0.0;
    bool showTable = false;
    std::string trendStatusText = std::string("");
    double trendStatusColor = 0.0;
    double atrChange = 0.0;
    std::string convergenceText = std::string("");
    double convergenceColor = 0.0;
    double priceDistance = 0.0;
    double avgDistance = 0.0;
    std::string divergenceText = std::string("");
    double divergenceColor = 0.0;
    std::string riskText = std::string("");
    int riskColor = 0;
    std::string suggestionText = std::string("");
    double suggestionColor = 0.0;
    double historySize = 0.0;
    std::string learningText = std::string("");
    int learningColor = 0;
    double recentProfit = 0.0;
    std::string currentProfitText = std::string("");
    double currentProfitColor = 0.0;
    std::string reminderTitle = std::string("");
    std::string marketStatus = std::string("");
    std::string riskReminder = std::string("");
    std::string operationAdvice = std::string("");
    double volatility = 0.0;
    std::string currentProfitInfo = std::string("");
    std::string profitText = std::string("");
    std::string pivotInfo = std::string("");
    std::string warningText = std::string("");
    std::string baseInfo = std::string("");
    bool tp_alert_enabled = false;
    int tp_alert = 0;
    bool tp_enabled = false;
    int tp_distance = 0;
    bool sl_enabled = false;
    int sl_distance = 0;
    int tpsl_percentage = 0;
    bool buySignal = false;
    bool sellSignal = false;
    double tp_price = 0.0;
    double sl_price = 0.0;
    bool alert_now = false;
    double current_increment = 0.0;
    double alertDistance = 0.0;
    bool nearBuySignal = false;
    bool nearSellSignal = false;
    bool _var_initialized = false;
    bool _ta_initialized_ = false;

    explicit GeneratedStrategy() : _ta_atr_1(14), _ta_pivothigh_2(1, 1), _ta_pivotlow_3(1, 1), _ta_highest_4(20), _ta_lowest_5(20), _ta_atr_6(14), _ta_atr_7(14), _ta_sma_8(5), _ta_change_9(5), _ta_sma_10(20), _ta_atr_11(14), supertrend(na<double>()), direction(1), entryPrice(na<double>()), exitPrice(na<double>()), positionActive(false), currentDirection(0), lastPivotHigh(na<double>()), lastPivotLow(na<double>()), pivotLength(5), keyResistance(na<double>()), keySupport(na<double>()), nearbyResistance(na<double>()), nearbySupport(na<double>()), recentHigh(na<double>()), recentLow(na<double>()), anchorPrice(na<double>()), anchorBar(na<double>()), velocity(0), barCount(0), profit(na<double>()), currentProfit(na<double>()), last_alert_price(0), accumulated_profit(0) {
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
        if (!_var_initialized) {
            profitHistory = std::vector<double>((size_t)(0), 0.0);
            pivotLevels = std::vector<double>((size_t)(0), 0.0);
            pivotBars = std::vector<int>((size_t)(0), 0);
            _var_initialized = true;
        } else {
        }
        if (!_ta_initialized_) {
            _ta_atr_1 = ta::ATR(get_input_int("ATR Length", 14));
            _ta_sma_8 = ta::SMA(get_input_int("Smoothness", 5));
            _ta_initialized_ = true;
        }
        atrLength = get_input_int("ATR Length", 14);
        atrMult = get_input_double("ATR Multiplier", 2.0);
        radiusStrength = get_input_double("Radius Strength", 0.002);
        smoothness = get_input_int("Smoothness", 5);
        upColor = get_input_int("Up Trend", pine_color::green);
        dnColor = get_input_int("Down Trend", pine_color::red);
        atr = (is_first_tick_ ? _ta_atr_1.compute(current_bar_.high, current_bar_.low, current_bar_.close) : _ta_atr_1.recompute(current_bar_.high, current_bar_.low, current_bar_.close));
        src = ((current_bar_.high + current_bar_.low) / 2.0);
        upperBand = (src + (atrMult * atr));
        lowerBand = (src - (atrMult * atr));
        if (is_na(supertrend)) {
            supertrend = lowerBand;
            direction = 1;
        }
        prevSupertrend = supertrend;
        if ((direction == 1)) {
            supertrend = (((current_bar_.close < prevSupertrend)) ? (upperBand) : (std::max((double)(lowerBand), (double)(prevSupertrend))));
        } else {
            supertrend = (((current_bar_.close > prevSupertrend)) ? (lowerBand) : (std::min((double)(upperBand), (double)(prevSupertrend))));
        }
        prevDirection = direction;
        if ((current_bar_.close < supertrend)) {
            direction = (-1);
        }
        if ((current_bar_.close > supertrend)) {
            direction = 1;
        }
        pivotHigh = (is_first_tick_ ? _ta_pivothigh_2.compute(current_bar_.high) : _ta_pivothigh_2.recompute(current_bar_.high));
        pivotLow = (is_first_tick_ ? _ta_pivotlow_3.compute(current_bar_.low) : _ta_pivotlow_3.recompute(current_bar_.low));
        if (!(is_na(pivotHigh))) {
            pivotLevels.insert(pivotLevels.begin(), pivotHigh);
            pivotBars.insert(pivotBars.begin(), (bar_index_ - pivotLength));
            lastPivotHigh = pivotHigh;
            if (((double)pivotLevels.size() > 10)) {
                [&](){ auto v=pivotLevels.back(); pivotLevels.pop_back(); return v; }();
                [&](){ auto v=pivotBars.back(); pivotBars.pop_back(); return v; }();
            }
        }
        if (!(is_na(pivotLow))) {
            pivotLevels.insert(pivotLevels.begin(), pivotLow);
            pivotBars.insert(pivotBars.begin(), (bar_index_ - pivotLength));
            lastPivotLow = pivotLow;
            if (((double)pivotLevels.size() > 10)) {
                [&](){ auto v=pivotLevels.back(); pivotLevels.pop_back(); return v; }();
                [&](){ auto v=pivotBars.back(); pivotBars.pop_back(); return v; }();
            }
        }
        if ((!(is_na(lastPivotHigh)) && !(is_na(lastPivotLow)))) {
            if ((direction == 1)) {
                keySupport = lastPivotLow;
                keyResistance = lastPivotHigh;
            } else {
                keyResistance = lastPivotHigh;
                keySupport = lastPivotLow;
            }
        }
        lookback = 20;
        recentHigh = (is_first_tick_ ? _ta_highest_4.compute(current_bar_.high) : _ta_highest_4.recompute(current_bar_.high));
        recentLow = (is_first_tick_ ? _ta_lowest_5.compute(current_bar_.low) : _ta_lowest_5.recompute(current_bar_.low));
        priceRange = ((is_first_tick_ ? _ta_atr_6.compute(current_bar_.high, current_bar_.low, current_bar_.close) : _ta_atr_6.recompute(current_bar_.high, current_bar_.low, current_bar_.close)) * 3);
        currentPrice = current_bar_.close;
        if (((!(is_na(recentHigh)) && (recentHigh > currentPrice)) && ((recentHigh - currentPrice) < (priceRange * 2)))) {
            nearbyResistance = recentHigh;
        }
        if (((!(is_na(recentLow)) && (recentLow < currentPrice)) && ((currentPrice - recentLow) < (priceRange * 2)))) {
            nearbySupport = recentLow;
        }
        finalResistance = ((!(is_na(nearbyResistance))) ? (nearbyResistance) : (keyResistance));
        finalSupport = ((!(is_na(nearbySupport))) ? (nearbySupport) : (keySupport));
        breakoutSignal = false;
        breakdownSignal = false;
        approachingResistance = false;
        approachingSupport = false;
        if (((!(is_na(finalResistance)) && (current_bar_.close > finalResistance)) && (_s_close[1] <= finalResistance))) {
            breakoutSignal = true;
        }
        if (((!(is_na(finalSupport)) && (current_bar_.close < finalSupport)) && (_s_close[1] >= finalSupport))) {
            breakdownSignal = true;
        }
        warningDistance = ((is_first_tick_ ? _ta_atr_7.compute(current_bar_.high, current_bar_.low, current_bar_.close) : _ta_atr_7.recompute(current_bar_.high, current_bar_.low, current_bar_.close)) * 0.5);
        if ((!(is_na(finalResistance)) && (current_bar_.close < finalResistance))) {
            distanceToResistance = (finalResistance - current_bar_.close);
            if (((distanceToResistance <= warningDistance) && (distanceToResistance > 0))) {
                approachingResistance = true;
            }
        }
        if ((!(is_na(finalSupport)) && (current_bar_.close > finalSupport))) {
            distanceToSupport = (current_bar_.close - finalSupport);
            if (((distanceToSupport <= warningDistance) && (distanceToSupport > 0))) {
                approachingSupport = true;
            }
        }
        trendChanged = (direction != prevDirection);
        if (trendChanged) {
            anchorPrice = supertrend;
            anchorBar = bar_index_;
            velocity = 0.0;
            barCount = 0;
            if ((positionActive && !(is_na(entryPrice)))) {
                if ((currentDirection == 1)) {
                    profit = (_s_close[1] - entryPrice);
                } else
                if ((currentDirection == (-1))) {
                    profit = (entryPrice - _s_close[1]);
                }
                if (!(is_na(profit))) {
                    profitHistory.insert(profitHistory.begin(), profit);
                    if (((double)profitHistory.size() > 5)) {
                        [&](){ auto v=profitHistory.back(); profitHistory.pop_back(); return v; }();
                    }
                }
                positionActive = false;
                exitPrice = _s_close[1];
            }
            if (((direction == 1) || (direction == (-1)))) {
                entryPrice = current_bar_.close;
                positionActive = true;
                currentDirection = direction;
                exitPrice = na<double>();
            }
        }
        barCount = (barCount + 1);
        if (!(is_na(anchorPrice))) {
            velocity = (velocity + (radiusStrength * barCount));
            if ((direction == 1)) {
                supertrend = (anchorPrice + velocity);
            } else {
                supertrend = (anchorPrice - velocity);
            }
        }
        curvedBand = (is_first_tick_ ? _ta_sma_8.compute(supertrend) : _ta_sma_8.recompute(supertrend));
        trendColor = (((direction == 1)) ? (upColor) : (dnColor));
        outerBand = (((direction == 1)) ? ((curvedBand + atr)) : ((curvedBand - atr)));
        showTable = get_input_bool("顯示智能提示", false);
        if (((barstate_islast_ || is_last_tick_) && showTable)) {
            trendStatusText = (((direction == 1)) ? (std::string("多空 ●")) : ((((direction == (-1))) ? (std::string("多空 ●")) : (std::string("無趨勢")))));
            trendStatusColor = (((direction == 1)) ? (pine_color::green) : ((((direction == (-1))) ? (pine_color::red) : (pine_color::gray))));
            atrChange = (is_first_tick_ ? _ta_change_9.compute(atr) : _ta_change_9.recompute(atr));
            convergenceText = (((atrChange < 0)) ? (std::string("正常 ■")) : (std::string("擴散")));
            convergenceColor = (((atrChange < 0)) ? (pine_color::green) : (pine_color::orange));
            priceDistance = std::abs((current_bar_.close - curvedBand));
            avgDistance = (is_first_tick_ ? _ta_sma_10.compute(priceDistance) : _ta_sma_10.recompute(priceDistance));
            divergenceText = (((priceDistance > (avgDistance * 1.5))) ? (std::string("發散")) : (std::string("收斂")));
            divergenceColor = (((priceDistance > (avgDistance * 1.5))) ? (pine_color::red) : (pine_color::gray));
            riskText = std::string("注意 ▲");
            riskColor = pine_color::orange;
            if ((positionActive && !(is_na(entryPrice)))) {
                if ((currentDirection == 1)) {
                    currentProfit = (current_bar_.close - entryPrice);
                } else
                if ((currentDirection == (-1))) {
                    currentProfit = (entryPrice - current_bar_.close);
                }
                if ((!(is_na(currentProfit)) && (currentProfit < (-50)))) {
                    riskText = std::string("高風險 ▲");
                    riskColor = pine_color::red;
                } else
                if ((!(is_na(currentProfit)) && (currentProfit > 0))) {
                    riskText = std::string("良好 ●");
                    riskColor = pine_color::green;
                }
            }
            suggestionText = (((direction != 0)) ? (std::string("良好時機 ✓")) : (std::string("等待信號")));
            suggestionColor = (((direction != 0)) ? (pine_color::green) : (pine_color::gray));
            historySize = (double)profitHistory.size();
            learningText = std::string("無標示");
            learningColor = pine_color::gray;
            if ((historySize >= 3)) {
                recentProfit = 0.0;
                for (int i = 0; i <= 2; i += 1) {
                    if ((i < historySize)) {
                        recentProfit += profitHistory[i];
                    }
                }
                if ((recentProfit > 0)) {
                    learningText = std::string("表現良好");
                    learningColor = pine_color::green;
                } else {
                    learningText = std::string("需要調整");
                    learningColor = pine_color::orange;
                }
            }
            if ((positionActive && !(is_na(entryPrice)))) {
                if ((currentDirection == 1)) {
                    currentProfit = (current_bar_.close - entryPrice);
                } else
                if ((currentDirection == (-1))) {
                    currentProfit = (entryPrice - current_bar_.close);
                }
                currentProfitText = ((!(is_na(currentProfit))) ? ((((currentProfit > 0)) ? ((std::string("+") + pine_str_tostring(currentProfit, std::string("#.##"), syminfo_mintick_))) : (pine_str_tostring(currentProfit, std::string("#.##"), syminfo_mintick_)))) : (std::string("-")));
                currentProfitColor = ((!(is_na(currentProfit))) ? ((((currentProfit > 0)) ? (pine_color::green) : ((((currentProfit < 0)) ? (pine_color::red) : (pine_color::gray))))) : (pine_color::gray));
            }
            if ((trendChanged && positionActive)) {
            }
            reminderTitle = std::string("💡 智能交易提醒");
            marketStatus = std::string("");
            if ((direction == 1)) {
                marketStatus = std::string("📈 當前多頭趨勢n建議關注支撐位");
            } else
            if ((direction == (-1))) {
                marketStatus = std::string("📉 當前空頭趨勢n建議關注阻力位");
            } else {
                marketStatus = std::string("⏸️ 趨勢不明確n建議等待突破");
            }
            riskReminder = std::string("");
            if ((positionActive && !(is_na(entryPrice)))) {
                if ((currentDirection == 1)) {
                    currentProfit = (current_bar_.close - entryPrice);
                } else
                if ((currentDirection == (-1))) {
                    currentProfit = (entryPrice - current_bar_.close);
                }
                if (!(is_na(currentProfit))) {
                    if ((currentProfit < (-30))) {
                        riskReminder = std::string("⚠️ 虧損擴大中n考慮止損退場");
                    } else
                    if ((currentProfit > 50)) {
                        riskReminder = std::string("✅ 獲利中n可考慮部分獲利");
                    } else {
                        riskReminder = std::string("📊 持倉中n密切觀察走勢");
                    }
                }
            } else {
                riskReminder = std::string("💤 目前無持倉n等待進場機會");
            }
            operationAdvice = std::string("");
            volatility = (((double)((is_first_tick_ ? _ta_atr_11.compute(current_bar_.high, current_bar_.low, current_bar_.close) : _ta_atr_11.recompute(current_bar_.high, current_bar_.low, current_bar_.close))) / (double)(current_bar_.close)) * 100);
            if ((volatility > 2.0)) {
                operationAdvice = std::string("⚡ 市場波動大n減少倉位操作");
            } else
            if ((volatility < 0.5)) {
                operationAdvice = std::string("😴 市場平靜n可考慮區間操作");
            } else {
                operationAdvice = std::string("⚖️ 正常波動n按計劃執行");
            }
            currentProfitInfo = std::string("");
            if ((positionActive && !(is_na(entryPrice)))) {
                if ((currentDirection == 1)) {
                    currentProfit = (current_bar_.close - entryPrice);
                } else
                if ((currentDirection == (-1))) {
                    currentProfit = (entryPrice - current_bar_.close);
                }
                if (!(is_na(currentProfit))) {
                    profitText = (((currentProfit > 0)) ? ((std::string("+") + pine_str_tostring(currentProfit, std::string("#.##"), syminfo_mintick_))) : (pine_str_tostring(currentProfit, std::string("#.##"), syminfo_mintick_)));
                    currentProfitInfo = ((((std::string("💰 目前") + (((currentProfit >= 0)) ? (std::string("賺")) : (std::string("虧")))) + std::string("：")) + profitText) + std::string(" 點"));
                } else {
                    currentProfitInfo = std::string("💰 目前盈虧：計算中");
                }
            } else {
                currentProfitInfo = std::string("💤 目前無持倉");
            }
            pivotInfo = std::string("");
            warningText = std::string("");
            if ((approachingResistance && !(is_na(finalResistance)))) {
                warningText = (std::string("⚠️ 接近阻力 ") + pine_str_tostring((finalResistance - current_bar_.close), std::string("#.##"), syminfo_mintick_));
            } else
            if ((approachingSupport && !(is_na(finalSupport)))) {
                warningText = (std::string("⚠️ 接近支撑 ") + pine_str_tostring((current_bar_.close - finalSupport), std::string("#.##"), syminfo_mintick_));
            }
            if ((!(is_na(finalResistance)) && !(is_na(finalSupport)))) {
                baseInfo = (((std::string("🎯 阻力: ") + pine_str_tostring(finalResistance, std::string("#.##"), syminfo_mintick_)) + std::string("n🛡️ 支撐: ")) + pine_str_tostring(finalSupport, std::string("#.##"), syminfo_mintick_));
                pivotInfo = (((warningText != std::string(""))) ? (((baseInfo + std::string("n")) + warningText)) : (baseInfo));
            } else
            if (!(is_na(finalResistance))) {
                baseInfo = ((std::string("🎯 阻力: ") + pine_str_tostring(finalResistance, std::string("#.##"), syminfo_mintick_)) + std::string("n🛡️ 支撐: 待確認"));
                pivotInfo = (((warningText != std::string(""))) ? (((baseInfo + std::string("n")) + warningText)) : (baseInfo));
            } else
            if (!(is_na(finalSupport))) {
                baseInfo = (std::string("🎯 阻力: 待確認n🛡️ 支撐: ") + pine_str_tostring(finalSupport, std::string("#.##"), syminfo_mintick_));
                pivotInfo = (((warningText != std::string(""))) ? (((baseInfo + std::string("n")) + warningText)) : (baseInfo));
            } else {
                pivotInfo = std::string("🎯 阻力: 待確認n🛡️ 支撐: 待確認");
            }
        }
        tp_alert_enabled = get_input_bool("啟用盈利通知", false);
        tp_alert = get_input_int("盈利通知間距", 0);
        tp_enabled = get_input_bool("啟用自動停利出場", false);
        tp_distance = get_input_int("自動停利價格", 100);
        sl_enabled = get_input_bool("啟用自動停損出場", false);
        sl_distance = get_input_int("自動停損價格", 100);
        tpsl_percentage = get_input_int("自動停利停損百分比(%)", 50);
        buySignal = (trendChanged && (direction == 1));
        sellSignal = (trendChanged && (direction == (-1)));
        if ((buySignal && (signed_position_size() <= 0))) {
            strategy_entry(std::string("Buy"), true, na<double>(), na<double>(), na<double>(), "");
            last_alert_price = current_bar_.close;
            accumulated_profit = 0.0;
            if ((tp_enabled || sl_enabled)) {
                tp_price = ((tp_enabled) ? ((current_bar_.close + tp_distance)) : (na<double>()));
                sl_price = ((sl_enabled) ? ((current_bar_.close - sl_distance)) : (na<double>()));
                strategy_exit(std::string("Exit Long"), std::string("Buy"), tp_price, sl_price, na<double>(), na<double>(), na<double>(), tpsl_percentage, "");
            }
        }
        if ((sellSignal && (signed_position_size() >= 0))) {
            strategy_entry(std::string("Sell"), false, na<double>(), na<double>(), na<double>(), "");
            last_alert_price = current_bar_.close;
            accumulated_profit = 0.0;
            if ((tp_enabled || sl_enabled)) {
                tp_price = ((tp_enabled) ? ((current_bar_.close - tp_distance)) : (na<double>()));
                sl_price = ((sl_enabled) ? ((current_bar_.close + sl_distance)) : (na<double>()));
                strategy_exit(std::string("Exit Short"), std::string("Sell"), tp_price, sl_price, na<double>(), na<double>(), na<double>(), tpsl_percentage, "");
            }
        }
        if (((signed_position_size() == 0) && !(is_na(last_alert_price)))) {
            last_alert_price = na<double>();
        }
        alert_now = false;
        current_increment = 0.0;
        if ((((signed_position_size() > 0) && tp_alert_enabled) && !(is_na(last_alert_price)))) {
            if (((current_bar_.close - last_alert_price) >= tp_alert)) {
                current_increment = (current_bar_.close - last_alert_price);
                accumulated_profit += current_increment;
                alert_now = true;
                last_alert_price = current_bar_.close;
            }
        }
        if ((((signed_position_size() < 0) && tp_alert_enabled) && !(is_na(last_alert_price)))) {
            if (((last_alert_price - current_bar_.close) >= tp_alert)) {
                current_increment = (last_alert_price - current_bar_.close);
                accumulated_profit += current_increment;
                alert_now = true;
                last_alert_price = current_bar_.close;
            }
        }
        if (alert_now) {
        }
        alertDistance = 30.0;
        nearBuySignal = false;
        if (((!(buySignal) && (direction == 1)) && (std::abs((current_bar_.close - curvedBand)) <= 30.0))) {
            nearBuySignal = true;
        }
        nearSellSignal = false;
        if (((!(sellSignal) && (direction == (-1))) && (std::abs((current_bar_.close - curvedBand)) <= 30.0))) {
            nearSellSignal = true;
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
