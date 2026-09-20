#include <pineforge/source/pine_strategy_host.hpp>
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
#include <optional>
#include <type_traits>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <unordered_map>
#include <pineforge/color.hpp>
#include <pineforge/log.hpp>
#include <pineforge/str_utils.hpp>
#include <pineforge/session_time.hpp>
#ifndef PINEFORGE_HAS_NATIVE_LOWERING_V1
#error "generated code requires pineforge-engine native lowering v1 (PINEFORGE_HAS_NATIVE_LOWERING_V1)"
#endif

using namespace pineforge;

// --- syminfo derivation helpers (PineForge G2) ---
static inline std::string _pf_derive_prefix(const std::string& tickerid) {
    std::size_t colon = tickerid.find(':');
    return (colon == std::string::npos) ? tickerid : tickerid.substr(0, colon);
}

static inline std::string _pf_derive_main_tickerid(const std::string& tickerid) {
    // Strip trailing digits (optionally followed by '!') from the symbol part.
    // e.g. "CME_MINI:ES1!" -> "CME_MINI:ES", "NYMEX:CL2!" -> "NYMEX:CL"
    std::string result = tickerid;
    std::size_t colon = result.find(':');
    std::size_t start = (colon == std::string::npos) ? 0 : colon + 1;
    // Find end of base symbol (strip trailing digits + optional '!')
    std::size_t end = result.size();
    if (end > start && result[end - 1] == '!') {
        --end;
    }
    while (end > start && std::isdigit((unsigned char)result[end - 1])) {
        --end;
    }
    return result.substr(0, end);
}

static inline std::string _pf_derive_country(const std::string& tickerid) {
    // Lookup country by exchange prefix (text before ':').
    std::size_t colon = tickerid.find(':');
    std::string prefix = (colon == std::string::npos)
        ? tickerid : tickerid.substr(0, colon);
    static const std::unordered_map<std::string, std::string> _tbl = {
        {"AMEX", "US"},
        {"AQUIS", "GB"},
        {"ARCA", "US"},
        {"ASX", "AU"},
        {"B3", "BR"},
        {"BMF", "BR"},
        {"BMFBOVESPA", "BR"},
        {"BSE", "IN"},
        {"CBOE", "US"},
        {"CBOT", "US"},
        {"CME", "US"},
        {"CME_MINI", "US"},
        {"COINBASE", "US"},
        {"COMEX", "US"},
        {"HKEX", "HK"},
        {"JSE", "ZA"},
        {"KOSPI", "KR"},
        {"KRX", "KR"},
        {"LSE", "GB"},
        {"MOEX", "RU"},
        {"NASDAQ", "US"},
        {"NSE", "IN"},
        {"NYMEX", "US"},
        {"NYSE", "US"},
        {"OSE", "JP"},
        {"OTC", "US"},
        {"SGX", "SG"},
        {"SIX", "CH"},
        {"SSE", "CN"},
        {"SZSE", "CN"},
        {"TSE", "JP"},
        {"TSX", "CA"},
        {"UPBIT", "KR"},
        {"VENTURE", "CA"},
        {"XETRA", "DE"}
    };
    auto it = _tbl.find(prefix);
    return (it != _tbl.end()) ? it->second : na<std::string>();
}
// --- end syminfo derivation helpers ---

class GeneratedStrategy : public pineforge::source::PineStrategyHost {
public:
    ta::RSI _ta_rsi_1;
    std::vector<double> _precalc__ta_rsi_1;
    ta::EMA _ta_ema_2;
    std::vector<double> _precalc__ta_ema_2;
    ta::EMA _ta_ema_3;
    std::vector<double> _precalc__ta_ema_3;
    ta::EMA _ta_ema_4;
    bool _use_precalc = false;
    Series<double> macd_hist;
    Series<int> momentum_bear_score;
    Series<int> momentum_bull_score;
    Series<double> rsi;
    int i_rsi_len = 0;
    double i_rsi_bull = 0.0;
    double i_rsi_bear = 0.0;
    int i_macd_fast = 0;
    int i_macd_slow = 0;
    int i_macd_sig = 0;
    bool rsi_bullish = false;
    bool rsi_bearish = false;
    bool rsi_momentum_up = false;
    bool rsi_momentum_dn = false;
    double macd_line = 0.0;
    double macd_signal = 0.0;
    bool macd_bullish = false;
    bool macd_bearish = false;
    bool long_entry = false;
    bool short_entry = false;
    bool _ta_initialized_ = false;
    bool _inputs_initialized_ = false;

