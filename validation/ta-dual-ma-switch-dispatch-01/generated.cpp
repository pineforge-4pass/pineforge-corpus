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
    ta::SMA _ta_sma_2;
    ta::WMA _ta_wma_3;
    ta::HMA _ta_hma_4;
    ta::SMA _ta_sma_5;
    ta::EMA _ta_ema_1_cs1;
    ta::SMA _ta_sma_2_cs1;
    ta::WMA _ta_wma_3_cs1;
    ta::HMA _ta_hma_4_cs1;
    ta::SMA _ta_sma_5_cs1;
    ta::Crossover _ta_crossover_6;
    ta::Crossunder _ta_crossunder_7;
    bool _use_precalc = false;
    std::string maType = std::string("");
    int fastLen = 0;
    int slowLen = 0;
    double src = 0.0;
    double fastMA = 0.0;
    double slowMA = 0.0;
    bool longCond = false;
    bool shortCond = false;
    bool _ta_initialized_ = false;
    bool _inputs_initialized_ = false;

    struct _PFScriptState {
        decltype(GeneratedStrategy::_ta_ema_1) _pf_value_0;
        decltype(GeneratedStrategy::_ta_sma_2) _pf_value_1;
        decltype(GeneratedStrategy::_ta_wma_3) _pf_value_2;
        decltype(GeneratedStrategy::_ta_hma_4) _pf_value_3;
        decltype(GeneratedStrategy::_ta_sma_5) _pf_value_4;
        decltype(GeneratedStrategy::_ta_ema_1_cs1) _pf_value_5;
        decltype(GeneratedStrategy::_ta_sma_2_cs1) _pf_value_6;
        decltype(GeneratedStrategy::_ta_wma_3_cs1) _pf_value_7;
        decltype(GeneratedStrategy::_ta_hma_4_cs1) _pf_value_8;
        decltype(GeneratedStrategy::_ta_sma_5_cs1) _pf_value_9;
        decltype(GeneratedStrategy::_ta_crossover_6) _pf_value_10;
        decltype(GeneratedStrategy::_ta_crossunder_7) _pf_value_11;
        decltype(GeneratedStrategy::maType) _pf_value_12;
        decltype(GeneratedStrategy::fastLen) _pf_value_13;
        decltype(GeneratedStrategy::slowLen) _pf_value_14;
        decltype(GeneratedStrategy::src) _pf_value_15;
        decltype(GeneratedStrategy::fastMA) _pf_value_16;
        decltype(GeneratedStrategy::slowMA) _pf_value_17;
        decltype(GeneratedStrategy::longCond) _pf_value_18;
        decltype(GeneratedStrategy::shortCond) _pf_value_19;
        decltype(GeneratedStrategy::_ta_initialized_) _pf_value_20;
        decltype(GeneratedStrategy::_inputs_initialized_) _pf_value_21;
    };
    static_assert(std::is_copy_constructible_v<_PFScriptState>, "generated Pine state must be deep-copy constructible");
    static_assert(std::is_copy_assignable_v<_PFScriptState>, "generated Pine state must be deep-copy assignable");
    std::optional<_PFScriptState> _pf_script_state_checkpoint_;

    void snapshot_script_state() override {
        _pf_script_state_checkpoint_.emplace(_PFScriptState{
            _ta_ema_1,
            _ta_sma_2,
            _ta_wma_3,
            _ta_hma_4,
            _ta_sma_5,
            _ta_ema_1_cs1,
            _ta_sma_2_cs1,
            _ta_wma_3_cs1,
            _ta_hma_4_cs1,
            _ta_sma_5_cs1,
            _ta_crossover_6,
            _ta_crossunder_7,
            maType,
            fastLen,
            slowLen,
            src,
            fastMA,
            slowMA,
            longCond,
            shortCond,
            _ta_initialized_,
            _inputs_initialized_,
        });
    }

    void restore_script_state() override {
        if (!_pf_script_state_checkpoint_) return;
        this->_ta_ema_1 = _pf_script_state_checkpoint_->_pf_value_0;
        this->_ta_sma_2 = _pf_script_state_checkpoint_->_pf_value_1;
        this->_ta_wma_3 = _pf_script_state_checkpoint_->_pf_value_2;
        this->_ta_hma_4 = _pf_script_state_checkpoint_->_pf_value_3;
        this->_ta_sma_5 = _pf_script_state_checkpoint_->_pf_value_4;
        this->_ta_ema_1_cs1 = _pf_script_state_checkpoint_->_pf_value_5;
        this->_ta_sma_2_cs1 = _pf_script_state_checkpoint_->_pf_value_6;
        this->_ta_wma_3_cs1 = _pf_script_state_checkpoint_->_pf_value_7;
        this->_ta_hma_4_cs1 = _pf_script_state_checkpoint_->_pf_value_8;
        this->_ta_sma_5_cs1 = _pf_script_state_checkpoint_->_pf_value_9;
        this->_ta_crossover_6 = _pf_script_state_checkpoint_->_pf_value_10;
        this->_ta_crossunder_7 = _pf_script_state_checkpoint_->_pf_value_11;
        this->maType = _pf_script_state_checkpoint_->_pf_value_12;
        this->fastLen = _pf_script_state_checkpoint_->_pf_value_13;
        this->slowLen = _pf_script_state_checkpoint_->_pf_value_14;
        this->src = _pf_script_state_checkpoint_->_pf_value_15;
        this->fastMA = _pf_script_state_checkpoint_->_pf_value_16;
        this->slowMA = _pf_script_state_checkpoint_->_pf_value_17;
        this->longCond = _pf_script_state_checkpoint_->_pf_value_18;
        this->shortCond = _pf_script_state_checkpoint_->_pf_value_19;
        this->_ta_initialized_ = _pf_script_state_checkpoint_->_pf_value_20;
        this->_inputs_initialized_ = _pf_script_state_checkpoint_->_pf_value_21;
    }

    void commit_script_state() override {
        snapshot_script_state();
    }

    explicit GeneratedStrategy() : _ta_ema_1(10), _ta_sma_2(10), _ta_wma_3(10), _ta_hma_4(10), _ta_sma_5(10), _ta_ema_1_cs1(30), _ta_sma_2_cs1(30), _ta_wma_3_cs1(30), _ta_hma_4_cs1(30), _ta_sma_5_cs1(30) {
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
        cfg.src_series_active = true;
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
        this->_ta_ema_1 = decltype(this->_ta_ema_1)(10);
        this->_ta_sma_2 = decltype(this->_ta_sma_2)(10);
        this->_ta_wma_3 = decltype(this->_ta_wma_3)(10);
        this->_ta_hma_4 = decltype(this->_ta_hma_4)(10);
        this->_ta_sma_5 = decltype(this->_ta_sma_5)(10);
        this->_ta_ema_1_cs1 = decltype(this->_ta_ema_1_cs1)(30);
        this->_ta_sma_2_cs1 = decltype(this->_ta_sma_2_cs1)(30);
        this->_ta_wma_3_cs1 = decltype(this->_ta_wma_3_cs1)(30);
        this->_ta_hma_4_cs1 = decltype(this->_ta_hma_4_cs1)(30);
        this->_ta_sma_5_cs1 = decltype(this->_ta_sma_5_cs1)(30);
        this->_ta_crossover_6 = decltype(this->_ta_crossover_6){};
        this->_ta_crossunder_7 = decltype(this->_ta_crossunder_7){};
        this->_use_precalc = false;
        this->maType = std::string("");
        this->fastLen = 0;
        this->slowLen = 0;
        this->src = 0.0;
        this->fastMA = 0.0;
        this->slowMA = 0.0;
        this->longCond = false;
        this->shortCond = false;
        this->_ta_initialized_ = false;
        this->_inputs_initialized_ = false;
        (void)bars; (void)n; (void)allow_precalculation;
    }

    double getMA_cs0(double source, int64_t length) {
        double _func_ret = 0.0;
        auto __switch_val_0 = maType;
        if (__switch_val_0 == std::string("EMA")) {
            _func_ret = (history_advances_new_bar() ? _ta_ema_1.compute(source) : _ta_ema_1.recompute(source));
        }
        else if (__switch_val_0 == std::string("SMA")) {
            _func_ret = (history_advances_new_bar() ? _ta_sma_2.compute(source) : _ta_sma_2.recompute(source));
        }
        else if (__switch_val_0 == std::string("WMA")) {
            _func_ret = (history_advances_new_bar() ? _ta_wma_3.compute(source) : _ta_wma_3.recompute(source));
        }
        else if (__switch_val_0 == std::string("HMA")) {
            _func_ret = (history_advances_new_bar() ? _ta_hma_4.compute(source) : _ta_hma_4.recompute(source));
        }
        else {
            _func_ret = (history_advances_new_bar() ? _ta_sma_5.compute(source) : _ta_sma_5.recompute(source));
        }
        return _func_ret;
    }

    double getMA_cs1(double source, int64_t length) {
        double _func_ret = 0.0;
        auto __switch_val_1 = maType;
        if (__switch_val_1 == std::string("EMA")) {
            _func_ret = (history_advances_new_bar() ? _ta_ema_1_cs1.compute(source) : _ta_ema_1_cs1.recompute(source));
        }
        else if (__switch_val_1 == std::string("SMA")) {
            _func_ret = (history_advances_new_bar() ? _ta_sma_2_cs1.compute(source) : _ta_sma_2_cs1.recompute(source));
        }
        else if (__switch_val_1 == std::string("WMA")) {
            _func_ret = (history_advances_new_bar() ? _ta_wma_3_cs1.compute(source) : _ta_wma_3_cs1.recompute(source));
        }
        else if (__switch_val_1 == std::string("HMA")) {
            _func_ret = (history_advances_new_bar() ? _ta_hma_4_cs1.compute(source) : _ta_hma_4_cs1.recompute(source));
        }
        else {
            _func_ret = (history_advances_new_bar() ? _ta_sma_5_cs1.compute(source) : _ta_sma_5_cs1.recompute(source));
        }
        return _func_ret;
    }

    void on_source_bar(const Bar& bar) override {
        if (!_inputs_initialized_) {
            maType = get_input_string("MA Type", std::string("EMA"));
            fastLen = get_input_int("Fast Length", 10);
            slowLen = get_input_int("Slow Length", 30);
            _inputs_initialized_ = true;
        }
        if (!_ta_initialized_) {
            _ta_ema_1 = ta::EMA(get_input_int("Fast Length", 10));
            _ta_sma_2 = ta::SMA(get_input_int("Fast Length", 10));
            _ta_wma_3 = ta::WMA(get_input_int("Fast Length", 10));
            _ta_hma_4 = ta::HMA(get_input_int("Fast Length", 10));
            _ta_sma_5 = ta::SMA(get_input_int("Fast Length", 10));
            _ta_ema_1_cs1 = ta::EMA(get_input_int("Slow Length", 30));
            _ta_sma_2_cs1 = ta::SMA(get_input_int("Slow Length", 30));
            _ta_wma_3_cs1 = ta::WMA(get_input_int("Slow Length", 30));
            _ta_hma_4_cs1 = ta::HMA(get_input_int("Slow Length", 30));
            _ta_sma_5_cs1 = ta::SMA(get_input_int("Slow Length", 30));
            _ta_initialized_ = true;
        }
        src = get_input_source("Source", _src_close_)[0];
        fastMA = getMA_cs0(src, [&](){ auto _pf_v = (fastLen); return is_na(_pf_v) ? na<int64_t>() : (int64_t)_pf_v; }());
        slowMA = getMA_cs1(src, [&](){ auto _pf_v = (slowLen); return is_na(_pf_v) ? na<int64_t>() : (int64_t)_pf_v; }());
        longCond = (history_advances_new_bar() ? _ta_crossover_6.compute(fastMA, slowMA) : _ta_crossover_6.recompute(fastMA, slowMA));
        shortCond = (history_advances_new_bar() ? _ta_crossunder_7.compute(fastMA, slowMA) : _ta_crossunder_7.recompute(fastMA, slowMA));
        if ([&](){ auto _pf_bool_v = (longCond); using _pf_bool_t = std::decay_t<decltype(_pf_bool_v)>; if constexpr (std::is_same_v<_pf_bool_t, bool>) { return _pf_bool_v; } else if constexpr (std::is_floating_point_v<_pf_bool_t> || std::is_integral_v<_pf_bool_t>) { return is_na(_pf_bool_v) ? false : (_pf_bool_v != 0); } else { return static_cast<bool>(_pf_bool_v); } }()) {
            strategy_entry(std::string("Long"), true, na<double>(), na<double>(), na<double>(), "");
        }
        if ([&](){ auto _pf_bool_v = (shortCond); using _pf_bool_t = std::decay_t<decltype(_pf_bool_v)>; if constexpr (std::is_same_v<_pf_bool_t, bool>) { return _pf_bool_v; } else if constexpr (std::is_floating_point_v<_pf_bool_t> || std::is_integral_v<_pf_bool_t>) { return is_na(_pf_bool_v) ? false : (_pf_bool_v != 0); } else { return static_cast<bool>(_pf_bool_v); } }()) {
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
