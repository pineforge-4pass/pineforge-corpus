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
#ifdef PF_VWAP_HAS_SESSION_ANCHOR
#define PF_VWAP_SESSION_ANCHOR_ARGS(tz, sess) , tz, sess
#else
#define PF_VWAP_SESSION_ANCHOR_ARGS(tz, sess)
#endif
#ifndef PINEFORGE_HAS_NATIVE_LOWERING_V1
#error "generated code requires pineforge-engine native lowering v1 (PINEFORGE_HAS_NATIVE_LOWERING_V1)"
#endif

using namespace pineforge;

#ifdef PF_VWAP_HAS_ANCHOR_INPUT
class _PFAnchoredVWAPBands {
    ta::AnchoredVWAPBands impl_;
public:
    explicit _PFAnchoredVWAPBands(double mult) : impl_(mult) {}
    ta::VWAPBandsResult compute(double src, double volume, int64_t timestamp, const std::string& tz, const std::string& session, bool anchor) {
        (void)timestamp; (void)tz; (void)session;
        return impl_.compute(src, volume, anchor);
    }
    ta::VWAPBandsResult recompute(double src, double volume, int64_t timestamp, const std::string& tz, const std::string& session, bool anchor) {
        (void)timestamp; (void)tz; (void)session;
        return impl_.recompute(src, volume, anchor);
    }
};
#else
class _PFAnchoredVWAPBands {
    ta::VWAP impl_;
    double mult_;
public:
    explicit _PFAnchoredVWAPBands(double mult) : mult_(mult) {}
    ta::VWAPBandsResult compute(double src, double volume, int64_t timestamp, const std::string& tz, const std::string& session, bool) {
#ifdef PF_VWAP_HAS_SESSION_ANCHOR
        return impl_.compute_bands(src, volume, timestamp, mult_, tz, session);
#else
        (void)tz; (void)session;
        return impl_.compute_bands(src, volume, timestamp, mult_);
#endif
    }
    ta::VWAPBandsResult recompute(double src, double volume, int64_t timestamp, const std::string& tz, const std::string& session, bool) {
#ifdef PF_VWAP_HAS_SESSION_ANCHOR
        return impl_.recompute_bands(src, volume, timestamp, mult_, tz, session);
#else
        (void)tz; (void)session;
        return impl_.recompute_bands(src, volume, timestamp, mult_);
#endif
    }
};
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
    _PFAnchoredVWAPBands _ta_vwap_anchored_bands_1;
    ta::Crossunder _ta_crossunder_2;
    ta::Crossover _ta_crossover_3;
    bool _use_precalc = false;
    double vw = 0.0;
    double upper_band = 0.0;
    double lower_band = 0.0;
    bool long_condition = false;
    bool exit_condition = false;
    bool _ta_initialized_ = false;
    bool _inputs_initialized_ = false;

    struct _PFScriptState {
        decltype(GeneratedStrategy::_ta_vwap_anchored_bands_1) _pf_value_0;
        decltype(GeneratedStrategy::_ta_crossunder_2) _pf_value_1;
        decltype(GeneratedStrategy::_ta_crossover_3) _pf_value_2;
        decltype(GeneratedStrategy::vw) _pf_value_3;
        decltype(GeneratedStrategy::upper_band) _pf_value_4;
        decltype(GeneratedStrategy::lower_band) _pf_value_5;
        decltype(GeneratedStrategy::long_condition) _pf_value_6;
        decltype(GeneratedStrategy::exit_condition) _pf_value_7;
        decltype(GeneratedStrategy::_ta_initialized_) _pf_value_8;
        decltype(GeneratedStrategy::_inputs_initialized_) _pf_value_9;
    };
    static_assert(std::is_copy_constructible_v<_PFScriptState>, "generated Pine state must be deep-copy constructible");
    static_assert(std::is_copy_assignable_v<_PFScriptState>, "generated Pine state must be deep-copy assignable");
    std::optional<_PFScriptState> _pf_script_state_checkpoint_;

    void snapshot_script_state() override {
        _pf_script_state_checkpoint_.emplace(_PFScriptState{
            _ta_vwap_anchored_bands_1,
            _ta_crossunder_2,
            _ta_crossover_3,
            vw,
            upper_band,
            lower_band,
            long_condition,
            exit_condition,
            _ta_initialized_,
            _inputs_initialized_,
        });
    }

    void restore_script_state() override {
        if (!_pf_script_state_checkpoint_) return;
        this->_ta_vwap_anchored_bands_1 = _pf_script_state_checkpoint_->_pf_value_0;
        this->_ta_crossunder_2 = _pf_script_state_checkpoint_->_pf_value_1;
        this->_ta_crossover_3 = _pf_script_state_checkpoint_->_pf_value_2;
        this->vw = _pf_script_state_checkpoint_->_pf_value_3;
        this->upper_band = _pf_script_state_checkpoint_->_pf_value_4;
        this->lower_band = _pf_script_state_checkpoint_->_pf_value_5;
        this->long_condition = _pf_script_state_checkpoint_->_pf_value_6;
        this->exit_condition = _pf_script_state_checkpoint_->_pf_value_7;
        this->_ta_initialized_ = _pf_script_state_checkpoint_->_pf_value_8;
        this->_inputs_initialized_ = _pf_script_state_checkpoint_->_pf_value_9;
    }

    void commit_script_state() override {
        snapshot_script_state();
    }

    explicit GeneratedStrategy() : _ta_vwap_anchored_bands_1(2) {
#if defined(PINEFORGE_HAS_EXPLICIT_PINE_EXECUTION_ADAPTER_V1)
        pineforge::source::PineStrategyHost::attach_pine_execution_adapter();
#elif defined(PINEFORGE_HAS_EXPLICIT_PINE_CAP_V1)
        pineforge::source::PineStrategyHost::enable_pine_intraday_cap();
#endif
        pineforge::source::PineStrategyConfig cfg{};
        cfg.initial_capital = 100000.0;
        cfg.default_qty_type = static_cast<int>(QtyType::FIXED);
        cfg.default_qty_value = 1.0;
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
        this->_ta_vwap_anchored_bands_1 = decltype(this->_ta_vwap_anchored_bands_1)(2);
        this->_ta_crossunder_2 = decltype(this->_ta_crossunder_2){};
        this->_ta_crossover_3 = decltype(this->_ta_crossover_3){};
        this->_use_precalc = false;
        this->vw = 0.0;
        this->upper_band = 0.0;
        this->lower_band = 0.0;
        this->long_condition = false;
        this->exit_condition = false;
        this->_ta_initialized_ = false;
        this->_inputs_initialized_ = false;
        (void)bars; (void)n; (void)allow_precalculation;
    }

    void on_source_bar(const Bar& bar) override {
        auto _result__ta_vwap_anchored_bands_1 = (history_advances_new_bar() ? _ta_vwap_anchored_bands_1.compute(current_bar_.close, current_bar_.volume, current_bar_.timestamp, syminfo_.timezone, syminfo_.session, tf_change(prev_bar_timestamp_, current_bar_.timestamp, std::string("1D"), syminfo_.timezone, syminfo_.session)) : _ta_vwap_anchored_bands_1.recompute(current_bar_.close, current_bar_.volume, current_bar_.timestamp, syminfo_.timezone, syminfo_.session, tf_change(prev_bar_timestamp_, current_bar_.timestamp, std::string("1D"), syminfo_.timezone, syminfo_.session)));
        double vw = _result__ta_vwap_anchored_bands_1.vwap;
        double upper_band = _result__ta_vwap_anchored_bands_1.upper;
        double lower_band = _result__ta_vwap_anchored_bands_1.lower;
        long_condition = (history_advances_new_bar() ? _ta_crossunder_2.compute(current_bar_.close, lower_band) : _ta_crossunder_2.recompute(current_bar_.close, lower_band));
        exit_condition = (history_advances_new_bar() ? _ta_crossover_3.compute(current_bar_.close, vw) : _ta_crossover_3.recompute(current_bar_.close, vw));
        if (long_condition) {
            strategy_entry(std::string("long"), true, na<double>(), na<double>(), na<double>(), "");
        }
        if (exit_condition) {
            strategy_close(std::string("long"), "", na<double>(), na<double>(), false, 98784247827ULL);
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
