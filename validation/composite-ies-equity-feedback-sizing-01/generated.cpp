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
    ta::DMI _ta_dmi_1;
    std::vector<ta::DMIResult> _precalc__ta_dmi_1;
    ta::ATR _ta_atr_2;
    std::vector<double> _precalc__ta_atr_2;
    bool _use_precalc = false;
    double entryStop;
    double entryTP;
    double i_risk_pct = 0.0;
    double i_atr_stop_mult = 0.0;
    double i_atr_tp_mult = 0.0;
    double i_adx_trend = 0.0;
    double i_trend_mult = 0.0;
    double i_neutral_mult = 0.0;
    double i_quality_mult = 0.0;
    double plus_di = 0.0;
    double minus_di = 0.0;
    double adx_val = 0.0;
    bool trending_regime = false;
    double regime_size_mult = 0.0;
    double atr_val = 0.0;
    double long_risk = 0.0;
    double account_risk = 0.0;
    double long_position_size = 0.0;
    bool long_entry = false;
    bool _var_initialized = false;
    bool _ta_initialized_ = false;
    bool _inputs_initialized_ = false;

    struct _PFScriptState {
        decltype(GeneratedStrategy::_ta_dmi_1) _pf_value_0;
        decltype(GeneratedStrategy::_ta_atr_2) _pf_value_1;
        decltype(GeneratedStrategy::entryStop) _pf_value_2;
        decltype(GeneratedStrategy::entryTP) _pf_value_3;
        decltype(GeneratedStrategy::i_risk_pct) _pf_value_4;
        decltype(GeneratedStrategy::i_atr_stop_mult) _pf_value_5;
        decltype(GeneratedStrategy::i_atr_tp_mult) _pf_value_6;
        decltype(GeneratedStrategy::i_adx_trend) _pf_value_7;
        decltype(GeneratedStrategy::i_trend_mult) _pf_value_8;
        decltype(GeneratedStrategy::i_neutral_mult) _pf_value_9;
        decltype(GeneratedStrategy::i_quality_mult) _pf_value_10;
        decltype(GeneratedStrategy::plus_di) _pf_value_11;
        decltype(GeneratedStrategy::minus_di) _pf_value_12;
        decltype(GeneratedStrategy::adx_val) _pf_value_13;
        decltype(GeneratedStrategy::trending_regime) _pf_value_14;
        decltype(GeneratedStrategy::regime_size_mult) _pf_value_15;
        decltype(GeneratedStrategy::atr_val) _pf_value_16;
        decltype(GeneratedStrategy::long_risk) _pf_value_17;
        decltype(GeneratedStrategy::account_risk) _pf_value_18;
        decltype(GeneratedStrategy::long_position_size) _pf_value_19;
        decltype(GeneratedStrategy::long_entry) _pf_value_20;
        decltype(GeneratedStrategy::_var_initialized) _pf_value_21;
        decltype(GeneratedStrategy::_ta_initialized_) _pf_value_22;
        decltype(GeneratedStrategy::_inputs_initialized_) _pf_value_23;
    };
    static_assert(std::is_copy_constructible_v<_PFScriptState>, "generated Pine state must be deep-copy constructible");
    static_assert(std::is_copy_assignable_v<_PFScriptState>, "generated Pine state must be deep-copy assignable");
    std::optional<_PFScriptState> _pf_script_state_checkpoint_;

    void snapshot_script_state() override {
        _pf_script_state_checkpoint_.emplace(_PFScriptState{
            _ta_dmi_1,
            _ta_atr_2,
            entryStop,
            entryTP,
            i_risk_pct,
            i_atr_stop_mult,
            i_atr_tp_mult,
            i_adx_trend,
            i_trend_mult,
            i_neutral_mult,
            i_quality_mult,
            plus_di,
            minus_di,
            adx_val,
            trending_regime,
            regime_size_mult,
            atr_val,
            long_risk,
            account_risk,
            long_position_size,
            long_entry,
            _var_initialized,
            _ta_initialized_,
            _inputs_initialized_,
        });
    }

    void restore_script_state() override {
        if (!_pf_script_state_checkpoint_) return;
        this->_ta_dmi_1 = _pf_script_state_checkpoint_->_pf_value_0;
        this->_ta_atr_2 = _pf_script_state_checkpoint_->_pf_value_1;
        this->entryStop = _pf_script_state_checkpoint_->_pf_value_2;
        this->entryTP = _pf_script_state_checkpoint_->_pf_value_3;
        this->i_risk_pct = _pf_script_state_checkpoint_->_pf_value_4;
        this->i_atr_stop_mult = _pf_script_state_checkpoint_->_pf_value_5;
        this->i_atr_tp_mult = _pf_script_state_checkpoint_->_pf_value_6;
        this->i_adx_trend = _pf_script_state_checkpoint_->_pf_value_7;
        this->i_trend_mult = _pf_script_state_checkpoint_->_pf_value_8;
        this->i_neutral_mult = _pf_script_state_checkpoint_->_pf_value_9;
        this->i_quality_mult = _pf_script_state_checkpoint_->_pf_value_10;
        this->plus_di = _pf_script_state_checkpoint_->_pf_value_11;
        this->minus_di = _pf_script_state_checkpoint_->_pf_value_12;
        this->adx_val = _pf_script_state_checkpoint_->_pf_value_13;
        this->trending_regime = _pf_script_state_checkpoint_->_pf_value_14;
        this->regime_size_mult = _pf_script_state_checkpoint_->_pf_value_15;
        this->atr_val = _pf_script_state_checkpoint_->_pf_value_16;
        this->long_risk = _pf_script_state_checkpoint_->_pf_value_17;
        this->account_risk = _pf_script_state_checkpoint_->_pf_value_18;
        this->long_position_size = _pf_script_state_checkpoint_->_pf_value_19;
        this->long_entry = _pf_script_state_checkpoint_->_pf_value_20;
        this->_var_initialized = _pf_script_state_checkpoint_->_pf_value_21;
        this->_ta_initialized_ = _pf_script_state_checkpoint_->_pf_value_22;
        this->_inputs_initialized_ = _pf_script_state_checkpoint_->_pf_value_23;
    }

    void commit_script_state() override {
        snapshot_script_state();
    }

    explicit GeneratedStrategy() : _ta_dmi_1(14, 14), _ta_atr_2(14), entryStop(na<double>()), entryTP(na<double>()) {
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
        this->_ta_dmi_1 = decltype(this->_ta_dmi_1)(14, 14);
        this->_precalc__ta_dmi_1 = decltype(this->_precalc__ta_dmi_1){};
        this->_ta_atr_2 = decltype(this->_ta_atr_2)(14);
        this->_precalc__ta_atr_2 = decltype(this->_precalc__ta_atr_2){};
        this->_use_precalc = false;
        this->entryStop = decltype(this->entryStop)(na<double>());
        this->entryTP = decltype(this->entryTP)(na<double>());
        this->i_risk_pct = 0.0;
        this->i_atr_stop_mult = 0.0;
        this->i_atr_tp_mult = 0.0;
        this->i_adx_trend = 0.0;
        this->i_trend_mult = 0.0;
        this->i_neutral_mult = 0.0;
        this->i_quality_mult = 0.0;
        this->plus_di = 0.0;
        this->minus_di = 0.0;
        this->adx_val = 0.0;
        this->trending_regime = false;
        this->regime_size_mult = 0.0;
        this->atr_val = 0.0;
        this->long_risk = 0.0;
        this->account_risk = 0.0;
        this->long_position_size = 0.0;
        this->long_entry = false;
        this->_var_initialized = false;
        this->_ta_initialized_ = false;
        this->_inputs_initialized_ = false;
        if (allow_precalculation) precalculate(bars, n);
    }

    void on_source_bar(const Bar& bar) override {
        if (!_var_initialized) {
            _var_initialized = true;
        } else {
        }
        if (!_inputs_initialized_) {
            i_risk_pct = get_input_double("Risk Per Trade %", 1.0);
            i_atr_stop_mult = get_input_double("ATR Stop Multiplier", 2.0);
            i_atr_tp_mult = get_input_double("ATR Target Multiplier", 4.0);
            i_adx_trend = get_input_double("ADX Trend Threshold", 25);
            i_trend_mult = get_input_double("Trending Size Mult", 1.2);
            i_neutral_mult = get_input_double("Neutral Size Mult", 1.0);
            i_quality_mult = get_input_double("Quality Mult (fixed)", 1.15);
            _inputs_initialized_ = true;
        }
        auto _result__ta_dmi_1 = (history_advances_new_bar() ? _ta_dmi_1.compute(current_bar_.high, current_bar_.low, current_bar_.close) : _ta_dmi_1.recompute(current_bar_.high, current_bar_.low, current_bar_.close));
        double plus_di = _result__ta_dmi_1.diplus;
        double minus_di = _result__ta_dmi_1.diminus;
        double adx_val = _result__ta_dmi_1.adx;
        trending_regime = ([&]{ auto _pna_l = (adx_val); auto _pna_r = (i_adx_trend); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) || _pfc_eq); }());
        regime_size_mult = ((trending_regime) ? (i_trend_mult) : (i_neutral_mult));
        atr_val = (history_advances_new_bar() ? _ta_atr_2.compute(current_bar_.high, current_bar_.low, current_bar_.close, prev_chart_close()) : _ta_atr_2.recompute(current_bar_.high, current_bar_.low, current_bar_.close, prev_chart_close()));
        long_risk = (atr_val * i_atr_stop_mult);
        account_risk = ((((current_equity() + open_profit(current_bar_.close)) * ((double)(i_risk_pct) / (double)(100.0))) * regime_size_mult) * i_quality_mult);
        long_position_size = ((([&]{ auto _pna_l = (long_risk); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) ? (((double)(account_risk) / (double)(long_risk))) : (0.0));
        long_entry = ((((([&]{ auto _pna_l = (pine_hour(current_bar_.timestamp, syminfo_.timezone)); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && (_pfc_eq); }()) && ([&]{ auto _pna_l = (pine_minute(current_bar_.timestamp, syminfo_.timezone)); auto _pna_r = (15); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && (_pfc_eq); }())) && ([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && (_pfc_eq); }())) && !(is_na(atr_val))) && !(is_na(adx_val)));
        if (long_entry) {
            entryStop = (current_bar_.close - (atr_val * i_atr_stop_mult));
            entryTP = (current_bar_.close + (atr_val * i_atr_tp_mult));
            strategy_entry(std::string("L"), true, na<double>(), na<double>(), long_position_size, std::string("dyn qty long"), "", 0, -1);
            strategy_exit(std::string("LX"), std::string("L"), entryTP, entryStop, na<double>(), na<double>(), na<double>(), 100.0, std::string("bracket"), na<double>(), "", na<double>(), na<double>());
        }
        if (trace_enabled_) {
            trace(std::string("ies_equity"), (double)((current_equity() + open_profit(current_bar_.close))));
            trace(std::string("ies_accountRisk"), (double)(account_risk));
            trace(std::string("ies_longRisk"), (double)(long_risk));
            trace(std::string("ies_qty"), (double)(long_position_size));
            trace(std::string("ies_longEntry"), (double)(long_entry));
            trace(std::string("ies_atr"), (double)(atr_val));
            trace(std::string("ies_adx"), (double)(adx_val));
        }
    }

    void precalculate(const Bar* bars, int n) {
        _use_precalc = false;
        if (n <= 0 || bars == nullptr) return;

        _precalc__ta_dmi_1.resize(n);
        _precalc__ta_atr_2.resize(n);

        _ta_dmi_1 = ta::DMI(14, 14);
        _ta_atr_2 = ta::ATR(14);


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
            _precalc__ta_dmi_1[i] = _ta_dmi_1.compute(bars[i].high, bars[i].low, bars[i].close);
            _precalc__ta_atr_2[i] = _ta_atr_2.compute(bars[i].high, bars[i].low, bars[i].close, (i > 0 ? bars[i - 1].close : na<double>()));
        }

        _ta_dmi_1 = ta::DMI(14, 14);
        _ta_atr_2 = ta::ATR(14);

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
