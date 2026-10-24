#include <pineforge/source/pine_strategy_host.hpp>
#include <pineforge/ta.hpp>
#if __has_include(<pineforge/checked_settings.hpp>)
#include <pineforge/checked_settings.hpp>
#endif
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


#ifdef PINEFORGE_HAS_RUN_FAILURE_CODES_V1
#define _PF_NO_DATA_STOP(function, call, line, english) ::pineforge::pine_no_data_stop(function, call, line, english)
#define _PF_OTHER_SYMBOL_STOP(function, symbol, call, line, english) ::pineforge::pine_other_symbol_stop(function, symbol, call, line, english)
#define _PF_ARRAY_STOP(reason, method, english) ::pineforge::pine_array_stop(reason, method, std::string(english).c_str())
#define _PF_COLLECTION_STOP(reason, object, english) ::pineforge::pine_collection_stop(object, reason, english)
#define _PF_NA_STOP(object, english) ::pineforge::pine_na_stop(object, english)
#define _PF_LIMIT_STOP(limit, max, english) ::pineforge::pine_limit_stop(limit, max, english)
#define _PF_UNSUPPORTED_STOP(reason, line, english) ::pineforge::pine_unsupported_stop(reason, line, english)
#define _PF_STRING_STOP(reason, english) ::pineforge::pine_string_stop(reason, english)
#define _PF_ENGINE_INVARIANT(english, legacy_type) ::pineforge::pine_engine_invariant(english)
#define _PF_INVARIANT_AT(container, index) _pf_invariant_at(container, index)
#define _PF_SETTING_FAILURE(strategy, entrypoint, message, reason) (strategy)->_pf_record_setting_failure(entrypoint, message, [] { return reason ? ::pineforge::RunFailureInfo(::pineforge::RunFailureCode::setting_rejected, {{"entrypoint", entrypoint}, {"reason", reason}}) : ::pineforge::RunFailureInfo(::pineforge::RunFailureCode::setting_rejected, {{"entrypoint", entrypoint}}); })
template <typename Container, typename Index>
decltype(auto) _pf_invariant_at(Container& container, Index index) {
    try { return container.at(index); }
    catch (const std::out_of_range& error) { ::pineforge::pine_engine_invariant(error.what()); }
}
#else
#define _PF_NO_DATA_STOP(function, call, line, english) pine_runtime_error(std::string(english))
#define _PF_OTHER_SYMBOL_STOP(function, symbol, call, line, english) pine_runtime_error(std::string(english))
#define _PF_ARRAY_STOP(reason, method, english) pine_runtime_error(english)
#define _PF_COLLECTION_STOP(reason, object, english) pine_runtime_error(english)
#define _PF_NA_STOP(object, english) throw std::runtime_error(english)
#define _PF_LIMIT_STOP(limit, max, english) throw std::length_error(english)
#define _PF_UNSUPPORTED_STOP(reason, line, english) pine_runtime_error(std::string(english))
#define _PF_STRING_STOP(reason, english) pine_runtime_error(std::string(english))
#define _PF_ENGINE_INVARIANT(english, legacy_type) throw legacy_type(english)
#define _PF_INVARIANT_AT(container, index) (container).at(index)
#define _PF_SETTING_FAILURE(strategy, entrypoint, message, reason) (strategy)->_pf_record_setting_failure(entrypoint, message)
#endif

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
    double _req_sec_0 = na<double>();
    double _req_sec_1 = na<double>();
    double _req_sec_2 = na<double>();
    std::unordered_map<std::string, Series<double>> _security_helper_series_;
    ta::EMA _ta_ema_1;
    ta::EMA _ta_ema_2;
    ta::EMA _ta_ema_3;
    ta::EMA _ta_ema_1_cs1;
    ta::EMA _ta_ema_2_cs1;
    ta::EMA _ta_ema_3_cs1;
    ta::EMA _ta_ema_1_cs2;
    ta::EMA _ta_ema_2_cs2;
    ta::EMA _ta_ema_3_cs2;
    bool _use_precalc = false;
    ta::EMA _sec0__ta_ema_1_v0;
    ta::EMA _sec0__ta_ema_1_v1;
    ta::EMA _sec0__ta_ema_1_v2;
    ta::EMA _sec0__ta_ema_2_v0;
    ta::EMA _sec0__ta_ema_2_v1;
    ta::EMA _sec0__ta_ema_2_v2;
    ta::EMA _sec0__ta_ema_3_v0;
    ta::EMA _sec0__ta_ema_3_v1;
    ta::EMA _sec1__ta_ema_1_v0;
    ta::EMA _sec1__ta_ema_1_v1;
    ta::EMA _sec1__ta_ema_1_v2;
    ta::EMA _sec1__ta_ema_2_v0;
    ta::EMA _sec1__ta_ema_2_v1;
    ta::EMA _sec1__ta_ema_2_v2;
    ta::EMA _sec1__ta_ema_3_v0;
    ta::EMA _sec1__ta_ema_3_v1;
    ta::EMA _sec2__ta_ema_1_v0;
    ta::EMA _sec2__ta_ema_1_v1;
    ta::EMA _sec2__ta_ema_1_v2;
    ta::EMA _sec2__ta_ema_2_v0;
    ta::EMA _sec2__ta_ema_2_v1;
    ta::EMA _sec2__ta_ema_2_v2;
    ta::EMA _sec2__ta_ema_3_v0;
    ta::EMA _sec2__ta_ema_3_v1;
    double h1 = 0.0;
    double h2 = 0.0;
    double h3 = 0.0;
    bool allBull = false;
    bool allBear = false;
    bool _ta_initialized_ = false;
    bool _inputs_initialized_ = false;

    struct _PFScriptState {
        decltype(GeneratedStrategy::_req_sec_0) _pf_value_0;
        decltype(GeneratedStrategy::_req_sec_1) _pf_value_1;
        decltype(GeneratedStrategy::_req_sec_2) _pf_value_2;
        decltype(GeneratedStrategy::_security_helper_series_) _pf_value_3;
        decltype(GeneratedStrategy::_ta_ema_1) _pf_value_4;
        decltype(GeneratedStrategy::_ta_ema_2) _pf_value_5;
        decltype(GeneratedStrategy::_ta_ema_3) _pf_value_6;
        decltype(GeneratedStrategy::_ta_ema_1_cs1) _pf_value_7;
        decltype(GeneratedStrategy::_ta_ema_2_cs1) _pf_value_8;
        decltype(GeneratedStrategy::_ta_ema_3_cs1) _pf_value_9;
        decltype(GeneratedStrategy::_ta_ema_1_cs2) _pf_value_10;
        decltype(GeneratedStrategy::_ta_ema_2_cs2) _pf_value_11;
        decltype(GeneratedStrategy::_ta_ema_3_cs2) _pf_value_12;
        decltype(GeneratedStrategy::_sec0__ta_ema_1_v0) _pf_value_13;
        decltype(GeneratedStrategy::_sec0__ta_ema_1_v1) _pf_value_14;
        decltype(GeneratedStrategy::_sec0__ta_ema_1_v2) _pf_value_15;
        decltype(GeneratedStrategy::_sec0__ta_ema_2_v0) _pf_value_16;
        decltype(GeneratedStrategy::_sec0__ta_ema_2_v1) _pf_value_17;
        decltype(GeneratedStrategy::_sec0__ta_ema_2_v2) _pf_value_18;
        decltype(GeneratedStrategy::_sec0__ta_ema_3_v0) _pf_value_19;
        decltype(GeneratedStrategy::_sec0__ta_ema_3_v1) _pf_value_20;
        decltype(GeneratedStrategy::_sec1__ta_ema_1_v0) _pf_value_21;
        decltype(GeneratedStrategy::_sec1__ta_ema_1_v1) _pf_value_22;
        decltype(GeneratedStrategy::_sec1__ta_ema_1_v2) _pf_value_23;
        decltype(GeneratedStrategy::_sec1__ta_ema_2_v0) _pf_value_24;
        decltype(GeneratedStrategy::_sec1__ta_ema_2_v1) _pf_value_25;
        decltype(GeneratedStrategy::_sec1__ta_ema_2_v2) _pf_value_26;
        decltype(GeneratedStrategy::_sec1__ta_ema_3_v0) _pf_value_27;
        decltype(GeneratedStrategy::_sec1__ta_ema_3_v1) _pf_value_28;
        decltype(GeneratedStrategy::_sec2__ta_ema_1_v0) _pf_value_29;
        decltype(GeneratedStrategy::_sec2__ta_ema_1_v1) _pf_value_30;
        decltype(GeneratedStrategy::_sec2__ta_ema_1_v2) _pf_value_31;
        decltype(GeneratedStrategy::_sec2__ta_ema_2_v0) _pf_value_32;
        decltype(GeneratedStrategy::_sec2__ta_ema_2_v1) _pf_value_33;
        decltype(GeneratedStrategy::_sec2__ta_ema_2_v2) _pf_value_34;
        decltype(GeneratedStrategy::_sec2__ta_ema_3_v0) _pf_value_35;
        decltype(GeneratedStrategy::_sec2__ta_ema_3_v1) _pf_value_36;
        decltype(GeneratedStrategy::h1) _pf_value_37;
        decltype(GeneratedStrategy::h2) _pf_value_38;
        decltype(GeneratedStrategy::h3) _pf_value_39;
        decltype(GeneratedStrategy::allBull) _pf_value_40;
        decltype(GeneratedStrategy::allBear) _pf_value_41;
        decltype(GeneratedStrategy::_ta_initialized_) _pf_value_42;
        decltype(GeneratedStrategy::_inputs_initialized_) _pf_value_43;
    };
    static_assert(std::is_copy_constructible_v<_PFScriptState>, "generated Pine state must be deep-copy constructible");
    static_assert(std::is_copy_assignable_v<_PFScriptState>, "generated Pine state must be deep-copy assignable");
    std::optional<_PFScriptState> _pf_script_state_checkpoint_;

    void snapshot_script_state() override {
        _pf_script_state_checkpoint_.emplace(_PFScriptState{
            _req_sec_0,
            _req_sec_1,
            _req_sec_2,
            _security_helper_series_,
            _ta_ema_1,
            _ta_ema_2,
            _ta_ema_3,
            _ta_ema_1_cs1,
            _ta_ema_2_cs1,
            _ta_ema_3_cs1,
            _ta_ema_1_cs2,
            _ta_ema_2_cs2,
            _ta_ema_3_cs2,
            _sec0__ta_ema_1_v0,
            _sec0__ta_ema_1_v1,
            _sec0__ta_ema_1_v2,
            _sec0__ta_ema_2_v0,
            _sec0__ta_ema_2_v1,
            _sec0__ta_ema_2_v2,
            _sec0__ta_ema_3_v0,
            _sec0__ta_ema_3_v1,
            _sec1__ta_ema_1_v0,
            _sec1__ta_ema_1_v1,
            _sec1__ta_ema_1_v2,
            _sec1__ta_ema_2_v0,
            _sec1__ta_ema_2_v1,
            _sec1__ta_ema_2_v2,
            _sec1__ta_ema_3_v0,
            _sec1__ta_ema_3_v1,
            _sec2__ta_ema_1_v0,
            _sec2__ta_ema_1_v1,
            _sec2__ta_ema_1_v2,
            _sec2__ta_ema_2_v0,
            _sec2__ta_ema_2_v1,
            _sec2__ta_ema_2_v2,
            _sec2__ta_ema_3_v0,
            _sec2__ta_ema_3_v1,
            h1,
            h2,
            h3,
            allBull,
            allBear,
            _ta_initialized_,
            _inputs_initialized_,
        });
    }

    void restore_script_state() override {
        if (!_pf_script_state_checkpoint_) return;
        this->_req_sec_0 = _pf_script_state_checkpoint_->_pf_value_0;
        this->_req_sec_1 = _pf_script_state_checkpoint_->_pf_value_1;
        this->_req_sec_2 = _pf_script_state_checkpoint_->_pf_value_2;
        this->_security_helper_series_ = _pf_script_state_checkpoint_->_pf_value_3;
        this->_ta_ema_1 = _pf_script_state_checkpoint_->_pf_value_4;
        this->_ta_ema_2 = _pf_script_state_checkpoint_->_pf_value_5;
        this->_ta_ema_3 = _pf_script_state_checkpoint_->_pf_value_6;
        this->_ta_ema_1_cs1 = _pf_script_state_checkpoint_->_pf_value_7;
        this->_ta_ema_2_cs1 = _pf_script_state_checkpoint_->_pf_value_8;
        this->_ta_ema_3_cs1 = _pf_script_state_checkpoint_->_pf_value_9;
        this->_ta_ema_1_cs2 = _pf_script_state_checkpoint_->_pf_value_10;
        this->_ta_ema_2_cs2 = _pf_script_state_checkpoint_->_pf_value_11;
        this->_ta_ema_3_cs2 = _pf_script_state_checkpoint_->_pf_value_12;
        this->_sec0__ta_ema_1_v0 = _pf_script_state_checkpoint_->_pf_value_13;
        this->_sec0__ta_ema_1_v1 = _pf_script_state_checkpoint_->_pf_value_14;
        this->_sec0__ta_ema_1_v2 = _pf_script_state_checkpoint_->_pf_value_15;
        this->_sec0__ta_ema_2_v0 = _pf_script_state_checkpoint_->_pf_value_16;
        this->_sec0__ta_ema_2_v1 = _pf_script_state_checkpoint_->_pf_value_17;
        this->_sec0__ta_ema_2_v2 = _pf_script_state_checkpoint_->_pf_value_18;
        this->_sec0__ta_ema_3_v0 = _pf_script_state_checkpoint_->_pf_value_19;
        this->_sec0__ta_ema_3_v1 = _pf_script_state_checkpoint_->_pf_value_20;
        this->_sec1__ta_ema_1_v0 = _pf_script_state_checkpoint_->_pf_value_21;
        this->_sec1__ta_ema_1_v1 = _pf_script_state_checkpoint_->_pf_value_22;
        this->_sec1__ta_ema_1_v2 = _pf_script_state_checkpoint_->_pf_value_23;
        this->_sec1__ta_ema_2_v0 = _pf_script_state_checkpoint_->_pf_value_24;
        this->_sec1__ta_ema_2_v1 = _pf_script_state_checkpoint_->_pf_value_25;
        this->_sec1__ta_ema_2_v2 = _pf_script_state_checkpoint_->_pf_value_26;
        this->_sec1__ta_ema_3_v0 = _pf_script_state_checkpoint_->_pf_value_27;
        this->_sec1__ta_ema_3_v1 = _pf_script_state_checkpoint_->_pf_value_28;
        this->_sec2__ta_ema_1_v0 = _pf_script_state_checkpoint_->_pf_value_29;
        this->_sec2__ta_ema_1_v1 = _pf_script_state_checkpoint_->_pf_value_30;
        this->_sec2__ta_ema_1_v2 = _pf_script_state_checkpoint_->_pf_value_31;
        this->_sec2__ta_ema_2_v0 = _pf_script_state_checkpoint_->_pf_value_32;
        this->_sec2__ta_ema_2_v1 = _pf_script_state_checkpoint_->_pf_value_33;
        this->_sec2__ta_ema_2_v2 = _pf_script_state_checkpoint_->_pf_value_34;
        this->_sec2__ta_ema_3_v0 = _pf_script_state_checkpoint_->_pf_value_35;
        this->_sec2__ta_ema_3_v1 = _pf_script_state_checkpoint_->_pf_value_36;
        this->h1 = _pf_script_state_checkpoint_->_pf_value_37;
        this->h2 = _pf_script_state_checkpoint_->_pf_value_38;
        this->h3 = _pf_script_state_checkpoint_->_pf_value_39;
        this->allBull = _pf_script_state_checkpoint_->_pf_value_40;
        this->allBear = _pf_script_state_checkpoint_->_pf_value_41;
        this->_ta_initialized_ = _pf_script_state_checkpoint_->_pf_value_42;
        this->_inputs_initialized_ = _pf_script_state_checkpoint_->_pf_value_43;
    }

    void commit_script_state() override {
        snapshot_script_state();
    }

    explicit GeneratedStrategy() : _ta_ema_1(12), _ta_ema_2(26), _ta_ema_3(9), _ta_ema_1_cs1(12), _ta_ema_2_cs1(26), _ta_ema_3_cs1(9), _ta_ema_1_cs2(12), _ta_ema_2_cs2(26), _ta_ema_3_cs2(9), _sec0__ta_ema_1_v0(12), _sec0__ta_ema_1_v1(12), _sec0__ta_ema_1_v2(12), _sec0__ta_ema_2_v0(26), _sec0__ta_ema_2_v1(26), _sec0__ta_ema_2_v2(26), _sec0__ta_ema_3_v0(9), _sec0__ta_ema_3_v1(9), _sec1__ta_ema_1_v0(12), _sec1__ta_ema_1_v1(12), _sec1__ta_ema_1_v2(12), _sec1__ta_ema_2_v0(26), _sec1__ta_ema_2_v1(26), _sec1__ta_ema_2_v2(26), _sec1__ta_ema_3_v0(9), _sec1__ta_ema_3_v1(9), _sec2__ta_ema_1_v0(12), _sec2__ta_ema_1_v1(12), _sec2__ta_ema_1_v2(12), _sec2__ta_ema_2_v0(26), _sec2__ta_ema_2_v1(26), _sec2__ta_ema_2_v2(26), _sec2__ta_ema_3_v0(9), _sec2__ta_ema_3_v1(9) {
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
#if defined(PINEFORGE_HAS_TRADE_HISTORY_POLICY_V1)
        this->set_trade_history_streamable(true);
#endif
        this->configure_pine_strategy(cfg);
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
    bool _pf_setting_failed_ = false;
    std::string _pf_setting_failure_;
#ifdef PINEFORGE_HAS_RUN_FAILURE_CODES_V1
    ::pineforge::RunFailureInfo _pf_setting_failure_info_{::pineforge::RunFailureCode::none, {}};
#ifdef PF_SETTINGS_API_VERSION
    using _pf_setting_error_base = ::pineforge::checked_settings::LatchedSettingsFailure;
#else
    using _pf_setting_error_base = std::runtime_error;
#endif
    struct _pf_latched_setting_error : _pf_setting_error_base, ::pineforge::RunFailureInfo {
        _pf_latched_setting_error(const std::string& text, const ::pineforge::RunFailureInfo& info)
            : _pf_setting_error_base(text), ::pineforge::RunFailureInfo(info) {}
    };
#endif
    void _pf_record_failure(const char* entrypoint, const char* message) noexcept {
#ifdef PINEFORGE_HAS_RUN_FAILURE_CODES_V1
        try { ::pineforge::note_run_failure_unknown(*this, entrypoint); } catch (...) {}
#else
        try { last_error_ = entrypoint; last_error_ += ": "; last_error_ += message; } catch (...) {}
#endif
    }
    void _pf_record_failure(const char* entrypoint, const std::exception& error) noexcept {
#ifdef PINEFORGE_HAS_RUN_FAILURE_CODES_V1
        try { ::pineforge::note_run_failure(*this, entrypoint, error); } catch (...) {}
#else
        _pf_record_failure(entrypoint, error.what());
#endif
    }
#ifdef PINEFORGE_HAS_RUN_FAILURE_CODES_V1
    template <typename FailureInfo>
    void _pf_record_setting_failure(const char* entrypoint, const char* message, FailureInfo failure_info) noexcept {
#else
    void _pf_record_setting_failure(const char* entrypoint, const char* message) noexcept {
#endif
        if (!_pf_setting_failed_) {
            _pf_setting_failed_ = true;
            try { _pf_setting_failure_ = entrypoint; _pf_setting_failure_ += ": "; _pf_setting_failure_ += message; } catch (...) {}
#ifdef PINEFORGE_HAS_RUN_FAILURE_CODES_V1
            try { _pf_setting_failure_info_ = failure_info(); }
            catch (...) { _pf_setting_failure_info_ = ::pineforge::RunFailureInfo(::pineforge::RunFailureCode::out_of_memory, {}); }
#endif
        }
        _pf_refuse_failed_setting(nullptr);
    }
    void _pf_require_settings_ok() const {
#ifdef PINEFORGE_HAS_RUN_FAILURE_CODES_V1
        if (_pf_setting_failed_) throw _pf_latched_setting_error(_pf_setting_failure_.empty() ? "legacy strategy setter failed" : _pf_setting_failure_, _pf_setting_failure_info_);
#elif defined(PF_SETTINGS_API_VERSION)
        if (_pf_setting_failed_) throw ::pineforge::checked_settings::LatchedSettingsFailure(_pf_setting_failure_.empty() ? "legacy strategy setter failed" : _pf_setting_failure_);
#else
        if (_pf_setting_failed_) throw std::runtime_error(_pf_setting_failure_.empty() ? "legacy strategy setter failed" : _pf_setting_failure_);
#endif
    }
    bool _pf_refuse_failed_setting(ReportC* out) noexcept {
        if (!_pf_setting_failed_) return false;
#ifdef PINEFORGE_HAS_RUN_FAILURE_CODES_V1
        try { ::pineforge::note_run_failure(*this, _pf_setting_failure_.empty() ? "legacy strategy setter failed" : _pf_setting_failure_, _pf_setting_failure_info_.run_failure()); } catch (...) {}
#else
        try { last_error_ = _pf_setting_failure_.empty() ? "legacy strategy setter failed" : _pf_setting_failure_; } catch (...) {}
#endif
        if (out) *out = ReportC{};
        return true;
    }
#ifdef PF_SETTINGS_API_VERSION
    static pineforge::source::PineStrategyConfig _pf_settings_declared_config() {
        pineforge::source::PineStrategyConfig cfg{};
        cfg.initial_capital = 1000000.0;
        cfg.default_qty_type = static_cast<int>(QtyType::FIXED);
        cfg.default_qty_value = 1.0;
        cfg.pyramiding = 1;
        cfg.commission_type = static_cast<int>(CommissionType::PERCENT);
        cfg.commission_value = 0.0;
        cfg.slippage = 0;
        return cfg;
    }
    std::vector<::pineforge::checked_settings::Setting> _pf_settings_inputs() const {
        return {
        };
    }
    std::vector<::pineforge::checked_settings::Setting> _pf_settings_overrides() const {
        const double _pf_nan = std::numeric_limits<double>::quiet_NaN();
        const auto _pf_defaults = _pf_settings_declared_config();
        return {
            {"initial_capital", "float", ::pineforge::checked_settings::number(_pf_defaults.initial_capital), {}, 0.0},
            {"commission_value", "float", ::pineforge::checked_settings::number(_pf_defaults.commission_value), {}, 0.0},
            {"default_qty_value", "float", ::pineforge::checked_settings::number(_pf_defaults.default_qty_value), {}, 0.0},
            {"pyramiding", "int", ::pineforge::checked_settings::number(_pf_defaults.pyramiding), {}, 0.0},
            {"slippage", "int", ::pineforge::checked_settings::number(_pf_defaults.slippage), {}, 0.0},
            {"process_orders_on_close", "bool", ::pineforge::checked_settings::number(_pf_defaults.process_orders_on_close), {}, _pf_nan},
            {"calc_on_order_fills", "bool", ::pineforge::checked_settings::number(_pf_defaults.calc_on_order_fills), {}, _pf_nan},
            {"close_entries_rule", "string", _pf_close_entries_rule_word(_pf_defaults.close_entries_rule_any), {"FIFO", "ANY"}, _pf_nan},
            {"default_qty_type", "string", _pf_default_qty_type_word(_pf_defaults.default_qty_type), {"fixed", "percent_of_equity", "cash"}, _pf_nan},
            {"commission_type", "string", _pf_commission_type_word(_pf_defaults.commission_type), {"percent", "cash_per_order", "cash_per_contract"}, _pf_nan},
        };
    }
    static std::string _pf_close_entries_rule_word(int _pf_value) {
        if (_pf_value == 0) return "FIFO";
        if (_pf_value == 1) return "ANY";
        return "invalid";
    }
    static std::string _pf_default_qty_type_word(int _pf_value) {
        if (_pf_value == 0) return "fixed";
        if (_pf_value == 1) return "percent_of_equity";
        if (_pf_value == 2) return "cash";
        return "invalid";
    }
    static std::string _pf_commission_type_word(int _pf_value) {
        if (_pf_value == 0) return "percent";
        if (_pf_value == 1) return "cash_per_order";
        if (_pf_value == 2) return "cash_per_contract";
        return "invalid";
    }
    void _pf_set_input_checked(const std::string& _pf_key, const std::string& _pf_value) {
        if (_pf_refuse_failed_setting(nullptr)) throw ::pineforge::checked_settings::Error{PF_SETTINGS_RUN_FAILED, last_error_.c_str()};
        ::pineforge::checked_settings::require(script_bars_processed() == 0 && stream_phase_ == StreamPhase::IDLE, "settings are frozen after execution begins", PF_SETTINGS_UNSUPPORTED);
        const auto _pf_inputs = _pf_settings_inputs();
        const ::pineforge::checked_settings::Setting* _pf_match = nullptr;
        for (const auto& _pf_input : _pf_inputs) {
            if (_pf_input.name != _pf_key) continue;
            ::pineforge::checked_settings::require(_pf_match == nullptr, "ambiguous input key", PF_SETTINGS_UNSUPPORTED);
            _pf_match = &_pf_input;
        }
        ::pineforge::checked_settings::require(_pf_match != nullptr, "unknown input key");
        const auto _pf_canonical = ::pineforge::checked_settings::validate(*_pf_match, _pf_value);
        set_input(_pf_key, _pf_canonical);
        ::pineforge::checked_settings::require(inputs_.count(_pf_key) && inputs_.at(_pf_key) == _pf_canonical, "input was not installed", PF_SETTINGS_UNSUPPORTED);
    }
    void _pf_set_override_checked(const std::string& _pf_key, const std::string& _pf_value) {
        if (_pf_refuse_failed_setting(nullptr)) throw ::pineforge::checked_settings::Error{PF_SETTINGS_RUN_FAILED, last_error_.c_str()};
        ::pineforge::checked_settings::require(script_bars_processed() == 0 && stream_phase_ == StreamPhase::IDLE, "settings are frozen after execution begins", PF_SETTINGS_UNSUPPORTED);
        for (const auto& _pf_override : _pf_settings_overrides()) {
            if (_pf_override.name != _pf_key) continue;
            auto _pf_alias = _pf_value;
            if (_pf_key == "close_entries_rule") {
                if (_pf_value == "0" || _pf_value == "fifo") _pf_alias = "FIFO";
                if (_pf_value == "1" || _pf_value == "any") _pf_alias = "ANY";
            }
            if (_pf_key == "default_qty_type") {
                if (_pf_value == "0" || _pf_value == "strategy.fixed") _pf_alias = "fixed";
                if (_pf_value == "1" || _pf_value == "strategy.percent_of_equity") _pf_alias = "percent_of_equity";
                if (_pf_value == "2" || _pf_value == "strategy.cash") _pf_alias = "cash";
            }
            if (_pf_key == "commission_type") {
                if (_pf_value == "0" || _pf_value == "strategy.commission.percent") _pf_alias = "percent";
                if (_pf_value == "1" || _pf_value == "strategy.commission.cash_per_order") _pf_alias = "cash_per_order";
                if (_pf_value == "2" || _pf_value == "strategy.commission.cash_per_contract") _pf_alias = "cash_per_contract";
            }
            const auto _pf_canonical = ::pineforge::checked_settings::validate(_pf_override, _pf_alias);
            set_strategy_override(_pf_key, _pf_canonical);
            return;
        }
        throw ::pineforge::checked_settings::Error{PF_SETTINGS_INVALID_ARGUMENT, "unknown override key"};
    }
    std::string _pf_settings_receipt() const {
        ::pineforge::checked_settings::require(!_pf_setting_failed_, _pf_setting_failure_.empty() ? "legacy strategy setter failed" : _pf_setting_failure_.c_str(), PF_SETTINGS_RUN_FAILED);
        std::string _pf_document = "{\"version\":1,\"inputs\":[";
        const auto _pf_inputs = _pf_settings_inputs();
        _pf_document += "],\"overrides\":[";
        const auto _pf_overrides = _pf_settings_overrides();
        _pf_document += ::pineforge::checked_settings::describe(_pf_overrides[0], ::pineforge::checked_settings::number((std::isnan(override_.initial_capital) ? config_.initial_capital : override_.initial_capital)));
        _pf_document += ',';
        _pf_document += ::pineforge::checked_settings::describe(_pf_overrides[1], ::pineforge::checked_settings::number((std::isnan(override_.commission_value) ? config_.commission_value : override_.commission_value)));
        _pf_document += ',';
        _pf_document += ::pineforge::checked_settings::describe(_pf_overrides[2], ::pineforge::checked_settings::number((std::isnan(override_.default_qty_value) ? config_.default_qty_value : override_.default_qty_value)));
        _pf_document += ',';
        _pf_document += ::pineforge::checked_settings::describe(_pf_overrides[3], ::pineforge::checked_settings::number((override_.pyramiding < 0 ? config_.pyramiding : override_.pyramiding)));
        _pf_document += ',';
        _pf_document += ::pineforge::checked_settings::describe(_pf_overrides[4], ::pineforge::checked_settings::number((override_.slippage < 0 ? config_.slippage : override_.slippage)));
        _pf_document += ',';
        _pf_document += ::pineforge::checked_settings::describe(_pf_overrides[5], ::pineforge::checked_settings::number(static_cast<bool>((override_.process_orders_on_close < 0 ? config_.process_orders_on_close : override_.process_orders_on_close))));
        _pf_document += ',';
        _pf_document += ::pineforge::checked_settings::describe(_pf_overrides[6], ::pineforge::checked_settings::number(static_cast<bool>((override_.calc_on_order_fills < 0 ? config_.calc_on_order_fills : override_.calc_on_order_fills))));
        _pf_document += ',';
        _pf_document += ::pineforge::checked_settings::describe(_pf_overrides[7], _pf_close_entries_rule_word((override_.close_entries_rule < 0 ? config_.close_entries_rule_any : override_.close_entries_rule)));
        _pf_document += ',';
        _pf_document += ::pineforge::checked_settings::describe(_pf_overrides[8], _pf_default_qty_type_word((override_.default_qty_type < 0 ? config_.default_qty_type : override_.default_qty_type)));
        _pf_document += ',';
        _pf_document += ::pineforge::checked_settings::describe(_pf_overrides[9], _pf_commission_type_word((override_.commission_type < 0 ? config_.commission_type : override_.commission_type)));
        return _pf_document + "]}";
    }
#endif

    void configure_security_evaluators() override {
        security_eval_states_.clear();
        register_security_eval(0, "60", input_tf_, false, false);
        register_security_eval(1, "240", input_tf_, false, false);
        register_security_eval(2, "D", input_tf_, false, false);
    }

#ifndef PINEFORGE_HAS_SCRIPT_RUN_PREPARE_V1
#error "Generated lifecycle reset requires a matching PineForge engine; rebuild with script-run preparation support"
#endif
    void prepare_script_run(const Bar* bars, int n, bool allow_precalculation) override {
        _pf_require_settings_ok();
        _pf_script_state_checkpoint_.reset();
        this->_req_sec_0 = na<double>();
        this->_req_sec_1 = na<double>();
        this->_req_sec_2 = na<double>();
        this->_security_helper_series_ = decltype(this->_security_helper_series_){};
        this->_ta_ema_1 = decltype(this->_ta_ema_1)(12);
        this->_ta_ema_2 = decltype(this->_ta_ema_2)(26);
        this->_ta_ema_3 = decltype(this->_ta_ema_3)(9);
        this->_ta_ema_1_cs1 = decltype(this->_ta_ema_1_cs1)(12);
        this->_ta_ema_2_cs1 = decltype(this->_ta_ema_2_cs1)(26);
        this->_ta_ema_3_cs1 = decltype(this->_ta_ema_3_cs1)(9);
        this->_ta_ema_1_cs2 = decltype(this->_ta_ema_1_cs2)(12);
        this->_ta_ema_2_cs2 = decltype(this->_ta_ema_2_cs2)(26);
        this->_ta_ema_3_cs2 = decltype(this->_ta_ema_3_cs2)(9);
        this->_use_precalc = false;
        this->_sec0__ta_ema_1_v0 = decltype(this->_sec0__ta_ema_1_v0)(12);
        this->_sec0__ta_ema_1_v1 = decltype(this->_sec0__ta_ema_1_v1)(12);
        this->_sec0__ta_ema_1_v2 = decltype(this->_sec0__ta_ema_1_v2)(12);
        this->_sec0__ta_ema_2_v0 = decltype(this->_sec0__ta_ema_2_v0)(26);
        this->_sec0__ta_ema_2_v1 = decltype(this->_sec0__ta_ema_2_v1)(26);
        this->_sec0__ta_ema_2_v2 = decltype(this->_sec0__ta_ema_2_v2)(26);
        this->_sec0__ta_ema_3_v0 = decltype(this->_sec0__ta_ema_3_v0)(9);
        this->_sec0__ta_ema_3_v1 = decltype(this->_sec0__ta_ema_3_v1)(9);
        this->_sec1__ta_ema_1_v0 = decltype(this->_sec1__ta_ema_1_v0)(12);
        this->_sec1__ta_ema_1_v1 = decltype(this->_sec1__ta_ema_1_v1)(12);
        this->_sec1__ta_ema_1_v2 = decltype(this->_sec1__ta_ema_1_v2)(12);
        this->_sec1__ta_ema_2_v0 = decltype(this->_sec1__ta_ema_2_v0)(26);
        this->_sec1__ta_ema_2_v1 = decltype(this->_sec1__ta_ema_2_v1)(26);
        this->_sec1__ta_ema_2_v2 = decltype(this->_sec1__ta_ema_2_v2)(26);
        this->_sec1__ta_ema_3_v0 = decltype(this->_sec1__ta_ema_3_v0)(9);
        this->_sec1__ta_ema_3_v1 = decltype(this->_sec1__ta_ema_3_v1)(9);
        this->_sec2__ta_ema_1_v0 = decltype(this->_sec2__ta_ema_1_v0)(12);
        this->_sec2__ta_ema_1_v1 = decltype(this->_sec2__ta_ema_1_v1)(12);
        this->_sec2__ta_ema_1_v2 = decltype(this->_sec2__ta_ema_1_v2)(12);
        this->_sec2__ta_ema_2_v0 = decltype(this->_sec2__ta_ema_2_v0)(26);
        this->_sec2__ta_ema_2_v1 = decltype(this->_sec2__ta_ema_2_v1)(26);
        this->_sec2__ta_ema_2_v2 = decltype(this->_sec2__ta_ema_2_v2)(26);
        this->_sec2__ta_ema_3_v0 = decltype(this->_sec2__ta_ema_3_v0)(9);
        this->_sec2__ta_ema_3_v1 = decltype(this->_sec2__ta_ema_3_v1)(9);
        this->h1 = 0.0;
        this->h2 = 0.0;
        this->h3 = 0.0;
        this->allBull = false;
        this->allBear = false;
        this->_ta_initialized_ = false;
        this->_inputs_initialized_ = false;
        (void)bars; (void)n; (void)allow_precalculation;
    }

    double f_macd_hist_cs0(double src, int fast, int slow, int sig) {
        double macd_val = ((history_advances_new_bar() ? _ta_ema_1.compute(src) : _ta_ema_1.recompute(src)) - (history_advances_new_bar() ? _ta_ema_2.compute(src) : _ta_ema_2.recompute(src)));
        double macd_sig = (history_advances_new_bar() ? _ta_ema_3.compute(macd_val) : _ta_ema_3.recompute(macd_val));
        return (macd_val - macd_sig);
    }

    double f_macd_hist_cs1(double src, int fast, int slow, int sig) {
        double macd_val = ((history_advances_new_bar() ? _ta_ema_1_cs1.compute(src) : _ta_ema_1_cs1.recompute(src)) - (history_advances_new_bar() ? _ta_ema_2_cs1.compute(src) : _ta_ema_2_cs1.recompute(src)));
        double macd_sig = (history_advances_new_bar() ? _ta_ema_3_cs1.compute(macd_val) : _ta_ema_3_cs1.recompute(macd_val));
        return (macd_val - macd_sig);
    }

    double f_macd_hist_cs2(double src, int fast, int slow, int sig) {
        double macd_val = ((history_advances_new_bar() ? _ta_ema_1_cs2.compute(src) : _ta_ema_1_cs2.recompute(src)) - (history_advances_new_bar() ? _ta_ema_2_cs2.compute(src) : _ta_ema_2_cs2.recompute(src)));
        double macd_sig = (history_advances_new_bar() ? _ta_ema_3_cs2.compute(macd_val) : _ta_ema_3_cs2.recompute(macd_val));
        return (macd_val - macd_sig);
    }

    void on_source_bar(const Bar& bar) override {
        h1 = _req_sec_0;
        h2 = _req_sec_1;
        h3 = _req_sec_2;
        allBull = ((([&]{ auto _pna_l = (h1); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()) && ([&]{ auto _pna_l = (h2); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) && ([&]{ auto _pna_l = (h3); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()));
        allBear = ((([&]{ auto _pna_l = (h1); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }()) && ([&]{ auto _pna_l = (h2); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }())) && ([&]{ auto _pna_l = (h3); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }()));
        if ((allBull && ([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) || _pfc_eq); }()))) {
            if (([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }())) {
                strategy_close(std::string("S"), std::string("flip to long"), na<double>(), na<double>(), false, 176093659159ULL);
            }
            strategy_entry(std::string("L"), true, na<double>(), na<double>(), na<double>(), std::string("3-TF MACD bull"));
        }
        if ((allBear && ([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) || _pfc_eq); }()))) {
            if (([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) {
                strategy_close(std::string("L"), std::string("flip to short"), na<double>(), na<double>(), false, 197568495639ULL);
            }
            strategy_entry(std::string("S"), false, na<double>(), na<double>(), na<double>(), std::string("3-TF MACD bear"));
        }
    }


    void _eval_security_0(const Bar& bar, bool is_complete) {
        double _sec0_f_macd_hist_1_macd_val = ((security_series_slot_is_new(0) ? _sec0__ta_ema_1_v2.compute(bar.close) : _sec0__ta_ema_1_v2.recompute(bar.close)) - (security_series_slot_is_new(0) ? _sec0__ta_ema_2_v2.compute(bar.close) : _sec0__ta_ema_2_v2.recompute(bar.close)));
        double _sec0_f_macd_hist_2_macd_sig = (security_series_slot_is_new(0) ? _sec0__ta_ema_3_v1.compute(_sec0_f_macd_hist_1_macd_val) : _sec0__ta_ema_3_v1.recompute(_sec0_f_macd_hist_1_macd_val));
        _req_sec_0 = (_sec0_f_macd_hist_1_macd_val - _sec0_f_macd_hist_2_macd_sig);
    }

    void _eval_security_1(const Bar& bar, bool is_complete) {
        double _sec1_f_macd_hist_3_macd_val = ((security_series_slot_is_new(1) ? _sec1__ta_ema_1_v2.compute(bar.close) : _sec1__ta_ema_1_v2.recompute(bar.close)) - (security_series_slot_is_new(1) ? _sec1__ta_ema_2_v2.compute(bar.close) : _sec1__ta_ema_2_v2.recompute(bar.close)));
        double _sec1_f_macd_hist_4_macd_sig = (security_series_slot_is_new(1) ? _sec1__ta_ema_3_v1.compute(_sec1_f_macd_hist_3_macd_val) : _sec1__ta_ema_3_v1.recompute(_sec1_f_macd_hist_3_macd_val));
        _req_sec_1 = (_sec1_f_macd_hist_3_macd_val - _sec1_f_macd_hist_4_macd_sig);
    }

    void _eval_security_2(const Bar& bar, bool is_complete) {
        double _sec2_f_macd_hist_5_macd_val = ((security_series_slot_is_new(2) ? _sec2__ta_ema_1_v2.compute(bar.close) : _sec2__ta_ema_1_v2.recompute(bar.close)) - (security_series_slot_is_new(2) ? _sec2__ta_ema_2_v2.compute(bar.close) : _sec2__ta_ema_2_v2.recompute(bar.close)));
        double _sec2_f_macd_hist_6_macd_sig = (security_series_slot_is_new(2) ? _sec2__ta_ema_3_v1.compute(_sec2_f_macd_hist_5_macd_val) : _sec2__ta_ema_3_v1.recompute(_sec2_f_macd_hist_5_macd_val));
        _req_sec_2 = (_sec2_f_macd_hist_5_macd_val - _sec2_f_macd_hist_6_macd_sig);
    }

    void evaluate_security(int sec_id, const Bar& bar, bool is_complete) override {
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
#if defined(PINEFORGE_HAS_RUN_FAILURE_CODES_V1) && defined(PF_SETTINGS_API_VERSION)
    int strategy_create_checked(const char* params_json, void** out, char* error, size_t error_capacity);
#endif
    void* strategy_create(const char* params_json) {
#if defined(PINEFORGE_HAS_RUN_FAILURE_CODES_V1) && defined(PF_SETTINGS_API_VERSION)
        void* out = nullptr;
        strategy_create_checked(nullptr, &out, nullptr, 0);
        return out;
#else
        try { return new GeneratedStrategy(); } catch (...) { return nullptr; }
#endif
    }
    void run_backtest(void* s, Bar* bars, int n, ReportC* out) {
        try {
        auto* strat = static_cast<GeneratedStrategy*>(s);
#ifndef PF_SETTINGS_API_VERSION
        if (strat->_pf_refuse_failed_setting(out)) return;
#endif
        strat->run(bars, n, "", "", false, 4, MagnifierDistribution::ENDPOINTS);
        if (!strat->_pf_refuse_failed_setting(out)) strat->fill_report(out);
        } catch (const std::exception& _pf_error) {
            if (out) *out = ReportC{};
            if (s) static_cast<GeneratedStrategy*>(s)->_pf_record_failure("run_backtest", _pf_error);
        } catch (...) {
            if (out) *out = ReportC{};
            if (s) static_cast<GeneratedStrategy*>(s)->_pf_record_failure("run_backtest", "unknown C++ exception");
        }
    }
    static void _pf_run_backtest_full_impl(void* s, Bar* bars, int n,
                           const char* input_tf, const char* script_tf,
                           int bar_magnifier, int magnifier_samples,
                           int magnifier_dist,
                           ReportC* out) {
        auto* strat = static_cast<GeneratedStrategy*>(s);
#ifndef PF_SETTINGS_API_VERSION
        if (strat->_pf_refuse_failed_setting(out)) return;
#endif
        std::string itf = input_tf ? input_tf : "";
        std::string stf = script_tf ? script_tf : "";
        strat->run(bars, n, itf, stf, bar_magnifier != 0, magnifier_samples,
                   static_cast<MagnifierDistribution>(magnifier_dist));
        if (!strat->_pf_refuse_failed_setting(out)) strat->fill_report(out);
    }
    void run_backtest_full(void* s, Bar* bars, int n, const char* input_tf, const char* script_tf,
                           int bar_magnifier, int magnifier_samples, int magnifier_dist, ReportC* out) {
        try { _pf_run_backtest_full_impl(s, bars, n, input_tf, script_tf, bar_magnifier, magnifier_samples, magnifier_dist, out); }
        catch (const std::exception& _pf_error) { if (out) *out = ReportC{}; if (s) static_cast<GeneratedStrategy*>(s)->_pf_record_failure("run_backtest_full", _pf_error); }
        catch (...) { if (out) *out = ReportC{}; if (s) static_cast<GeneratedStrategy*>(s)->_pf_record_failure("run_backtest_full", "unknown C++ exception"); }
    }
    void strategy_free(void* s) {
        try { delete static_cast<GeneratedStrategy*>(s); } catch (...) {}
    }
    void report_free(ReportC* report) {
        try { BacktestEngine::free_report(report); } catch (...) {}
    }
    void strategy_set_input(void* s, const char* key, const char* value) {
        if (!s || !key || !value) return;
        try { static_cast<GeneratedStrategy*>(s)->set_input(key, value); }
        catch (const std::exception& _pf_error) { _PF_SETTING_FAILURE(static_cast<GeneratedStrategy*>(s), "strategy_set_input", _pf_error.what(), nullptr); }
        catch (...) { _PF_SETTING_FAILURE(static_cast<GeneratedStrategy*>(s), "strategy_set_input", "unknown C++ exception", nullptr); }
    }
    void strategy_set_override(void* s, const char* key, const char* value) {
        if (!s || !key || !value) return;
        try { static_cast<GeneratedStrategy*>(s)->set_strategy_override(key, value); }
#ifdef PINEFORGE_HAS_RUN_FAILURE_CODES_V1
        catch (const std::invalid_argument& _pf_error) { _PF_SETTING_FAILURE(static_cast<GeneratedStrategy*>(s), "strategy_set_override", _pf_error.what(), "unparseable_value"); }
        catch (const std::out_of_range& _pf_error) { _PF_SETTING_FAILURE(static_cast<GeneratedStrategy*>(s), "strategy_set_override", _pf_error.what(), "unparseable_value"); }
#endif
        catch (const std::exception& _pf_error) { _PF_SETTING_FAILURE(static_cast<GeneratedStrategy*>(s), "strategy_set_override", _pf_error.what(), nullptr); }
        catch (...) { _PF_SETTING_FAILURE(static_cast<GeneratedStrategy*>(s), "strategy_set_override", "unknown C++ exception", nullptr); }
    }
    void strategy_set_magnifier_volume_weighted(void* s, int on) {
        if (!s) return;
        try { static_cast<GeneratedStrategy*>(s)->set_magnifier_volume_weighted(on != 0); }
        catch (const std::exception& _pf_error) { _PF_SETTING_FAILURE(static_cast<GeneratedStrategy*>(s), "strategy_set_magnifier_volume_weighted", _pf_error.what(), nullptr); }
        catch (...) { _PF_SETTING_FAILURE(static_cast<GeneratedStrategy*>(s), "strategy_set_magnifier_volume_weighted", "unknown C++ exception", nullptr); }
    }
#ifdef PF_SETTINGS_API_VERSION
    uint32_t strategy_settings_api_version(void) { return PF_SETTINGS_API_VERSION; }
    int strategy_create_checked(const char* params_json, void** out, char* error, size_t error_capacity) {
        if (out) *out = nullptr;
        return ::pineforge::checked_settings::boundary(error, error_capacity, [&] {
            ::pineforge::checked_settings::require(out != nullptr, "strategy output pointer is null");
            ::pineforge::checked_settings::require(!params_json || !*params_json, "params_json is reserved; use checked setters", PF_SETTINGS_UNSUPPORTED);
            *out = new GeneratedStrategy();
        });
    }
    int strategy_set_input_checked(void* s, const char* key, const char* value, char* error, size_t error_capacity) {
        return ::pineforge::checked_settings::boundary(error, error_capacity, [&] {
            ::pineforge::checked_settings::require(s && key && value, "null strategy, key or value");
            static_cast<GeneratedStrategy*>(s)->_pf_set_input_checked(key, value);
        });
    }
    int strategy_set_override_checked(void* s, const char* key, const char* value, char* error, size_t error_capacity) {
        return ::pineforge::checked_settings::boundary(error, error_capacity, [&] {
            ::pineforge::checked_settings::require(s && key && value, "null strategy, key or value");
            static_cast<GeneratedStrategy*>(s)->_pf_set_override_checked(key, value);
        });
    }
    int strategy_get_effective_settings(void* s, char* json, size_t capacity, size_t* required, char* error, size_t error_capacity) {
        if (required) *required = 0;
        return ::pineforge::checked_settings::boundary(error, error_capacity, [&] {
            ::pineforge::checked_settings::require(s != nullptr, "null strategy");
            ::pineforge::checked_settings::receipt(static_cast<GeneratedStrategy*>(s)->_pf_settings_receipt(), json, capacity, required);
        });
    }
    int run_backtest_full_checked(void* s, Bar* bars, int n, const char* input_tf, const char* script_tf, int bar_magnifier, int magnifier_samples, int magnifier_dist, ReportC* out, char* error, size_t error_capacity) {
        return ::pineforge::checked_settings::boundary(error, error_capacity, [&] {
            ::pineforge::checked_settings::require(s && out && n >= 0 && (n == 0 || bars), "invalid batch arguments");
            _pf_run_backtest_full_impl(s, bars, n, input_tf, script_tf, bar_magnifier, magnifier_samples, magnifier_dist, out);
            const auto& _pf_error = static_cast<GeneratedStrategy*>(s)->last_error();
            ::pineforge::checked_settings::require(_pf_error.empty(), _pf_error.c_str(), PF_SETTINGS_RUN_FAILED);
        });
    }
#endif
#ifdef PF_SETTINGS_API_VERSION
#ifdef PF_CAPABILITIES_API_VERSION
    uint32_t strategy_capabilities_api_version(void) { return PF_CAPABILITIES_API_VERSION; }
    int strategy_capabilities_receipt(void* s, char* json, size_t capacity, size_t* required, char* error, size_t error_capacity) {
        if (required) *required = 0;
        return ::pineforge::checked_settings::boundary(error, error_capacity, [&] {
            ::pineforge::checked_settings::require(s != nullptr, "null strategy");
            ::pineforge::checked_settings::receipt("{\"declarations\":{\"backtest_fill_limits_assumption\":0,\"calc_on_every_history_tick\":false,\"calc_on_every_tick\":false,\"calc_on_order_fills\":false,\"currency\":\"currency.NONE\",\"dynamic_requests\":true,\"fill_orders_on_standard_ohlc\":false,\"process_orders_on_close\":false,\"timeframe\":\"\",\"timeframe_gaps\":true,\"use_bar_magnifier\":false},\"requests\":[{\"feed\":\"chart\",\"function\":\"request.security\",\"gaps\":\"barmerge.gaps_off\",\"heikinashi\":false,\"lookahead\":\"barmerge.lookahead_off\",\"symbol\":\"syminfo.tickerid\",\"timeframe\":\"60\"},{\"feed\":\"chart\",\"function\":\"request.security\",\"gaps\":\"barmerge.gaps_off\",\"heikinashi\":false,\"lookahead\":\"barmerge.lookahead_off\",\"symbol\":\"syminfo.tickerid\",\"timeframe\":\"240\"},{\"feed\":\"chart\",\"function\":\"request.security\",\"gaps\":\"barmerge.gaps_off\",\"heikinashi\":false,\"lookahead\":\"barmerge.lookahead_off\",\"symbol\":\"syminfo.tickerid\",\"timeframe\":\"D\"}],\"requirements\":{\"auxiliary_security_feeds\":false,\"fx_curve\":false,\"historical_probe_overrides\":false,\"intrabar_persistence\":false,\"native_security_feeds\":false,\"recorded_series\":false},\"unresolved\":[],\"version\":1}", json, capacity, required);
        });
    }
    uint32_t strategy_confirmed_bar_api_version(void) { return 1u; }
    int strategy_confirmed_bar_receipt(void* s, char* json, size_t capacity, size_t* required, char* error, size_t error_capacity) {
        if (required) *required = 0;
        return ::pineforge::checked_settings::boundary(error, error_capacity, [&] {
            ::pineforge::checked_settings::require(s != nullptr, "null strategy");
            ::pineforge::checked_settings::receipt("{\"intrabar_persistence\":false,\"orders\":[\"strategy.close (unproven order shape)\",\"strategy.entry (unproven order shape)\"],\"requests\":[{\"expression\":\"f_macd_hist(close,12,26,9)\",\"feed\":\"chart\",\"function\":\"request.security\",\"gaps\":\"barmerge.gaps_off\",\"heikinashi\":false,\"lookahead\":\"barmerge.lookahead_off\",\"symbol\":\"syminfo.tickerid\",\"timeframe\":\"60\"},{\"expression\":\"f_macd_hist(close,12,26,9)\",\"feed\":\"chart\",\"function\":\"request.security\",\"gaps\":\"barmerge.gaps_off\",\"heikinashi\":false,\"lookahead\":\"barmerge.lookahead_off\",\"symbol\":\"syminfo.tickerid\",\"timeframe\":\"240\"},{\"expression\":\"f_macd_hist(close,12,26,9)\",\"feed\":\"chart\",\"function\":\"request.security\",\"gaps\":\"barmerge.gaps_off\",\"heikinashi\":false,\"lookahead\":\"barmerge.lookahead_off\",\"symbol\":\"syminfo.tickerid\",\"timeframe\":\"D\"}],\"version\":1}", json, capacity, required);
        });
    }
    uint32_t strategy_order_shapes_api_version(void) { return 1u; }
    int strategy_order_shapes_receipt(void* s, char* json, size_t capacity, size_t* required, char* error, size_t error_capacity) {
        if (required) *required = 0;
        return ::pineforge::checked_settings::boundary(error, error_capacity, [&] {
            ::pineforge::checked_settings::require(s != nullptr, "null strategy");
            ::pineforge::checked_settings::receipt("{\"calls\":[{\"call\":\"close\",\"comment\":\"literal\",\"context\":\"straight\",\"id\":\"literal\",\"immediately\":\"absent\",\"qty\":\"absent\",\"qty_percent\":\"absent\",\"site\":0,\"target\":\"short\"},{\"call\":\"entry\",\"comment\":\"literal\",\"context\":\"straight\",\"direction\":\"long\",\"id\":\"literal\",\"limit\":\"absent\",\"oca_name\":\"absent\",\"oca_type\":\"absent\",\"qty\":\"absent\",\"qty_type\":\"absent\",\"site\":1,\"stop\":\"absent\"},{\"call\":\"close\",\"comment\":\"literal\",\"context\":\"straight\",\"id\":\"literal\",\"immediately\":\"absent\",\"qty\":\"absent\",\"qty_percent\":\"absent\",\"site\":2,\"target\":\"long\"},{\"call\":\"entry\",\"comment\":\"literal\",\"context\":\"straight\",\"direction\":\"short\",\"id\":\"literal\",\"limit\":\"absent\",\"oca_name\":\"absent\",\"oca_type\":\"absent\",\"qty\":\"absent\",\"qty_type\":\"absent\",\"site\":3,\"stop\":\"absent\"}],\"entry_ids\":{\"long\":1,\"multi_site\":0,\"shared\":0,\"short\":1},\"host_reads\":[\"StreamPhase\",\"clear_security\",\"commit_script_state\",\"config_\",\"configure_pine_strategy\",\"configure_security_evaluators\",\"evaluate_security\",\"fill_report\",\"history_advances_new_bar\",\"input_tf_\",\"inputs_\",\"last_error\",\"last_error_\",\"on_source_bar\",\"override_\",\"prepare_script_run\",\"register_security_eval\",\"restore_script_state\",\"run\",\"script_bars_processed\",\"security_eval_states_\",\"security_series_slot_is_new\",\"set_input\",\"set_magnifier_volume_weighted\",\"set_strategy_override\",\"set_trade_history_streamable\",\"signed_position_size\",\"snapshot_script_state\",\"strategy_close\",\"strategy_entry\",\"stream_phase_\"],\"process_orders_on_close\":false,\"settings\":{\"close_entries_rule\":\"FIFO\",\"commission_type\":\"percent\",\"commission_value\":0.0,\"default_qty_type\":\"fixed\",\"default_qty_value\":1.0,\"initial_capital\":1000000.0,\"margin_long\":100.0,\"margin_short\":100.0,\"pyramiding\":1,\"slippage\":0},\"unmodeled\":[],\"version\":1}", json, capacity, required);
        });
    }
#endif
#endif
    int strategy_reads_trade_history(void) {
        return 0;
    }
}
