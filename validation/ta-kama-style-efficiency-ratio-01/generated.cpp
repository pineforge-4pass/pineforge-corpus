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
    math::Sum _ta_sum_1;
    bool _use_precalc = false;
    Series<double> _s_close;
    Series<double> kama;
    Series<bool> kamaDown;
    Series<bool> kamaUp;
    Series<double> src;
    Series<double> _udf_series_arg_1;
    int len = 0;
    int fastLen = 0;
    int slowLen = 0;
    double er = 0.0;
    double fastAlpha = 0.0;
    double slowAlpha = 0.0;
    double sc = 0.0;
    bool longCond = false;
    bool shortCond = false;
    bool _var_initialized = false;
    bool _ta_initialized_ = false;
    bool _inputs_initialized_ = false;

    struct _PFScriptState {
        decltype(GeneratedStrategy::_ta_sum_1) _pf_value_0;
        decltype(GeneratedStrategy::_s_close) _pf_value_1;
        decltype(GeneratedStrategy::kama) _pf_value_2;
        decltype(GeneratedStrategy::kamaDown) _pf_value_3;
        decltype(GeneratedStrategy::kamaUp) _pf_value_4;
        decltype(GeneratedStrategy::src) _pf_value_5;
        decltype(GeneratedStrategy::_udf_series_arg_1) _pf_value_6;
        decltype(GeneratedStrategy::len) _pf_value_7;
        decltype(GeneratedStrategy::fastLen) _pf_value_8;
        decltype(GeneratedStrategy::slowLen) _pf_value_9;
        decltype(GeneratedStrategy::er) _pf_value_10;
        decltype(GeneratedStrategy::fastAlpha) _pf_value_11;
        decltype(GeneratedStrategy::slowAlpha) _pf_value_12;
        decltype(GeneratedStrategy::sc) _pf_value_13;
        decltype(GeneratedStrategy::longCond) _pf_value_14;
        decltype(GeneratedStrategy::shortCond) _pf_value_15;
        decltype(GeneratedStrategy::_var_initialized) _pf_value_16;
        decltype(GeneratedStrategy::_ta_initialized_) _pf_value_17;
        decltype(GeneratedStrategy::_inputs_initialized_) _pf_value_18;
    };
    static_assert(std::is_copy_constructible_v<_PFScriptState>, "generated Pine state must be deep-copy constructible");
    static_assert(std::is_copy_assignable_v<_PFScriptState>, "generated Pine state must be deep-copy assignable");
    std::optional<_PFScriptState> _pf_script_state_checkpoint_;

    void snapshot_script_state() override {
        _pf_script_state_checkpoint_.emplace(_PFScriptState{
            _ta_sum_1,
            _s_close,
            kama,
            kamaDown,
            kamaUp,
            src,
            _udf_series_arg_1,
            len,
            fastLen,
            slowLen,
            er,
            fastAlpha,
            slowAlpha,
            sc,
            longCond,
            shortCond,
            _var_initialized,
            _ta_initialized_,
            _inputs_initialized_,
        });
    }

    void restore_script_state() override {
        if (!_pf_script_state_checkpoint_) return;
        this->_ta_sum_1 = _pf_script_state_checkpoint_->_pf_value_0;
        this->_s_close = _pf_script_state_checkpoint_->_pf_value_1;
        this->kama = _pf_script_state_checkpoint_->_pf_value_2;
        this->kamaDown = _pf_script_state_checkpoint_->_pf_value_3;
        this->kamaUp = _pf_script_state_checkpoint_->_pf_value_4;
        this->src = _pf_script_state_checkpoint_->_pf_value_5;
        this->_udf_series_arg_1 = _pf_script_state_checkpoint_->_pf_value_6;
        this->len = _pf_script_state_checkpoint_->_pf_value_7;
        this->fastLen = _pf_script_state_checkpoint_->_pf_value_8;
        this->slowLen = _pf_script_state_checkpoint_->_pf_value_9;
        this->er = _pf_script_state_checkpoint_->_pf_value_10;
        this->fastAlpha = _pf_script_state_checkpoint_->_pf_value_11;
        this->slowAlpha = _pf_script_state_checkpoint_->_pf_value_12;
        this->sc = _pf_script_state_checkpoint_->_pf_value_13;
        this->longCond = _pf_script_state_checkpoint_->_pf_value_14;
        this->shortCond = _pf_script_state_checkpoint_->_pf_value_15;
        this->_var_initialized = _pf_script_state_checkpoint_->_pf_value_16;
        this->_ta_initialized_ = _pf_script_state_checkpoint_->_pf_value_17;
        this->_inputs_initialized_ = _pf_script_state_checkpoint_->_pf_value_18;
    }

    void commit_script_state() override {
        snapshot_script_state();
    }

    explicit GeneratedStrategy() : _ta_sum_1(14) {
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
        this->_ta_sum_1 = decltype(this->_ta_sum_1)(14);
        this->_use_precalc = false;
        this->_s_close = decltype(this->_s_close){};
        this->kama = decltype(this->kama){};
        this->kamaDown = decltype(this->kamaDown){};
        this->kamaUp = decltype(this->kamaUp){};
        this->src = decltype(this->src){};
        this->_udf_series_arg_1 = decltype(this->_udf_series_arg_1){};
        this->len = 0;
        this->fastLen = 0;
        this->slowLen = 0;
        this->er = 0.0;
        this->fastAlpha = 0.0;
        this->slowAlpha = 0.0;
        this->sc = 0.0;
        this->longCond = false;
        this->shortCond = false;
        this->_var_initialized = false;
        this->_ta_initialized_ = false;
        this->_inputs_initialized_ = false;
        (void)bars; (void)n; (void)allow_precalculation;
    }

    double calcEfficiencyRatio_cs0(const Series<double>& src, int64_t length) {
        double direction = std::abs((src[0] - src[([&](){ auto _pf_idx_v = (length); using _pf_idx_t = std::decay_t<decltype(_pf_idx_v)>; if constexpr (std::is_same_v<_pf_idx_t, bool>) return (int)_pf_idx_v; else return is_na(_pf_idx_v) ? na<int>() : (int)_pf_idx_v; }())]));
        double volatilitySum = (history_advances_new_bar() ? _ta_sum_1.compute(std::abs((src[0] - src[1]))) : _ta_sum_1.recompute(std::abs((src[0] - src[1]))));
        double er = ((([&]{ auto _pna_l = (volatilitySum); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && (!_pfc_eq); }())) ? (((double)(direction) / (double)(volatilitySum))) : (0));
        return er;
    }

    void on_source_bar(const Bar& bar) override {
        if (history_advances_new_bar() && bar_index_ == 0) _udf_series_arg_1.clear();
        if (history_advances_new_bar()) _udf_series_arg_1.push(_udf_series_arg_1.current());
        if (history_advances_new_bar()) _s_close.push(current_bar_.close);
        else _s_close.update(current_bar_.close);
        if (!_var_initialized) {
            kama.push(current_bar_.close);
            _var_initialized = true;
        } else {
            if (history_advances_new_bar()) kama.push(kama[0]);
            else kama.update(kama[0]);
        }
        if (!_inputs_initialized_) {
            len = get_input_int("Length", 14);
            fastLen = get_input_int("Fast Alpha Period", 2);
            slowLen = get_input_int("Slow Alpha Period", 30);
            _inputs_initialized_ = true;
        }
        if (!_ta_initialized_) {
            _ta_sum_1 = math::Sum(get_input_int("Length", 14));
            _ta_initialized_ = true;
        }
        er = calcEfficiencyRatio_cs0(([&]() -> const Series<double>& { double _sv = ([&]() { auto _pf_series_raw = (current_bar_.close); return is_na(_pf_series_raw) ? na<double>() : static_cast<double>(_pf_series_raw); }()); _udf_series_arg_1.update(_sv); return _udf_series_arg_1; }()), [&](){ auto _pf_v = (len); return is_na(_pf_v) ? na<int64_t>() : (int64_t)_pf_v; }());
        fastAlpha = ((double)(2.0) / (double)((fastLen + 1)));
        slowAlpha = ((double)(2.0) / (double)((slowLen + 1)));
        sc = std::pow(((er * (fastAlpha - slowAlpha)) + slowAlpha), 2);
        kama.update((([&]{ auto _nz_v = (kama[1]); return is_na(_nz_v) ? (0.0) : _nz_v; }()) + (sc * (current_bar_.close - ([&]{ auto _nz_v = (kama[1]); return is_na(_nz_v) ? (0.0) : _nz_v; }())))));
        if (history_advances_new_bar()) kamaUp.push(([&]{ auto _pna_l = (kama[0]); auto _pna_r = (kama[1]); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()));
        else kamaUp.update(([&]{ auto _pna_l = (kama[0]); auto _pna_r = (kama[1]); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()));
        if (history_advances_new_bar()) kamaDown.push(([&]{ auto _pna_l = (kama[0]); auto _pna_r = (kama[1]); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }()));
        else kamaDown.update(([&]{ auto _pna_l = (kama[0]); auto _pna_r = (kama[1]); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }()));
        longCond = (kamaUp[0] && !(kamaUp[1]));
        shortCond = (kamaDown[0] && !(kamaDown[1]));
        if (longCond) {
            strategy_entry(std::string("Long"), true, na<double>(), na<double>(), na<double>(), "");
        }
        if (shortCond) {
            strategy_entry(std::string("Short"), false, na<double>(), na<double>(), na<double>(), "");
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
