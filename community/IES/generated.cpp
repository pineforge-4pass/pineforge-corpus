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
    double _req_sec_0 = na<double>();
    double _req_sec_1 = na<double>();
    double _req_sec_2 = na<double>();
    std::unordered_map<std::string, Series<double>> _security_helper_series_;
    ta::TR _ta_tr_1;
    ta::RMA _ta_rma_2;
    ta::RMA _ta_rma_3;
    ta::RMA _ta_rma_4;
    ta::RMA _ta_rma_5;
    ta::ATR _ta_atr_6;
    ta::SMA _ta_sma_7;
    ta::EMA _ta_ema_8;
    ta::EMA _ta_ema_9;
    ta::EMA _ta_ema_10;
    ta::RSI _ta_rsi_11;
    ta::EMA _ta_ema_12;
    ta::EMA _ta_ema_13;
    ta::EMA _ta_ema_14;
    ta::EMA _ta_ema_15;
    ta::EMA _ta_ema_16;
    ta::SMA _ta_sma_17;
    ta::StdDev _ta_stdev_18;
    ta::EMA _ta_ema_19;
    ta::ATR _ta_atr_20;
    ta::EMA _ta_ema_21;
    ta::EMA _ta_ema_22;
    ta::EMA _ta_ema_23;
    ta::EMA _ta_ema_21_cs1;
    ta::EMA _ta_ema_22_cs1;
    ta::EMA _ta_ema_23_cs1;
    ta::EMA _ta_ema_21_cs2;
    ta::EMA _ta_ema_22_cs2;
    ta::EMA _ta_ema_23_cs2;
    ta::PivotHigh _ta_pivothigh_24;
    ta::PivotLow _ta_pivotlow_25;
    ta::SMA _ta_sma_26;
    ta::Crossover _ta_crossover_27;
    ta::Crossover _ta_crossover_28;
    ta::Crossunder _ta_crossunder_29;
    ta::Crossunder _ta_crossunder_30;
    ta::Change _ta_change_31;
    ta::EMA _sec0__ta_ema_21_v0;
    ta::EMA _sec0__ta_ema_21_v1;
    ta::EMA _sec0__ta_ema_21_v2;
    ta::EMA _sec0__ta_ema_22_v0;
    ta::EMA _sec0__ta_ema_22_v1;
    ta::EMA _sec0__ta_ema_22_v2;
    ta::EMA _sec0__ta_ema_23_v0;
    ta::EMA _sec0__ta_ema_23_v1;
    ta::EMA _sec1__ta_ema_21_v0;
    ta::EMA _sec1__ta_ema_21_v1;
    ta::EMA _sec1__ta_ema_21_v2;
    ta::EMA _sec1__ta_ema_22_v0;
    ta::EMA _sec1__ta_ema_22_v1;
    ta::EMA _sec1__ta_ema_22_v2;
    ta::EMA _sec1__ta_ema_23_v0;
    ta::EMA _sec1__ta_ema_23_v1;
    ta::EMA _sec2__ta_ema_21_v0;
    ta::EMA _sec2__ta_ema_21_v1;
    ta::EMA _sec2__ta_ema_21_v2;
    ta::EMA _sec2__ta_ema_22_v0;
    ta::EMA _sec2__ta_ema_22_v1;
    ta::EMA _sec2__ta_ema_22_v2;
    ta::EMA _sec2__ta_ema_23_v0;
    ta::EMA _sec2__ta_ema_23_v1;
    Series<double> _s_high;
    Series<double> _s_low;
    double last_swing_high;
    double last_swing_low;
    int bars_since_trade;
    int daily_trades;
    double entry_price;
    double initial_stop;
    double trailing_stop;
    bool is_long;
    bool is_short;
    double dash;
    Series<double> macd_hist;
    Series<double> pressure_smooth;
    Series<double> rsi;
    Series<bool> squeeze_on;
    std::string g_regime = std::string("");
    int i_adx_len = 0;
    double i_adx_trend = 0.0;
    int i_atr_len = 0;
    double i_vol_exp = 0.0;
    double i_vol_con = 0.0;
    bool i_avoid_volatile = false;
    std::string g_bias = std::string("");
    int i_ma_fast = 0;
    int i_ma_slow = 0;
    int i_ma_trend = 0;
    double i_bias_thresh = 0.0;
    std::string g_momentum = std::string("");
    int i_rsi_len = 0;
    double i_rsi_bull = 0.0;
    double i_rsi_bear = 0.0;
    int i_macd_fast = 0;
    int i_macd_slow = 0;
    int i_macd_sig = 0;
    std::string g_structure = std::string("");
    int i_swing_left = 0;
    int i_swing_right = 0;
    bool i_require_struct = false;
    std::string g_risk = std::string("");
    double i_risk_pct = 0.0;
    double i_atr_stop_mult = 0.0;
    double i_rr_target1 = 0.0;
    double i_rr_target2 = 0.0;
    double i_trail_start = 0.0;
    double i_trail_offset = 0.0;
    double i_channel_mult = 0.0;
    std::string g_frequency = std::string("");
    bool i_use_trade_limit = false;
    int i_max_trades = 0;
    int i_cooldown = 0;
    bool i_require_close = false;
    bool i_allow_flip = false;
    std::string g_filters = std::string("");
    bool i_use_vol_filter = false;
    int i_vol_len = 0;
    double i_vol_mult = 0.0;
    int i_min_qual = 0;
    std::string g_mtf = std::string("");
    bool i_use_mtf = false;
    bool i_mtf_filter = false;
    std::string i_mtf_tf1 = std::string("");
    std::string i_mtf_tf2 = std::string("");
    std::string i_mtf_tf3 = std::string("");
    int i_mtf_majority = 0;
    std::string g_pressure = std::string("");
    bool i_use_pressure = false;
    int i_pressure_len = 0;
    int i_pressure_smooth = 0;
    int i_pressure_mom = 0;
    double i_pressure_high = 0.0;
    double i_pressure_low = 0.0;
    double i_pressure_thresh = 0.0;
    std::string g_squeeze = std::string("");
    bool i_use_squeeze = false;
    int i_bb_len = 0;
    double i_bb_mult = 0.0;
    int i_kc_len = 0;
    double i_kc_mult = 0.0;
    std::string g_exits = std::string("");
    bool i_scale_out = false;
    int i_scale_pct = 0;
    std::string g_adaptive = std::string("");
    bool i_adapt_size = false;
    double i_trend_mult = 0.0;
    double i_range_mult = 0.0;
    bool i_adapt_stops = false;
    double i_vol_stop_mult = 0.0;
    std::string g_visual = std::string("");
    int i_bull_col = 0;
    int i_bull_glow = 0;
    int i_bear_col = 0;
    int i_bear_glow = 0;
    int i_neutral_col = 0;
    int i_trend_bg = 0;
    int i_range_bg = 0;
    int i_vol_bg = 0;
    bool i_show_dash = false;
    bool i_show_levels = false;
    bool i_show_quality = false;
    bool i_show_bg = false;
    bool i_show_trend_band = false;
    bool i_show_channel = false;
    bool i_show_signals = false;
    bool i_show_zones = false;
    int i_band_style = 0;
    int i_signal_style = 0;
    std::string g_concepts = std::string("");
    bool i_show_concepts = false;
    bool i_show_squeeze_boxes = false;
    bool i_show_sweep_marks = false;
    bool i_show_structure_levels = false;
    bool i_show_mtf_ribbon = false;
    bool i_show_fvg = false;
    bool i_show_order_blocks = false;
    bool i_show_session_boxes = false;
    double atr_val = 0.0;
    double atr_ma = 0.0;
    double vol_ratio = 0.0;
    int regime = 0;
    bool regime_allows_trading = false;
    bool trending_regime = false;
    bool ranging_regime = false;
    bool volatile_regime = false;
    double regime_strength = 0.0;
    double regime_confidence = 0.0;
    double ma_fast = 0.0;
    double ma_slow = 0.0;
    double ma_trend = 0.0;
    double ma_spread = 0.0;
    bool ma_bullish = false;
    bool ma_bearish = false;
    bool price_above_structure = false;
    bool price_below_structure = false;
    double bias_score = 0.0;
    double bear_bias_score = 0.0;
    bool bullish_bias = false;
    bool bearish_bias = false;
    bool rsi_bullish = false;
    bool rsi_bearish = false;
    bool rsi_momentum_up = false;
    bool rsi_momentum_down = false;
    double macd_line = 0.0;
    double macd_signal = 0.0;
    bool macd_bullish = false;
    bool macd_bearish = false;
    int momentum_bull_score = 0;
    int momentum_bear_score = 0;
    double range_val = 0.0;
    double raw_buy = 0.0;
    double raw_sell = 0.0;
    double pressure_ratio = 0.0;
    double pressure_momentum = 0.0;
    int pressure_state = 0;
    bool pressure_bull = false;
    bool pressure_bear = false;
    double bb_basis = 0.0;
    double bb_dev = 0.0;
    double bb_upper = 0.0;
    double bb_lower = 0.0;
    double kc_basis = 0.0;
    double kc_range = 0.0;
    double kc_upper = 0.0;
    double kc_lower = 0.0;
    bool squeeze_release = false;
    double mtf_hist_1 = 0.0;
    double mtf_hist_2 = 0.0;
    double mtf_hist_3 = 0.0;
    double mtf_bull_count = 0.0;
    double mtf_bear_count = 0.0;
    bool mtf_bias_long = false;
    bool mtf_bias_short = false;
    bool mtf_allows_long = false;
    bool mtf_allows_short = false;
    double swing_high = 0.0;
    double swing_low = 0.0;
    bool sweep_high = false;
    bool sweep_low = false;
    bool above_swing_low = false;
    bool below_swing_high = false;
    int struct_bull_score = 0;
    int struct_bear_score = 0;
    bool struct_allows_long = false;
    bool struct_allows_short = false;
    double vol_ma = 0.0;
    bool volume_confirm = false;
    int max_score = 0;
    double bull_total = 0.0;
    double bear_total = 0.0;
    bool bull_qualified = false;
    bool bear_qualified = false;
    double bull_quality = 0.0;
    double bear_quality = 0.0;
    std::string bull_grade = std::string("");
    std::string bear_grade = std::string("");
    bool bull_premium = false;
    bool bear_premium = false;
    bool momentum_trigger_long = false;
    bool momentum_trigger_short = false;
    bool squeeze_trigger_long = false;
    bool squeeze_trigger_short = false;
    bool sweep_trigger_long = false;
    bool sweep_trigger_short = false;
    bool cross_ma_long = false;
    bool cross_macd_long = false;
    bool cross_ma_short = false;
    bool cross_macd_short = false;
    bool bull_trigger = false;
    bool bear_trigger = false;
    bool new_day = false;
    bool within_trade_limit = false;
    bool entry_confirm = false;
    bool long_entry = false;
    bool short_entry = false;
    double adaptive_stop_mult = 0.0;
    double long_stop_struct = 0.0;
    double long_stop_atr = 0.0;
    double long_stop = 0.0;
    double short_stop_struct = 0.0;
    double short_stop_atr = 0.0;
    double short_stop = 0.0;
    double long_risk = 0.0;
    double short_risk = 0.0;
    double adaptive_target_mult = 0.0;
    double long_tp1 = 0.0;
    double long_tp2 = 0.0;
    double short_tp1 = 0.0;
    double short_tp2 = 0.0;
    double long_trail_start = 0.0;
    double short_trail_start = 0.0;
    double long_trail_offset = 0.0;
    double short_trail_offset = 0.0;
    double regime_size_mult = 0.0;
    double quality_mult = 0.0;
    double account_risk = 0.0;
    double long_position_size = 0.0;
    double short_position_size = 0.0;
    double profit = 0.0;
    double new_trail = 0.0;
    double trend_bg_color = 0.0;
    int ma_color = 0;
    std::string regime_text = std::string("");
    int regime_col = 0;
    std::string pos_text = std::string("");
    int pos_col = 0;
    double unrealized_pnl = 0.0;
    double unrealized_pct = 0.0;
    std::string quality_text = std::string("");
    double quality_col = 0.0;
    bool _var_initialized = false;
    bool _ta_initialized_ = false;

    explicit GeneratedStrategy() : _ta_tr_1(true), _ta_rma_2(14), _ta_rma_3(14), _ta_rma_4(14), _ta_rma_5(14), _ta_atr_6(14), _ta_sma_7(42), _ta_ema_8(21), _ta_ema_9(55), _ta_ema_10(200), _ta_rsi_11(14), _ta_ema_12(12), _ta_ema_13(26), _ta_ema_14(9), _ta_ema_15(14), _ta_ema_16(5), _ta_sma_17(20), _ta_stdev_18(20), _ta_ema_19(20), _ta_atr_20(20), _ta_ema_21(12), _ta_ema_22(26), _ta_ema_23(9), _ta_ema_21_cs1(12), _ta_ema_22_cs1(26), _ta_ema_23_cs1(9), _ta_ema_21_cs2(12), _ta_ema_22_cs2(26), _ta_ema_23_cs2(9), _ta_pivothigh_24(10, 5), _ta_pivotlow_25(10, 5), _ta_sma_26(20), _sec0__ta_ema_21_v0(12), _sec0__ta_ema_21_v1(12), _sec0__ta_ema_21_v2(12), _sec0__ta_ema_22_v0(26), _sec0__ta_ema_22_v1(26), _sec0__ta_ema_22_v2(26), _sec0__ta_ema_23_v0(9), _sec0__ta_ema_23_v1(9), _sec1__ta_ema_21_v0(12), _sec1__ta_ema_21_v1(12), _sec1__ta_ema_21_v2(12), _sec1__ta_ema_22_v0(26), _sec1__ta_ema_22_v1(26), _sec1__ta_ema_22_v2(26), _sec1__ta_ema_23_v0(9), _sec1__ta_ema_23_v1(9), _sec2__ta_ema_21_v0(12), _sec2__ta_ema_21_v1(12), _sec2__ta_ema_21_v2(12), _sec2__ta_ema_22_v0(26), _sec2__ta_ema_22_v1(26), _sec2__ta_ema_22_v2(26), _sec2__ta_ema_23_v0(9), _sec2__ta_ema_23_v1(9), last_swing_high(na<double>()), last_swing_low(na<double>()), bars_since_trade(999), daily_trades(0), entry_price(na<double>()), initial_stop(na<double>()), trailing_stop(na<double>()), is_long(false), is_short(false) {
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
        register_security_eval(0, get_input_string("MTF 1H", std::string("60")), input_tf_, false, false);
        register_security_eval(1, get_input_string("MTF 4H", std::string("240")), input_tf_, false, false);
        register_security_eval(2, get_input_string("MTF 1D", std::string("1D")), input_tf_, false, false);
    }

    std::tuple<double, double, double> f_adx_calc_cs0(int len) {
        double tr_val = (is_first_tick_ ? _ta_tr_1.compute(current_bar_.high, current_bar_.low, current_bar_.close) : _ta_tr_1.recompute(current_bar_.high, current_bar_.low, current_bar_.close));
        double plus_dm = std::max((double)((current_bar_.high - _s_high[1])), (double)(0));
        double minus_dm = std::max((double)((_s_low[1] - current_bar_.low)), (double)(0));
        if ((plus_dm > minus_dm)) {
            minus_dm = 0;
        } else {
            plus_dm = 0;
        }
        double smooth_tr = (is_first_tick_ ? _ta_rma_2.compute(tr_val) : _ta_rma_2.recompute(tr_val));
        double smooth_plus = (is_first_tick_ ? _ta_rma_3.compute(plus_dm) : _ta_rma_3.recompute(plus_dm));
        double smooth_minus = (is_first_tick_ ? _ta_rma_4.compute(minus_dm) : _ta_rma_4.recompute(minus_dm));
        double plus_di = (((smooth_tr > 0)) ? (((double)((100 * smooth_plus)) / (double)(smooth_tr))) : (0));
        double minus_di = (((smooth_tr > 0)) ? (((double)((100 * smooth_minus)) / (double)(smooth_tr))) : (0));
        double di_sum = (plus_di + minus_di);
        double dx = (((di_sum > 0)) ? (((double)((100 * std::abs((plus_di - minus_di)))) / (double)(di_sum))) : (0));
        double adx_val = (is_first_tick_ ? _ta_rma_5.compute(dx) : _ta_rma_5.recompute(dx));
        return std::make_tuple(adx_val, plus_di, minus_di);
    }

    double f_macd_hist_cs0(double src, int fast, int slow, int sig) {
        double macd_val = ((is_first_tick_ ? _ta_ema_21.compute(src) : _ta_ema_21.recompute(src)) - (is_first_tick_ ? _ta_ema_22.compute(src) : _ta_ema_22.recompute(src)));
        double macd_sig = (is_first_tick_ ? _ta_ema_23.compute(macd_val) : _ta_ema_23.recompute(macd_val));
        return (macd_val - macd_sig);
    }

    double f_macd_hist_cs1(double src, int fast, int slow, int sig) {
        double macd_val = ((is_first_tick_ ? _ta_ema_21_cs1.compute(src) : _ta_ema_21_cs1.recompute(src)) - (is_first_tick_ ? _ta_ema_22_cs1.compute(src) : _ta_ema_22_cs1.recompute(src)));
        double macd_sig = (is_first_tick_ ? _ta_ema_23_cs1.compute(macd_val) : _ta_ema_23_cs1.recompute(macd_val));
        return (macd_val - macd_sig);
    }

    double f_macd_hist_cs2(double src, int fast, int slow, int sig) {
        double macd_val = ((is_first_tick_ ? _ta_ema_21_cs2.compute(src) : _ta_ema_21_cs2.recompute(src)) - (is_first_tick_ ? _ta_ema_22_cs2.compute(src) : _ta_ema_22_cs2.recompute(src)));
        double macd_sig = (is_first_tick_ ? _ta_ema_23_cs2.compute(macd_val) : _ta_ema_23_cs2.recompute(macd_val));
        return (macd_val - macd_sig);
    }

    double f_signal_quality(int total, int max_total, bool bias, int momentum, int structure, bool trending, bool pressure, bool sweep, bool squeeze, bool mtf) {
        double quality = 0.0;
        quality += (((double)((double)(total)) / (double)((double)(max_total))) * 50);
        quality += ((bias) ? (15) : (0));
        quality += (((double)((double)(momentum)) / (double)(3.0)) * 10);
        quality += (((double)((double)(structure)) / (double)(2.0)) * 10);
        quality += ((trending) ? (5) : (0));
        quality += ((pressure) ? (5) : (0));
        quality += ((sweep) ? (2.5) : (0));
        quality += ((squeeze) ? (2.5) : (0));
        quality += ((mtf) ? (5) : (0));
        return std::min((double)(quality), (double)(100));
    }

    void on_bar(const Bar& bar) override {
        if (is_first_tick_) _s_high.push(current_bar_.high);
        else _s_high.update(current_bar_.high);
        if (is_first_tick_) _s_low.push(current_bar_.low);
        else _s_low.update(current_bar_.low);
        if (!_var_initialized) {
            _var_initialized = true;
        } else {
        }
        if (!_ta_initialized_) {
            _ta_rma_2 = ta::RMA(get_input_int("ADX Period", 14));
            _ta_rma_3 = ta::RMA(get_input_int("ADX Period", 14));
            _ta_rma_4 = ta::RMA(get_input_int("ADX Period", 14));
            _ta_rma_5 = ta::RMA(get_input_int("ADX Period", 14));
            _ta_atr_6 = ta::ATR(get_input_int("ATR Period", 14));
            _ta_sma_7 = ta::SMA(get_input_int("ATR Period", 14) * 3);
            _ta_ema_8 = ta::EMA(get_input_int("Fast MA", 21));
            _ta_ema_9 = ta::EMA(get_input_int("Slow MA", 55));
            _ta_ema_10 = ta::EMA(get_input_int("Trend MA", 200));
            _ta_rsi_11 = ta::RSI(get_input_int("RSI Period", 14));
            _ta_ema_12 = ta::EMA(get_input_int("MACD Fast", 12));
            _ta_ema_13 = ta::EMA(get_input_int("MACD Slow", 26));
            _ta_ema_14 = ta::EMA(get_input_int("MACD Signal", 9));
            _ta_ema_15 = ta::EMA(get_input_int("Pressure Period", 14));
            _ta_ema_16 = ta::EMA(get_input_int("Pressure Smoothing", 5));
            _ta_sma_17 = ta::SMA(get_input_int("BB Length", 20));
            _ta_stdev_18 = ta::StdDev(get_input_int("BB Length", 20));
            _ta_ema_19 = ta::EMA(get_input_int("KC Length", 20));
            _ta_atr_20 = ta::ATR(get_input_int("KC Length", 20));
            _ta_ema_21 = ta::EMA(get_input_int("MACD Fast", 12));
            _ta_ema_22 = ta::EMA(get_input_int("MACD Slow", 26));
            _ta_ema_23 = ta::EMA(get_input_int("MACD Signal", 9));
            _ta_ema_21_cs1 = ta::EMA(get_input_int("MACD Fast", 12));
            _ta_ema_22_cs1 = ta::EMA(get_input_int("MACD Slow", 26));
            _ta_ema_23_cs1 = ta::EMA(get_input_int("MACD Signal", 9));
            _ta_ema_21_cs2 = ta::EMA(get_input_int("MACD Fast", 12));
            _ta_ema_22_cs2 = ta::EMA(get_input_int("MACD Slow", 26));
            _ta_ema_23_cs2 = ta::EMA(get_input_int("MACD Signal", 9));
            _ta_pivothigh_24 = ta::PivotHigh(get_input_int("Swing Left Bars", 10), get_input_int("Swing Right Bars", 5));
            _ta_pivotlow_25 = ta::PivotLow(get_input_int("Swing Left Bars", 10), get_input_int("Swing Right Bars", 5));
            _ta_sma_26 = ta::SMA(get_input_int("Volume MA Period", 20));
            _sec0__ta_ema_21_v0 = ta::EMA(get_input_int("MACD Fast", 12));
            _sec0__ta_ema_21_v1 = ta::EMA(get_input_int("MACD Fast", 12));
            _sec0__ta_ema_21_v2 = ta::EMA(get_input_int("MACD Fast", 12));
            _sec0__ta_ema_22_v0 = ta::EMA(get_input_int("MACD Slow", 26));
            _sec0__ta_ema_22_v1 = ta::EMA(get_input_int("MACD Slow", 26));
            _sec0__ta_ema_22_v2 = ta::EMA(get_input_int("MACD Slow", 26));
            _sec0__ta_ema_23_v0 = ta::EMA(get_input_int("MACD Signal", 9));
            _sec0__ta_ema_23_v1 = ta::EMA(get_input_int("MACD Signal", 9));
            _sec1__ta_ema_21_v0 = ta::EMA(get_input_int("MACD Fast", 12));
            _sec1__ta_ema_21_v1 = ta::EMA(get_input_int("MACD Fast", 12));
            _sec1__ta_ema_21_v2 = ta::EMA(get_input_int("MACD Fast", 12));
            _sec1__ta_ema_22_v0 = ta::EMA(get_input_int("MACD Slow", 26));
            _sec1__ta_ema_22_v1 = ta::EMA(get_input_int("MACD Slow", 26));
            _sec1__ta_ema_22_v2 = ta::EMA(get_input_int("MACD Slow", 26));
            _sec1__ta_ema_23_v0 = ta::EMA(get_input_int("MACD Signal", 9));
            _sec1__ta_ema_23_v1 = ta::EMA(get_input_int("MACD Signal", 9));
            _sec2__ta_ema_21_v0 = ta::EMA(get_input_int("MACD Fast", 12));
            _sec2__ta_ema_21_v1 = ta::EMA(get_input_int("MACD Fast", 12));
            _sec2__ta_ema_21_v2 = ta::EMA(get_input_int("MACD Fast", 12));
            _sec2__ta_ema_22_v0 = ta::EMA(get_input_int("MACD Slow", 26));
            _sec2__ta_ema_22_v1 = ta::EMA(get_input_int("MACD Slow", 26));
            _sec2__ta_ema_22_v2 = ta::EMA(get_input_int("MACD Slow", 26));
            _sec2__ta_ema_23_v0 = ta::EMA(get_input_int("MACD Signal", 9));
            _sec2__ta_ema_23_v1 = ta::EMA(get_input_int("MACD Signal", 9));
            _ta_initialized_ = true;
        }
        g_regime = std::string("═══════════ Regime Detection ═══════════");
        i_adx_len = get_input_int("ADX Period", 14);
        i_adx_trend = get_input_double("Trend Threshold", 25);
        i_atr_len = get_input_int("ATR Period", 14);
        i_vol_exp = get_input_double("Volatility Expansion", 1.4);
        i_vol_con = get_input_double("Volatility Contraction", 0.6);
        i_avoid_volatile = get_input_bool("Avoid Volatile Regime", true);
        g_bias = std::string("═══════════ Directional Bias ═══════════");
        i_ma_fast = get_input_int("Fast MA", 21);
        i_ma_slow = get_input_int("Slow MA", 55);
        i_ma_trend = get_input_int("Trend MA", 200);
        i_bias_thresh = get_input_double("Bias Threshold", 30);
        g_momentum = std::string("═══════════ Momentum Filter ═══════════");
        i_rsi_len = get_input_int("RSI Period", 14);
        i_rsi_bull = get_input_double("RSI Bullish Level", 55);
        i_rsi_bear = get_input_double("RSI Bearish Level", 45);
        i_macd_fast = get_input_int("MACD Fast", 12);
        i_macd_slow = get_input_int("MACD Slow", 26);
        i_macd_sig = get_input_int("MACD Signal", 9);
        g_structure = std::string("═══════════ Structure ═══════════");
        i_swing_left = get_input_int("Swing Left Bars", 10);
        i_swing_right = get_input_int("Swing Right Bars", 5);
        i_require_struct = get_input_bool("Require Structure Alignment", true);
        g_risk = std::string("═══════════ Risk Management ═══════════");
        i_risk_pct = get_input_double("Risk Per Trade %", 1.0);
        i_atr_stop_mult = get_input_double("ATR Stop Multiplier", 2.0);
        i_rr_target1 = get_input_double("R:R Target 1", 1.5);
        i_rr_target2 = get_input_double("R:R Target 2", 2.5);
        i_trail_start = get_input_double("Trailing Start (R)", 1.0);
        i_trail_offset = get_input_double("Trailing Offset (R)", 0.5);
        i_channel_mult = get_input_double("Channel Multiple", 3.0);
        g_frequency = std::string("═══════════ Trade Frequency ═══════════");
        i_use_trade_limit = get_input_bool("Use Daily Trade Limit", true);
        i_max_trades = get_input_int("Max Daily Trades", 30);
        i_cooldown = get_input_int("Signal Cooldown Bars", 1);
        i_require_close = get_input_bool("Confirm On Bar Close", true);
        i_allow_flip = get_input_bool("Allow Flip / Re-Entry", true);
        g_filters = std::string("═══════════ Additional Filters ═══════════");
        i_use_vol_filter = get_input_bool("Use Volume Filter", true);
        i_vol_len = get_input_int("Volume MA Period", 20);
        i_vol_mult = get_input_double("Volume Multiplier", 1.0);
        i_min_qual = get_input_int("Minimum Qualification Score", 5);
        g_mtf = std::string("═══════════ MTF Bias ═══════════");
        i_use_mtf = get_input_bool("Use MTF Bias", true);
        i_mtf_filter = get_input_bool("Require MTF Alignment", false);
        i_mtf_tf1 = get_input_string("MTF 1H", std::string("60"));
        i_mtf_tf2 = get_input_string("MTF 4H", std::string("240"));
        i_mtf_tf3 = get_input_string("MTF 1D", std::string("1D"));
        i_mtf_majority = get_input_int("MTF Majority", 2);
        g_pressure = std::string("═══════════ Pressure/Flow ═══════════");
        i_use_pressure = get_input_bool("Use Pressure Gauge", true);
        i_pressure_len = get_input_int("Pressure Period", 14);
        i_pressure_smooth = get_input_int("Pressure Smoothing", 5);
        i_pressure_mom = get_input_int("Pressure Momentum", 10);
        i_pressure_high = get_input_double("Extreme Buy Level", 0.7);
        i_pressure_low = get_input_double("Extreme Sell Level", 0.3);
        i_pressure_thresh = get_input_double("Momentum Threshold", 0.05);
        g_squeeze = std::string("═══════════ Squeeze/Volatility ═══════════");
        i_use_squeeze = get_input_bool("Use Squeeze Release", true);
        i_bb_len = get_input_int("BB Length", 20);
        i_bb_mult = get_input_double("BB Mult", 2.0);
        i_kc_len = get_input_int("KC Length", 20);
        i_kc_mult = get_input_double("KC Mult", 1.5);
        g_exits = std::string("═══════════ Exit Scaling ═══════════");
        i_scale_out = get_input_bool("Scale Out at TP1", true);
        i_scale_pct = get_input_int("TP1 Size %", 50);
        g_adaptive = std::string("═══════════ Regime Adaptation ═══════════");
        i_adapt_size = get_input_bool("Adaptive Position Sizing", true);
        i_trend_mult = get_input_double("Trending Regime Size Mult", 1.2);
        i_range_mult = get_input_double("Ranging Regime Size Mult", 0.8);
        i_adapt_stops = get_input_bool("Adaptive Stop Distances", true);
        i_vol_stop_mult = get_input_double("Volatile Stop Multiplier", 1.3);
        g_visual = std::string("═══════════ Visual Settings ═══════════");
        i_bull_col = get_input_int("Bullish Color", pine_color::new_color(0, (int)(0)));
        i_bull_glow = get_input_int("Bull Glow", pine_color::new_color(0, (int)(0)));
        i_bear_col = get_input_int("Bearish Color", pine_color::new_color(0, (int)(0)));
        i_bear_glow = get_input_int("Bear Glow", pine_color::new_color(0, (int)(0)));
        i_neutral_col = get_input_int("Neutral Color", pine_color::new_color(0, (int)(0)));
        i_trend_bg = get_input_int("Trend Regime BG", pine_color::new_color(0, (int)(0)));
        i_range_bg = get_input_int("Range Regime BG", pine_color::new_color(0, (int)(0)));
        i_vol_bg = get_input_int("Volatile Regime BG", pine_color::new_color(0, (int)(0)));
        i_show_dash = get_input_bool("Show Dashboard", true);
        i_show_levels = get_input_bool("Show Trade Levels", true);
        i_show_quality = get_input_bool("Show Signal Quality", true);
        i_show_bg = get_input_bool("Show Background Shading", false);
        i_show_trend_band = get_input_bool("Show Trend Band", true);
        i_show_channel = get_input_bool("Show Dynamic Channel", true);
        i_show_signals = get_input_bool("Show Clean Signals", true);
        i_show_zones = get_input_bool("Show S/R Zones", true);
        i_band_style = get_input_int("Band Style", 0);
        i_signal_style = get_input_int("Signal Style", 0);
        g_concepts = std::string("═══════════ Institutional Concepts ═══════════");
        i_show_concepts = get_input_bool("Show On-Chart Concepts", true);
        i_show_squeeze_boxes = get_input_bool("Squeeze Zones (Boxes)", true);
        i_show_sweep_marks = get_input_bool("Liquidity Sweeps (Marks)", false);
        i_show_structure_levels = get_input_bool("Structure Levels", true);
        i_show_mtf_ribbon = get_input_bool("MTF Bias Ribbon", false);
        i_show_fvg = get_input_bool("Fair Value Gaps", true);
        i_show_order_blocks = get_input_bool("Order Blocks", true);
        i_show_session_boxes = get_input_bool("Session Boxes", true);
        auto [adx, plus_di, minus_di] = f_adx_calc_cs0(i_adx_len);
        atr_val = (is_first_tick_ ? _ta_atr_6.compute(current_bar_.high, current_bar_.low, current_bar_.close) : _ta_atr_6.recompute(current_bar_.high, current_bar_.low, current_bar_.close));
        atr_ma = (is_first_tick_ ? _ta_sma_7.compute(atr_val) : _ta_sma_7.recompute(atr_val));
        vol_ratio = (((atr_ma > 0)) ? (((double)(atr_val) / (double)(atr_ma))) : (1.0));
        regime = 0;
        if (((vol_ratio >= i_vol_exp) && (adx < i_adx_trend))) {
            regime = 3;
        } else
        if ((adx >= i_adx_trend)) {
            regime = 1;
        } else
        if ((vol_ratio <= i_vol_con)) {
            regime = 2;
        }
        regime_allows_trading = ((regime != 3) || !(i_avoid_volatile));
        trending_regime = (regime == 1);
        ranging_regime = (regime == 2);
        volatile_regime = (regime == 3);
        regime_strength = 0.0;
        if (trending_regime) {
            regime_strength = std::min((double)((((double)(adx) / (double)(50.0)) * 100)), (double)(100));
        } else
        if (ranging_regime) {
            regime_strength = std::min((double)((((double)((1 - vol_ratio)) / (double)((1 - i_vol_con))) * 100)), (double)(100));
        } else
        if (volatile_regime) {
            regime_strength = std::min((double)((((double)(vol_ratio) / (double)(i_vol_exp)) * 100)), (double)(100));
        } else {
            regime_strength = 50.0;
        }
        regime_confidence = ((double)(regime_strength) / (double)(100.0));
        ma_fast = (is_first_tick_ ? _ta_ema_8.compute(current_bar_.close) : _ta_ema_8.recompute(current_bar_.close));
        ma_slow = (is_first_tick_ ? _ta_ema_9.compute(current_bar_.close) : _ta_ema_9.recompute(current_bar_.close));
        ma_trend = (is_first_tick_ ? _ta_ema_10.compute(current_bar_.close) : _ta_ema_10.recompute(current_bar_.close));
        ma_spread = (((double)((ma_fast - ma_slow)) / (double)(ma_slow)) * 100);
        ma_bullish = ((ma_fast > ma_slow) && (ma_slow > ma_trend));
        ma_bearish = ((ma_fast < ma_slow) && (ma_slow < ma_trend));
        price_above_structure = ((current_bar_.close > ma_fast) && (current_bar_.close > ma_slow));
        price_below_structure = ((current_bar_.close < ma_fast) && (current_bar_.close < ma_slow));
        bias_score = 0.0;
        if (ma_bullish) {
            bias_score += 30;
        }
        if (price_above_structure) {
            bias_score += 20;
        }
        if ((current_bar_.close > ma_trend)) {
            bias_score += 20;
        }
        if ((plus_di > minus_di)) {
            bias_score += 30;
        }
        bear_bias_score = 0.0;
        if (ma_bearish) {
            bear_bias_score += 30;
        }
        if (price_below_structure) {
            bear_bias_score += 20;
        }
        if ((current_bar_.close < ma_trend)) {
            bear_bias_score += 20;
        }
        if ((minus_di > plus_di)) {
            bear_bias_score += 30;
        }
        bullish_bias = (bias_score >= i_bias_thresh);
        bearish_bias = (bear_bias_score >= i_bias_thresh);
        rsi.push((is_first_tick_ ? _ta_rsi_11.compute(current_bar_.close) : _ta_rsi_11.recompute(current_bar_.close)));
        rsi_bullish = (rsi[0] > i_rsi_bull);
        rsi_bearish = (rsi[0] < i_rsi_bear);
        rsi_momentum_up = (rsi[0] > rsi[3]);
        rsi_momentum_down = (rsi[0] < rsi[3]);
        macd_line = ((is_first_tick_ ? _ta_ema_12.compute(current_bar_.close) : _ta_ema_12.recompute(current_bar_.close)) - (is_first_tick_ ? _ta_ema_13.compute(current_bar_.close) : _ta_ema_13.recompute(current_bar_.close)));
        macd_signal = (is_first_tick_ ? _ta_ema_14.compute(macd_line) : _ta_ema_14.recompute(macd_line));
        macd_hist.push((macd_line - macd_signal));
        macd_bullish = ((macd_hist[0] > 0) && (macd_hist[0] > macd_hist[1]));
        macd_bearish = ((macd_hist[0] < 0) && (macd_hist[0] < macd_hist[1]));
        momentum_bull_score = 0;
        if (rsi_bullish) {
            momentum_bull_score += 1;
        }
        if (rsi_momentum_up) {
            momentum_bull_score += 1;
        }
        if (macd_bullish) {
            momentum_bull_score += 1;
        }
        momentum_bear_score = 0;
        if (rsi_bearish) {
            momentum_bear_score += 1;
        }
        if (rsi_momentum_down) {
            momentum_bear_score += 1;
        }
        if (macd_bearish) {
            momentum_bear_score += 1;
        }
        range_val = (current_bar_.high - current_bar_.low);
        raw_buy = (((range_val > 0)) ? (((double)((current_bar_.close - current_bar_.low)) / (double)(range_val))) : (0.5));
        raw_sell = (((range_val > 0)) ? (((double)((current_bar_.high - current_bar_.close)) / (double)(range_val))) : (0.5));
        pressure_ratio = (is_first_tick_ ? _ta_ema_15.compute(raw_buy) : _ta_ema_15.recompute(raw_buy));
        pressure_smooth.push((is_first_tick_ ? _ta_ema_16.compute(pressure_ratio) : _ta_ema_16.recompute(pressure_ratio)));
        pressure_momentum = (pressure_smooth[0] - pressure_smooth[i_pressure_mom]);
        pressure_state = 0;
        if ((pressure_smooth[0] >= i_pressure_high)) {
            pressure_state = 2;
        } else
        if ((pressure_smooth[0] > (0.5 + i_pressure_thresh))) {
            pressure_state = 1;
        } else
        if ((pressure_smooth[0] <= i_pressure_low)) {
            pressure_state = (-2);
        } else
        if ((pressure_smooth[0] < (0.5 - i_pressure_thresh))) {
            pressure_state = (-1);
        }
        pressure_bull = (i_use_pressure && ((pressure_state >= 1) || (pressure_momentum > i_pressure_thresh)));
        pressure_bear = (i_use_pressure && ((pressure_state <= (-1)) || (pressure_momentum < (-i_pressure_thresh))));
        bb_basis = (is_first_tick_ ? _ta_sma_17.compute(current_bar_.close) : _ta_sma_17.recompute(current_bar_.close));
        bb_dev = ((is_first_tick_ ? _ta_stdev_18.compute(current_bar_.close) : _ta_stdev_18.recompute(current_bar_.close)) * i_bb_mult);
        bb_upper = (bb_basis + bb_dev);
        bb_lower = (bb_basis - bb_dev);
        kc_basis = (is_first_tick_ ? _ta_ema_19.compute(current_bar_.close) : _ta_ema_19.recompute(current_bar_.close));
        kc_range = ((is_first_tick_ ? _ta_atr_20.compute(current_bar_.high, current_bar_.low, current_bar_.close) : _ta_atr_20.recompute(current_bar_.high, current_bar_.low, current_bar_.close)) * i_kc_mult);
        kc_upper = (kc_basis + kc_range);
        kc_lower = (kc_basis - kc_range);
        squeeze_on.push(((bb_lower > kc_lower) && (bb_upper < kc_upper)));
        squeeze_release = ((i_use_squeeze && squeeze_on[1]) && !(squeeze_on[0]));
        mtf_hist_1 = _req_sec_0;
        mtf_hist_2 = _req_sec_1;
        mtf_hist_3 = _req_sec_2;
        mtf_bull_count = (((((mtf_hist_1 > 0)) ? (1) : (0)) + (((mtf_hist_2 > 0)) ? (1) : (0))) + (((mtf_hist_3 > 0)) ? (1) : (0)));
        mtf_bear_count = (((((mtf_hist_1 < 0)) ? (1) : (0)) + (((mtf_hist_2 < 0)) ? (1) : (0))) + (((mtf_hist_3 < 0)) ? (1) : (0)));
        mtf_bias_long = (i_use_mtf && (mtf_bull_count >= i_mtf_majority));
        mtf_bias_short = (i_use_mtf && (mtf_bear_count >= i_mtf_majority));
        mtf_allows_long = (!(i_mtf_filter) || mtf_bias_long);
        mtf_allows_short = (!(i_mtf_filter) || mtf_bias_short);
        swing_high = (is_first_tick_ ? _ta_pivothigh_24.compute(current_bar_.high) : _ta_pivothigh_24.recompute(current_bar_.high));
        swing_low = (is_first_tick_ ? _ta_pivotlow_25.compute(current_bar_.low) : _ta_pivotlow_25.recompute(current_bar_.low));
        if (!(is_na(swing_high))) {
            last_swing_high = swing_high;
        }
        if (!(is_na(swing_low))) {
            last_swing_low = swing_low;
        }
        sweep_high = ((!(is_na(last_swing_high)) && (current_bar_.high > last_swing_high)) && (current_bar_.close < last_swing_high));
        sweep_low = ((!(is_na(last_swing_low)) && (current_bar_.low < last_swing_low)) && (current_bar_.close > last_swing_low));
        above_swing_low = (current_bar_.close > (is_na(last_swing_low) ? current_bar_.low : last_swing_low));
        below_swing_high = (current_bar_.close < (is_na(last_swing_high) ? current_bar_.high : last_swing_high));
        struct_bull_score = 0;
        if (above_swing_low) {
            struct_bull_score += 1;
        }
        if ((current_bar_.close > ma_fast)) {
            struct_bull_score += 1;
        }
        struct_bear_score = 0;
        if (below_swing_high) {
            struct_bear_score += 1;
        }
        if ((current_bar_.close < ma_fast)) {
            struct_bear_score += 1;
        }
        struct_allows_long = (!(i_require_struct) || (struct_bull_score >= 1));
        struct_allows_short = (!(i_require_struct) || (struct_bear_score >= 1));
        vol_ma = (is_first_tick_ ? _ta_sma_26.compute(current_bar_.volume) : _ta_sma_26.recompute(current_bar_.volume));
        volume_confirm = (!(i_use_vol_filter) || (current_bar_.volume >= (vol_ma * i_vol_mult)));
        max_score = 14;
        bull_total = (((((((((bullish_bias) ? (3) : (0)) + momentum_bull_score) + struct_bull_score) + ((trending_regime) ? (2) : (0))) + ((pressure_bull) ? (1) : (0))) + ((sweep_low) ? (1) : (0))) + ((squeeze_release) ? (1) : (0))) + ((mtf_bias_long) ? (1) : (0)));
        bear_total = (((((((((bearish_bias) ? (3) : (0)) + momentum_bear_score) + struct_bear_score) + ((trending_regime) ? (2) : (0))) + ((pressure_bear) ? (1) : (0))) + ((sweep_high) ? (1) : (0))) + ((squeeze_release) ? (1) : (0))) + ((mtf_bias_short) ? (1) : (0)));
        bull_qualified = (bull_total >= i_min_qual);
        bear_qualified = (bear_total >= i_min_qual);
        bull_quality = f_signal_quality(bull_total, 14, bullish_bias, momentum_bull_score, struct_bull_score, trending_regime, pressure_bull, sweep_low, squeeze_release, mtf_bias_long);
        bear_quality = f_signal_quality(bear_total, 14, bearish_bias, momentum_bear_score, struct_bear_score, trending_regime, pressure_bear, sweep_high, squeeze_release, mtf_bias_short);
        bull_grade = (((bull_quality >= 80)) ? (std::string("A")) : ((((bull_quality >= 60)) ? (std::string("B")) : ((((bull_quality >= 40)) ? (std::string("C")) : (std::string("D")))))));
        bear_grade = (((bear_quality >= 80)) ? (std::string("A")) : ((((bear_quality >= 60)) ? (std::string("B")) : ((((bear_quality >= 40)) ? (std::string("C")) : (std::string("D")))))));
        bull_premium = (bull_quality >= 75);
        bear_premium = (bear_quality >= 75);
        momentum_trigger_long = (rsi_momentum_up && (rsi[0] > 50));
        momentum_trigger_short = (rsi_momentum_down && (rsi[0] < 50));
        squeeze_trigger_long = (squeeze_release && pressure_bull);
        squeeze_trigger_short = (squeeze_release && pressure_bear);
        sweep_trigger_long = sweep_low;
        sweep_trigger_short = sweep_high;
        cross_ma_long = (is_first_tick_ ? _ta_crossover_27.compute(current_bar_.close, ma_fast) : _ta_crossover_27.recompute(current_bar_.close, ma_fast));
        cross_macd_long = (is_first_tick_ ? _ta_crossover_28.compute(macd_hist[0], 0) : _ta_crossover_28.recompute(macd_hist[0], 0));
        cross_ma_short = (is_first_tick_ ? _ta_crossunder_29.compute(current_bar_.close, ma_fast) : _ta_crossunder_29.recompute(current_bar_.close, ma_fast));
        cross_macd_short = (is_first_tick_ ? _ta_crossunder_30.compute(macd_hist[0], 0) : _ta_crossunder_30.recompute(macd_hist[0], 0));
        bull_trigger = ((((cross_ma_long || cross_macd_long) || momentum_trigger_long) || squeeze_trigger_long) || sweep_trigger_long);
        bear_trigger = ((((cross_ma_short || cross_macd_short) || momentum_trigger_short) || squeeze_trigger_short) || sweep_trigger_short);
        bars_since_trade = (bars_since_trade + 1);
        new_day = ((is_first_tick_ ? _ta_change_31.compute(pine_time(current_bar_.timestamp, std::string("D"), std::string(""), std::string(""), script_tf_)) : _ta_change_31.recompute(pine_time(current_bar_.timestamp, std::string("D"), std::string(""), std::string(""), script_tf_))) != 0);
        if (new_day) {
            daily_trades = 0;
        }
        within_trade_limit = (!(i_use_trade_limit) || (daily_trades < i_max_trades));
        entry_confirm = (!(i_require_close) || is_last_tick_);
        long_entry = ((((((((bull_qualified && bull_trigger) && regime_allows_trading) && struct_allows_long) && volume_confirm) && (bars_since_trade >= i_cooldown)) && within_trade_limit) && mtf_allows_long) && entry_confirm);
        short_entry = ((((((((bear_qualified && bear_trigger) && regime_allows_trading) && struct_allows_short) && volume_confirm) && (bars_since_trade >= i_cooldown)) && within_trade_limit) && mtf_allows_short) && entry_confirm);
        adaptive_stop_mult = i_atr_stop_mult;
        if (i_adapt_stops) {
            if (volatile_regime) {
                adaptive_stop_mult = (i_atr_stop_mult * i_vol_stop_mult);
            } else
            if (ranging_regime) {
                adaptive_stop_mult = (i_atr_stop_mult * 0.85);
            } else
            if (trending_regime) {
                adaptive_stop_mult = (i_atr_stop_mult * 1.1);
            }
        }
        long_stop_struct = (is_na(last_swing_low) ? (current_bar_.close - (atr_val * adaptive_stop_mult)) : last_swing_low);
        long_stop_atr = (current_bar_.close - (atr_val * adaptive_stop_mult));
        long_stop = std::min((double)(long_stop_struct), (double)(long_stop_atr));
        short_stop_struct = (is_na(last_swing_high) ? (current_bar_.close + (atr_val * adaptive_stop_mult)) : last_swing_high);
        short_stop_atr = (current_bar_.close + (atr_val * adaptive_stop_mult));
        short_stop = std::max((double)(short_stop_struct), (double)(short_stop_atr));
        long_risk = (current_bar_.close - long_stop);
        short_risk = (short_stop - current_bar_.close);
        adaptive_target_mult = 1.0;
        if ((vol_ratio >= i_vol_exp)) {
            adaptive_target_mult = 1.3;
        } else
        if ((vol_ratio <= i_vol_con)) {
            adaptive_target_mult = 0.85;
        }
        long_tp1 = (current_bar_.close + ((long_risk * i_rr_target1) * adaptive_target_mult));
        long_tp2 = (current_bar_.close + ((long_risk * i_rr_target2) * adaptive_target_mult));
        short_tp1 = (current_bar_.close - ((short_risk * i_rr_target1) * adaptive_target_mult));
        short_tp2 = (current_bar_.close - ((short_risk * i_rr_target2) * adaptive_target_mult));
        long_trail_start = (current_bar_.close + (long_risk * i_trail_start));
        short_trail_start = (current_bar_.close - (short_risk * i_trail_start));
        long_trail_offset = (long_risk * i_trail_offset);
        short_trail_offset = (short_risk * i_trail_offset);
        regime_size_mult = 1.0;
        if (i_adapt_size) {
            if (trending_regime) {
                regime_size_mult = (i_trend_mult * regime_confidence);
            } else
            if (ranging_regime) {
                regime_size_mult = i_range_mult;
            } else
            if (volatile_regime) {
                regime_size_mult = 0.5;
            } else {
                regime_size_mult = 1.0;
            }
        }
        quality_mult = 1.0;
        if ((bull_premium || bear_premium)) {
            quality_mult = 1.15;
        }
        account_risk = ((((current_equity() + open_profit(current_bar_.close)) * ((double)(i_risk_pct) / (double)(100))) * regime_size_mult) * quality_mult);
        long_position_size = (((long_risk > 0)) ? (((double)(account_risk) / (double)(long_risk))) : (0));
        short_position_size = (((short_risk > 0)) ? (((double)(account_risk) / (double)(short_risk))) : (0));
        if ((long_entry && ((signed_position_size() == 0) || (i_allow_flip && (signed_position_size() < 0))))) {
            if ((signed_position_size() < 0)) {
                strategy_close(std::string("Short"), "", na<double>(), na<double>(), false);
            }
            strategy_entry(std::string("Long"), true, na<double>(), na<double>(), long_position_size, "", "", 0, -1);
            entry_price = current_bar_.close;
            initial_stop = long_stop;
            trailing_stop = long_stop;
            is_long = true;
            is_short = false;
            bars_since_trade = 0;
            daily_trades = (daily_trades + 1);
        }
        if ((short_entry && ((signed_position_size() == 0) || (i_allow_flip && (signed_position_size() > 0))))) {
            if ((signed_position_size() > 0)) {
                strategy_close(std::string("Long"), "", na<double>(), na<double>(), false);
            }
            strategy_entry(std::string("Short"), false, na<double>(), na<double>(), short_position_size, "", "", 0, -1);
            entry_price = current_bar_.close;
            initial_stop = short_stop;
            trailing_stop = short_stop;
            is_long = false;
            is_short = true;
            bars_since_trade = 0;
            daily_trades = (daily_trades + 1);
        }
        if (((signed_position_size() > 0) && is_long)) {
            profit = (current_bar_.close - entry_price);
            if ((profit >= (long_risk * i_trail_start))) {
                new_trail = (current_bar_.close - long_trail_offset);
                if ((new_trail > trailing_stop)) {
                    trailing_stop = new_trail;
                }
            }
        }
        if (((signed_position_size() < 0) && is_short)) {
            profit = (entry_price - current_bar_.close);
            if ((profit >= (short_risk * i_trail_start))) {
                new_trail = (current_bar_.close + short_trail_offset);
                if ((new_trail < trailing_stop)) {
                    trailing_stop = new_trail;
                }
            }
        }
        if ((signed_position_size() > 0)) {
            if (i_scale_out) {
                strategy_exit(std::string("Long TP1"), std::string("Long"), long_tp1, na<double>(), na<double>(), na<double>(), na<double>(), i_scale_pct, "", na<double>(), "");
            }
            strategy_exit(std::string("Long Exit"), std::string("Long"), long_tp2, trailing_stop, na<double>(), na<double>(), na<double>(), 100.0, "", na<double>(), "");
        }
        if ((signed_position_size() < 0)) {
            if (i_scale_out) {
                strategy_exit(std::string("Short TP1"), std::string("Short"), short_tp1, na<double>(), na<double>(), na<double>(), na<double>(), i_scale_pct, "", na<double>(), "");
            }
            strategy_exit(std::string("Short Exit"), std::string("Short"), short_tp2, trailing_stop, na<double>(), na<double>(), na<double>(), 100.0, "", na<double>(), "");
        }
        if ((signed_position_size() == 0)) {
            entry_price = na<double>();
            initial_stop = na<double>();
            trailing_stop = na<double>();
            is_long = false;
            is_short = false;
        }
        trend_bg_color = ((ma_bullish) ? (pine_color::new_color(i_bull_col, (int)(95))) : (((ma_bearish) ? (pine_color::new_color(i_bear_col, (int)(95))) : (pine_color::new_color(i_neutral_col, (int)(98))))));
        ma_color = ((ma_bullish) ? (i_bull_col) : (((ma_bearish) ? (i_bear_col) : (i_neutral_col))));
        if ((i_show_dash && barstate_islast_)) {
            auto __switch_val_0 = regime;
            if (__switch_val_0 == 1) {
                regime_text = std::string("TRENDING");
            }
            else if (__switch_val_0 == 2) {
                regime_text = std::string("RANGING");
            }
            else if (__switch_val_0 == 3) {
                regime_text = std::string("VOLATILE");
            }
            else {
                regime_text = std::string("NEUTRAL");
            }
            regime_col = (((regime == 1)) ? (i_bull_col) : ((((regime == 3)) ? (i_bear_col) : (i_neutral_col))));
            pos_text = (((signed_position_size() > 0)) ? (std::string("LONG")) : ((((signed_position_size() < 0)) ? (std::string("SHORT")) : (std::string("FLAT")))));
            pos_col = (((signed_position_size() > 0)) ? (i_bull_col) : ((((signed_position_size() < 0)) ? (i_bear_col) : (i_neutral_col))));
            if ((signed_position_size() != 0)) {
                unrealized_pnl = (((signed_position_size() > 0)) ? ((current_bar_.close - entry_price)) : ((entry_price - current_bar_.close)));
                unrealized_pct = (((entry_price > 0)) ? ((((double)(unrealized_pnl) / (double)(entry_price)) * 100)) : (0));
            }
            quality_text = (((bull_quality > bear_quality)) ? ((((bull_grade + std::string(" (")) + pine_str_tostring(bull_quality, std::string("#"), syminfo_mintick_)) + std::string("%)"))) : ((((bear_grade + std::string(" (")) + pine_str_tostring(bear_quality, std::string("#"), syminfo_mintick_)) + std::string("%)"))));
            quality_col = (((bull_quality > bear_quality)) ? (((bull_premium) ? (pine_color::new_color(0, (int)(0))) : (i_bull_col))) : (((bear_premium) ? (pine_color::new_color(0, (int)(0))) : (i_bear_col))));
        }
    }

    void _eval_security_0(const Bar& bar, bool is_complete) {
        double _sec0_f_macd_hist_1_macd_val = ((is_complete ? _sec0__ta_ema_21_v2.compute(bar.close) : _sec0__ta_ema_21_v2.recompute(bar.close)) - (is_complete ? _sec0__ta_ema_22_v2.compute(bar.close) : _sec0__ta_ema_22_v2.recompute(bar.close)));
        double _sec0_f_macd_hist_2_macd_sig = (is_complete ? _sec0__ta_ema_23_v1.compute(_sec0_f_macd_hist_1_macd_val) : _sec0__ta_ema_23_v1.recompute(_sec0_f_macd_hist_1_macd_val));
        _req_sec_0 = (_sec0_f_macd_hist_1_macd_val - _sec0_f_macd_hist_2_macd_sig);
    }

    void _eval_security_1(const Bar& bar, bool is_complete) {
        double _sec1_f_macd_hist_3_macd_val = ((is_complete ? _sec1__ta_ema_21_v2.compute(bar.close) : _sec1__ta_ema_21_v2.recompute(bar.close)) - (is_complete ? _sec1__ta_ema_22_v2.compute(bar.close) : _sec1__ta_ema_22_v2.recompute(bar.close)));
        double _sec1_f_macd_hist_4_macd_sig = (is_complete ? _sec1__ta_ema_23_v1.compute(_sec1_f_macd_hist_3_macd_val) : _sec1__ta_ema_23_v1.recompute(_sec1_f_macd_hist_3_macd_val));
        _req_sec_1 = (_sec1_f_macd_hist_3_macd_val - _sec1_f_macd_hist_4_macd_sig);
    }

    void _eval_security_2(const Bar& bar, bool is_complete) {
        double _sec2_f_macd_hist_5_macd_val = ((is_complete ? _sec2__ta_ema_21_v2.compute(bar.close) : _sec2__ta_ema_21_v2.recompute(bar.close)) - (is_complete ? _sec2__ta_ema_22_v2.compute(bar.close) : _sec2__ta_ema_22_v2.recompute(bar.close)));
        double _sec2_f_macd_hist_6_macd_sig = (is_complete ? _sec2__ta_ema_23_v1.compute(_sec2_f_macd_hist_5_macd_val) : _sec2__ta_ema_23_v1.recompute(_sec2_f_macd_hist_5_macd_val));
        _req_sec_2 = (_sec2_f_macd_hist_5_macd_val - _sec2_f_macd_hist_6_macd_sig);
    }

    void evaluate_security(int sec_id, const Bar& bar, bool is_complete) override {
        if (!_ta_initialized_) {
            _ta_rma_2 = ta::RMA(get_input_int("ADX Period", 14));
            _ta_rma_3 = ta::RMA(get_input_int("ADX Period", 14));
            _ta_rma_4 = ta::RMA(get_input_int("ADX Period", 14));
            _ta_rma_5 = ta::RMA(get_input_int("ADX Period", 14));
            _ta_atr_6 = ta::ATR(get_input_int("ATR Period", 14));
            _ta_sma_7 = ta::SMA(get_input_int("ATR Period", 14) * 3);
            _ta_ema_8 = ta::EMA(get_input_int("Fast MA", 21));
            _ta_ema_9 = ta::EMA(get_input_int("Slow MA", 55));
            _ta_ema_10 = ta::EMA(get_input_int("Trend MA", 200));
            _ta_rsi_11 = ta::RSI(get_input_int("RSI Period", 14));
            _ta_ema_12 = ta::EMA(get_input_int("MACD Fast", 12));
            _ta_ema_13 = ta::EMA(get_input_int("MACD Slow", 26));
            _ta_ema_14 = ta::EMA(get_input_int("MACD Signal", 9));
            _ta_ema_15 = ta::EMA(get_input_int("Pressure Period", 14));
            _ta_ema_16 = ta::EMA(get_input_int("Pressure Smoothing", 5));
            _ta_sma_17 = ta::SMA(get_input_int("BB Length", 20));
            _ta_stdev_18 = ta::StdDev(get_input_int("BB Length", 20));
            _ta_ema_19 = ta::EMA(get_input_int("KC Length", 20));
            _ta_atr_20 = ta::ATR(get_input_int("KC Length", 20));
            _ta_ema_21 = ta::EMA(get_input_int("MACD Fast", 12));
            _ta_ema_22 = ta::EMA(get_input_int("MACD Slow", 26));
            _ta_ema_23 = ta::EMA(get_input_int("MACD Signal", 9));
            _ta_ema_21_cs1 = ta::EMA(get_input_int("MACD Fast", 12));
            _ta_ema_22_cs1 = ta::EMA(get_input_int("MACD Slow", 26));
            _ta_ema_23_cs1 = ta::EMA(get_input_int("MACD Signal", 9));
            _ta_ema_21_cs2 = ta::EMA(get_input_int("MACD Fast", 12));
            _ta_ema_22_cs2 = ta::EMA(get_input_int("MACD Slow", 26));
            _ta_ema_23_cs2 = ta::EMA(get_input_int("MACD Signal", 9));
            _ta_pivothigh_24 = ta::PivotHigh(get_input_int("Swing Left Bars", 10), get_input_int("Swing Right Bars", 5));
            _ta_pivotlow_25 = ta::PivotLow(get_input_int("Swing Left Bars", 10), get_input_int("Swing Right Bars", 5));
            _ta_sma_26 = ta::SMA(get_input_int("Volume MA Period", 20));
            _sec0__ta_ema_21_v0 = ta::EMA(get_input_int("MACD Fast", 12));
            _sec0__ta_ema_21_v1 = ta::EMA(get_input_int("MACD Fast", 12));
            _sec0__ta_ema_21_v2 = ta::EMA(get_input_int("MACD Fast", 12));
            _sec0__ta_ema_22_v0 = ta::EMA(get_input_int("MACD Slow", 26));
            _sec0__ta_ema_22_v1 = ta::EMA(get_input_int("MACD Slow", 26));
            _sec0__ta_ema_22_v2 = ta::EMA(get_input_int("MACD Slow", 26));
            _sec0__ta_ema_23_v0 = ta::EMA(get_input_int("MACD Signal", 9));
            _sec0__ta_ema_23_v1 = ta::EMA(get_input_int("MACD Signal", 9));
            _sec1__ta_ema_21_v0 = ta::EMA(get_input_int("MACD Fast", 12));
            _sec1__ta_ema_21_v1 = ta::EMA(get_input_int("MACD Fast", 12));
            _sec1__ta_ema_21_v2 = ta::EMA(get_input_int("MACD Fast", 12));
            _sec1__ta_ema_22_v0 = ta::EMA(get_input_int("MACD Slow", 26));
            _sec1__ta_ema_22_v1 = ta::EMA(get_input_int("MACD Slow", 26));
            _sec1__ta_ema_22_v2 = ta::EMA(get_input_int("MACD Slow", 26));
            _sec1__ta_ema_23_v0 = ta::EMA(get_input_int("MACD Signal", 9));
            _sec1__ta_ema_23_v1 = ta::EMA(get_input_int("MACD Signal", 9));
            _sec2__ta_ema_21_v0 = ta::EMA(get_input_int("MACD Fast", 12));
            _sec2__ta_ema_21_v1 = ta::EMA(get_input_int("MACD Fast", 12));
            _sec2__ta_ema_21_v2 = ta::EMA(get_input_int("MACD Fast", 12));
            _sec2__ta_ema_22_v0 = ta::EMA(get_input_int("MACD Slow", 26));
            _sec2__ta_ema_22_v1 = ta::EMA(get_input_int("MACD Slow", 26));
            _sec2__ta_ema_22_v2 = ta::EMA(get_input_int("MACD Slow", 26));
            _sec2__ta_ema_23_v0 = ta::EMA(get_input_int("MACD Signal", 9));
            _sec2__ta_ema_23_v1 = ta::EMA(get_input_int("MACD Signal", 9));
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