    struct _PFScriptState {
        decltype(GeneratedStrategy::_ta_rsi_1) _pf_value_0;
        decltype(GeneratedStrategy::_ta_ema_2) _pf_value_1;
        decltype(GeneratedStrategy::_ta_ema_3) _pf_value_2;
        decltype(GeneratedStrategy::_ta_ema_4) _pf_value_3;
        decltype(GeneratedStrategy::macd_hist) _pf_value_4;
        decltype(GeneratedStrategy::momentum_bear_score) _pf_value_5;
        decltype(GeneratedStrategy::momentum_bull_score) _pf_value_6;
        decltype(GeneratedStrategy::rsi) _pf_value_7;
        decltype(GeneratedStrategy::i_rsi_len) _pf_value_8;
        decltype(GeneratedStrategy::i_rsi_bull) _pf_value_9;
        decltype(GeneratedStrategy::i_rsi_bear) _pf_value_10;
        decltype(GeneratedStrategy::i_macd_fast) _pf_value_11;
        decltype(GeneratedStrategy::i_macd_slow) _pf_value_12;
        decltype(GeneratedStrategy::i_macd_sig) _pf_value_13;
        decltype(GeneratedStrategy::rsi_bullish) _pf_value_14;
        decltype(GeneratedStrategy::rsi_bearish) _pf_value_15;
        decltype(GeneratedStrategy::rsi_momentum_up) _pf_value_16;
        decltype(GeneratedStrategy::rsi_momentum_dn) _pf_value_17;
        decltype(GeneratedStrategy::macd_line) _pf_value_18;
        decltype(GeneratedStrategy::macd_signal) _pf_value_19;
        decltype(GeneratedStrategy::macd_bullish) _pf_value_20;
        decltype(GeneratedStrategy::macd_bearish) _pf_value_21;
        decltype(GeneratedStrategy::long_entry) _pf_value_22;
        decltype(GeneratedStrategy::short_entry) _pf_value_23;
        decltype(GeneratedStrategy::_ta_initialized_) _pf_value_24;
        decltype(GeneratedStrategy::_inputs_initialized_) _pf_value_25;
    };
    static_assert(std::is_copy_constructible_v<_PFScriptState>, "generated Pine state must be deep-copy constructible");
    static_assert(std::is_copy_assignable_v<_PFScriptState>, "generated Pine state must be deep-copy assignable");
    std::optional<_PFScriptState> _pf_script_state_checkpoint_;

    void snapshot_script_state() override {
        _pf_script_state_checkpoint_.emplace(_PFScriptState{
            _ta_rsi_1,
            _ta_ema_2,
            _ta_ema_3,
            _ta_ema_4,
            macd_hist,
            momentum_bear_score,
            momentum_bull_score,
            rsi,
            i_rsi_len,
            i_rsi_bull,
            i_rsi_bear,
            i_macd_fast,
            i_macd_slow,
            i_macd_sig,
            rsi_bullish,
            rsi_bearish,
            rsi_momentum_up,
            rsi_momentum_dn,
            macd_line,
            macd_signal,
            macd_bullish,
            macd_bearish,
            long_entry,
            short_entry,
            _ta_initialized_,
            _inputs_initialized_,
        });
    }

    void restore_script_state() override {
        if (!_pf_script_state_checkpoint_) return;
        this->_ta_rsi_1 = _pf_script_state_checkpoint_->_pf_value_0;
        this->_ta_ema_2 = _pf_script_state_checkpoint_->_pf_value_1;
        this->_ta_ema_3 = _pf_script_state_checkpoint_->_pf_value_2;
        this->_ta_ema_4 = _pf_script_state_checkpoint_->_pf_value_3;
        this->macd_hist = _pf_script_state_checkpoint_->_pf_value_4;
        this->momentum_bear_score = _pf_script_state_checkpoint_->_pf_value_5;
        this->momentum_bull_score = _pf_script_state_checkpoint_->_pf_value_6;
        this->rsi = _pf_script_state_checkpoint_->_pf_value_7;
        this->i_rsi_len = _pf_script_state_checkpoint_->_pf_value_8;
        this->i_rsi_bull = _pf_script_state_checkpoint_->_pf_value_9;
        this->i_rsi_bear = _pf_script_state_checkpoint_->_pf_value_10;
        this->i_macd_fast = _pf_script_state_checkpoint_->_pf_value_11;
        this->i_macd_slow = _pf_script_state_checkpoint_->_pf_value_12;
        this->i_macd_sig = _pf_script_state_checkpoint_->_pf_value_13;
        this->rsi_bullish = _pf_script_state_checkpoint_->_pf_value_14;
        this->rsi_bearish = _pf_script_state_checkpoint_->_pf_value_15;
        this->rsi_momentum_up = _pf_script_state_checkpoint_->_pf_value_16;
        this->rsi_momentum_dn = _pf_script_state_checkpoint_->_pf_value_17;
        this->macd_line = _pf_script_state_checkpoint_->_pf_value_18;
        this->macd_signal = _pf_script_state_checkpoint_->_pf_value_19;
        this->macd_bullish = _pf_script_state_checkpoint_->_pf_value_20;
        this->macd_bearish = _pf_script_state_checkpoint_->_pf_value_21;
        this->long_entry = _pf_script_state_checkpoint_->_pf_value_22;
        this->short_entry = _pf_script_state_checkpoint_->_pf_value_23;
        this->_ta_initialized_ = _pf_script_state_checkpoint_->_pf_value_24;
        this->_inputs_initialized_ = _pf_script_state_checkpoint_->_pf_value_25;
    }

