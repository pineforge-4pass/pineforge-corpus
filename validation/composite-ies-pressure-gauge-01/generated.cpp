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
    ta::EMA _ta_ema_1;
    ta::EMA _ta_ema_2;
    bool _use_precalc = false;
    Series<bool> pressure_bear;
    Series<bool> pressure_bull;
    Series<double> pressure_smooth;
    int i_pressure_len = 0;
    int i_pressure_smooth = 0;
    int i_pressure_mom = 0;
    double i_pressure_high = 0.0;
    double i_pressure_low = 0.0;
    double i_pressure_thresh = 0.0;
    double range_val = 0.0;
    double raw_buy = 0.0;
    double pressure_ratio = 0.0;
    double pressure_momentum = 0.0;
    int pressure_state = 0;
    bool long_entry = false;
    bool short_entry = false;
    bool _ta_initialized_ = false;
    bool _inputs_initialized_ = false;

    struct _PFScriptState {
        decltype(GeneratedStrategy::_ta_ema_1) _pf_value_0;
        decltype(GeneratedStrategy::_ta_ema_2) _pf_value_1;
        decltype(GeneratedStrategy::pressure_bear) _pf_value_2;
        decltype(GeneratedStrategy::pressure_bull) _pf_value_3;
        decltype(GeneratedStrategy::pressure_smooth) _pf_value_4;
        decltype(GeneratedStrategy::i_pressure_len) _pf_value_5;
        decltype(GeneratedStrategy::i_pressure_smooth) _pf_value_6;
        decltype(GeneratedStrategy::i_pressure_mom) _pf_value_7;
        decltype(GeneratedStrategy::i_pressure_high) _pf_value_8;
        decltype(GeneratedStrategy::i_pressure_low) _pf_value_9;
        decltype(GeneratedStrategy::i_pressure_thresh) _pf_value_10;
        decltype(GeneratedStrategy::range_val) _pf_value_11;
        decltype(GeneratedStrategy::raw_buy) _pf_value_12;
        decltype(GeneratedStrategy::pressure_ratio) _pf_value_13;
        decltype(GeneratedStrategy::pressure_momentum) _pf_value_14;
        decltype(GeneratedStrategy::pressure_state) _pf_value_15;
        decltype(GeneratedStrategy::long_entry) _pf_value_16;
        decltype(GeneratedStrategy::short_entry) _pf_value_17;
        decltype(GeneratedStrategy::_ta_initialized_) _pf_value_18;
        decltype(GeneratedStrategy::_inputs_initialized_) _pf_value_19;
    };
    static_assert(std::is_copy_constructible_v<_PFScriptState>, "generated Pine state must be deep-copy constructible");
    static_assert(std::is_copy_assignable_v<_PFScriptState>, "generated Pine state must be deep-copy assignable");
    std::optional<_PFScriptState> _pf_script_state_checkpoint_;

    void snapshot_script_state() override {
        _pf_script_state_checkpoint_.emplace(_PFScriptState{
            _ta_ema_1,
            _ta_ema_2,
            pressure_bear,
            pressure_bull,
            pressure_smooth,
            i_pressure_len,
            i_pressure_smooth,
            i_pressure_mom,
            i_pressure_high,
            i_pressure_low,
            i_pressure_thresh,
            range_val,
            raw_buy,
            pressure_ratio,
            pressure_momentum,
            pressure_state,
            long_entry,
            short_entry,
            _ta_initialized_,
            _inputs_initialized_,
        });
    }

    void restore_script_state() override {
        if (!_pf_script_state_checkpoint_) return;
        this->_ta_ema_1 = _pf_script_state_checkpoint_->_pf_value_0;
        this->_ta_ema_2 = _pf_script_state_checkpoint_->_pf_value_1;
        this->pressure_bear = _pf_script_state_checkpoint_->_pf_value_2;
        this->pressure_bull = _pf_script_state_checkpoint_->_pf_value_3;
        this->pressure_smooth = _pf_script_state_checkpoint_->_pf_value_4;
        this->i_pressure_len = _pf_script_state_checkpoint_->_pf_value_5;
        this->i_pressure_smooth = _pf_script_state_checkpoint_->_pf_value_6;
        this->i_pressure_mom = _pf_script_state_checkpoint_->_pf_value_7;
        this->i_pressure_high = _pf_script_state_checkpoint_->_pf_value_8;
        this->i_pressure_low = _pf_script_state_checkpoint_->_pf_value_9;
        this->i_pressure_thresh = _pf_script_state_checkpoint_->_pf_value_10;
        this->range_val = _pf_script_state_checkpoint_->_pf_value_11;
        this->raw_buy = _pf_script_state_checkpoint_->_pf_value_12;
        this->pressure_ratio = _pf_script_state_checkpoint_->_pf_value_13;
        this->pressure_momentum = _pf_script_state_checkpoint_->_pf_value_14;
        this->pressure_state = _pf_script_state_checkpoint_->_pf_value_15;
        this->long_entry = _pf_script_state_checkpoint_->_pf_value_16;
        this->short_entry = _pf_script_state_checkpoint_->_pf_value_17;
        this->_ta_initialized_ = _pf_script_state_checkpoint_->_pf_value_18;
        this->_inputs_initialized_ = _pf_script_state_checkpoint_->_pf_value_19;
    }

    void commit_script_state() override {
        snapshot_script_state();
    }

    explicit GeneratedStrategy() : _ta_ema_1(14), _ta_ema_2(5) {
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
        this->_ta_ema_1 = decltype(this->_ta_ema_1)(14);
        this->_ta_ema_2 = decltype(this->_ta_ema_2)(5);
        this->_use_precalc = false;
        this->pressure_bear = decltype(this->pressure_bear){};
        this->pressure_bull = decltype(this->pressure_bull){};
        this->pressure_smooth = decltype(this->pressure_smooth){};
        this->i_pressure_len = 0;
        this->i_pressure_smooth = 0;
        this->i_pressure_mom = 0;
        this->i_pressure_high = 0.0;
        this->i_pressure_low = 0.0;
        this->i_pressure_thresh = 0.0;
        this->range_val = 0.0;
        this->raw_buy = 0.0;
        this->pressure_ratio = 0.0;
        this->pressure_momentum = 0.0;
        this->pressure_state = 0;
        this->long_entry = false;
        this->short_entry = false;
        this->_ta_initialized_ = false;
        this->_inputs_initialized_ = false;
        (void)bars; (void)n; (void)allow_precalculation;
    }

    void on_source_bar(const Bar& bar) override {
        if (!_inputs_initialized_) {
            i_pressure_len = get_input_int("Pressure Period", 14);
            i_pressure_smooth = get_input_int("Pressure Smoothing", 5);
            i_pressure_mom = get_input_int("Pressure Momentum", 10);
            i_pressure_high = get_input_double("Extreme Buy Level", 0.7);
            i_pressure_low = get_input_double("Extreme Sell Level", 0.3);
            i_pressure_thresh = get_input_double("Momentum Threshold", 0.05);
            _inputs_initialized_ = true;
        }
        if (!_ta_initialized_) {
            _ta_ema_1 = ta::EMA(get_input_int("Pressure Period", 14));
            _ta_ema_2 = ta::EMA(get_input_int("Pressure Smoothing", 5));
            _ta_initialized_ = true;
        }
        range_val = (current_bar_.high - current_bar_.low);
        raw_buy = ((([&]{ auto _pna_l = (range_val); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) ? (((double)((current_bar_.close - current_bar_.low)) / (double)(range_val))) : (0.5));
        pressure_ratio = (history_advances_new_bar() ? _ta_ema_1.compute(raw_buy) : _ta_ema_1.recompute(raw_buy));
        if (history_advances_new_bar()) pressure_smooth.push((history_advances_new_bar() ? _ta_ema_2.compute(pressure_ratio) : _ta_ema_2.recompute(pressure_ratio)));
        else pressure_smooth.update((history_advances_new_bar() ? _ta_ema_2.compute(pressure_ratio) : _ta_ema_2.recompute(pressure_ratio)));
        pressure_momentum = (pressure_smooth[0] - pressure_smooth[i_pressure_mom]);
        pressure_state = 0;
        if (([&]{ auto _pna_l = (pressure_smooth[0]); auto _pna_r = (i_pressure_high); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) || _pfc_eq); }())) {
            pressure_state = 2;
        } else
        if (([&]{ auto _pna_l = (pressure_smooth[0]); auto _pna_r = ((0.5 + i_pressure_thresh)); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) {
            pressure_state = 1;
        } else
        if (([&]{ auto _pna_l = (pressure_smooth[0]); auto _pna_r = (i_pressure_low); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) || _pfc_eq); }())) {
            pressure_state = (-2);
        } else
        if (([&]{ auto _pna_l = (pressure_smooth[0]); auto _pna_r = ((0.5 - i_pressure_thresh)); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }())) {
            pressure_state = (-1);
        }
        if (history_advances_new_bar()) pressure_bull.push((([&]{ auto _pna_l = (pressure_state); auto _pna_r = (1); return !is_na(_pna_l) && !is_na(_pna_r) && (_pna_l >= _pna_r); }()) || ([&]{ auto _pna_l = (pressure_momentum); auto _pna_r = (i_pressure_thresh); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())));
        else pressure_bull.update((([&]{ auto _pna_l = (pressure_state); auto _pna_r = (1); return !is_na(_pna_l) && !is_na(_pna_r) && (_pna_l >= _pna_r); }()) || ([&]{ auto _pna_l = (pressure_momentum); auto _pna_r = (i_pressure_thresh); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())));
        if (history_advances_new_bar()) pressure_bear.push((([&]{ auto _pna_l = (pressure_state); auto _pna_r = ((-1)); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) || _pfc_eq); }()) || ([&]{ auto _pna_l = (pressure_momentum); auto _pna_r = ((-i_pressure_thresh)); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }())));
        else pressure_bear.update((([&]{ auto _pna_l = (pressure_state); auto _pna_r = ((-1)); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) || _pfc_eq); }()) || ([&]{ auto _pna_l = (pressure_momentum); auto _pna_r = ((-i_pressure_thresh)); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }())));
        long_entry = ((pressure_bull[0] && !(pressure_bull[1])) && ([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) || _pfc_eq); }()));
        short_entry = ((pressure_bear[0] && !(pressure_bear[1])) && ([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) || _pfc_eq); }()));
        if (long_entry) {
            if (([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }())) {
                strategy_close(std::string("S"), std::string("flip"), na<double>(), na<double>(), false, 313532612631ULL);
            }
            strategy_entry(std::string("L"), true, na<double>(), na<double>(), 1, std::string("pressure bull"), "", 0, -1);
        }
        if (short_entry) {
            if (([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) {
                strategy_close(std::string("L"), std::string("flip"), na<double>(), na<double>(), false, 335007449111ULL);
            }
            strategy_entry(std::string("S"), false, na<double>(), na<double>(), 1, std::string("pressure bear"), "", 0, -1);
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
