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
    ta::EMA _ta_ema_2;
    std::vector<double> _precalc__ta_ema_2;
    ta::ATR _ta_atr_3;
    std::vector<double> _precalc__ta_atr_3;
    ta::Crossover _ta_crossover_4;
    ta::Crossunder _ta_crossunder_5;
    bool _use_precalc = false;
    int fastLength = 0;
    int slowLength = 0;
    int atrLength = 0;
    double stopAtr = 0.0;
    double targetAtr = 0.0;
    bool allowShort = false;
    double fastEma = 0.0;
    double slowEma = 0.0;
    double atrValue = 0.0;
    bool longSignal = false;
    bool shortSignal = false;
    double longStop = 0.0;
    double longTarget = 0.0;
    double shortStop = 0.0;
    double shortTarget = 0.0;
    bool _ta_initialized_ = false;
    bool _inputs_initialized_ = false;

    struct _PFScriptState {
        decltype(GeneratedStrategy::_ta_ema_1) _pf_value_0;
        decltype(GeneratedStrategy::_ta_ema_2) _pf_value_1;
        decltype(GeneratedStrategy::_ta_atr_3) _pf_value_2;
        decltype(GeneratedStrategy::_ta_crossover_4) _pf_value_3;
        decltype(GeneratedStrategy::_ta_crossunder_5) _pf_value_4;
        decltype(GeneratedStrategy::fastLength) _pf_value_5;
        decltype(GeneratedStrategy::slowLength) _pf_value_6;
        decltype(GeneratedStrategy::atrLength) _pf_value_7;
        decltype(GeneratedStrategy::stopAtr) _pf_value_8;
        decltype(GeneratedStrategy::targetAtr) _pf_value_9;
        decltype(GeneratedStrategy::allowShort) _pf_value_10;
        decltype(GeneratedStrategy::fastEma) _pf_value_11;
        decltype(GeneratedStrategy::slowEma) _pf_value_12;
        decltype(GeneratedStrategy::atrValue) _pf_value_13;
        decltype(GeneratedStrategy::longSignal) _pf_value_14;
        decltype(GeneratedStrategy::shortSignal) _pf_value_15;
        decltype(GeneratedStrategy::longStop) _pf_value_16;
        decltype(GeneratedStrategy::longTarget) _pf_value_17;
        decltype(GeneratedStrategy::shortStop) _pf_value_18;
        decltype(GeneratedStrategy::shortTarget) _pf_value_19;
        decltype(GeneratedStrategy::_ta_initialized_) _pf_value_20;
        decltype(GeneratedStrategy::_inputs_initialized_) _pf_value_21;
    };
    static_assert(std::is_copy_constructible_v<_PFScriptState>, "generated Pine state must be deep-copy constructible");
    static_assert(std::is_copy_assignable_v<_PFScriptState>, "generated Pine state must be deep-copy assignable");
    std::optional<_PFScriptState> _pf_script_state_checkpoint_;

    void snapshot_script_state() override {
        _pf_script_state_checkpoint_.emplace(_PFScriptState{
            _ta_ema_1,
            _ta_ema_2,
            _ta_atr_3,
            _ta_crossover_4,
            _ta_crossunder_5,
            fastLength,
            slowLength,
            atrLength,
            stopAtr,
            targetAtr,
            allowShort,
            fastEma,
            slowEma,
            atrValue,
            longSignal,
            shortSignal,
            longStop,
            longTarget,
            shortStop,
            shortTarget,
            _ta_initialized_,
            _inputs_initialized_,
        });
    }

    void restore_script_state() override {
        if (!_pf_script_state_checkpoint_) return;
        this->_ta_ema_1 = _pf_script_state_checkpoint_->_pf_value_0;
        this->_ta_ema_2 = _pf_script_state_checkpoint_->_pf_value_1;
        this->_ta_atr_3 = _pf_script_state_checkpoint_->_pf_value_2;
        this->_ta_crossover_4 = _pf_script_state_checkpoint_->_pf_value_3;
        this->_ta_crossunder_5 = _pf_script_state_checkpoint_->_pf_value_4;
        this->fastLength = _pf_script_state_checkpoint_->_pf_value_5;
        this->slowLength = _pf_script_state_checkpoint_->_pf_value_6;
        this->atrLength = _pf_script_state_checkpoint_->_pf_value_7;
        this->stopAtr = _pf_script_state_checkpoint_->_pf_value_8;
        this->targetAtr = _pf_script_state_checkpoint_->_pf_value_9;
        this->allowShort = _pf_script_state_checkpoint_->_pf_value_10;
        this->fastEma = _pf_script_state_checkpoint_->_pf_value_11;
        this->slowEma = _pf_script_state_checkpoint_->_pf_value_12;
        this->atrValue = _pf_script_state_checkpoint_->_pf_value_13;
        this->longSignal = _pf_script_state_checkpoint_->_pf_value_14;
        this->shortSignal = _pf_script_state_checkpoint_->_pf_value_15;
        this->longStop = _pf_script_state_checkpoint_->_pf_value_16;
        this->longTarget = _pf_script_state_checkpoint_->_pf_value_17;
        this->shortStop = _pf_script_state_checkpoint_->_pf_value_18;
        this->shortTarget = _pf_script_state_checkpoint_->_pf_value_19;
        this->_ta_initialized_ = _pf_script_state_checkpoint_->_pf_value_20;
        this->_inputs_initialized_ = _pf_script_state_checkpoint_->_pf_value_21;
    }

    void commit_script_state() override {
        snapshot_script_state();
    }

    explicit GeneratedStrategy() : _ta_ema_1(18), _ta_ema_2(47), _ta_atr_3(16) {
#if defined(PINEFORGE_HAS_EXPLICIT_PINE_EXECUTION_ADAPTER_V1)
        pineforge::source::PineStrategyHost::attach_pine_execution_adapter();
#elif defined(PINEFORGE_HAS_EXPLICIT_PINE_CAP_V1)
        pineforge::source::PineStrategyHost::enable_pine_intraday_cap();
#endif
        pineforge::source::PineStrategyConfig cfg{};
        cfg.initial_capital = 100000.0;
        cfg.default_qty_type = static_cast<int>(QtyType::PERCENT_OF_EQUITY);
        cfg.default_qty_value = 10.0;
        cfg.pyramiding = 0;
        cfg.commission_type = static_cast<int>(CommissionType::PERCENT);
        cfg.commission_value = 0.05;
        cfg.slippage = 1;
        cfg.margin_long = 100.0;
        cfg.margin_short = 100.0;
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
        this->_ta_ema_1 = decltype(this->_ta_ema_1)(18);
        this->_precalc__ta_ema_1 = decltype(this->_precalc__ta_ema_1){};
        this->_ta_ema_2 = decltype(this->_ta_ema_2)(47);
        this->_precalc__ta_ema_2 = decltype(this->_precalc__ta_ema_2){};
        this->_ta_atr_3 = decltype(this->_ta_atr_3)(16);
        this->_precalc__ta_atr_3 = decltype(this->_precalc__ta_atr_3){};
        this->_ta_crossover_4 = decltype(this->_ta_crossover_4){};
        this->_ta_crossunder_5 = decltype(this->_ta_crossunder_5){};
        this->_use_precalc = false;
        this->fastLength = 0;
        this->slowLength = 0;
        this->atrLength = 0;
        this->stopAtr = 0.0;
        this->targetAtr = 0.0;
        this->allowShort = false;
        this->fastEma = 0.0;
        this->slowEma = 0.0;
        this->atrValue = 0.0;
        this->longSignal = false;
        this->shortSignal = false;
        this->longStop = 0.0;
        this->longTarget = 0.0;
        this->shortStop = 0.0;
        this->shortTarget = 0.0;
        this->_ta_initialized_ = false;
        this->_inputs_initialized_ = false;
        if (allow_precalculation) precalculate(bars, n);
    }

    void on_source_bar(const Bar& bar) override {
        if (!_inputs_initialized_) {
            fastLength = get_input_int("Fast EMA", 18);
            slowLength = get_input_int("Slow EMA", 47);
            atrLength = get_input_int("ATR Length", 16);
            stopAtr = get_input_double("Stop ATR", 1.8);
            targetAtr = get_input_double("Target ATR", 2.7);
            allowShort = get_input_bool("Allow Short", true);
            _inputs_initialized_ = true;
        }
        if (!_ta_initialized_) {
            _ta_ema_1 = ta::EMA(get_input_int("Fast EMA", 18));
            _ta_ema_2 = ta::EMA(get_input_int("Slow EMA", 47));
            _ta_atr_3 = ta::ATR(get_input_int("ATR Length", 16));
            _ta_initialized_ = true;
        }
        fastEma = (history_advances_new_bar() ? _ta_ema_1.compute(current_bar_.close) : _ta_ema_1.recompute(current_bar_.close));
        slowEma = (history_advances_new_bar() ? _ta_ema_2.compute(current_bar_.close) : _ta_ema_2.recompute(current_bar_.close));
        atrValue = (history_advances_new_bar() ? _ta_atr_3.compute(current_bar_.high, current_bar_.low, current_bar_.close, prev_chart_close()) : _ta_atr_3.recompute(current_bar_.high, current_bar_.low, current_bar_.close, prev_chart_close()));
        longSignal = (history_advances_new_bar() ? _ta_crossover_4.compute(fastEma, slowEma) : _ta_crossover_4.recompute(fastEma, slowEma));
        shortSignal = (allowShort && (history_advances_new_bar() ? _ta_crossunder_5.compute(fastEma, slowEma) : _ta_crossunder_5.recompute(fastEma, slowEma)));
        if ([&](){ auto _pf_bool_v = (longSignal); using _pf_bool_t = std::decay_t<decltype(_pf_bool_v)>; if constexpr (std::is_same_v<_pf_bool_t, bool>) { return _pf_bool_v; } else if constexpr (std::is_floating_point_v<_pf_bool_t> || std::is_integral_v<_pf_bool_t>) { return is_na(_pf_bool_v) ? false : (_pf_bool_v != 0); } else { return static_cast<bool>(_pf_bool_v); } }()) {
            strategy_entry(std::string("Aurora Long"), true, na<double>(), na<double>(), na<double>(), "");
        }
        if (shortSignal) {
            strategy_entry(std::string("Aurora Short"), false, na<double>(), na<double>(), na<double>(), "");
        }
        if (([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) {
            longStop = ((signed_position_size() == 0.0 ? na<double>() : position_entry_price_) - (atrValue * stopAtr));
            longTarget = ((signed_position_size() == 0.0 ? na<double>() : position_entry_price_) + (atrValue * targetAtr));
            strategy_exit(std::string("Aurora Long Risk"), std::string("Aurora Long"), longTarget, longStop, na<double>(), na<double>(), na<double>(), 100.0, "", na<double>(), "", na<double>(), na<double>());
        }
        if (([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }())) {
            shortStop = ((signed_position_size() == 0.0 ? na<double>() : position_entry_price_) + (atrValue * stopAtr));
            shortTarget = ((signed_position_size() == 0.0 ? na<double>() : position_entry_price_) - (atrValue * targetAtr));
            strategy_exit(std::string("Aurora Short Risk"), std::string("Aurora Short"), shortTarget, shortStop, na<double>(), na<double>(), na<double>(), 100.0, "", na<double>(), "", na<double>(), na<double>());
        }
    }

    void precalculate(const Bar* bars, int n) {
        _use_precalc = false;
        if (n <= 0 || bars == nullptr) return;

        _precalc__ta_ema_1.resize(n);
        _precalc__ta_ema_2.resize(n);
        _precalc__ta_atr_3.resize(n);

        _ta_ema_1 = ta::EMA(get_input_int("Fast EMA", 18));
        _ta_ema_2 = ta::EMA(get_input_int("Slow EMA", 47));
        _ta_atr_3 = ta::ATR(get_input_int("ATR Length", 16));


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
            _precalc__ta_ema_1[i] = _ta_ema_1.compute(bars[i].close);
            _precalc__ta_ema_2[i] = _ta_ema_2.compute(bars[i].close);
            _precalc__ta_atr_3[i] = _ta_atr_3.compute(bars[i].high, bars[i].low, bars[i].close, (i > 0 ? bars[i - 1].close : na<double>()));
        }

        _ta_ema_1 = ta::EMA(get_input_int("Fast EMA", 18));
        _ta_ema_2 = ta::EMA(get_input_int("Slow EMA", 47));
        _ta_atr_3 = ta::ATR(get_input_int("ATR Length", 16));

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