    void commit_script_state() override {
        snapshot_script_state();
    }

    explicit GeneratedStrategy() : _ta_rsi_1(14), _ta_ema_2(12), _ta_ema_3(26), _ta_ema_4(9) {
#if defined(PINEFORGE_HAS_EXPLICIT_PINE_EXECUTION_ADAPTER_V1)
        pineforge::source::PineStrategyHost::attach_pine_execution_adapter();
#elif defined(PINEFORGE_HAS_EXPLICIT_PINE_CAP_V1)
        pineforge::source::PineStrategyHost::enable_pine_intraday_cap();
#endif
        pineforge::source::PineStrategyConfig cfg{};
        cfg.initial_capital = 1000000.0;
        cfg.default_qty_type = static_cast<int>(QtyType::FIXED);
        cfg.default_qty_value = 1.0;
        cfg.pyramiding = 1;
        cfg.commission_type = static_cast<int>(CommissionType::PERCENT);
        cfg.commission_value = 0.0;
        cfg.slippage = 0;
        configure_pine_strategy(cfg);
    }

    void set_strategy_override(const std::string& key, const std::string& value) {
        pineforge::source::StrategyOverrides overrides{};
        if (key == "initial_capital") {
            overrides.initial_capital = std::stod(value);
        } else if (key == "commission_value") {
            overrides.commission_value = std::stod(value);
        } else if (key == "default_qty_value") {
            overrides.default_qty_value = std::stod(value);
        } else if (key == "pyramiding") {
            overrides.pyramiding = std::stoi(value);
        } else if (key == "slippage") {
            overrides.slippage = std::stoi(value);
        } else if (key == "process_orders_on_close") {
            overrides.process_orders_on_close = (value == "true" || value == "1");
        } else if (key == "calc_on_order_fills") {
            overrides.calc_on_order_fills = (value == "true" || value == "1");
        } else if (key == "close_entries_rule") {
            overrides.close_entries_rule = (value == "ANY" || value == "any" || value == "1");
        } else if (key == "default_qty_type") {
            if (value == "fixed" || value == "strategy.fixed" || value == "0") overrides.default_qty_type = static_cast<int>(QtyType::FIXED);
            else if (value == "percent_of_equity" || value == "strategy.percent_of_equity" || value == "1") overrides.default_qty_type = static_cast<int>(QtyType::PERCENT_OF_EQUITY);
            else if (value == "cash" || value == "strategy.cash" || value == "2") overrides.default_qty_type = static_cast<int>(QtyType::CASH);
            else return;
        } else if (key == "commission_type") {
            if (value == "percent" || value == "strategy.commission.percent" || value == "0") overrides.commission_type = static_cast<int>(CommissionType::PERCENT);
            else if (value == "cash_per_order" || value == "strategy.commission.cash_per_order" || value == "1") overrides.commission_type = static_cast<int>(CommissionType::CASH_PER_ORDER);
            else if (value == "cash_per_contract" || value == "strategy.commission.cash_per_contract" || value == "2") overrides.commission_type = static_cast<int>(CommissionType::CASH_PER_CONTRACT);
            else return;
        } else {
            return;
        }
        pineforge::source::PineStrategyHost::set_strategy_override(overrides);
    }

#ifndef PINEFORGE_HAS_SCRIPT_RUN_PREPARE_V1
#error "Generated lifecycle reset requires a matching PineForge engine; rebuild with script-run preparation support"
#endif
    void prepare_script_run(const Bar* bars, int n, bool allow_precalculation) override {
        _pf_script_state_checkpoint_.reset();
        this->_ta_rsi_1 = decltype(this->_ta_rsi_1)(14);
        this->_precalc__ta_rsi_1 = decltype(this->_precalc__ta_rsi_1){};
        this->_ta_ema_2 = decltype(this->_ta_ema_2)(12);
        this->_precalc__ta_ema_2 = decltype(this->_precalc__ta_ema_2){};
        this->_ta_ema_3 = decltype(this->_ta_ema_3)(26);
        this->_precalc__ta_ema_3 = decltype(this->_precalc__ta_ema_3){};
        this->_ta_ema_4 = decltype(this->_ta_ema_4)(9);
        this->_use_precalc = false;
        this->macd_hist = decltype(this->macd_hist){};
        this->momentum_bear_score = decltype(this->momentum_bear_score){};
        this->momentum_bull_score = decltype(this->momentum_bull_score){};
        this->rsi = decltype(this->rsi){};
        this->i_rsi_len = 0;
        this->i_rsi_bull = 0.0;
        this->i_rsi_bear = 0.0;
        this->i_macd_fast = 0;
        this->i_macd_slow = 0;
        this->i_macd_sig = 0;
        this->rsi_bullish = false;
        this->rsi_bearish = false;
        this->rsi_momentum_up = false;
        this->rsi_momentum_dn = false;
        this->macd_line = 0.0;
        this->macd_signal = 0.0;
        this->macd_bullish = false;
        this->macd_bearish = false;
        this->long_entry = false;
        this->short_entry = false;
        this->_ta_initialized_ = false;
        this->_inputs_initialized_ = false;
        if (allow_precalculation) precalculate(bars, n);
    }

