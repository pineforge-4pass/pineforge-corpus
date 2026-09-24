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
    std::vector<double> _precalc__ta_ema_1;
    bool _use_precalc = false;
    double rangeFilter = na<double>();
    Series<int> filterDir;
    int filterLen = 0;
    double filterMult = 0.0;
    double avgRange = 0.0;
    double smoothRng = 0.0;
    double hiTarget = 0.0;
    double loTarget = 0.0;
    double newFilter = 0.0;
    bool longCond = false;
    bool shortCond = false;
    bool _var_initialized = false;
    bool _pf_var_init_rangeFilter = false;
    bool _ta_initialized_ = false;
    bool _inputs_initialized_ = false;

    struct _PFScriptState {
        decltype(GeneratedStrategy::_ta_ema_1) _pf_value_0;
        decltype(GeneratedStrategy::rangeFilter) _pf_value_1;
        decltype(GeneratedStrategy::filterDir) _pf_value_2;
        decltype(GeneratedStrategy::filterLen) _pf_value_3;
        decltype(GeneratedStrategy::filterMult) _pf_value_4;
        decltype(GeneratedStrategy::avgRange) _pf_value_5;
        decltype(GeneratedStrategy::smoothRng) _pf_value_6;
        decltype(GeneratedStrategy::hiTarget) _pf_value_7;
        decltype(GeneratedStrategy::loTarget) _pf_value_8;
        decltype(GeneratedStrategy::newFilter) _pf_value_9;
        decltype(GeneratedStrategy::longCond) _pf_value_10;
        decltype(GeneratedStrategy::shortCond) _pf_value_11;
        decltype(GeneratedStrategy::_var_initialized) _pf_value_12;
        decltype(GeneratedStrategy::_pf_var_init_rangeFilter) _pf_value_13;
        decltype(GeneratedStrategy::_ta_initialized_) _pf_value_14;
        decltype(GeneratedStrategy::_inputs_initialized_) _pf_value_15;
    };
    static_assert(std::is_copy_constructible_v<_PFScriptState>, "generated Pine state must be deep-copy constructible");
    static_assert(std::is_copy_assignable_v<_PFScriptState>, "generated Pine state must be deep-copy assignable");
    std::optional<_PFScriptState> _pf_script_state_checkpoint_;

    void snapshot_script_state() override {
        _pf_script_state_checkpoint_.emplace(_PFScriptState{
            _ta_ema_1,
            rangeFilter,
            filterDir,
            filterLen,
            filterMult,
            avgRange,
            smoothRng,
            hiTarget,
            loTarget,
            newFilter,
            longCond,
            shortCond,
            _var_initialized,
            _pf_var_init_rangeFilter,
            _ta_initialized_,
            _inputs_initialized_,
        });
    }

    void restore_script_state() override {
        if (!_pf_script_state_checkpoint_) return;
        this->_ta_ema_1 = _pf_script_state_checkpoint_->_pf_value_0;
        this->rangeFilter = _pf_script_state_checkpoint_->_pf_value_1;
        this->filterDir = _pf_script_state_checkpoint_->_pf_value_2;
        this->filterLen = _pf_script_state_checkpoint_->_pf_value_3;
        this->filterMult = _pf_script_state_checkpoint_->_pf_value_4;
        this->avgRange = _pf_script_state_checkpoint_->_pf_value_5;
        this->smoothRng = _pf_script_state_checkpoint_->_pf_value_6;
        this->hiTarget = _pf_script_state_checkpoint_->_pf_value_7;
        this->loTarget = _pf_script_state_checkpoint_->_pf_value_8;
        this->newFilter = _pf_script_state_checkpoint_->_pf_value_9;
        this->longCond = _pf_script_state_checkpoint_->_pf_value_10;
        this->shortCond = _pf_script_state_checkpoint_->_pf_value_11;
        this->_var_initialized = _pf_script_state_checkpoint_->_pf_value_12;
        this->_pf_var_init_rangeFilter = _pf_script_state_checkpoint_->_pf_value_13;
        this->_ta_initialized_ = _pf_script_state_checkpoint_->_pf_value_14;
        this->_inputs_initialized_ = _pf_script_state_checkpoint_->_pf_value_15;
    }

    void commit_script_state() override {
        snapshot_script_state();
    }

    explicit GeneratedStrategy() : _ta_ema_1(50) {
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
        this->_ta_ema_1 = decltype(this->_ta_ema_1)(50);
        this->_precalc__ta_ema_1 = decltype(this->_precalc__ta_ema_1){};
        this->_use_precalc = false;
        this->rangeFilter = na<double>();
        this->filterDir = decltype(this->filterDir){};
        this->filterLen = 0;
        this->filterMult = 0.0;
        this->avgRange = 0.0;
        this->smoothRng = 0.0;
        this->hiTarget = 0.0;
        this->loTarget = 0.0;
        this->newFilter = 0.0;
        this->longCond = false;
        this->shortCond = false;
        this->_var_initialized = false;
        this->_pf_var_init_rangeFilter = false;
        this->_ta_initialized_ = false;
        this->_inputs_initialized_ = false;
        if (allow_precalculation) precalculate(bars, n);
    }

    void on_source_bar(const Bar& bar) override {
        if (!_var_initialized) {
            filterDir.push(0);
            _var_initialized = true;
        } else {
            if (history_advances_new_bar()) filterDir.push(filterDir[0]);
            else filterDir.update(filterDir[0]);
        }
        if (!_inputs_initialized_) {
            filterLen = get_input_int("Filter Length", 50);
            filterMult = get_input_double("Range Multiplier", 2.5);
            _inputs_initialized_ = true;
        }
        if (!_ta_initialized_) {
            _ta_ema_1 = ta::EMA(get_input_int("Filter Length", 50));
            _ta_initialized_ = true;
        }
        avgRange = (history_advances_new_bar() ? _ta_ema_1.compute((current_bar_.high - current_bar_.low)) : _ta_ema_1.recompute((current_bar_.high - current_bar_.low)));
        smoothRng = (avgRange * filterMult);
        if (!_pf_var_init_rangeFilter) {
            rangeFilter = current_bar_.close;
            _pf_var_init_rangeFilter = true;
        }
        hiTarget = (rangeFilter + smoothRng);
        loTarget = (rangeFilter - smoothRng);
        if (([&]{ auto _pna_l = (current_bar_.close); auto _pna_r = (hiTarget); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) {
            rangeFilter = (current_bar_.close - smoothRng);
            filterDir.update(1);
        } else
        if (([&]{ auto _pna_l = (current_bar_.close); auto _pna_r = (loTarget); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }())) {
            rangeFilter = (current_bar_.close + smoothRng);
            filterDir.update((-1));
        } else
        if (([&]{ auto _pna_l = (filterDir[0]); auto _pna_r = (1); return !is_na(_pna_l) && !is_na(_pna_r) && (_pna_l == _pna_r); }())) {
            newFilter = (current_bar_.close - smoothRng);
            rangeFilter = ([&]() -> double { double _v0 = (double)(rangeFilter); double _v1 = (double)(newFilter); if (is_na(_v0) || is_na(_v1)) return na<double>(); double _out = _v0; _out = std::max(_out, _v1); return _out; }());
        } else
        if (([&]{ auto _pna_l = (filterDir[0]); auto _pna_r = ((-1)); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && (_pfc_eq); }())) {
            newFilter = (current_bar_.close + smoothRng);
            rangeFilter = ([&]() -> double { double _v0 = (double)(rangeFilter); double _v1 = (double)(newFilter); if (is_na(_v0) || is_na(_v1)) return na<double>(); double _out = _v0; _out = std::min(_out, _v1); return _out; }());
        }
        longCond = (([&]{ auto _pna_l = (filterDir[0]); auto _pna_r = (1); return !is_na(_pna_l) && !is_na(_pna_r) && (_pna_l == _pna_r); }()) && ([&]{ auto _pna_l = (filterDir[1]); auto _pna_r = (1); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && (!_pfc_eq); }()));
        shortCond = (([&]{ auto _pna_l = (filterDir[0]); auto _pna_r = ((-1)); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && (_pfc_eq); }()) && ([&]{ auto _pna_l = (filterDir[1]); auto _pna_r = ((-1)); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && (!_pfc_eq); }()));
        if (longCond) {
            strategy_entry(std::string("Long"), true, na<double>(), na<double>(), na<double>(), "");
        }
        if (shortCond) {
            strategy_entry(std::string("Short"), false, na<double>(), na<double>(), na<double>(), "");
        }
    }

    void precalculate(const Bar* bars, int n) {
        _use_precalc = false;
        if (n <= 0 || bars == nullptr) return;

        _precalc__ta_ema_1.resize(n);

        _ta_ema_1 = ta::EMA(get_input_int("Filter Length", 50));


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
            _precalc__ta_ema_1[i] = _ta_ema_1.compute((bars[i].high - bars[i].low));
        }

        _ta_ema_1 = ta::EMA(get_input_int("Filter Length", 50));

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
