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
    math::Sum _ta_sum_1;
    ta::SMA _ta_sma_2;
    ta::SMA _ta_sma_3;
    ta::EMA _ta_ema_4;
    ta::EMA _ta_ema_5;
    ta::EMA _ta_ema_6;
    ta::EMA _ta_ema_7;
    ta::EMA _ta_ema_8;
    ta::EMA _ta_ema_9;
    math::Sum _ta_sum_10;
    Series<double> _s_close;
    Series<double> kama;
    double original_qty;
    double peak_sigma;
    double current_pct;
    int bars_since_adj;
    std::string last_adapt_action;
    double entry_close;
    int pos_age;
    double dash;
    Series<double> er;
    Series<bool> in_long;
    Series<bool> in_short;
    Series<bool> is_gray;
    Series<double> kama_slope;
    Series<bool> slope_accel;
    std::string input_direction = std::string("");
    std::string input_trade_mode = std::string("");
    bool input_use_dev_gate = false;
    double input_max_dev = 0.0;
    double input_trim_start = 0.0;
    double input_trim_step = 0.0;
    double input_min_hold = 0.0;
    double input_reload_buf = 0.0;
    double input_trend_shift = 0.0;
    int input_cooldown = 0;
    int input_er_len = 0;
    int input_fast_period = 0;
    int input_slow_period = 0;
    int input_dev_len = 0;
    double input_band_mult = 0.0;
    int input_col_bull = 0;
    int input_col_bear = 0;
    int input_col_neutral = 0;
    int input_col_band = 0;
    int input_kama_width = 0;
    bool input_show_dash = false;
    std::string input_dash_pos = std::string("");
    std::string input_dash_size = std::string("");
    bool input_show_bands = false;
    bool input_show_sigs = false;
    int input_th_accent = 0;
    int input_th_label = 0;
    int input_th_hdr_bg = 0;
    int input_th_bg1 = 0;
    int input_th_bg2 = 0;
    int input_th_border = 0;
    int ER_LEN = 0;
    int FAST_PERIOD = 0;
    int SLOW_PERIOD = 0;
    int DEV_LEN = 0;
    double BAND_MULT = 0.0;
    double net_move = 0.0;
    double total_path = 0.0;
    double fast_sc = 0.0;
    double slow_sc = 0.0;
    double sc = 0.0;
    double kama_dev = 0.0;
    double band_upper = 0.0;
    double band_lower = 0.0;
    double band_width = 0.0;
    double band_pos = 0.0;
    bool in_lower_z = false;
    bool in_upper_z = false;
    bool in_mid_z = false;
    double bw_pct = 0.0;
    double bw_avg = 0.0;
    double bw_ratio = 0.0;
    double bwr_long_avg = 0.0;
    double bwr_dev = 0.0;
    bool volatile_mkt = false;
    bool tight_coil = false;
    double dist_from_fv_norm = 0.0;
    double abs_dist = 0.0;
    double sc_threshold = 0.0;
    bool is_choppy = false;
    double kama_slope_raw = 0.0;
    bool is_green = false;
    bool is_red = false;
    double er_avg = 0.0;
    double er_change_rate = 0.0;
    double er_noise_floor = 0.0;
    double er_strong_thresh = 0.0;
    bool er_rising = false;
    double er_slope = 0.0;
    bool er_building = false;
    double coil_ratio = 0.0;
    bool mostly_coiling = false;
    bool strong_trend = false;
    double body_size = 0.0;
    double full_range = 0.0;
    double body_ratio = 0.0;
    double upper_wick = 0.0;
    double lower_wick = 0.0;
    double wick_ratio_upper = 0.0;
    double wick_ratio_lower = 0.0;
    bool rejection_top = false;
    bool rejection_bottom = false;
    bool in_trade = false;
    bool flat = false;
    bool long_goes_gray = false;
    bool short_goes_gray = false;
    bool long_goes_red = false;
    bool short_goes_green = false;
    bool new_position = false;
    bool dev_ok = false;
    double _shift = 0.0;
    double _pa_shift = 0.0;
    double _eff_shift = 0.0;
    double _gray_penalty = 0.0;
    double _trim_thresh = 0.0;
    double _step_frac = 0.0;
    double _min_frac = 0.0;
    bool _cooldown_ok = false;
    double _trim_depth = 0.0;
    double _trim_frac = 0.0;
    double _target_pct = 0.0;
    double _reload_thresh = 0.0;
    bool _color_ok = false;
    bool _reload_zone = false;
    double _pct_diff = 0.0;
    bool _sig_change = false;
    double _target_qty = 0.0;
    double _current_qty = 0.0;
    double _delta = 0.0;
    bool color_long = false;
    bool color_short = false;
    bool any_long_entry = false;
    bool any_short_entry = false;
    double sigma_moved = 0.0;
    int state_color = 0;
    double p_kama = 0.0;
    double band_color = 0.0;
    double p_up = 0.0;
    double p_dn = 0.0;
    double _dash_pos = 0.0;
    double _sz = 0.0;
    double _hdr_sz = 0.0;
    double _bg0 = 0.0;
    double _bg1 = 0.0;
    double _bg2 = 0.0;
    int _lbl = 0;
    int _acc = 0;
    bool _live = false;
    double _pnl = 0.0;
    double _avg = 0.0;
    double _pnl_pct = 0.0;
    std::string _state_t = std::string("");
    int _state_c = 0;
    bool _h_in = false;
    std::string _sig_t = std::string("");
    int _sig_c = 0;
    double _pos_val = 0.0;
    double _pos_pct = 0.0;
    std::string _pos_t = std::string("");
    int _pos_c = 0;
    std::string _pnl_pfx = std::string("");
    std::string _pnl_t = std::string("");
    int _pnl_c = 0;
    int _er_c = 0;
    std::string _er_l = std::string("");
    std::string _cr_t = std::string("");
    std::string _zone_t = std::string("");
    std::string _vol_t = std::string("");
    std::string _bnd_t = std::string("");
    int _bnd_c = 0;
    std::string _dev_t = std::string("");
    double _dev_thresh = 0.0;
    int _dev_c = 0;
    std::string _adp_t = std::string("");
    int _adp_c = 0;
    std::string _act_t = std::string("");
    bool _var_initialized = false;
    bool _ta_initialized_ = false;

    explicit GeneratedStrategy() : _ta_sum_1(10), _ta_sma_2(20), _ta_sma_3(20), _ta_ema_4(40), _ta_ema_5(20), _ta_ema_6(3), _ta_ema_7(20), _ta_ema_8(5), _ta_ema_9(3), _ta_sum_10(10), original_qty(na<double>()), peak_sigma(0), current_pct(1), bars_since_adj(0), entry_close(na<double>()), pos_age(0), dash(na<double>()) {
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
        if (!_var_initialized) {
            kama.push(na<double>());
            _var_initialized = true;
        } else {
            if (is_first_tick_) kama.push(kama[0]);
        }
        if (!_ta_initialized_) {
            _ta_sum_1 = math::Sum(get_input_int("ER Length", 10));
            _ta_sma_2 = ta::SMA(get_input_int("Deviation Length", 20));
            _ta_sma_3 = ta::SMA(get_input_int("Deviation Length", 20));
            _ta_ema_4 = ta::EMA(get_input_int("Deviation Length", 20) * 2);
            _ta_ema_5 = ta::EMA(get_input_int("Deviation Length", 20));
            _ta_ema_7 = ta::EMA(get_input_int("Deviation Length", 20));
            _ta_sum_10 = math::Sum(get_input_int("ER Length", 10));
            _ta_initialized_ = true;
        }
        input_direction = get_input_string("Trade Direction", std::string("Long"));
        input_trade_mode = get_input_string("Trade Behaviour", std::string("Hold"));
        input_use_dev_gate = get_input_bool("Band Deviation Gate", true);
        input_max_dev = get_input_double("Max Entry Deviation (σ)", 1.5);
        input_trim_start = get_input_double("Trim Start (σ)", 1.0);
        input_trim_step = get_input_double("Trim Step (%)", 20.0);
        input_min_hold = get_input_double("Min Hold (%)", 25.0);
        input_reload_buf = get_input_double("Reload Buffer", 0.5);
        input_trend_shift = get_input_double("Trend Quality Shift (σ)", 0.5);
        input_cooldown = get_input_int("Cooldown (bars)", 2);
        input_er_len = get_input_int("ER Length", 10);
        input_fast_period = get_input_int("Fast Period", 2);
        input_slow_period = get_input_int("Slow Period", 30);
        input_dev_len = get_input_int("Deviation Length", 20);
        input_band_mult = get_input_double("Band Multiplier", 2.0);
        input_col_bull = get_input_int("Trending Up", 0);
        input_col_bear = get_input_int("Trending Down", 0);
        input_col_neutral = get_input_int("Coiling", 0);
        input_col_band = get_input_int("Bands", 0);
        input_kama_width = get_input_int("Fair Value Line Width", 3);
        input_show_dash = get_input_bool("Show Dashboard", true);
        input_dash_pos = get_input_string("Position", std::string("Top Right"));
        input_dash_size = get_input_string("Text Size", std::string("Small"));
        input_show_bands = get_input_bool("Show Bands", true);
        input_show_sigs = get_input_bool("Show Signals", true);
        input_th_accent = get_input_int("Accent", 0);
        input_th_label = get_input_int("Labels", 0);
        input_th_hdr_bg = get_input_int("Header Background", 0);
        input_th_bg1 = get_input_int("Row Background", 0);
        input_th_bg2 = get_input_int("Alt Background", 0);
        input_th_border = get_input_int("Border", 0);
        ER_LEN = input_er_len;
        FAST_PERIOD = input_fast_period;
        SLOW_PERIOD = input_slow_period;
        DEV_LEN = input_dev_len;
        BAND_MULT = input_band_mult;
        net_move = std::abs((current_bar_.close - _s_close[ER_LEN]));
        total_path = (is_first_tick_ ? _ta_sum_1.compute(std::abs((current_bar_.close - _s_close[1]))) : _ta_sum_1.recompute(std::abs((current_bar_.close - _s_close[1]))));
        er.push((((total_path != 0.0)) ? (((double)(net_move) / (double)(total_path))) : (0.0)));
        fast_sc = ((double)(2.0) / (double)((FAST_PERIOD + 1)));
        slow_sc = ((double)(2.0) / (double)((SLOW_PERIOD + 1)));
        sc = std::pow(((er[0] * (fast_sc - slow_sc)) + slow_sc), 2);
        kama.update(((is_na(kama[1])) ? (current_bar_.close) : ((kama[1] + (sc * (current_bar_.close - kama[1]))))));
        kama_dev = std::sqrt((is_first_tick_ ? _ta_sma_2.compute(std::pow((current_bar_.close - kama[0]), 2)) : _ta_sma_2.recompute(std::pow((current_bar_.close - kama[0]), 2))));
        band_upper = (kama[0] + (BAND_MULT * kama_dev));
        band_lower = (kama[0] - (BAND_MULT * kama_dev));
        band_width = (band_upper - band_lower);
        band_pos = (((band_width > 0.0)) ? (((double)((current_bar_.close - band_lower)) / (double)(band_width))) : (0.5));
        in_lower_z = (band_pos <= 0.33);
        in_upper_z = (band_pos >= 0.67);
        in_mid_z = (!(in_lower_z) && !(in_upper_z));
        bw_pct = (((kama[0] > 0.0)) ? ((((double)(band_width) / (double)(kama[0])) * 100.0)) : (0.0));
        bw_avg = (is_first_tick_ ? _ta_sma_3.compute(bw_pct) : _ta_sma_3.recompute(bw_pct));
        bw_ratio = ((((is_na(bw_avg) ? 0.0 : bw_avg) > 0.0)) ? (((double)(bw_pct) / (double)(bw_avg))) : (1.0));
        bwr_long_avg = (is_first_tick_ ? _ta_ema_4.compute(bw_ratio) : _ta_ema_4.recompute(bw_ratio));
        bwr_dev = (is_first_tick_ ? _ta_ema_5.compute(std::abs((bw_ratio - bwr_long_avg))) : _ta_ema_5.recompute(std::abs((bw_ratio - bwr_long_avg))));
        volatile_mkt = (bw_ratio > (bwr_long_avg + bwr_dev));
        tight_coil = (bw_ratio < (bwr_long_avg - bwr_dev));
        dist_from_fv_norm = (((kama_dev > 0.0)) ? (((double)((current_bar_.close - kama[0])) / (double)(kama_dev))) : (0.0));
        abs_dist = std::abs(dist_from_fv_norm);
        sc_threshold = std::sqrt((std::pow(slow_sc, 2) * std::pow(fast_sc, 2)));
        is_choppy = (sc < sc_threshold);
        kama_slope_raw = (kama[0] - kama[1]);
        kama_slope.push((is_first_tick_ ? _ta_ema_6.compute(kama_slope_raw) : _ta_ema_6.recompute(kama_slope_raw)));
        slope_accel.push((kama_slope[0] > kama_slope[1]));
        is_green = (((!(is_choppy) && (kama_slope[0] > 0.0)) && (slope_accel[0] || slope_accel[1])) && (current_bar_.close > kama[0]));
        is_red = ((((!(is_choppy) && (kama_slope[0] < 0.0)) && !(slope_accel[0])) && !(slope_accel[1])) && (current_bar_.close < kama[0]));
        is_gray.push((!(is_green) && !(is_red)));
        er_avg = (is_first_tick_ ? _ta_ema_7.compute(er[0]) : _ta_ema_7.recompute(er[0]));
        er_change_rate = (is_first_tick_ ? _ta_ema_8.compute(std::abs((er[0] - er[1]))) : _ta_ema_8.recompute(std::abs((er[0] - er[1]))));
        er_noise_floor = std::max((double)((er_avg * 0.6)), (double)(0.1));
        er_strong_thresh = std::min((double)((er_avg + (er_change_rate * 4.0))), (double)(0.85));
        er_rising = (er[0] > er[1]);
        er_slope = (is_first_tick_ ? _ta_ema_9.compute((er[0] - er[3])) : _ta_ema_9.recompute((er[0] - er[3])));
        er_building = ((er_slope > 0.0) && (er[0] > er_noise_floor));
        coil_ratio = ((double)((is_first_tick_ ? _ta_sum_10.compute(((is_gray[0]) ? (1) : (0))) : _ta_sum_10.recompute(((is_gray[0]) ? (1) : (0))))) / (double)(ER_LEN));
        mostly_coiling = (coil_ratio > 0.5);
        strong_trend = ((er[0] >= er_strong_thresh) && slope_accel[0]);
        body_size = std::abs((current_bar_.close - current_bar_.open));
        full_range = (current_bar_.high - current_bar_.low);
        body_ratio = (((full_range > 0.0)) ? (((double)(body_size) / (double)(full_range))) : (0.5));
        upper_wick = (current_bar_.high - std::max((double)(current_bar_.open), (double)(current_bar_.close)));
        lower_wick = (std::min((double)(current_bar_.open), (double)(current_bar_.close)) - current_bar_.low);
        wick_ratio_upper = (((full_range > 0.0)) ? (((double)(upper_wick) / (double)(full_range))) : (0.0));
        wick_ratio_lower = (((full_range > 0.0)) ? (((double)(lower_wick) / (double)(full_range))) : (0.0));
        rejection_top = ((wick_ratio_upper > 0.4) && in_upper_z);
        rejection_bottom = ((wick_ratio_lower > 0.4) && in_lower_z);
        in_long.push((signed_position_size() > 0));
        in_short.push((signed_position_size() < 0));
        in_trade = (in_long[0] || in_short[0]);
        flat = !(in_trade);
        long_goes_gray = ((in_long[0] && is_gray[0]) && !(is_gray[1]));
        short_goes_gray = ((in_short[0] && is_gray[0]) && !(is_gray[1]));
        long_goes_red = (in_long[0] && is_red);
        short_goes_green = (in_short[0] && is_green);
        new_position = ((in_long[0] && !(in_long[1])) || (in_short[0] && !(in_short[1])));
        if (new_position) {
            original_qty = std::abs(signed_position_size());
            peak_sigma = abs_dist;
            current_pct = 1.0;
            bars_since_adj = 0;
            last_adapt_action = std::string("ENTRY");
        }
        if (flat) {
            original_qty = na<double>();
            peak_sigma = 0.0;
            current_pct = 1.0;
            bars_since_adj = 0;
            last_adapt_action = std::string("NONE");
        }
        if ((in_trade && !(new_position))) {
            bars_since_adj += 1;
            if ((abs_dist > peak_sigma)) {
                peak_sigma = abs_dist;
            }
        }
        dev_ok = (!(input_use_dev_gate) || (abs_dist <= input_max_dev));
        if ((input_trade_mode == std::string("Hold"))) {
            if (long_goes_red) {
                strategy_close(std::string("Long"), std::string("Color Ended"), na<double>(), na<double>(), false);
            }
            if (short_goes_green) {
                strategy_close(std::string("Short"), std::string("Color Ended"), na<double>(), na<double>(), false);
            }
        }
        if ((input_trade_mode == std::string("Exit"))) {
            if ((in_long[0] && !(is_green))) {
                strategy_close(std::string("Long"), std::string("Color Ended"), na<double>(), na<double>(), false);
            }
            if ((in_short[0] && !(is_red))) {
                strategy_close(std::string("Short"), std::string("Color Ended"), na<double>(), na<double>(), false);
            }
        }
        if ((((input_trade_mode == std::string("Adaptive")) && in_trade) && !(new_position))) {
            if (long_goes_red) {
                strategy_close(std::string("Long"), std::string("Color Ended"), na<double>(), na<double>(), false);
                last_adapt_action = std::string("EXIT");
            } else
            if (short_goes_green) {
                strategy_close(std::string("Short"), std::string("Color Ended"), na<double>(), na<double>(), false);
                last_adapt_action = std::string("EXIT");
            } else
            if ((!(is_na(original_qty)) && (original_qty > 0))) {
                _shift = ((strong_trend) ? (input_trend_shift) : (0.0));
                _pa_shift = ((((in_long[0] && rejection_top) || (in_short[0] && rejection_bottom))) ? ((-0.25)) : (0.0));
                _eff_shift = std::max((double)((_shift + _pa_shift)), (double)(0.0));
                _gray_penalty = ((is_gray[0]) ? (0.25) : (0.0));
                _trim_thresh = ((input_trim_start + _eff_shift) - _gray_penalty);
                _step_frac = ((double)(input_trim_step) / (double)(100.0));
                _min_frac = ((double)(input_min_hold) / (double)(100.0));
                _cooldown_ok = (bars_since_adj >= input_cooldown);
                _trim_depth = (((abs_dist > _trim_thresh)) ? (((double)((abs_dist - _trim_thresh)) / (double)(std::max((double)((_trim_thresh * 0.5)), (double)(0.25))))) : (0.0));
                _trim_frac = std::min((double)((_trim_depth * _step_frac)), (double)((1.0 - _min_frac)));
                _target_pct = std::max((double)((1.0 - _trim_frac)), (double)(_min_frac));
                _reload_thresh = (peak_sigma * input_reload_buf);
                _color_ok = ((in_long[0] && is_green) || (in_short[0] && is_red));
                _reload_zone = ((abs_dist < _reload_thresh) && _color_ok);
                if (_reload_zone) {
                    _target_pct = std::max((double)(_target_pct), (double)(std::min((double)((current_pct + _step_frac)), (double)(1.0))));
                }
                _pct_diff = (_target_pct - current_pct);
                _sig_change = (std::abs(_pct_diff) >= (_step_frac * 0.5));
                if ((_cooldown_ok && _sig_change)) {
                    _target_qty = std::max((double)((original_qty * _target_pct)), (double)(1.0));
                    _current_qty = std::abs(signed_position_size());
                    _delta = (_target_qty - _current_qty);
                    if (((_delta < 0) && (std::abs(_delta) >= 1.0))) {
                        strategy_close(((in_long[0]) ? (std::string("Long")) : (std::string("Short"))), std::string("Adapt Trim"), std::abs(_delta), na<double>(), false);
                        current_pct = _target_pct;
                        bars_since_adj = 0;
                        last_adapt_action = std::string("TRIM");
                    } else
                    if ((((_delta > 0) && _color_ok) && dev_ok)) {
                        strategy_order(((in_long[0]) ? (std::string("Long")) : (std::string("Short"))), ((in_long[0]) ? (true) : (false)), _delta, na<double>(), na<double>(), "", 0);
                        current_pct = _target_pct;
                        bars_since_adj = 0;
                        last_adapt_action = std::string("RELOAD");
                    }
                }
            }
        }
        color_long = (((flat && is_green) && dev_ok) && (input_direction != std::string("Short")));
        color_short = (((flat && is_red) && dev_ok) && (input_direction != std::string("Long")));
        if (color_long) {
            strategy_entry(std::string("Long"), true, na<double>(), na<double>(), na<double>(), std::string("Trend Long"));
        }
        if (color_short) {
            strategy_entry(std::string("Short"), false, na<double>(), na<double>(), na<double>(), std::string("Trend Short"));
        }
        any_long_entry = color_long;
        any_short_entry = color_short;
        if (new_position) {
            entry_close = _s_close[1];
        }
        sigma_moved = ((in_long[0]) ? (((double)((current_bar_.close - entry_close)) / (double)(std::max((double)(kama_dev), (double)(1e-10))))) : (((in_short[0]) ? (((double)((entry_close - current_bar_.close)) / (double)(std::max((double)(kama_dev), (double)(1e-10))))) : (0.0))));
        if (new_position) {
            pos_age = 1;
        } else
        if (in_trade) {
            pos_age += 1;
        } else {
            pos_age = 0;
        }
        state_color = ((is_green) ? (input_col_bull) : (((is_red) ? (input_col_bear) : (input_col_neutral))));
        band_color = ((input_show_bands) ? (pine_color::new_color(input_col_band, (int)(65))) : (na<double>()));
        auto __switch_val_0 = input_dash_pos;
        if (__switch_val_0 == std::string("Top Right")) {
        }
        else if (__switch_val_0 == std::string("Top Left")) {
        }
        else if (__switch_val_0 == std::string("Bottom Right")) {
        }
        else if (__switch_val_0 == std::string("Bottom Left")) {
        }
        else if (__switch_val_0 == std::string("Middle Right")) {
        }
        else if (__switch_val_0 == std::string("Middle Left")) {
        }
        else {
        }
        auto __switch_val_1 = input_dash_size;
        if (__switch_val_1 == std::string("Tiny")) {
        }
        else if (__switch_val_1 == std::string("Normal")) {
        }
        else {
        }
        auto __switch_val_2 = input_dash_size;
        if (__switch_val_2 == std::string("Tiny")) {
        }
        else if (__switch_val_2 == std::string("Normal")) {
        }
        else {
        }
        if (input_show_dash) {
            if (is_na(dash)) {
            }
            _bg0 = pine_color::new_color(input_th_hdr_bg, (int)(0));
            _bg1 = pine_color::new_color(input_th_bg1, (int)(0));
            _bg2 = pine_color::new_color(input_th_bg2, (int)(0));
            _lbl = input_th_label;
            _acc = input_th_accent;
            _live = (signed_position_size() != 0);
            _pnl = open_profit(current_bar_.close);
            _avg = position_entry_price_;
            _pnl_pct = ((((_avg > 0.0) && _live)) ? ((((((double)(current_bar_.close) / (double)(_avg)) - 1.0) * 100.0) * ((in_long[0]) ? (1.0) : ((-1.0))))) : (0.0));
            _state_t = ((is_green) ? (std::string("▲ TRENDING UP")) : (((is_red) ? (std::string("▼ TRENDING DOWN")) : (((tight_coil) ? (std::string("◈ COILING TIGHT")) : (std::string("◈ COILING")))))));
            _state_c = ((is_green) ? (input_col_bull) : (((is_red) ? (input_col_bear) : (((tight_coil) ? (input_col_band) : (input_col_neutral))))));
            _h_in = (in_long[0] || in_short[0]);
            _sig_t = (((long_goes_red || short_goes_green)) ? (std::string("✕ COLOR ENDED")) : (((((long_goes_gray || short_goes_gray) && (input_trade_mode == std::string("Exit")))) ? (std::string("✕ COLOR ENDED")) : (((color_long) ? (std::string("▲ TREND LONG")) : (((color_short) ? (std::string("▼ TREND SHORT")) : (((((_h_in && (last_adapt_action == std::string("TRIM"))) && (bars_since_adj < 2))) ? (std::string("◑ ADAPT TRIM")) : (((((_h_in && (last_adapt_action == std::string("RELOAD"))) && (bars_since_adj < 2))) ? (std::string("▲ ADAPT RELOAD")) : (((is_green) ? (std::string("● GREEN — WATCHING")) : (((is_red) ? (std::string("● RED — WATCHING")) : ((((_h_in && is_gray[0])) ? (std::string("◈ GRAY — HOLDING")) : (std::string("◈ GRAY")))))))))))))))))));
            _sig_c = (((long_goes_red || short_goes_green)) ? (input_col_bear) : (((((long_goes_gray || short_goes_gray) && (input_trade_mode == std::string("Exit")))) ? (input_col_bear) : (((color_long) ? (input_col_bull) : (((color_short) ? (input_col_bear) : (((((_h_in && (last_adapt_action == std::string("TRIM"))) && (bars_since_adj < 2))) ? (_acc) : (((((_h_in && (last_adapt_action == std::string("RELOAD"))) && (bars_since_adj < 2))) ? (input_col_bull) : (((is_green) ? (input_col_bull) : (((is_red) ? (input_col_bear) : ((((_h_in && is_gray[0])) ? (_acc) : (_lbl))))))))))))))))));
            _pos_val = (std::abs(signed_position_size()) * current_bar_.close);
            _pos_pct = ((((current_equity() + open_profit(current_bar_.close)) > 0)) ? ((((double)(_pos_val) / (double)((current_equity() + open_profit(current_bar_.close)))) * 100.0)) : (0.0));
            _pos_t = ((in_long[0]) ? (((((((std::string("● LONG  ") + pine_str_tostring(signed_position_size(), std::string("#.##"), syminfo_mintick_)) + std::string("  $")) + pine_str_tostring(_pos_val, std::string("#,###.##"), syminfo_mintick_)) + std::string(" (")) + pine_str_tostring(_pos_pct, std::string("#.#"), syminfo_mintick_)) + std::string("%)"))) : (((in_short[0]) ? (((((((std::string("● SHORT  ") + pine_str_tostring(std::abs(signed_position_size()), std::string("#.##"), syminfo_mintick_)) + std::string("  $")) + pine_str_tostring(_pos_val, std::string("#,###.##"), syminfo_mintick_)) + std::string(" (")) + pine_str_tostring(_pos_pct, std::string("#.#"), syminfo_mintick_)) + std::string("%)"))) : (std::string("─")))));
            _pos_c = ((in_long[0]) ? (input_col_bull) : (((in_short[0]) ? (input_col_bear) : (_lbl))));
            _pnl_pfx = (((_pnl >= 0.0)) ? (std::string("+")) : (std::string("")));
            _pnl_t = ((_live) ? (((((_pnl_pfx + pine_str_tostring(_pnl, std::string("#.##"), syminfo_mintick_)) + std::string("  (")) + pine_str_tostring(_pnl_pct, std::string("#.##"), syminfo_mintick_)) + std::string("%)"))) : (std::string("─")));
            _pnl_c = (((_pnl > 0.0)) ? (input_col_bull) : ((((_pnl < 0.0)) ? (input_col_bear) : (_lbl))));
            _er_c = (((er[0] >= er_strong_thresh)) ? (input_col_bull) : ((((er[0] >= er_noise_floor)) ? (_acc) : (input_col_bear))));
            _er_l = (((er[0] >= er_strong_thresh)) ? (std::string("TRENDING")) : ((((er[0] >= er_noise_floor)) ? (std::string("BUILDING")) : (std::string("COILING")))));
            _cr_t = ((mostly_coiling) ? (((std::string("  COIL ") + pine_str_tostring((coil_ratio * 100.0), std::string("#"), syminfo_mintick_)) + std::string("%"))) : (std::string("")));
            _zone_t = ((in_lower_z) ? (std::string("LOWER")) : (((in_upper_z) ? (std::string("UPPER")) : (std::string("MID")))));
            _vol_t = ((volatile_mkt) ? (std::string("VOLATILE")) : (((tight_coil) ? (std::string("TIGHT COIL")) : (std::string("")))));
            _bnd_t = (((pine_str_tostring(band_pos, std::string("#.##"), syminfo_mintick_) + std::string(" ")) + _zone_t) + (((_vol_t != std::string(""))) ? ((std::string("  ") + _vol_t)) : (std::string(""))));
            _bnd_c = ((volatile_mkt) ? (input_col_bear) : (((tight_coil) ? (input_col_band) : (((in_mid_z) ? (_acc) : (input_col_band))))));
            _dev_t = ((_h_in) ? ((((((pine_str_tostring(sigma_moved, std::string("#.##"), syminfo_mintick_) + std::string("σ  age:")) + std::to_string(pos_age)) + std::string("  dist:")) + pine_str_tostring(dist_from_fv_norm, std::string("#.##"), syminfo_mintick_)) + std::string("σ"))) : (std::string("─")));
            _dev_thresh = ((input_use_dev_gate) ? (input_max_dev) : (1.5));
            _dev_c = ((!(_h_in)) ? (_lbl) : ((((abs_dist > _dev_thresh)) ? (_acc) : (_lbl))));
            _adp_t = std::string("─");
            _adp_c = _lbl;
            if (((input_trade_mode == std::string("Adaptive")) && _h_in)) {
                _act_t = last_adapt_action;
                _adp_t = ((((pine_str_tostring((current_pct * 100.0), std::string("#"), syminfo_mintick_) + std::string("%  pk:")) + pine_str_tostring(peak_sigma, std::string("#.##"), syminfo_mintick_)) + std::string("σ  ")) + _act_t);
                _adp_c = (((current_pct >= 0.9)) ? (input_col_bull) : ((((current_pct >= 0.5)) ? (_acc) : (input_col_bear))));
            }
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
