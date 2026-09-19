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
    ta::TR _ta_tr_1;
    ta::RMA _ta_rma_2;
    ta::RMA _ta_rma_3;
    ta::RMA _ta_rma_4;
    ta::RMA _ta_rma_5;
    ta::ATR _ta_atr_6;
    std::vector<double> _precalc__ta_atr_6;
    ta::SMA _ta_sma_7;
    bool _use_precalc = false;
    Series<double> _s_high;
    Series<double> _s_low;
    Series<bool> trending_regime;
    int i_adx_len = 0;
    double i_adx_trend = 0.0;
    int i_atr_len = 0;
    double i_vol_exp = 0.0;
    double i_vol_con = 0.0;
    double adx = 0.0;
    double plus_di = 0.0;
    double minus_di = 0.0;
    double atr_val = 0.0;
    double atr_ma = 0.0;
    double vol_ratio = 0.0;
    int regime = 0;
    bool long_entry = false;
    bool long_exit = false;
    bool _ta_initialized_ = false;
    bool _inputs_initialized_ = false;

    struct _PFScriptState {
        decltype(GeneratedStrategy::_ta_tr_1) _pf_value_0;
        decltype(GeneratedStrategy::_ta_rma_2) _pf_value_1;
        decltype(GeneratedStrategy::_ta_rma_3) _pf_value_2;
        decltype(GeneratedStrategy::_ta_rma_4) _pf_value_3;
        decltype(GeneratedStrategy::_ta_rma_5) _pf_value_4;
        decltype(GeneratedStrategy::_ta_atr_6) _pf_value_5;
        decltype(GeneratedStrategy::_ta_sma_7) _pf_value_6;
        decltype(GeneratedStrategy::_s_high) _pf_value_7;
        decltype(GeneratedStrategy::_s_low) _pf_value_8;
        decltype(GeneratedStrategy::trending_regime) _pf_value_9;
        decltype(GeneratedStrategy::i_adx_len) _pf_value_10;
        decltype(GeneratedStrategy::i_adx_trend) _pf_value_11;
        decltype(GeneratedStrategy::i_atr_len) _pf_value_12;
        decltype(GeneratedStrategy::i_vol_exp) _pf_value_13;
        decltype(GeneratedStrategy::i_vol_con) _pf_value_14;
        decltype(GeneratedStrategy::adx) _pf_value_15;
        decltype(GeneratedStrategy::plus_di) _pf_value_16;
        decltype(GeneratedStrategy::minus_di) _pf_value_17;
        decltype(GeneratedStrategy::atr_val) _pf_value_18;
        decltype(GeneratedStrategy::atr_ma) _pf_value_19;
        decltype(GeneratedStrategy::vol_ratio) _pf_value_20;
        decltype(GeneratedStrategy::regime) _pf_value_21;
        decltype(GeneratedStrategy::long_entry) _pf_value_22;
        decltype(GeneratedStrategy::long_exit) _pf_value_23;
        decltype(GeneratedStrategy::_ta_initialized_) _pf_value_24;
        decltype(GeneratedStrategy::_inputs_initialized_) _pf_value_25;
    };
    static_assert(std::is_copy_constructible_v<_PFScriptState>, "generated Pine state must be deep-copy constructible");
    static_assert(std::is_copy_assignable_v<_PFScriptState>, "generated Pine state must be deep-copy assignable");
    std::optional<_PFScriptState> _pf_script_state_checkpoint_;

    void snapshot_script_state() override {
        _pf_script_state_checkpoint_.emplace(_PFScriptState{
            _ta_tr_1,
            _ta_rma_2,
            _ta_rma_3,
            _ta_rma_4,
            _ta_rma_5,
            _ta_atr_6,
            _ta_sma_7,
            _s_high,
            _s_low,
            trending_regime,
            i_adx_len,
            i_adx_trend,
            i_atr_len,
            i_vol_exp,
            i_vol_con,
            adx,
            plus_di,
            minus_di,
            atr_val,
            atr_ma,
            vol_ratio,
            regime,
            long_entry,
            long_exit,
            _ta_initialized_,
            _inputs_initialized_,
        });
    }

    void restore_script_state() override {
        if (!_pf_script_state_checkpoint_) return;
        this->_ta_tr_1 = _pf_script_state_checkpoint_->_pf_value_0;
        this->_ta_rma_2 = _pf_script_state_checkpoint_->_pf_value_1;
        this->_ta_rma_3 = _pf_script_state_checkpoint_->_pf_value_2;
        this->_ta_rma_4 = _pf_script_state_checkpoint_->_pf_value_3;
        this->_ta_rma_5 = _pf_script_state_checkpoint_->_pf_value_4;
        this->_ta_atr_6 = _pf_script_state_checkpoint_->_pf_value_5;
        this->_ta_sma_7 = _pf_script_state_checkpoint_->_pf_value_6;
        this->_s_high = _pf_script_state_checkpoint_->_pf_value_7;
        this->_s_low = _pf_script_state_checkpoint_->_pf_value_8;
        this->trending_regime = _pf_script_state_checkpoint_->_pf_value_9;
        this->i_adx_len = _pf_script_state_checkpoint_->_pf_value_10;
        this->i_adx_trend = _pf_script_state_checkpoint_->_pf_value_11;
        this->i_atr_len = _pf_script_state_checkpoint_->_pf_value_12;
        this->i_vol_exp = _pf_script_state_checkpoint_->_pf_value_13;
        this->i_vol_con = _pf_script_state_checkpoint_->_pf_value_14;
        this->adx = _pf_script_state_checkpoint_->_pf_value_15;
        this->plus_di = _pf_script_state_checkpoint_->_pf_value_16;
        this->minus_di = _pf_script_state_checkpoint_->_pf_value_17;
        this->atr_val = _pf_script_state_checkpoint_->_pf_value_18;
        this->atr_ma = _pf_script_state_checkpoint_->_pf_value_19;
        this->vol_ratio = _pf_script_state_checkpoint_->_pf_value_20;
        this->regime = _pf_script_state_checkpoint_->_pf_value_21;
        this->long_entry = _pf_script_state_checkpoint_->_pf_value_22;
        this->long_exit = _pf_script_state_checkpoint_->_pf_value_23;
        this->_ta_initialized_ = _pf_script_state_checkpoint_->_pf_value_24;
        this->_inputs_initialized_ = _pf_script_state_checkpoint_->_pf_value_25;
    }

    void commit_script_state() override {
        snapshot_script_state();
    }

    explicit GeneratedStrategy() : _ta_tr_1(true), _ta_rma_2(14), _ta_rma_3(14), _ta_rma_4(14), _ta_rma_5(14), _ta_atr_6(14), _ta_sma_7(42) {
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
        this->_ta_tr_1 = decltype(this->_ta_tr_1)(true);
        this->_ta_rma_2 = decltype(this->_ta_rma_2)(14);
        this->_ta_rma_3 = decltype(this->_ta_rma_3)(14);
        this->_ta_rma_4 = decltype(this->_ta_rma_4)(14);
        this->_ta_rma_5 = decltype(this->_ta_rma_5)(14);
        this->_ta_atr_6 = decltype(this->_ta_atr_6)(14);
        this->_precalc__ta_atr_6 = decltype(this->_precalc__ta_atr_6){};
        this->_ta_sma_7 = decltype(this->_ta_sma_7)(42);
        this->_use_precalc = false;
        this->_s_high = decltype(this->_s_high){};
        this->_s_low = decltype(this->_s_low){};
        this->trending_regime = decltype(this->trending_regime){};
        this->i_adx_len = 0;
        this->i_adx_trend = 0.0;
        this->i_atr_len = 0;
        this->i_vol_exp = 0.0;
        this->i_vol_con = 0.0;
        this->adx = 0.0;
        this->plus_di = 0.0;
        this->minus_di = 0.0;
        this->atr_val = 0.0;
        this->atr_ma = 0.0;
        this->vol_ratio = 0.0;
        this->regime = 0;
        this->long_entry = false;
        this->long_exit = false;
        this->_ta_initialized_ = false;
        this->_inputs_initialized_ = false;
        if (allow_precalculation) precalculate(bars, n);
    }

    std::tuple<double, double, double> f_adx_calc_cs0(int len) {
        double tr_val = (history_advances_new_bar() ? _ta_tr_1.compute(current_bar_.high, current_bar_.low, current_bar_.close, prev_chart_close()) : _ta_tr_1.recompute(current_bar_.high, current_bar_.low, current_bar_.close, prev_chart_close()));
        double plus_dm = ([&]() -> double { double _v0 = (double)((current_bar_.high - _s_high[1])); double _v1 = (double)(0); if (is_na(_v0) || is_na(_v1)) return na<double>(); double _out = _v0; _out = std::max(_out, _v1); return _out; }());
        double minus_dm = ([&]() -> double { double _v0 = (double)((_s_low[1] - current_bar_.low)); double _v1 = (double)(0); if (is_na(_v0) || is_na(_v1)) return na<double>(); double _out = _v0; _out = std::max(_out, _v1); return _out; }());
        if (([&]{ auto _pna_l = (plus_dm); auto _pna_r = (minus_dm); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) {
            minus_dm = 0;
        } else {
            plus_dm = 0;
        }
        double smooth_tr = (history_advances_new_bar() ? _ta_rma_2.compute(tr_val) : _ta_rma_2.recompute(tr_val));
        double smooth_plus = (history_advances_new_bar() ? _ta_rma_3.compute(plus_dm) : _ta_rma_3.recompute(plus_dm));
        double smooth_minus = (history_advances_new_bar() ? _ta_rma_4.compute(minus_dm) : _ta_rma_4.recompute(minus_dm));
        double plus_di = ((([&]{ auto _pna_l = (smooth_tr); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) ? (((double)((100 * smooth_plus)) / (double)(smooth_tr))) : (0));
        double minus_di = ((([&]{ auto _pna_l = (smooth_tr); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) ? (((double)((100 * smooth_minus)) / (double)(smooth_tr))) : (0));
        double di_sum = (plus_di + minus_di);
        double dx = ((([&]{ auto _pna_l = (di_sum); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) ? (((double)((100 * std::abs((plus_di - minus_di)))) / (double)(di_sum))) : (0));
        double adx_val = (history_advances_new_bar() ? _ta_rma_5.compute(dx) : _ta_rma_5.recompute(dx));
        return std::make_tuple(adx_val, plus_di, minus_di);
    }

    void on_source_bar(const Bar& bar) override {
        if (history_advances_new_bar()) _s_high.push(current_bar_.high);
        else _s_high.update(current_bar_.high);
        if (history_advances_new_bar()) _s_low.push(current_bar_.low);
        else _s_low.update(current_bar_.low);
        if (!_inputs_initialized_) {
            i_adx_len = get_input_int("ADX Period", 14);
            i_adx_trend = get_input_double("Trend Threshold", 25);
            i_atr_len = get_input_int("ATR Period", 14);
            i_vol_exp = get_input_double("Volatility Expansion", 1.4);
            i_vol_con = get_input_double("Volatility Contraction", 0.6);
            _inputs_initialized_ = true;
        }
        if (!_ta_initialized_) {
            _ta_rma_2 = ta::RMA(get_input_int("ADX Period", 14));
            _ta_rma_3 = ta::RMA(get_input_int("ADX Period", 14));
            _ta_rma_4 = ta::RMA(get_input_int("ADX Period", 14));
            _ta_rma_5 = ta::RMA(get_input_int("ADX Period", 14));
            _ta_atr_6 = ta::ATR(get_input_int("ATR Period", 14));
            _ta_sma_7 = ta::SMA((get_input_int("ATR Period", 14) * 3));
            _ta_initialized_ = true;
        }
        auto [adx, plus_di, minus_di] = f_adx_calc_cs0(i_adx_len);
        atr_val = (history_advances_new_bar() ? _ta_atr_6.compute(current_bar_.high, current_bar_.low, current_bar_.close, prev_chart_close()) : _ta_atr_6.recompute(current_bar_.high, current_bar_.low, current_bar_.close, prev_chart_close()));
        atr_ma = (history_advances_new_bar() ? _ta_sma_7.compute(atr_val) : _ta_sma_7.recompute(atr_val));
        vol_ratio = ((([&]{ auto _pna_l = (atr_ma); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) ? (((double)(atr_val) / (double)(atr_ma))) : (1.0));
        regime = 0;
        if ((([&]{ auto _pna_l = (vol_ratio); auto _pna_r = (i_vol_exp); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) || _pfc_eq); }()) && ([&]{ auto _pna_l = (adx); auto _pna_r = (i_adx_trend); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }()))) {
            regime = 3;
        } else
        if (([&]{ auto _pna_l = (adx); auto _pna_r = (i_adx_trend); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) || _pfc_eq); }())) {
            regime = 1;
        } else
        if (([&]{ auto _pna_l = (vol_ratio); auto _pna_r = (i_vol_con); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) || _pfc_eq); }())) {
            regime = 2;
        }
        if (history_advances_new_bar()) trending_regime.push(([&]{ auto _pna_l = (regime); auto _pna_r = (1); return !is_na(_pna_l) && !is_na(_pna_r) && (_pna_l == _pna_r); }()));
        else trending_regime.update(([&]{ auto _pna_l = (regime); auto _pna_r = (1); return !is_na(_pna_l) && !is_na(_pna_r) && (_pna_l == _pna_r); }()));
        long_entry = ((trending_regime[0] && !(trending_regime[1])) && ([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && (_pfc_eq); }()));
        long_exit = (!(trending_regime[0]) && ([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()));
        if (long_entry) {
            strategy_entry(std::string("L"), true, na<double>(), na<double>(), 1, std::string("trending ON"), "", 0, -1);
        }
        if (long_exit) {
            strategy_close(std::string("L"), std::string("trending OFF"), na<double>(), na<double>(), false, 373662154771ULL);
        }
    }

    void precalculate(const Bar* bars, int n) {
        _use_precalc = false;
        if (n <= 0 || bars == nullptr) return;

        _precalc__ta_atr_6.resize(n);

        _ta_atr_6 = ta::ATR(14);

        _s_high.clear();
        _s_low.clear();

        for (int i = 0; i < n; ++i) {
            _s_high.push(bars[i].high);
            _s_low.push(bars[i].low);
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
            _precalc__ta_atr_6[i] = _ta_atr_6.compute(bars[i].high, bars[i].low, bars[i].close, (i > 0 ? bars[i - 1].close : na<double>()));
        }

        _ta_atr_6 = ta::ATR(14);
        _s_high.clear();
        _s_low.clear();

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
