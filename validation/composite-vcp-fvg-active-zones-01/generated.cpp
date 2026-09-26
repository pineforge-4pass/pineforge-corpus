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
    ta::ATR _ta_atr_1;
    std::vector<double> _precalc__ta_atr_1;
    bool _use_precalc = false;
    Series<double> _s_close;
    Series<double> _s_high;
    Series<double> _s_low;
    Series<double> _s_open;
    std::vector<double> fvgTops;
    std::vector<double> fvgBottoms;
    std::vector<bool> fvgBullish;
    double atrVal = 0.0;
    bool bullFVG = false;
    bool bearFVG = false;
    double minFVGSize = 0.0;
    double fvgTop = 0.0;
    double fvgBottom = 0.0;
    bool inBullFVG = false;
    bool inBearFVG = false;
    bool _var_initialized = false;
    bool _ta_initialized_ = false;
    bool _inputs_initialized_ = false;

    struct _PFScriptState {
        decltype(GeneratedStrategy::_ta_atr_1) _pf_value_0;
        decltype(GeneratedStrategy::_s_close) _pf_value_1;
        decltype(GeneratedStrategy::_s_high) _pf_value_2;
        decltype(GeneratedStrategy::_s_low) _pf_value_3;
        decltype(GeneratedStrategy::_s_open) _pf_value_4;
        decltype(GeneratedStrategy::fvgTops) _pf_value_5;
        decltype(GeneratedStrategy::fvgBottoms) _pf_value_6;
        decltype(GeneratedStrategy::fvgBullish) _pf_value_7;
        decltype(GeneratedStrategy::atrVal) _pf_value_8;
        decltype(GeneratedStrategy::bullFVG) _pf_value_9;
        decltype(GeneratedStrategy::bearFVG) _pf_value_10;
        decltype(GeneratedStrategy::minFVGSize) _pf_value_11;
        decltype(GeneratedStrategy::fvgTop) _pf_value_12;
        decltype(GeneratedStrategy::fvgBottom) _pf_value_13;
        decltype(GeneratedStrategy::inBullFVG) _pf_value_14;
        decltype(GeneratedStrategy::inBearFVG) _pf_value_15;
        decltype(GeneratedStrategy::_var_initialized) _pf_value_16;
        decltype(GeneratedStrategy::_ta_initialized_) _pf_value_17;
        decltype(GeneratedStrategy::_inputs_initialized_) _pf_value_18;
    };
    static_assert(std::is_copy_constructible_v<_PFScriptState>, "generated Pine state must be deep-copy constructible");
    static_assert(std::is_copy_assignable_v<_PFScriptState>, "generated Pine state must be deep-copy assignable");
    std::optional<_PFScriptState> _pf_script_state_checkpoint_;

    void snapshot_script_state() override {
        _pf_script_state_checkpoint_.emplace(_PFScriptState{
            _ta_atr_1,
            _s_close,
            _s_high,
            _s_low,
            _s_open,
            fvgTops,
            fvgBottoms,
            fvgBullish,
            atrVal,
            bullFVG,
            bearFVG,
            minFVGSize,
            fvgTop,
            fvgBottom,
            inBullFVG,
            inBearFVG,
            _var_initialized,
            _ta_initialized_,
            _inputs_initialized_,
        });
    }

    void restore_script_state() override {
        if (!_pf_script_state_checkpoint_) return;
        this->_ta_atr_1 = _pf_script_state_checkpoint_->_pf_value_0;
        this->_s_close = _pf_script_state_checkpoint_->_pf_value_1;
        this->_s_high = _pf_script_state_checkpoint_->_pf_value_2;
        this->_s_low = _pf_script_state_checkpoint_->_pf_value_3;
        this->_s_open = _pf_script_state_checkpoint_->_pf_value_4;
        this->fvgTops = _pf_script_state_checkpoint_->_pf_value_5;
        this->fvgBottoms = _pf_script_state_checkpoint_->_pf_value_6;
        this->fvgBullish = _pf_script_state_checkpoint_->_pf_value_7;
        this->atrVal = _pf_script_state_checkpoint_->_pf_value_8;
        this->bullFVG = _pf_script_state_checkpoint_->_pf_value_9;
        this->bearFVG = _pf_script_state_checkpoint_->_pf_value_10;
        this->minFVGSize = _pf_script_state_checkpoint_->_pf_value_11;
        this->fvgTop = _pf_script_state_checkpoint_->_pf_value_12;
        this->fvgBottom = _pf_script_state_checkpoint_->_pf_value_13;
        this->inBullFVG = _pf_script_state_checkpoint_->_pf_value_14;
        this->inBearFVG = _pf_script_state_checkpoint_->_pf_value_15;
        this->_var_initialized = _pf_script_state_checkpoint_->_pf_value_16;
        this->_ta_initialized_ = _pf_script_state_checkpoint_->_pf_value_17;
        this->_inputs_initialized_ = _pf_script_state_checkpoint_->_pf_value_18;
    }

    void commit_script_state() override {
        snapshot_script_state();
    }

    explicit GeneratedStrategy() : _ta_atr_1(14) {
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
        this->_ta_atr_1 = decltype(this->_ta_atr_1)(14);
        this->_precalc__ta_atr_1 = decltype(this->_precalc__ta_atr_1){};
        this->_use_precalc = false;
        this->_s_close = decltype(this->_s_close){};
        this->_s_high = decltype(this->_s_high){};
        this->_s_low = decltype(this->_s_low){};
        this->_s_open = decltype(this->_s_open){};
        this->fvgTops = decltype(this->fvgTops){};
        this->fvgBottoms = decltype(this->fvgBottoms){};
        this->fvgBullish = decltype(this->fvgBullish){};
        this->atrVal = 0.0;
        this->bullFVG = false;
        this->bearFVG = false;
        this->minFVGSize = 0.0;
        this->fvgTop = 0.0;
        this->fvgBottom = 0.0;
        this->inBullFVG = false;
        this->inBearFVG = false;
        this->_var_initialized = false;
        this->_ta_initialized_ = false;
        this->_inputs_initialized_ = false;
        if (allow_precalculation) precalculate(bars, n);
    }

    void on_source_bar(const Bar& bar) override {
        if (history_advances_new_bar()) _s_close.push(current_bar_.close);
        else _s_close.update(current_bar_.close);
        if (history_advances_new_bar()) _s_high.push(current_bar_.high);
        else _s_high.update(current_bar_.high);
        if (history_advances_new_bar()) _s_low.push(current_bar_.low);
        else _s_low.update(current_bar_.low);
        if (history_advances_new_bar()) _s_open.push(current_bar_.open);
        else _s_open.update(current_bar_.open);
        if (!_var_initialized) {
            fvgTops = std::vector<double>();
            fvgBottoms = std::vector<double>();
            fvgBullish = std::vector<bool>();
            _var_initialized = true;
        } else {
        }
        atrVal = (history_advances_new_bar() ? _ta_atr_1.compute(current_bar_.high, current_bar_.low, current_bar_.close, prev_chart_close()) : _ta_atr_1.recompute(current_bar_.high, current_bar_.low, current_bar_.close, prev_chart_close()));
        bullFVG = (([&]{ auto _pna_l = (current_bar_.low); auto _pna_r = (_s_high[2]); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()) && ([&]{ auto _pna_l = (_s_close[1]); auto _pna_r = (_s_open[1]); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()));
        bearFVG = (([&]{ auto _pna_l = (current_bar_.high); auto _pna_r = (_s_low[2]); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }()) && ([&]{ auto _pna_l = (_s_close[1]); auto _pna_r = (_s_open[1]); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }()));
        minFVGSize = (atrVal * 0.3);
        fvgTop = na<double>();
        fvgBottom = na<double>();
        if ((bullFVG && ([&]{ auto _pna_l = ((current_bar_.low - _s_high[2])); auto _pna_r = (minFVGSize); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) || _pfc_eq); }()))) {
            fvgTop = current_bar_.low;
            fvgBottom = _s_high[2];
            fvgTops.push_back(fvgTop);
            fvgBottoms.push_back(fvgBottom);
            fvgBullish.push_back(true);
        }
        if ((bearFVG && ([&]{ auto _pna_l = ((_s_low[2] - current_bar_.high)); auto _pna_r = (minFVGSize); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) || _pfc_eq); }()))) {
            fvgTop = _s_low[2];
            fvgBottom = current_bar_.high;
            fvgTops.push_back(fvgTop);
            fvgBottoms.push_back(fvgBottom);
            fvgBullish.push_back(false);
        }
        while (([&]{ auto _pna_l = ((double)fvgTops.size()); auto _pna_r = (30); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) {
            [&](auto&& __pf_array){ if(__pf_array.empty()) pine_runtime_error("Cannot use shift() if array is empty."); using __pf_array_value_type=typename std::decay_t<decltype(__pf_array)>::value_type; __pf_array_value_type __pf_array_value=__pf_array.front(); __pf_array.erase(__pf_array.begin()); return __pf_array_value; }((fvgTops));
            [&](auto&& __pf_array){ if(__pf_array.empty()) pine_runtime_error("Cannot use shift() if array is empty."); using __pf_array_value_type=typename std::decay_t<decltype(__pf_array)>::value_type; __pf_array_value_type __pf_array_value=__pf_array.front(); __pf_array.erase(__pf_array.begin()); return __pf_array_value; }((fvgBottoms));
            [&](auto&& __pf_array){ if(__pf_array.empty()) pine_runtime_error("Cannot use shift() if array is empty."); using __pf_array_value_type=typename std::decay_t<decltype(__pf_array)>::value_type; __pf_array_value_type __pf_array_value=__pf_array.front(); __pf_array.erase(__pf_array.begin()); return __pf_array_value; }((fvgBullish));
        }
        inBullFVG = false;
        inBearFVG = false;
        if (([&]{ auto _pna_l = ((double)fvgTops.size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) {
            int _for_start_0 = (0);
            int _for_end_0 = [&](){ auto _pf_v = ((((double)fvgTops.size() - 1))); return is_na(_pf_v) ? na<int>() : (int)_pf_v; }();
            int _for_step_0 = (1);
            if (!is_na(_for_step_0) && _for_step_0 < 0) _for_step_0 = -_for_step_0;
            if (_for_step_0 == 0) _for_step_0 = 1;
            const bool _for_down_0 = (_for_start_0 > _for_end_0);
            for (int i = _for_start_0; !is_na(_for_start_0) && !is_na(_for_end_0) && !is_na(_for_step_0) && (_for_down_0 ? (i >= _for_end_0) : (i <= _for_end_0)); i += (_for_down_0 ? -_for_step_0 : _for_step_0), _for_end_0 = [&](){ auto _pf_v = ((((double)fvgTops.size() - 1))); return is_na(_pf_v) ? na<int>() : (int)_pf_v; }()) {
                double fTop = [&](auto&& __pf_array)->decltype(auto){ return [&](auto&& __pf_raw_index_value)->decltype(auto){ using __pf_raw_index_type=std::decay_t<decltype(__pf_raw_index_value)>; if constexpr(!std::is_same_v<__pf_raw_index_type,bool>) { if(is_na(__pf_raw_index_value)) pine_runtime_error(std::string("Index na is out of bounds. Array size is ")+std::to_string((int64_t)__pf_array.size())); } if constexpr(std::is_floating_point_v<__pf_raw_index_type>) { if(!std::isfinite(__pf_raw_index_value)) { std::string __pf_raw_index_text=__pf_raw_index_value>0?"inf":"-inf"; pine_runtime_error(std::string("Index ")+__pf_raw_index_text+" is out of bounds. Array size is "+std::to_string((int64_t)__pf_array.size())); } long double __pf_raw_index_wide=(long double)__pf_raw_index_value; if(__pf_raw_index_wide<(long double)std::numeric_limits<int64_t>::min()||__pf_raw_index_wide>(long double)std::numeric_limits<int64_t>::max()) pine_runtime_error(std::string("Index ")+std::to_string((double)__pf_raw_index_value)+" is out of bounds. Array size is "+std::to_string((int64_t)__pf_array.size())); } int64_t __pf_raw_index=(int64_t)__pf_raw_index_value; int64_t __pf_array_size=(int64_t)__pf_array.size(); int64_t __pf_array_index=__pf_raw_index<0?__pf_raw_index+__pf_array_size:__pf_raw_index; if(__pf_array_index<0||__pf_array_index>=__pf_array_size) pine_runtime_error(std::string("Index ")+std::to_string(__pf_raw_index)+" is out of bounds. Array size is "+std::to_string(__pf_array_size)); if constexpr(std::is_lvalue_reference_v<decltype(__pf_array)>) return (__pf_array[(size_t)__pf_array_index]); else { using __pf_array_value_type=typename std::decay_t<decltype(__pf_array)>::value_type; return __pf_array_value_type(__pf_array[(size_t)__pf_array_index]); } }((i)); }((fvgTops));
                double fBottom = [&](auto&& __pf_array)->decltype(auto){ return [&](auto&& __pf_raw_index_value)->decltype(auto){ using __pf_raw_index_type=std::decay_t<decltype(__pf_raw_index_value)>; if constexpr(!std::is_same_v<__pf_raw_index_type,bool>) { if(is_na(__pf_raw_index_value)) pine_runtime_error(std::string("Index na is out of bounds. Array size is ")+std::to_string((int64_t)__pf_array.size())); } if constexpr(std::is_floating_point_v<__pf_raw_index_type>) { if(!std::isfinite(__pf_raw_index_value)) { std::string __pf_raw_index_text=__pf_raw_index_value>0?"inf":"-inf"; pine_runtime_error(std::string("Index ")+__pf_raw_index_text+" is out of bounds. Array size is "+std::to_string((int64_t)__pf_array.size())); } long double __pf_raw_index_wide=(long double)__pf_raw_index_value; if(__pf_raw_index_wide<(long double)std::numeric_limits<int64_t>::min()||__pf_raw_index_wide>(long double)std::numeric_limits<int64_t>::max()) pine_runtime_error(std::string("Index ")+std::to_string((double)__pf_raw_index_value)+" is out of bounds. Array size is "+std::to_string((int64_t)__pf_array.size())); } int64_t __pf_raw_index=(int64_t)__pf_raw_index_value; int64_t __pf_array_size=(int64_t)__pf_array.size(); int64_t __pf_array_index=__pf_raw_index<0?__pf_raw_index+__pf_array_size:__pf_raw_index; if(__pf_array_index<0||__pf_array_index>=__pf_array_size) pine_runtime_error(std::string("Index ")+std::to_string(__pf_raw_index)+" is out of bounds. Array size is "+std::to_string(__pf_array_size)); if constexpr(std::is_lvalue_reference_v<decltype(__pf_array)>) return (__pf_array[(size_t)__pf_array_index]); else { using __pf_array_value_type=typename std::decay_t<decltype(__pf_array)>::value_type; return __pf_array_value_type(__pf_array[(size_t)__pf_array_index]); } }((i)); }((fvgBottoms));
                bool isBull = [&](auto&& __pf_array)->decltype(auto){ return [&](auto&& __pf_raw_index_value)->decltype(auto){ using __pf_raw_index_type=std::decay_t<decltype(__pf_raw_index_value)>; if constexpr(!std::is_same_v<__pf_raw_index_type,bool>) { if(is_na(__pf_raw_index_value)) pine_runtime_error(std::string("Index na is out of bounds. Array size is ")+std::to_string((int64_t)__pf_array.size())); } if constexpr(std::is_floating_point_v<__pf_raw_index_type>) { if(!std::isfinite(__pf_raw_index_value)) { std::string __pf_raw_index_text=__pf_raw_index_value>0?"inf":"-inf"; pine_runtime_error(std::string("Index ")+__pf_raw_index_text+" is out of bounds. Array size is "+std::to_string((int64_t)__pf_array.size())); } long double __pf_raw_index_wide=(long double)__pf_raw_index_value; if(__pf_raw_index_wide<(long double)std::numeric_limits<int64_t>::min()||__pf_raw_index_wide>(long double)std::numeric_limits<int64_t>::max()) pine_runtime_error(std::string("Index ")+std::to_string((double)__pf_raw_index_value)+" is out of bounds. Array size is "+std::to_string((int64_t)__pf_array.size())); } int64_t __pf_raw_index=(int64_t)__pf_raw_index_value; int64_t __pf_array_size=(int64_t)__pf_array.size(); int64_t __pf_array_index=__pf_raw_index<0?__pf_raw_index+__pf_array_size:__pf_raw_index; if(__pf_array_index<0||__pf_array_index>=__pf_array_size) pine_runtime_error(std::string("Index ")+std::to_string(__pf_raw_index)+" is out of bounds. Array size is "+std::to_string(__pf_array_size)); if constexpr(std::is_lvalue_reference_v<decltype(__pf_array)>) return (__pf_array[(size_t)__pf_array_index]); else { using __pf_array_value_type=typename std::decay_t<decltype(__pf_array)>::value_type; return __pf_array_value_type(__pf_array[(size_t)__pf_array_index]); } }((i)); }((fvgBullish));
                if ((([&]{ auto _pna_l = (current_bar_.low); auto _pna_r = (fTop); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) || _pfc_eq); }()) && ([&]{ auto _pna_l = (current_bar_.high); auto _pna_r = (fBottom); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) || _pfc_eq); }()))) {
                    if (isBull) {
                        inBullFVG = true;
                    } else {
                        inBearFVG = true;
                    }
                }
            }
        }
        if ((inBullFVG && ([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && (_pfc_eq); }()))) {
            strategy_entry(std::string("L"), true, na<double>(), na<double>(), 1, std::string("bull-fvg touch"), "", 0, -1);
        }
        if ((inBearFVG && ([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()))) {
            strategy_close(std::string("L"), std::string("bear-fvg touch exit"), na<double>(), na<double>(), false, 412316860435ULL);
        }
    }

    void precalculate(const Bar* bars, int n) {
        _use_precalc = false;
        if (n <= 0 || bars == nullptr) return;

        _precalc__ta_atr_1.resize(n);

        _ta_atr_1 = ta::ATR(14);

        _s_close.clear();
        _s_high.clear();
        _s_low.clear();
        _s_open.clear();

        for (int i = 0; i < n; ++i) {
            _s_close.push(bars[i].close);
            _s_high.push(bars[i].high);
            _s_low.push(bars[i].low);
            _s_open.push(bars[i].open);
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
            _precalc__ta_atr_1[i] = _ta_atr_1.compute(bars[i].high, bars[i].low, bars[i].close, (i > 0 ? bars[i - 1].close : na<double>()));
        }

        _ta_atr_1 = ta::ATR(14);
        _s_close.clear();
        _s_high.clear();
        _s_low.clear();
        _s_open.clear();

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