    void on_source_bar(const Bar& bar) override {
        if (!_inputs_initialized_) {
            i_rsi_len = get_input_int("RSI Period", 14);
            i_rsi_bull = get_input_double("RSI Bullish Level", 55);
            i_rsi_bear = get_input_double("RSI Bearish Level", 45);
            i_macd_fast = get_input_int("MACD Fast", 12);
            i_macd_slow = get_input_int("MACD Slow", 26);
            i_macd_sig = get_input_int("MACD Signal", 9);
            _inputs_initialized_ = true;
        }
        if (!_ta_initialized_) {
            _ta_rsi_1 = ta::RSI(get_input_int("RSI Period", 14));
            _ta_ema_2 = ta::EMA(get_input_int("MACD Fast", 12));
            _ta_ema_3 = ta::EMA(get_input_int("MACD Slow", 26));
            _ta_ema_4 = ta::EMA(get_input_int("MACD Signal", 9));
            _ta_initialized_ = true;
        }
        if (history_advances_new_bar()) rsi.push((history_advances_new_bar() ? _ta_rsi_1.compute(current_bar_.close) : _ta_rsi_1.recompute(current_bar_.close)));
        else rsi.update((history_advances_new_bar() ? _ta_rsi_1.compute(current_bar_.close) : _ta_rsi_1.recompute(current_bar_.close)));
        rsi_bullish = ([&]{ auto _pna_l = (rsi[0]); auto _pna_r = (i_rsi_bull); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }());
        rsi_bearish = ([&]{ auto _pna_l = (rsi[0]); auto _pna_r = (i_rsi_bear); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }());
        rsi_momentum_up = ([&]{ auto _pna_l = (rsi[0]); auto _pna_r = (rsi[3]); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }());
        rsi_momentum_dn = ([&]{ auto _pna_l = (rsi[0]); auto _pna_r = (rsi[3]); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }());
        macd_line = ((_use_precalc ? _precalc__ta_ema_2[bar_index_] : (history_advances_new_bar() ? _ta_ema_2.compute(current_bar_.close) : _ta_ema_2.recompute(current_bar_.close))) - (_use_precalc ? _precalc__ta_ema_3[bar_index_] : (history_advances_new_bar() ? _ta_ema_3.compute(current_bar_.close) : _ta_ema_3.recompute(current_bar_.close))));
        macd_signal = (history_advances_new_bar() ? _ta_ema_4.compute(macd_line) : _ta_ema_4.recompute(macd_line));
        if (history_advances_new_bar()) macd_hist.push((macd_line - macd_signal));
        else macd_hist.update((macd_line - macd_signal));
        macd_bullish = (([&]{ auto _pna_l = (macd_hist[0]); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()) && ([&]{ auto _pna_l = (macd_hist[0]); auto _pna_r = (macd_hist[1]); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()));
        macd_bearish = (([&]{ auto _pna_l = (macd_hist[0]); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }()) && ([&]{ auto _pna_l = (macd_hist[0]); auto _pna_r = (macd_hist[1]); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }()));
        if (history_advances_new_bar()) momentum_bull_score.push(0);
        else momentum_bull_score.update(0);
        if (rsi_bullish) {
            momentum_bull_score.update(momentum_bull_score[0] + 1);
        }
        if (rsi_momentum_up) {
            momentum_bull_score.update(momentum_bull_score[0] + 1);
        }
        if (macd_bullish) {
            momentum_bull_score.update(momentum_bull_score[0] + 1);
        }
        if (history_advances_new_bar()) momentum_bear_score.push(0);
        else momentum_bear_score.update(0);
        if (rsi_bearish) {
            momentum_bear_score.update(momentum_bear_score[0] + 1);
        }
        if (rsi_momentum_dn) {
            momentum_bear_score.update(momentum_bear_score[0] + 1);
        }
        if (macd_bearish) {
            momentum_bear_score.update(momentum_bear_score[0] + 1);
        }
        long_entry = ((([&]{ auto _pna_l = (momentum_bull_score[0]); auto _pna_r = (2); return !is_na(_pna_l) && !is_na(_pna_r) && (_pna_l >= _pna_r); }()) && ([&]{ auto _pna_l = (momentum_bull_score[1]); auto _pna_r = (2); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }())) && ([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) || _pfc_eq); }()));
        short_entry = ((([&]{ auto _pna_l = (momentum_bear_score[0]); auto _pna_r = (2); return !is_na(_pna_l) && !is_na(_pna_r) && (_pna_l >= _pna_r); }()) && ([&]{ auto _pna_l = (momentum_bear_score[1]); auto _pna_r = (2); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }())) && ([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) || _pfc_eq); }()));
        if (long_entry) {
            if (([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }())) {
                strategy_close(std::string("S"), std::string("flip"), na<double>(), na<double>(), false, 352187318295ULL);
            }
            strategy_entry(std::string("L"), true, na<double>(), na<double>(), 1, std::string("mom 2of3 long"), "", 0, -1);
        }
        if (short_entry) {
            if (([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) {
                strategy_close(std::string("L"), std::string("flip"), na<double>(), na<double>(), false, 373662154775ULL);
            }
            strategy_entry(std::string("S"), false, na<double>(), na<double>(), 1, std::string("mom 2of3 short"), "", 0, -1);
        }
    }

    void precalculate(const Bar* bars, int n) {
        _use_precalc = false;
        if (n <= 0 || bars == nullptr) return;

        _precalc__ta_rsi_1.resize(n);
        _precalc__ta_ema_2.resize(n);
        _precalc__ta_ema_3.resize(n);

        _ta_rsi_1 = ta::RSI(14);
        _ta_ema_2 = ta::EMA(12);
        _ta_ema_3 = ta::EMA(26);


        for (int i = 0; i < n; ++i) {
            if (_src_series_active_) {
                const double _pc_o = bars[i].open;
                const double _pc_h = bars[i].high;
                const double _pc_l = bars[i].low;
                const double _pc_c = bars[i].close;
                const double _pc_v = bars[i].volume;
                _src_open_.push(_pc_o);   _src_high_.push(_pc_h);   _src_low_.push(_pc_l);
                _src_close_.push(_pc_c);  _src_volume_.push(_pc_v);
                _src_hl2_.push((_pc_h + _pc_l) / 2.0);
                _src_hlc3_.push((_pc_h + _pc_l + _pc_c) / 3.0);
                _src_ohlc4_.push((_pc_o + _pc_h + _pc_l + _pc_c) / 4.0);
                _src_hlcc4_.push((_pc_h + _pc_l + _pc_c + _pc_c) / 4.0);
            }
            _precalc__ta_rsi_1[i] = _ta_rsi_1.compute(bars[i].close);
            _precalc__ta_ema_2[i] = _ta_ema_2.compute(bars[i].close);
            _precalc__ta_ema_3[i] = _ta_ema_3.compute(bars[i].close);
        }

        _ta_rsi_1 = ta::RSI(14);
        _ta_ema_2 = ta::EMA(12);
        _ta_ema_3 = ta::EMA(26);

        _use_precalc = true;
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
            || !itf.empty() || !stf.empty();
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
