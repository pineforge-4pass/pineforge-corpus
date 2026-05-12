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

struct TradeState {
    bool inPosition = false;
    int direction = 0;
    double entryPrice = 0.0;
    double stopLoss = 0.0;
    double takeProfit = 0.0;
    double trailStop = 0.0;
    int entryBar = 0;
    static TradeState create() { return TradeState{}; }
};

struct ConfluenceScore {
    int structure = 0;
    int momentum = 0;
    int volume = 0;
    int mtf = 0;
    int session = 0;
    int liquidity = 0;
    int quantum = 0;
    int smartMoney = 0;
    int harmonic = 0;
    int total = 0;
    static ConfluenceScore create() { return ConfluenceScore{}; }
};

struct QuantumState {
    double coherence = 0.0;
    double entanglement = 0.0;
    double superposition = 0.0;
    bool isCollapsed = false;
    double wavePhase = 0.0;
    static QuantumState create() { return QuantumState{}; }
};

struct LiquidityState {
    double sweepStrength = 0.0;
    bool liquidityTaken = false;
    double absorptionRatio = 0.0;
    int liquidityLevel = 0;
    static LiquidityState create() { return LiquidityState{}; }
};

struct SmartMoneyState {
    bool isAccumulating = false;
    bool isDistributing = false;
    double institutionalDelta = 0.0;
    double smartMoneyScore = 0.0;
    static SmartMoneyState create() { return SmartMoneyState{}; }
};

struct MarketRegime {
    int trend = 0;
    bool isTrending = false;
    double volatility = 0.0;
    double strength = 0.0;
    static MarketRegime create() { return MarketRegime{}; }
};

class GeneratedStrategy : public BacktestEngine {
public:
    double _req_sec_0 = na<double>();
    double _req_sec_1 = na<double>();
    double _req_sec_2 = na<double>();
    std::unordered_map<std::string, Series<double>> _security_helper_series_;
    ta::ATR _ta_atr_1;
    ta::PivotHigh _ta_pivothigh_2;
    ta::PivotLow _ta_pivotlow_3;
    ta::RSI _ta_rsi_4;
    ta::EMA _ta_ema_5;
    ta::Change _ta_change_6;
    ta::SMA _ta_sma_7;
    math::Sum _ta_sum_8;
    ta::StdDev _ta_stdev_9;
    ta::Change _ta_change_10;
    ta::Change _ta_change_11;
    ta::EMA _ta_ema_12;
    ta::RSI _ta_rsi_13;
    ta::Change _ta_change_14;
    ta::Change _ta_change_15;
    ta::RMA _ta_rma_16;
    ta::RMA _ta_rma_17;
    ta::RMA _ta_rma_18;
    ta::RMA _ta_rma_19;
    ta::Lowest _ta_lowest_20;
    ta::Highest _ta_highest_21;
    ta::EMA _sec1__ta_ema_12;
    ta::RSI _sec2__ta_rsi_13;
    Series<double> _s_close;
    Series<double> _s_high;
    Series<double> _s_low;
    Series<double> _s_open;
    int VCP_BULL_ENTRY;
    int VCP_BEAR_ENTRY;
    int VCP_BULL_EXIT;
    int VCP_BEAR_EXIT;
    int VCP_NEUTRAL;
    int VCP_SL_COLOR;
    int VCP_TP_COLOR;
    int VCP_QUANTUM;
    int VCP_ENTANGLEMENT;
    int VCP_COHERENCE;
    int VCP_SUPERPOSITION;
    int VCP_STRUCTURE_BULL;
    int VCP_STRUCTURE_BEAR;
    int VCP_LIQUIDITY_BULL;
    int VCP_LIQUIDITY_BEAR;
    int VCP_INSTITUTIONAL;
    int VCP_SMART_MONEY;
    int VCP_HARMONIC;
    int VCP_RESONANCE;
    int VCP_WARNING;
    int VCP_GLOW;
    int VCP_FVG_BULL;
    int VCP_FVG_BEAR;
    int VCP_ZONE_BULL;
    int VCP_ZONE_BEAR;
    int VCP_BACKGROUND;
    TradeState trade;
    ConfluenceScore confluence;
    MarketRegime regime;
    QuantumState quantum;
    LiquidityState liquidity;
    SmartMoneyState smartMoney;
    std::vector<double> fvgBoxes;
    std::vector<double> fvgTops;
    std::vector<double> fvgBottoms;
    std::vector<bool> fvgBullish;
    Series<bool> seriesQuantumCollapsed;
    Series<double> seriesQuantumCoherence;
    Series<double> seriesQuantumEntanglement;
    Series<bool> seriesLiquidityTaken;
    Series<bool> seriesSmartMoneyAccumulating;
    Series<bool> seriesSmartMoneyDistributing;
    Series<int> seriesConfluenceHarmonic;
    Series<bool> seriesRegimeTrending;
    Series<int> seriesRegimeTrend;
    double animationPhase;
    int lastAnimationBar;
    double lastSwingHigh;
    double lastSwingLow;
    int lastSwingHighBar;
    int lastSwingLowBar;
    double prevSwingHigh;
    double prevSwingLow;
    Series<int> structureDirection;
    double dashboard;
    Series<bool> momBearish;
    Series<bool> momBullish;
    Series<bool> mtfBearish;
    Series<bool> mtfBullish;
    Series<double> rsiSmooth;
    Series<bool> volAnomaly;
    std::string grpStrategy = std::string("");
    bool useLongTrades = false;
    bool useShortTrades = false;
    int minConfluence = 0;
    bool useRegimeFilter = false;
    bool useSessionFilter = false;
    std::string grpAdvanced = std::string("");
    bool useQuantumFilter = false;
    bool useLiquidityFilter = false;
    bool useSmartMoney = false;
    bool useHarmonicPattern = false;
    double quantumThreshold = 0.0;
    bool showGlowEffect = false;
    std::string colorScheme = std::string("");
    std::string grpRisk = std::string("");
    double riskPercent = 0.0;
    double rrRatio = 0.0;
    bool useTrailStop = false;
    double trailATRMult = 0.0;
    double slATRMult = 0.0;
    std::string grpStructure = std::string("");
    int pivotStrength = 0;
    bool requireBOS = false;
    bool requireFVG = false;
    std::string grpMomentum = std::string("");
    int rsiLength = 0;
    int rsiOverbought = 0;
    int rsiOversold = 0;
    bool requireMomAlign = false;
    std::string grpMTF = std::string("");
    bool useMTFFilter = false;
    std::string htfTimeframe = std::string("");
    int mtfTrendLength = 0;
    std::string grpVolume = std::string("");
    bool useVolumeFilter = false;
    int volMALength = 0;
    double volThreshold = 0.0;
    std::string grpSession = std::string("");
    std::string activeSession = std::string("");
    std::string sessionTZ = std::string("");
    std::string grpVisual = std::string("");
    bool showEntrySignals = false;
    bool showSLTP = false;
    bool showDashboard = false;
    bool showFVGZones = false;
    double atrVal = 0.0;
    double fvgTop = 0.0;
    double fvgBottom = 0.0;
    double pivotHigh = 0.0;
    double pivotLow = 0.0;
    bool bosUp = false;
    bool bosDown = false;
    bool chochUp = false;
    bool chochDown = false;
    int structureSignal = 0;
    bool bullFVG = false;
    bool bearFVG = false;
    double minFVGSize = 0.0;
    double newBox = 0.0;
    double oldBox = 0.0;
    bool inBullFVG = false;
    bool inBearFVG = false;
    double rsiValue = 0.0;
    double rsiMomentum = 0.0;
    bool rsiDivBull = false;
    bool rsiDivBear = false;
    int momentumSignal = 0;
    double volMA = 0.0;
    double volRatio = 0.0;
    bool highVolume = false;
    double buyVolume = 0.0;
    double sellVolume = 0.0;
    double volumeDelta = 0.0;
    double cumDelta = 0.0;
    double volStd = 0.0;
    double volZScore = 0.0;
    bool volumeBullish = false;
    bool volumeBearish = false;
    int volumeSignal = 0;
    double absorption = 0.0;
    double _smPriceChange = 0.0;
    double _smVolumeChange = 0.0;
    double smScore = 0.0;
    double _htfEmaGlobal = 0.0;
    double _htfRsiGlobal = 0.0;
    double htfClose = 0.0;
    double htfEMA = 0.0;
    double htfRSI = 0.0;
    int htfTrend = 0;
    bool htfMomBull = false;
    bool htfMomBear = false;
    int mtfSignal = 0;
    double trendAlignment = 0.0;
    bool inActiveSession = false;
    int sessionSignal = 0;
    int adxLen = 0;
    double upMove = 0.0;
    double downMove = 0.0;
    double plusDM = 0.0;
    double minusDM = 0.0;
    double trueRange = 0.0;
    double plusDI = 0.0;
    double minusDI = 0.0;
    double dx = 0.0;
    double adxValue = 0.0;
    bool regimeAllowsLong = false;
    bool regimeAllowsShort = false;
    bool _confStructBull = false;
    bool _confMomBull = false;
    bool _confMtfBull = false;
    bool _confVolBull = false;
    bool _confSessBull = false;
    bool _confQuantBull = false;
    bool _confSmBull = false;
    bool bullConfluence = false;
    bool _confStructBear = false;
    bool _confMomBear = false;
    bool _confMtfBear = false;
    bool _confVolBear = false;
    bool _confSessBear = false;
    bool _confQuantBear = false;
    bool _confSmBear = false;
    bool bearConfluence = false;
    bool _fvgLong = false;
    bool _bosLong = false;
    bool longEntry = false;
    bool _fvgShort = false;
    bool _bosShort = false;
    bool shortEntry = false;
    double _recentLow = 0.0;
    double _recentHigh = 0.0;
    double sl = 0.0;
    double tp = 0.0;
    double trailDistance = 0.0;
    double newTrail = 0.0;
    int longEntryColor = 0;
    int shortEntryColor = 0;
    double glowAlpha = 0.0;
    int structureColor = 0;
    int bgColor = 0;
    double sessionBg = 0.0;
    double quantumBg = 0.0;
    double smBg = 0.0;
    double headerBg = 0.0;
    std::string positionText = std::string("");
    int positionCol = 0;
    std::string structText = std::string("");
    int structCol = 0;
    std::string regimeText = std::string("");
    int regimeCol = 0;
    std::string momText = std::string("");
    int momCol = 0;
    std::string volText = std::string("");
    int volCol = 0;
    std::string mtfText = std::string("");
    int mtfCol = 0;
    std::string sessText = std::string("");
    int sessCol = 0;
    int maxConfluence = 0;
    std::string confText = std::string("");
    int confCol = 0;
    std::string liqText = std::string("");
    int liqCol = 0;
    std::string quantumText = std::string("");
    int quantumCol = 0;
    std::string smText = std::string("");
    int smCol = 0;
    std::string harmonicText = std::string("");
    int harmonicCol = 0;
    double pnlPct = 0.0;
    int pnlCol = 0;
    bool _var_initialized = false;
    bool _ta_initialized_ = false;

