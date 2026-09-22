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
    std::vector<double> _req_sec_lower_tf_0{};
    std::unordered_map<std::string, Series<double>> _security_helper_series_;
    ta::EMA _ta_ema_1;
    std::vector<double> _precalc__ta_ema_1;
    ta::EMA _ta_ema_2;
    std::vector<double> _precalc__ta_ema_2;
    ta::Crossover _ta_crossover_3;
    ta::Crossunder _ta_crossunder_4;
    bool _use_precalc = false;
    std::vector<double> ltfCloses;
    double ltfSum = 0.0;
    double drift = 0.0;
    double emaFast = 0.0;
    double emaSlow = 0.0;
    bool entryCond = false;
    bool exitCond = false;
    bool _ta_initialized_ = false;
    bool _inputs_initialized_ = false;

    struct _PFScriptState {
        decltype(GeneratedStrategy::_req_sec_lower_tf_0) _pf_value_0;
        decltype(GeneratedStrategy::_security_helper_series_) _pf_value_1;
        decltype(GeneratedStrategy::_ta_ema_1) _pf_value_2;
        decltype(GeneratedStrategy::_ta_ema_2) _pf_value_3;
        decltype(GeneratedStrategy::_ta_crossover_3) _pf_value_4;
        decltype(GeneratedStrategy::_ta_crossunder_4) _pf_value_5;
        decltype(GeneratedStrategy::ltfCloses) _pf_value_6;
        decltype(GeneratedStrategy::ltfSum) _pf_value_7;
        decltype(GeneratedStrategy::drift) _pf_value_8;
        decltype(GeneratedStrategy::emaFast) _pf_value_9;
        decltype(GeneratedStrategy::emaSlow) _pf_value_10;
        decltype(GeneratedStrategy::entryCond) _pf_value_11;
        decltype(GeneratedStrategy::exitCond) _pf_value_12;
        decltype(GeneratedStrategy::_ta_initialized_) _pf_value_13;
        decltype(GeneratedStrategy::_inputs_initialized_) _pf_value_14;
    };
    static_assert(std::is_copy_constructible_v<_PFScriptState>, "generated Pine state must be deep-copy constructible");
    static_assert(std::is_copy_assignable_v<_PFScriptState>, "generated Pine state must be deep-copy assignable");
    std::optional<_PFScriptState> _pf_script_state_checkpoint_;

    void snapshot_script_state() override {
        _pf_script_state_checkpoint_.emplace(_PFScriptState{
            _req_sec_lower_tf_0,
            _security_helper_series_,
            _ta_ema_1,
            _ta_ema_2,
            _ta_crossover_3,
            _ta_crossunder_4,
            ltfCloses,
            ltfSum,
            drift,
            emaFast,
            emaSlow,
            entryCond,
            exitCond,
            _ta_initialized_,
            _inputs_initialized_,
        });
    }

    void restore_script_state() override {
        if (!_pf_script_state_checkpoint_) return;
        this->_req_sec_lower_tf_0 = _pf_script_state_checkpoint_->_pf_value_0;
        this->_security_helper_series_ = _pf_script_state_checkpoint_->_pf_value_1;
        this->_ta_ema_1 = _pf_script_state_checkpoint_->_pf_value_2;
        this->_ta_ema_2 = _pf_script_state_checkpoint_->_pf_value_3;
        this->_ta_crossover_3 = _pf_script_state_checkpoint_->_pf_value_4;
        this->_ta_crossunder_4 = _pf_script_state_checkpoint_->_pf_value_5;
        this->ltfCloses = _pf_script_state_checkpoint_->_pf_value_6;
        this->ltfSum = _pf_script_state_checkpoint_->_pf_value_7;
        this->drift = _pf_script_state_checkpoint_->_pf_value_8;
        this->emaFast = _pf_script_state_checkpoint_->_pf_value_9;
        this->emaSlow = _pf_script_state_checkpoint_->_pf_value_10;
        this->entryCond = _pf_script_state_checkpoint_->_pf_value_11;
        this->exitCond = _pf_script_state_checkpoint_->_pf_value_12;
        this->_ta_initialized_ = _pf_script_state_checkpoint_->_pf_value_13;
        this->_inputs_initialized_ = _pf_script_state_checkpoint_->_pf_value_14;
    }

    void commit_script_state() override {
        snapshot_script_state();
    }

    explicit GeneratedStrategy() : _ta_ema_1(9), _ta_ema_2(21) {
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

    void configure_security_evaluators() override {
        security_eval_states_.clear();
        register_security_lower_tf_eval(0, "1", input_tf_);
    }

#ifndef PINEFORGE_HAS_SCRIPT_RUN_PREPARE_V1
#error "Generated lifecycle reset requires a matching PineForge engine; rebuild with script-run preparation support"
#endif
    void prepare_script_run(const Bar* bars, int n, bool allow_precalculation) override {
        _pf_script_state_checkpoint_.reset();
        this->_req_sec_lower_tf_0 = decltype(this->_req_sec_lower_tf_0){};
        this->_security_helper_series_ = decltype(this->_security_helper_series_){};
        this->_ta_ema_1 = decltype(this->_ta_ema_1)(9);
        this->_precalc__ta_ema_1 = decltype(this->_precalc__ta_ema_1){};
        this->_ta_ema_2 = decltype(this->_ta_ema_2)(21);
        this->_precalc__ta_ema_2 = decltype(this->_precalc__ta_ema_2){};
        this->_ta_crossover_3 = decltype(this->_ta_crossover_3){};
        this->_ta_crossunder_4 = decltype(this->_ta_crossunder_4){};
        this->_use_precalc = false;
        this->ltfCloses = decltype(this->ltfCloses){};
        this->ltfSum = 0.0;
        this->drift = 0.0;
        this->emaFast = 0.0;
        this->emaSlow = 0.0;
        this->entryCond = false;
        this->exitCond = false;
        this->_ta_initialized_ = false;
        this->_inputs_initialized_ = false;
        if (allow_precalculation) precalculate(bars, n);
    }

    void on_source_bar(const Bar& bar) override {
        ltfCloses = _req_sec_lower_tf_0;
        ltfSum = 0.0;
        if (([&]{ auto _pna_l = ((double)ltfCloses.size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) {
            int _for_start_0 = (0);
            int _for_end_0 = [&](){ double _pf_v = (double)((((double)ltfCloses.size() - 1))); return is_na(_pf_v) ? na<int>() : (int)_pf_v; }();
            int _for_step_0 = (1);
            if (!is_na(_for_step_0) && _for_step_0 < 0) _for_step_0 = -_for_step_0;
            if (_for_step_0 == 0) _for_step_0 = 1;
            const bool _for_down_0 = (_for_start_0 > _for_end_0);
            for (int i = _for_start_0; !is_na(_for_start_0) && !is_na(_for_end_0) && !is_na(_for_step_0) && (_for_down_0 ? (i >= _for_end_0) : (i <= _for_end_0)); i += (_for_down_0 ? -_for_step_0 : _for_step_0), _for_end_0 = [&](){ double _pf_v = (double)((((double)ltfCloses.size() - 1))); return is_na(_pf_v) ? na<int>() : (int)_pf_v; }()) {
                ltfSum = (ltfSum + [&](auto&& __pf_array)->decltype(auto){ return [&](auto&& __pf_raw_index_value)->decltype(auto){ using __pf_raw_index_type=std::decay_t<decltype(__pf_raw_index_value)>; if constexpr(!std::is_same_v<__pf_raw_index_type,bool>) { if(is_na(__pf_raw_index_value)) pine_runtime_error(std::string("Index na is out of bounds. Array size is ")+std::to_string((int64_t)__pf_array.size())); } if constexpr(std::is_floating_point_v<__pf_raw_index_type>) { if(!std::isfinite(__pf_raw_index_value)) { std::string __pf_raw_index_text=__pf_raw_index_value>0?"inf":"-inf"; pine_runtime_error(std::string("Index ")+__pf_raw_index_text+" is out of bounds. Array size is "+std::to_string((int64_t)__pf_array.size())); } long double __pf_raw_index_wide=(long double)__pf_raw_index_value; if(__pf_raw_index_wide<(long double)std::numeric_limits<int64_t>::min()||__pf_raw_index_wide>(long double)std::numeric_limits<int64_t>::max()) pine_runtime_error(std::string("Index ")+std::to_string((double)__pf_raw_index_value)+" is out of bounds. Array size is "+std::to_string((int64_t)__pf_array.size())); } int64_t __pf_raw_index=(int64_t)__pf_raw_index_value; int64_t __pf_array_size=(int64_t)__pf_array.size(); int64_t __pf_array_index=__pf_raw_index<0?__pf_raw_index+__pf_array_size:__pf_raw_index; if(__pf_array_index<0||__pf_array_index>=__pf_array_size) pine_runtime_error(std::string("Index ")+std::to_string(__pf_raw_index)+" is out of bounds. Array size is "+std::to_string(__pf_array_size)); if constexpr(std::is_lvalue_reference_v<decltype(__pf_array)>) return (__pf_array[(size_t)__pf_array_index]); else { using __pf_array_value_type=typename std::decay_t<decltype(__pf_array)>::value_type; return __pf_array_value_type(__pf_array[(size_t)__pf_array_index]); } }((i)); }((ltfCloses)));
            }
        }
        drift = (ltfSum - (15.0 * current_bar_.close));
        emaFast = (history_advances_new_bar() ? _ta_ema_1.compute(current_bar_.close) : _ta_ema_1.recompute(current_bar_.close));
        emaSlow = (history_advances_new_bar() ? _ta_ema_2.compute(current_bar_.close) : _ta_ema_2.recompute(current_bar_.close));
        entryCond = ((history_advances_new_bar() ? _ta_crossover_3.compute(emaFast, emaSlow) : _ta_crossover_3.recompute(emaFast, emaSlow)) && ([&]{ auto _pna_l = (std::abs(drift)); auto _pna_r = ((current_bar_.close * 0.001)); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()));
        exitCond = (history_advances_new_bar() ? _ta_crossunder_4.compute(emaFast, emaSlow) : _ta_crossunder_4.recompute(emaFast, emaSlow));
        if ((entryCond && ([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && (_pfc_eq); }()))) {
            strategy_entry(std::string("L"), true, na<double>(), na<double>(), 1, std::string("ltf-entry"), "", 0, -1);
        }
        if ((exitCond && ([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()))) {
            strategy_close(std::string("L"), std::string("ltf-exit"), na<double>(), na<double>(), false, 210453397523ULL);
        }
    }

    void precalculate(const Bar* bars, int n) {
        _use_precalc = false;
        if (n <= 0 || bars == nullptr) return;

        _precalc__ta_ema_1.resize(n);
        _precalc__ta_ema_2.resize(n);

        _ta_ema_1 = ta::EMA(9);
        _ta_ema_2 = ta::EMA(21);


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
        }

        _ta_ema_1 = ta::EMA(9);
        _ta_ema_2 = ta::EMA(21);

        _use_precalc = true;
    }


    void _eval_security_0(const Bar& bar, bool is_complete) {
        if (security_lower_tf_sub_bar_index(0) == 0) _req_sec_lower_tf_0.clear();
        _req_sec_lower_tf_0.push_back(bar.close);
    }

    void evaluate_security(int sec_id, const Bar& bar, bool is_complete) override {
        switch (sec_id) {
            case 0: _eval_security_0(bar, is_complete); break;
        }
    }
    void clear_security(int sec_id) override {
        switch (sec_id) {
            case 0:
                _req_sec_lower_tf_0.clear();
                break;
        }
    }
};

extern "C" {
    void* strategy_create(const char* params_json) {
        return new GeneratedStrategy();
    }
    void run_backtest(void* s, Bar* bars, int n, ReportC* out) {
        auto* strat = static_cast<GeneratedStrategy*>(s);
        strat->run(bars, n, "", "", false, 4, MagnifierDistribution::ENDPOINTS);
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
        strat->run(bars, n, itf, stf, bar_magnifier != 0, magnifier_samples,
                   static_cast<MagnifierDistribution>(magnifier_dist));
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