    explicit GeneratedStrategy() : _ta_atr_1(14), _ta_pivothigh_2(5, 5), _ta_pivotlow_3(5, 5), _ta_rsi_4(14), _ta_ema_5(3), _ta_change_6(3), _ta_sma_7(20), _ta_sum_8(10), _ta_stdev_9(20), _ta_change_10(5), _ta_change_11(5), _ta_ema_12(20), _ta_rsi_13(14), _ta_rma_16(14), _ta_rma_17(14), _ta_rma_18(14), _ta_rma_19(14), _ta_lowest_20(10), _ta_highest_21(10), _sec1__ta_ema_12(20), _sec2__ta_rsi_13(14), animationPhase(0), lastAnimationBar(0), lastSwingHigh(na<double>()), lastSwingLow(na<double>()), lastSwingHighBar(na<double>()), lastSwingLowBar(na<double>()), prevSwingHigh(na<double>()), prevSwingLow(na<double>()) {
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

    void configure_security_evaluators() override {
        security_eval_states_.clear();
        register_security_eval(0, get_input_string("Higher Timeframe", std::string("60")), input_tf_, false, false);
        register_security_eval(1, get_input_string("Higher Timeframe", std::string("60")), input_tf_, false, false);
        register_security_eval(2, get_input_string("Higher Timeframe", std::string("60")), input_tf_, false, false);
    }

    std::tuple<int, int, int, int, int, int, int, int, int> calcConfluence() {
        int structScore = 0;
        int momScore = 0;
        int volScore = 0;
        int mtfScore = 0;
        int sessScore = 0;
        int liqScore = 0;
        int quantumScore = 0;
        smScore = 0;
        int harmonicScore = 0;
        if (requireBOS) {
            structScore = (((structureSignal != 0)) ? (1) : (0));
        } else {
            structScore = (((structureDirection[0] != 0)) ? (1) : (0));
        }
        if (requireFVG) {
            if ((inBullFVG && (structureDirection[0] > 0))) {
                structScore += 1;
            }
            if ((inBearFVG && (structureDirection[0] < 0))) {
                structScore += 1;
            }
        }
        if (requireMomAlign) {
            momScore = (((momentumSignal != 0)) ? (1) : (0));
        } else {
            momScore = 1;
        }
        if (useVolumeFilter) {
            volScore = (((volumeSignal != 0)) ? (1) : (0));
            if (volAnomaly[0]) {
                volScore += 1;
            }
        } else {
            volScore = 1;
        }
        if (useMTFFilter) {
            mtfScore = (((mtfSignal != 0)) ? (1) : (0));
        } else {
            mtfScore = 1;
        }
        if (useSessionFilter) {
            sessScore = sessionSignal;
        } else {
            sessScore = 1;
        }
        if (useLiquidityFilter) {
            if ((liquidity.liquidityLevel >= 2)) {
                liqScore = 1;
            }
            if (liquidity.liquidityTaken) {
                liqScore += 1;
            }
        } else {
            liqScore = 1;
        }
        if (useQuantumFilter) {
            quantumScore = ((quantum.isCollapsed) ? (1) : (0));
            if ((quantum.coherence > 0.8)) {
                quantumScore += 1;
            }
        } else {
            quantumScore = 1;
        }
        if (useSmartMoney) {
            if ((smartMoney.smartMoneyScore > 50)) {
                smScore = 1;
            }
            if ((smartMoney.isAccumulating || smartMoney.isDistributing)) {
                smScore += 1;
            }
        } else {
            smScore = 1;
        }
        if (useHarmonicPattern) {
            int harmonicPeriod = 8;
            double harmonicPhase = ((double)((std::fmod((double)(bar_index_), (double)(harmonicPeriod)) * 360)) / (double)(harmonicPeriod));
            bool harmonicActive = ((std::sin(((double)((harmonicPhase * M_PI)) / (double)(180))) * structureDirection[0]) > 0.5);
            harmonicScore = ((harmonicActive) ? (1) : (0));
        } else {
            harmonicScore = 1;
        }
        return std::make_tuple(structScore, momScore, volScore, mtfScore, sessScore, liqScore, quantumScore, smScore, harmonicScore);
    }

    double calcStopLoss(bool isLong) {
        sl = na<double>();
        if (isLong) {
            sl = std::min((double)((current_bar_.close - (atrVal * slATRMult))), (double)((_recentLow - (atrVal * 0.2))));
        } else {
            sl = std::max((double)((current_bar_.close + (atrVal * slATRMult))), (double)((_recentHigh + (atrVal * 0.2))));
        }
        return sl;
    }

    double calcTakeProfit(double entry, double sl, bool isLong) {
        tp = na<double>();
        double risk = std::abs((entry - sl));
        if (isLong) {
            tp = (entry + (risk * rrRatio));
        } else {
            tp = (entry - (risk * rrRatio));
        }
        return tp;
    }

    int getEntryColor(bool isLong) {
        int _func_ret = 0;
        if ((colorScheme == std::string("Professional"))) {
            _func_ret = ((isLong) ? (0) : (0));
        } else
        if ((colorScheme == std::string("Classic"))) {
            _func_ret = ((isLong) ? (0) : (0));
        } else
        if ((colorScheme == std::string("Neon"))) {
            _func_ret = ((isLong) ? (0) : (0));
        } else {
            _func_ret = ((isLong) ? (VCP_BULL_ENTRY) : (VCP_BEAR_ENTRY));
        }
        return _func_ret;
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
            trade = TradeState{.inPosition = false, .direction = 0, .entryPrice = na<double>(), .stopLoss = na<double>(), .takeProfit = na<double>(), .trailStop = na<double>(), .entryBar = 0};
            confluence = ConfluenceScore{.structure = 0, .momentum = 0, .volume = 0, .mtf = 0, .session = 0, .liquidity = 0, .quantum = 0, .smartMoney = 0, .harmonic = 0, .total = 0};
            regime = MarketRegime{.trend = 0, .isTrending = false, .volatility = 0.0, .strength = 0.0};
            quantum = QuantumState{.coherence = 0.0, .entanglement = 0.0, .superposition = 0.0, .isCollapsed = false, .wavePhase = 0.0};
            liquidity = LiquidityState{.sweepStrength = 0.0, .liquidityTaken = false, .absorptionRatio = 0.0, .liquidityLevel = 0};
            smartMoney = SmartMoneyState{.isAccumulating = false, .isDistributing = false, .institutionalDelta = 0.0, .smartMoneyScore = 0.0};
            fvgBoxes = std::vector<double>();
            fvgTops = std::vector<double>();
            fvgBottoms = std::vector<double>();
            fvgBullish = std::vector<bool>();
            seriesQuantumCollapsed.push(false);
            seriesQuantumCoherence.push(0);
            seriesQuantumEntanglement.push(0);
            seriesLiquidityTaken.push(false);
            seriesSmartMoneyAccumulating.push(false);
            seriesSmartMoneyDistributing.push(false);
            seriesConfluenceHarmonic.push(0);
            seriesRegimeTrending.push(false);
            seriesRegimeTrend.push(0);
            structureDirection.push(0);
            _var_initialized = true;
        } else {
            if (is_first_tick_) seriesQuantumCollapsed.push(seriesQuantumCollapsed[0]);
            if (is_first_tick_) seriesQuantumCoherence.push(seriesQuantumCoherence[0]);
            if (is_first_tick_) seriesQuantumEntanglement.push(seriesQuantumEntanglement[0]);
            if (is_first_tick_) seriesLiquidityTaken.push(seriesLiquidityTaken[0]);
            if (is_first_tick_) seriesSmartMoneyAccumulating.push(seriesSmartMoneyAccumulating[0]);
            if (is_first_tick_) seriesSmartMoneyDistributing.push(seriesSmartMoneyDistributing[0]);
            if (is_first_tick_) seriesConfluenceHarmonic.push(seriesConfluenceHarmonic[0]);
            if (is_first_tick_) seriesRegimeTrending.push(seriesRegimeTrending[0]);
            if (is_first_tick_) seriesRegimeTrend.push(seriesRegimeTrend[0]);
            if (is_first_tick_) structureDirection.push(structureDirection[0]);
        }
        if (!_ta_initialized_) {
            _ta_pivothigh_2 = ta::PivotHigh(get_input_int("Pivot Strength", 5), get_input_int("Pivot Strength", 5));
            _ta_pivotlow_3 = ta::PivotLow(get_input_int("Pivot Strength", 5), get_input_int("Pivot Strength", 5));
            _ta_rsi_4 = ta::RSI(get_input_int("RSI Length", 14));
            _ta_sma_7 = ta::SMA(get_input_int("Volume MA Length", 20));
            _ta_ema_12 = ta::EMA(get_input_int("MTF Trend Length", 20));
            _ta_rsi_13 = ta::RSI(get_input_int("RSI Length", 14));
            _ta_lowest_20 = ta::Lowest(get_input_int("Pivot Strength", 5) * 2);
            _ta_highest_21 = ta::Highest(get_input_int("Pivot Strength", 5) * 2);
            _sec1__ta_ema_12 = ta::EMA(get_input_int("MTF Trend Length", 20));
            _sec2__ta_rsi_13 = ta::RSI(get_input_int("RSI Length", 14));
            _ta_initialized_ = true;
        }
        grpStrategy = std::string("Strategy Settings");
        useLongTrades = get_input_bool("Enable Long Trades", true);
        useShortTrades = get_input_bool("Enable Short Trades", true);
        minConfluence = get_input_int("Minimum Confluence Score", 3);
        useRegimeFilter = get_input_bool("Use Regime Filter", true);
        useSessionFilter = get_input_bool("Use Session Filter", true);
        grpAdvanced = std::string("Advanced Features");
        useQuantumFilter = get_input_bool("Use Quantum Confluence", true);
        useLiquidityFilter = get_input_bool("Use Liquidity Analysis", true);
        useSmartMoney = get_input_bool("Use Smart Money Concepts", true);
        useHarmonicPattern = get_input_bool("Use Harmonic Patterns", true);
        quantumThreshold = get_input_double("Quantum Coherence Threshold", 0.7);
        showGlowEffect = get_input_bool("Show Glow Effects", true);
        colorScheme = get_input_string("Color Scheme", std::string("Quantum"));
        grpRisk = std::string("Risk Management");
        riskPercent = get_input_double("Risk Per Trade (%)", 1.0);
        rrRatio = get_input_double("Risk:Reward Ratio", 2.0);
        useTrailStop = get_input_bool("Use Trailing Stop", true);
        trailATRMult = get_input_double("Trail ATR Multiplier", 2.0);
        slATRMult = get_input_double("Stop Loss ATR Multiplier", 1.5);
        grpStructure = std::string("Market Structure");
        pivotStrength = get_input_int("Pivot Strength", 5);
        requireBOS = get_input_bool("Require BOS/CHoCH", true);
        requireFVG = get_input_bool("Require FVG Confluence", true);
        grpMomentum = std::string("Momentum Settings");
        rsiLength = get_input_int("RSI Length", 14);
        rsiOverbought = get_input_int("RSI Overbought", 70);
        rsiOversold = get_input_int("RSI Oversold", 30);
        requireMomAlign = get_input_bool("Require Momentum Alignment", true);
        grpMTF = std::string("Multi-Timeframe");
        useMTFFilter = get_input_bool("Use MTF Filter", true);
        htfTimeframe = get_input_string("Higher Timeframe", std::string("60"));
        mtfTrendLength = get_input_int("MTF Trend Length", 20);
        grpVolume = std::string("Volume Analysis");
        useVolumeFilter = get_input_bool("Use Volume Filter", true);
        volMALength = get_input_int("Volume MA Length", 20);
        volThreshold = get_input_double("Volume Threshold", 1.2);
        grpSession = std::string("Session Settings");
        activeSession = get_input_string("Active Trading Session", std::string("0800-1600"));
        sessionTZ = get_input_string("Timezone", std::string("America/New_York"));
        grpVisual = std::string("Visual Settings");
        showEntrySignals = get_input_bool("Show Entry Signals", true);
        showSLTP = get_input_bool("Show SL/TP Levels", true);
        showDashboard = get_input_bool("Show Dashboard", true);
        showFVGZones = get_input_bool("Show FVG Zones", true);
        atrVal = (is_first_tick_ ? _ta_atr_1.compute(current_bar_.high, current_bar_.low, current_bar_.close) : _ta_atr_1.recompute(current_bar_.high, current_bar_.low, current_bar_.close));
        fvgTop = na<double>();
        fvgBottom = na<double>();
        pivotHigh = (is_first_tick_ ? _ta_pivothigh_2.compute(current_bar_.high) : _ta_pivothigh_2.recompute(current_bar_.high));
        pivotLow = (is_first_tick_ ? _ta_pivotlow_3.compute(current_bar_.low) : _ta_pivotlow_3.recompute(current_bar_.low));
        if (!(is_na(pivotHigh))) {
            prevSwingHigh = lastSwingHigh;
            lastSwingHigh = pivotHigh;
            lastSwingHighBar = (bar_index_ - pivotStrength);
        }
        if (!(is_na(pivotLow))) {
            prevSwingLow = lastSwingLow;
            lastSwingLow = pivotLow;
            lastSwingLowBar = (bar_index_ - pivotStrength);
        }
        bosUp = ((!(is_na(lastSwingHigh)) && (current_bar_.close > lastSwingHigh)) && (structureDirection[0] <= 0));
        bosDown = ((!(is_na(lastSwingLow)) && (current_bar_.close < lastSwingLow)) && (structureDirection[0] >= 0));
        chochUp = ((!(is_na(prevSwingHigh)) && (current_bar_.close > prevSwingHigh)) && (structureDirection[0] < 0));
        chochDown = ((!(is_na(prevSwingLow)) && (current_bar_.close < prevSwingLow)) && (structureDirection[0] > 0));
        if ((bosUp || chochUp)) {
            structureDirection.update(1);
        }
        if ((bosDown || chochDown)) {
            structureDirection.update((-1));
        }
        structureSignal = (((bosUp || chochUp)) ? (1) : ((((bosDown || chochDown)) ? ((-1)) : (0))));
        bullFVG = ((current_bar_.low > _s_high[2]) && (_s_close[1] > _s_open[1]));
        bearFVG = ((current_bar_.high < _s_low[2]) && (_s_close[1] < _s_open[1]));
        minFVGSize = (atrVal * 0.3);
        if ((bullFVG && ((current_bar_.low - _s_high[2]) >= minFVGSize))) {
            fvgTop = current_bar_.low;
            fvgBottom = _s_high[2];
            fvgTops.push_back(fvgTop);
            fvgBottoms.push_back(fvgBottom);
            fvgBullish.push_back(true);
            if (showFVGZones) {
                fvgBoxes.push_back(newBox);
            }
        }
        if ((bearFVG && ((_s_low[2] - current_bar_.high) >= minFVGSize))) {
            fvgTop = _s_low[2];
            fvgBottom = current_bar_.high;
            fvgTops.push_back(fvgTop);
            fvgBottoms.push_back(fvgBottom);
            fvgBullish.push_back(false);
            if (showFVGZones) {
                fvgBoxes.push_back(newBox);
            }
        }
        while (((double)fvgTops.size() > 30)) {
            [&](){ auto v=fvgTops.front(); fvgTops.erase(fvgTops.begin()); return v; }();
            [&](){ auto v=fvgBottoms.front(); fvgBottoms.erase(fvgBottoms.begin()); return v; }();
            [&](){ auto v=fvgBullish.front(); fvgBullish.erase(fvgBullish.begin()); return v; }();
            if (((double)fvgBoxes.size() > 0)) {
                oldBox = [&](){ auto v=fvgBoxes.front(); fvgBoxes.erase(fvgBoxes.begin()); return v; }();
            }
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
        rsiValue = (is_first_tick_ ? _ta_rsi_4.compute(current_bar_.close) : _ta_rsi_4.recompute(current_bar_.close));
        rsiSmooth.push((is_first_tick_ ? _ta_ema_5.compute(rsiValue) : _ta_ema_5.recompute(rsiValue)));
        rsiMomentum = (is_first_tick_ ? _ta_change_6.compute(rsiSmooth[0]) : _ta_change_6.recompute(rsiSmooth[0]));
        momBullish.push((((rsiSmooth[0] > 50) && (rsiMomentum > 0)) && (rsiSmooth[0] < rsiOverbought)));
        momBearish.push((((rsiSmooth[0] < 50) && (rsiMomentum < 0)) && (rsiSmooth[0] > rsiOversold)));
        rsiDivBull = (((rsiSmooth[0] < 40) && (current_bar_.close > _s_close[5])) && (rsiSmooth[0] > rsiSmooth[5]));
        rsiDivBear = (((rsiSmooth[0] > 60) && (current_bar_.close < _s_close[5])) && (rsiSmooth[0] < rsiSmooth[5]));
        momentumSignal = ((momBullish[0]) ? (1) : (((momBearish[0]) ? ((-1)) : (0))));
        volMA = (is_first_tick_ ? _ta_sma_7.compute(current_bar_.volume) : _ta_sma_7.recompute(current_bar_.volume));
        volRatio = ((double)(current_bar_.volume) / (double)(volMA));
        highVolume = (volRatio >= volThreshold);
        buyVolume = (((current_bar_.close > current_bar_.open)) ? (current_bar_.volume) : (((double)((current_bar_.volume * (current_bar_.close - current_bar_.low))) / (double)(((current_bar_.high - current_bar_.low) + 0.0001)))));
        sellVolume = (((current_bar_.close < current_bar_.open)) ? (current_bar_.volume) : (((double)((current_bar_.volume * (current_bar_.high - current_bar_.close))) / (double)(((current_bar_.high - current_bar_.low) + 0.0001)))));
        volumeDelta = (buyVolume - sellVolume);
        cumDelta = (is_first_tick_ ? _ta_sum_8.compute(volumeDelta) : _ta_sum_8.recompute(volumeDelta));
        volStd = (is_first_tick_ ? _ta_stdev_9.compute(current_bar_.volume) : _ta_stdev_9.recompute(current_bar_.volume));
        volZScore = ((double)((current_bar_.volume - volMA)) / (double)(volStd));
        volAnomaly.push((std::abs(volZScore) > 2.0));
        volumeBullish = ((highVolume && (volumeDelta > 0)) && (cumDelta > 0));
        volumeBearish = ((highVolume && (volumeDelta < 0)) && (cumDelta < 0));
        volumeSignal = ((volumeBullish) ? (1) : (((volumeBearish) ? ((-1)) : (((highVolume) ? (0) : (0))))));
        if (useLiquidityFilter) {
            liquidity.sweepStrength = ((double)(std::abs(volumeDelta)) / (double)(volMA));
            liquidity.liquidityTaken = (liquidity.sweepStrength > 2.0);
            absorption = ((double)(std::abs(volumeDelta)) / (double)(current_bar_.volume));
            liquidity.absorptionRatio = absorption;
            if ((liquidity.absorptionRatio > 0.7)) {
                liquidity.liquidityLevel = 3;
            } else
            if ((liquidity.absorptionRatio > 0.5)) {
                liquidity.liquidityLevel = 2;
            } else
            if ((liquidity.absorptionRatio > 0.3)) {
                liquidity.liquidityLevel = 1;
            } else {
                liquidity.liquidityLevel = 0;
            }
        }
        _smPriceChange = (is_first_tick_ ? _ta_change_10.compute(current_bar_.close) : _ta_change_10.recompute(current_bar_.close));
        _smVolumeChange = (is_first_tick_ ? _ta_change_11.compute(current_bar_.volume) : _ta_change_11.recompute(current_bar_.volume));
        if (useSmartMoney) {
            smartMoney.institutionalDelta = (volumeDelta * volRatio);
            smartMoney.isAccumulating = (((_smPriceChange > 0) && (_smVolumeChange > 0)) && (smartMoney.institutionalDelta > 0));
            smartMoney.isDistributing = (((_smPriceChange < 0) && (_smVolumeChange > 0)) && (smartMoney.institutionalDelta < 0));
            smScore = 0.0;
            if (smartMoney.isAccumulating) {
                smScore += ((double)(std::abs(smartMoney.institutionalDelta)) / (double)(volMA));
            }
            if (smartMoney.isDistributing) {
                smScore -= ((double)(std::abs(smartMoney.institutionalDelta)) / (double)(volMA));
            }
            if (volAnomaly[0]) {
                smScore *= 1.5;
            }
            smartMoney.smartMoneyScore = std::min((double)(100), (double)(std::max((double)((-100)), (double)((smScore * 10)))));
        }
        _htfEmaGlobal = (is_first_tick_ ? _ta_ema_12.compute(current_bar_.close) : _ta_ema_12.recompute(current_bar_.close));
        _htfRsiGlobal = (is_first_tick_ ? _ta_rsi_13.compute(current_bar_.close) : _ta_rsi_13.recompute(current_bar_.close));
        htfClose = _req_sec_0;
        htfEMA = _req_sec_1;
        htfRSI = _req_sec_2;
        htfTrend = (((htfClose > htfEMA)) ? (1) : ((((htfClose < htfEMA)) ? ((-1)) : (0))));
        htfMomBull = (htfRSI > 50);
        htfMomBear = (htfRSI < 50);
        mtfBullish.push(((htfTrend > 0) && htfMomBull));
        mtfBearish.push(((htfTrend < 0) && htfMomBear));
        mtfSignal = ((mtfBullish[0]) ? (1) : (((mtfBearish[0]) ? ((-1)) : (0))));
        if (useQuantumFilter) {
            trendAlignment = ((double)((((structureDirection[0] + momentumSignal) + volumeSignal) + mtfSignal)) / (double)(4));
            quantum.coherence = std::abs(trendAlignment);
            quantum.entanglement = ((((htfTrend == structureDirection[0])) ? (0.5) : ((-0.5))) + (((mtfSignal == momentumSignal)) ? (0.5) : ((-0.5))));
            quantum.superposition = ((double)((quantum.coherence + quantum.entanglement)) / (double)(2));
            quantum.isCollapsed = (quantum.superposition > quantumThreshold);
            quantum.wavePhase = std::fmod((double)(((double)(current_bar_.timestamp) / (double)(1000))), (double)(360));
        }
        if (barstate_islast_) {
            if ((bar_index_ != lastAnimationBar)) {
                animationPhase = std::fmod((double)((animationPhase + 5)), (double)(360));
                lastAnimationBar = bar_index_;
            }
        }
        inActiveSession = !(is_na(pine_time(current_bar_.timestamp, script_tf_, activeSession, sessionTZ, script_tf_)));
        sessionSignal = ((inActiveSession) ? (1) : (0));
        adxLen = 14;
        upMove = (is_first_tick_ ? _ta_change_14.compute(current_bar_.high) : _ta_change_14.recompute(current_bar_.high));
        downMove = (-(is_first_tick_ ? _ta_change_15.compute(current_bar_.low) : _ta_change_15.recompute(current_bar_.low)));
        plusDM = ((is_na(upMove)) ? (na<double>()) : (((((upMove > downMove) && (upMove > 0))) ? (upMove) : (0))));
        minusDM = ((is_na(downMove)) ? (na<double>()) : (((((downMove > upMove) && (downMove > 0))) ? (downMove) : (0))));
        trueRange = (is_first_tick_ ? _ta_rma_16.compute((std::isnan(_s_close[1]) ? (current_bar_.high - current_bar_.low) : std::max(current_bar_.high - current_bar_.low, std::max(std::abs(current_bar_.high - _s_close[1]), std::abs(current_bar_.low - _s_close[1]))))) : _ta_rma_16.recompute((std::isnan(_s_close[1]) ? (current_bar_.high - current_bar_.low) : std::max(current_bar_.high - current_bar_.low, std::max(std::abs(current_bar_.high - _s_close[1]), std::abs(current_bar_.low - _s_close[1]))))));
        plusDI = ((double)((100 * (is_first_tick_ ? _ta_rma_17.compute(plusDM) : _ta_rma_17.recompute(plusDM)))) / (double)(trueRange));
        minusDI = ((double)((100 * (is_first_tick_ ? _ta_rma_18.compute(minusDM) : _ta_rma_18.recompute(minusDM)))) / (double)(trueRange));
        dx = ((double)((100 * std::abs((plusDI - minusDI)))) / (double)((plusDI + minusDI)));
        adxValue = (is_first_tick_ ? _ta_rma_19.compute(dx) : _ta_rma_19.recompute(dx));
        regime.isTrending = (adxValue > 25);
        regime.trend = (((plusDI > minusDI)) ? (1) : ((-1)));
        regime.volatility = (((double)(atrVal) / (double)(current_bar_.close)) * 100);
        regime.strength = adxValue;
        regimeAllowsLong = ((!(useRegimeFilter) || (regime.isTrending && (regime.trend > 0))) || !(regime.isTrending));
        regimeAllowsShort = ((!(useRegimeFilter) || (regime.isTrending && (regime.trend < 0))) || !(regime.isTrending));
        auto [structScore, momScore, volScore, mtfScore, sessScore, liqScore, quantumScore, smScore, harmonicScore] = calcConfluence();
        confluence.structure = structScore;
        confluence.momentum = momScore;
        confluence.volume = volScore;
        confluence.mtf = mtfScore;
        confluence.session = sessScore;
        confluence.liquidity = liqScore;
        confluence.quantum = quantumScore;
        confluence.smartMoney = smScore;
        confluence.harmonic = harmonicScore;
        confluence.total = ((((((((structScore + momScore) + volScore) + mtfScore) + sessScore) + liqScore) + quantumScore) + smScore) + harmonicScore);
        seriesQuantumCollapsed.update(quantum.isCollapsed);
        seriesQuantumCoherence.update(quantum.coherence);
        seriesQuantumEntanglement.update(quantum.entanglement);
        seriesLiquidityTaken.update(liquidity.liquidityTaken);
        seriesSmartMoneyAccumulating.update(smartMoney.isAccumulating);
        seriesSmartMoneyDistributing.update(smartMoney.isDistributing);
        seriesConfluenceHarmonic.update(confluence.harmonic);
        seriesRegimeTrending.update(regime.isTrending);
        seriesRegimeTrend.update(regime.trend);
        _confStructBull = (structureDirection[0] > 0);
        _confMomBull = (!(requireMomAlign) || (momentumSignal >= 0));
        _confMtfBull = (!(useMTFFilter) || (mtfSignal >= 0));
        _confVolBull = (!(useVolumeFilter) || (volumeSignal >= 0));
        _confSessBull = (!(useSessionFilter) || inActiveSession);
        _confQuantBull = (!(useQuantumFilter) || (quantum.superposition > 0));
        _confSmBull = (!(useSmartMoney) || (smartMoney.smartMoneyScore >= 0));
        bullConfluence = (((((((((confluence.total >= minConfluence) && _confStructBull) && _confMomBull) && _confMtfBull) && _confVolBull) && _confSessBull) && regimeAllowsLong) && _confQuantBull) && _confSmBull);
        _confStructBear = (structureDirection[0] < 0);
        _confMomBear = (!(requireMomAlign) || (momentumSignal <= 0));
        _confMtfBear = (!(useMTFFilter) || (mtfSignal <= 0));
        _confVolBear = (!(useVolumeFilter) || (volumeSignal <= 0));
        _confSessBear = (!(useSessionFilter) || inActiveSession);
        _confQuantBear = (!(useQuantumFilter) || (quantum.superposition < 0));
        _confSmBear = (!(useSmartMoney) || (smartMoney.smartMoneyScore <= 0));
        bearConfluence = (((((((((confluence.total >= minConfluence) && _confStructBear) && _confMomBear) && _confMtfBear) && _confVolBear) && _confSessBear) && regimeAllowsShort) && _confQuantBear) && _confSmBear);
        _fvgLong = ((requireFVG) ? (inBullFVG) : (true));
        _bosLong = ((requireBOS) ? ((bosUp || chochUp)) : (true));
        longEntry = ((((useLongTrades && bullConfluence) && _fvgLong) && _bosLong) && (signed_position_size() == 0));
        _fvgShort = ((requireFVG) ? (inBearFVG) : (true));
        _bosShort = ((requireBOS) ? ((bosDown || chochDown)) : (true));
        shortEntry = ((((useShortTrades && bearConfluence) && _fvgShort) && _bosShort) && (signed_position_size() == 0));
        _recentLow = (is_first_tick_ ? _ta_lowest_20.compute(current_bar_.low) : _ta_lowest_20.recompute(current_bar_.low));
        _recentHigh = (is_first_tick_ ? _ta_highest_21.compute(current_bar_.high) : _ta_highest_21.recompute(current_bar_.high));
        if (longEntry) {
            sl = calcStopLoss(true);
            tp = calcTakeProfit(current_bar_.close, sl, true);
            trade.inPosition = true;
            trade.direction = 1;
            trade.entryPrice = current_bar_.close;
            trade.stopLoss = sl;
            trade.takeProfit = tp;
            trade.trailStop = sl;
            trade.entryBar = bar_index_;
            strategy_entry(std::string("Long"), true, na<double>(), na<double>(), na<double>(), "");
            strategy_exit(std::string("Long Exit"), std::string("Long"), tp, sl, na<double>(), na<double>(), na<double>(), 100.0, "", na<double>(), "");
        }
        if (shortEntry) {
            sl = calcStopLoss(false);
            tp = calcTakeProfit(current_bar_.close, sl, false);
            trade.inPosition = true;
            trade.direction = (-1);
            trade.entryPrice = current_bar_.close;
            trade.stopLoss = sl;
            trade.takeProfit = tp;
            trade.trailStop = sl;
            trade.entryBar = bar_index_;
            strategy_entry(std::string("Short"), false, na<double>(), na<double>(), na<double>(), "");
            strategy_exit(std::string("Short Exit"), std::string("Short"), tp, sl, na<double>(), na<double>(), na<double>(), 100.0, "", na<double>(), "");
        }
        if ((useTrailStop && (signed_position_size() != 0))) {
            trailDistance = (atrVal * trailATRMult);
            if ((signed_position_size() > 0)) {
                newTrail = (current_bar_.high - trailDistance);
                if ((newTrail > trade.trailStop)) {
                    trade.trailStop = newTrail;
                    strategy_exit(std::string("Long Exit"), std::string("Long"), trade.takeProfit, trade.trailStop, na<double>(), na<double>(), na<double>(), 100.0, "", na<double>(), "");
                }
            }
            if ((signed_position_size() < 0)) {
                newTrail = (current_bar_.low + trailDistance);
                if ((newTrail < trade.trailStop)) {
                    trade.trailStop = newTrail;
                    strategy_exit(std::string("Short Exit"), std::string("Short"), trade.takeProfit, trade.trailStop, na<double>(), na<double>(), na<double>(), 100.0, "", na<double>(), "");
                }
            }
        }
        if (((signed_position_size() == 0) && trade.inPosition)) {
            trade.inPosition = false;
            trade.direction = 0;
            trade.entryPrice = na<double>();
            trade.stopLoss = na<double>();
            trade.takeProfit = na<double>();
            trade.trailStop = na<double>();
        }
        longEntryColor = getEntryColor(true);
        shortEntryColor = getEntryColor(false);
        if ((showGlowEffect && (longEntry || shortEntry))) {
            glowAlpha = (50 + (std::sin(((double)((animationPhase * M_PI)) / (double)(180))) * 30));
            longEntryColor = pine_color::new_color(longEntryColor, (int)(std::round((100 - glowAlpha))));
            shortEntryColor = pine_color::new_color(shortEntryColor, (int)(std::round((100 - glowAlpha))));
        }
        structureColor = (((structureDirection[0] > 0)) ? (VCP_STRUCTURE_BULL) : ((((structureDirection[0] < 0)) ? (VCP_STRUCTURE_BEAR) : (VCP_NEUTRAL))));
        bgColor = na<double>();
        if ((signed_position_size() > 0)) {
            bgColor = pine_color::new_color(VCP_BULL_ENTRY, (int)(95));
        } else
        if ((signed_position_size() < 0)) {
            bgColor = pine_color::new_color(VCP_BEAR_ENTRY, (int)(95));
        }
        sessionBg = ((inActiveSession) ? (pine_color::new_color(VCP_NEUTRAL, (int)(97))) : (na<double>()));
        quantumBg = (((useQuantumFilter && quantum.isCollapsed)) ? (pine_color::new_color(VCP_QUANTUM, (int)(98))) : (na<double>()));
        smBg = (((useSmartMoney && (smartMoney.isAccumulating || smartMoney.isDistributing))) ? (pine_color::new_color(VCP_SMART_MONEY, (int)(99))) : (na<double>()));
        if (showDashboard) {
            if (barstate_islast_) {
                headerBg = (((colorScheme == std::string("Quantum"))) ? (pine_color::new_color(VCP_GLOW, (int)(90))) : (pine_color::new_color(0, (int)(10))));
                positionText = (((signed_position_size() > 0)) ? (std::string("LONG")) : ((((signed_position_size() < 0)) ? (std::string("SHORT")) : (std::string("FLAT")))));
                positionCol = (((signed_position_size() > 0)) ? (VCP_BULL_ENTRY) : ((((signed_position_size() < 0)) ? (VCP_BEAR_ENTRY) : (VCP_NEUTRAL))));
                structText = (((structureDirection[0] > 0)) ? (std::string("BULLISH")) : ((((structureDirection[0] < 0)) ? (std::string("BEARISH")) : (std::string("NEUTRAL")))));
                structCol = (((structureDirection[0] > 0)) ? (VCP_STRUCTURE_BULL) : ((((structureDirection[0] < 0)) ? (VCP_STRUCTURE_BEAR) : (VCP_NEUTRAL))));
                regimeText = ((regime.isTrending) ? ((((regime.trend > 0)) ? (std::string("TREND UP")) : (std::string("TREND DOWN")))) : (std::string("RANGING")));
                regimeCol = ((regime.isTrending) ? ((((regime.trend > 0)) ? (VCP_STRUCTURE_BULL) : (VCP_STRUCTURE_BEAR))) : (VCP_NEUTRAL));
                momText = (((momentumSignal > 0)) ? (std::string("BULLISH")) : ((((momentumSignal < 0)) ? (std::string("BEARISH")) : (std::string("NEUTRAL")))));
                momCol = (((momentumSignal > 0)) ? (VCP_BULL_ENTRY) : ((((momentumSignal < 0)) ? (VCP_BEAR_ENTRY) : (VCP_NEUTRAL))));
                volText = (((volumeSignal > 0)) ? (std::string("BUY PRESSURE")) : ((((volumeSignal < 0)) ? (std::string("SELL PRESSURE")) : (std::string("NEUTRAL")))));
                volCol = (((volumeSignal > 0)) ? (VCP_BULL_ENTRY) : ((((volumeSignal < 0)) ? (VCP_BEAR_ENTRY) : (VCP_NEUTRAL))));
                mtfText = (((mtfSignal > 0)) ? (std::string("BULLISH")) : ((((mtfSignal < 0)) ? (std::string("BEARISH")) : (std::string("NEUTRAL")))));
                mtfCol = (((mtfSignal > 0)) ? (VCP_BULL_ENTRY) : ((((mtfSignal < 0)) ? (VCP_BEAR_ENTRY) : (VCP_NEUTRAL))));
                sessText = ((inActiveSession) ? (std::string("ACTIVE")) : (std::string("INACTIVE")));
                sessCol = ((inActiveSession) ? (VCP_BULL_ENTRY) : (VCP_NEUTRAL));
                maxConfluence = 9;
                confText = ((std::to_string(confluence.total) + std::string("/")) + std::to_string(maxConfluence));
                confCol = (((confluence.total >= minConfluence)) ? (VCP_BULL_ENTRY) : (VCP_NEUTRAL));
                if (useLiquidityFilter) {
                    liqText = ((liquidity.liquidityTaken) ? (std::string("SWEEP")) : (std::string("NORMAL")));
                    liqCol = ((liquidity.liquidityTaken) ? (VCP_LIQUIDITY_BULL) : (VCP_NEUTRAL));
                }
                if (useQuantumFilter) {
                    quantumText = ((quantum.isCollapsed) ? (std::string("COLLAPSED")) : (std::string("SUPERPOS")));
                    quantumCol = ((quantum.isCollapsed) ? (VCP_QUANTUM) : (VCP_ENTANGLEMENT));
                }
                if (useSmartMoney) {
                    smText = ((smartMoney.isAccumulating) ? (std::string("ACCUM")) : (((smartMoney.isDistributing) ? (std::string("DIST")) : (std::string("NEUTRAL")))));
                    smCol = ((smartMoney.isAccumulating) ? (VCP_SMART_MONEY) : (((smartMoney.isDistributing) ? (VCP_WARNING) : (VCP_NEUTRAL))));
                }
                if (useHarmonicPattern) {
                    harmonicText = (((confluence.harmonic > 0)) ? (std::string("ACTIVE")) : (std::string("INACTIVE")));
                    harmonicCol = (((confluence.harmonic > 0)) ? (VCP_HARMONIC) : (VCP_NEUTRAL));
                }
                if ((signed_position_size() != 0)) {
                    pnlPct = (((signed_position_size() > 0)) ? ((((double)((current_bar_.close - trade.entryPrice)) / (double)(trade.entryPrice)) * 100)) : ((((double)((trade.entryPrice - current_bar_.close)) / (double)(trade.entryPrice)) * 100)));
                    pnlCol = (((pnlPct > 0)) ? (VCP_BULL_ENTRY) : (VCP_BEAR_ENTRY));
                } else {
                }
            }
        }
        if (trace_enabled_) {
            trace(std::string("conf_total"), (double)(confluence.total));
            trace(std::string("conf_struct"), (double)(_confStructBull));
            trace(std::string("conf_mom"), (double)(_confMomBull));
            trace(std::string("conf_mtf"), (double)(_confMtfBull));
            trace(std::string("conf_vol"), (double)(_confVolBull));
            trace(std::string("conf_sess"), (double)(_confSessBull));
            trace(std::string("conf_regime_long"), (double)(regimeAllowsLong));
            trace(std::string("conf_quant"), (double)(_confQuantBull));
            trace(std::string("conf_sm"), (double)(_confSmBull));
            trace(std::string("bull_confluence"), (double)(bullConfluence));
            trace(std::string("fvg_long"), (double)(_fvgLong));
            trace(std::string("bos_long"), (double)(_bosLong));
            trace(std::string("long_entry"), (double)(longEntry));
            trace(std::string("pos_size"), (double)(signed_position_size()));
            trace(std::string("struct_dir"), (double)(structureDirection[0]));
            trace(std::string("mom_signal"), (double)(momentumSignal));
            trace(std::string("mtf_signal"), (double)(mtfSignal));
            trace(std::string("vol_signal"), (double)(volumeSignal));
            trace(std::string("adx_value"), (double)(adxValue));
            trace(std::string("quantum_super"), (double)(quantum.superposition));
            trace(std::string("sm_score"), (double)(smartMoney.smartMoneyScore));
            trace(std::string("in_bull_fvg"), (double)(inBullFVG));
            trace(std::string("bos_up"), (double)(bosUp));
            trace(std::string("bos_down"), (double)(bosDown));
            trace(std::string("choch_up"), (double)(chochUp));
            trace(std::string("choch_down"), (double)(chochDown));
            trace(std::string("last_swing_high"), (double)((is_na(lastSwingHigh) ? 0.0 : lastSwingHigh)));
            trace(std::string("prev_swing_high"), (double)((is_na(prevSwingHigh) ? 0.0 : prevSwingHigh)));
            trace(std::string("last_swing_low"), (double)((is_na(lastSwingLow) ? 0.0 : lastSwingLow)));
            trace(std::string("prev_swing_low"), (double)((is_na(prevSwingLow) ? 0.0 : prevSwingLow)));
            trace(std::string("pivot_high"), (double)((is_na(pivotHigh) ? 0.0 : pivotHigh)));
            trace(std::string("pivot_low"), (double)((is_na(pivotLow) ? 0.0 : pivotLow)));
            trace(std::string("vcp_longEntry"), (double)(longEntry));
            trace(std::string("vcp_shortEntry"), (double)(shortEntry));
            trace(std::string("vcp_tradeInPosition"), (double)(trade.inPosition));
            trace(std::string("vcp_positionSize"), (double)(signed_position_size()));
            trace(std::string("vcp_trailStop"), (double)(trade.trailStop));
        }
    }

    void _eval_security_0(const Bar& bar, bool is_complete) {
        _req_sec_0 = bar.close;
    }

    void _eval_security_1(const Bar& bar, bool is_complete) {
        auto _secval_11 = is_complete ? _sec1__ta_ema_12.compute(bar.close) : _sec1__ta_ema_12.recompute(bar.close);
        _req_sec_1 = _secval_11;
    }

    void _eval_security_2(const Bar& bar, bool is_complete) {
        auto _secval_12 = is_complete ? _sec2__ta_rsi_13.compute(bar.close) : _sec2__ta_rsi_13.recompute(bar.close);
        _req_sec_2 = _secval_12;
    }

    void evaluate_security(int sec_id, const Bar& bar, bool is_complete) override {
        if (!_ta_initialized_) {
            _ta_pivothigh_2 = ta::PivotHigh(get_input_int("Pivot Strength", 5), get_input_int("Pivot Strength", 5));
            _ta_pivotlow_3 = ta::PivotLow(get_input_int("Pivot Strength", 5), get_input_int("Pivot Strength", 5));
            _ta_rsi_4 = ta::RSI(get_input_int("RSI Length", 14));
            _ta_sma_7 = ta::SMA(get_input_int("Volume MA Length", 20));
            _ta_ema_12 = ta::EMA(get_input_int("MTF Trend Length", 20));
            _ta_rsi_13 = ta::RSI(get_input_int("RSI Length", 14));
            _ta_lowest_20 = ta::Lowest(get_input_int("Pivot Strength", 5) * 2);
            _ta_highest_21 = ta::Highest(get_input_int("Pivot Strength", 5) * 2);
            _sec1__ta_ema_12 = ta::EMA(get_input_int("MTF Trend Length", 20));
            _sec2__ta_rsi_13 = ta::RSI(get_input_int("RSI Length", 14));
            _ta_initialized_ = true;
        }
        switch (sec_id) {
            case 0: _eval_security_0(bar, is_complete); break;
            case 1: _eval_security_1(bar, is_complete); break;
            case 2: _eval_security_2(bar, is_complete); break;
        }
    }
    void clear_security(int sec_id) override {
        switch (sec_id) {
            case 0: _req_sec_0 = na<double>(); break;
            case 1: _req_sec_1 = na<double>(); break;
            case 2: _req_sec_2 = na<double>(); break;
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
