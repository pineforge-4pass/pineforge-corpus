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
    ta::RSI _ta_rsi_3;
    std::vector<double> _precalc__ta_rsi_3;
    ta::ATR _ta_atr_4;
    std::vector<double> _precalc__ta_atr_4;
    ta::Crossover _ta_crossover_5;
    ta::Crossunder _ta_crossunder_6;
    bool _use_precalc = false;
    int openedAt;
    Series<double> _strat_position_size;
    int fastLength = 0;
    int slowLength = 0;
    int momentumLength = 0;
    double entryMomentum = 0.0;
    double exitMomentum = 0.0;
    bool confirmedOnly = false;
    int maximumHoldBars = 0;
    int atrLength = 0;
    double stopAtr = 0.0;
    double fastEma = 0.0;
    double slowEma = 0.0;
    double momentum = 0.0;
    double atrValue = 0.0;
    bool signalBarReady = false;
    bool entrySignal = false;
    bool exitSignal = false;
    bool holdExpired = false;
    bool _var_initialized = false;
    bool _ta_initialized_ = false;
    bool _inputs_initialized_ = false;

    struct _PFScriptState {
        decltype(GeneratedStrategy::_ta_ema_1) _pf_value_0;
        decltype(GeneratedStrategy::_ta_ema_2) _pf_value_1;
        decltype(GeneratedStrategy::_ta_rsi_3) _pf_value_2;
        decltype(GeneratedStrategy::_ta_atr_4) _pf_value_3;
        decltype(GeneratedStrategy::_ta_crossover_5) _pf_value_4;
        decltype(GeneratedStrategy::_ta_crossunder_6) _pf_value_5;
        decltype(GeneratedStrategy::openedAt) _pf_value_6;
        decltype(GeneratedStrategy::_strat_position_size) _pf_value_7;
        decltype(GeneratedStrategy::fastLength) _pf_value_8;
        decltype(GeneratedStrategy::slowLength) _pf_value_9;
        decltype(GeneratedStrategy::momentumLength) _pf_value_10;
        decltype(GeneratedStrategy::entryMomentum) _pf_value_11;
        decltype(GeneratedStrategy::exitMomentum) _pf_value_12;
        decltype(GeneratedStrategy::confirmedOnly) _pf_value_13;
        decltype(GeneratedStrategy::maximumHoldBars) _pf_value_14;
        decltype(GeneratedStrategy::atrLength) _pf_value_15;
        decltype(GeneratedStrategy::stopAtr) _pf_value_16;
        decltype(GeneratedStrategy::fastEma) _pf_value_17;
        decltype(GeneratedStrategy::slowEma) _pf_value_18;
        decltype(GeneratedStrategy::momentum) _pf_value_19;
        decltype(GeneratedStrategy::atrValue) _pf_value_20;
        decltype(GeneratedStrategy::signalBarReady) _pf_value_21;
        decltype(GeneratedStrategy::entrySignal) _pf_value_22;
        decltype(GeneratedStrategy::exitSignal) _pf_value_23;
        decltype(GeneratedStrategy::holdExpired) _pf_value_24;
        decltype(GeneratedStrategy::_var_initialized) _pf_value_25;
        decltype(GeneratedStrategy::_ta_initialized_) _pf_value_26;
        decltype(GeneratedStrategy::_inputs_initialized_) _pf_value_27;
    };
    static_assert(std::is_copy_constructible_v<_PFScriptState>, "generated Pine state must be deep-copy constructible");
    static_assert(std::is_copy_assignable_v<_PFScriptState>, "generated Pine state must be deep-copy assignable");
    std::optional<_PFScriptState> _pf_script_state_checkpoint_;

    void snapshot_script_state() override {
        _pf_script_state_checkpoint_.emplace(_PFScriptState{
            _ta_ema_1,
            _ta_ema_2,
            _ta_rsi_3,
            _ta_atr_4,
            _ta_crossover_5,
            _ta_crossunder_6,
            openedAt,
            _strat_position_size,
            fastLength,
            slowLength,
            momentumLength,
            entryMomentum,
            exitMomentum,
            confirmedOnly,
            maximumHoldBars,
            atrLength,
            stopAtr,
            fastEma,
            slowEma,
            momentum,
            atrValue,
            signalBarReady,
            entrySignal,
            exitSignal,
            holdExpired,
            _var_initialized,
            _ta_initialized_,
            _inputs_initialized_,
        });
    }

    void restore_script_state() override {
        if (!_pf_script_state_checkpoint_) return;
        this->_ta_ema_1 = _pf_script_state_checkpoint_->_pf_value_0;
        this->_ta_ema_2 = _pf_script_state_checkpoint_->_pf_value_1;
        this->_ta_rsi_3 = _pf_script_state_checkpoint_->_pf_value_2;
        this->_ta_atr_4 = _pf_script_state_checkpoint_->_pf_value_3;
        this->_ta_crossover_5 = _pf_script_state_checkpoint_->_pf_value_4;
        this->_ta_crossunder_6 = _pf_script_state_checkpoint_->_pf_value_5;
        this->openedAt = _pf_script_state_checkpoint_->_pf_value_6;
        this->_strat_position_size = _pf_script_state_checkpoint_->_pf_value_7;
        this->fastLength = _pf_script_state_checkpoint_->_pf_value_8;
        this->slowLength = _pf_script_state_checkpoint_->_pf_value_9;
        this->momentumLength = _pf_script_state_checkpoint_->_pf_value_10;
        this->entryMomentum = _pf_script_state_checkpoint_->_pf_value_11;
        this->exitMomentum = _pf_script_state_checkpoint_->_pf_value_12;
        this->confirmedOnly = _pf_script_state_checkpoint_->_pf_value_13;
        this->maximumHoldBars = _pf_script_state_checkpoint_->_pf_value_14;
        this->atrLength = _pf_script_state_checkpoint_->_pf_value_15;
        this->stopAtr = _pf_script_state_checkpoint_->_pf_value_16;
        this->fastEma = _pf_script_state_checkpoint_->_pf_value_17;
        this->slowEma = _pf_script_state_checkpoint_->_pf_value_18;
        this->momentum = _pf_script_state_checkpoint_->_pf_value_19;
        this->atrValue = _pf_script_state_checkpoint_->_pf_value_20;
        this->signalBarReady = _pf_script_state_checkpoint_->_pf_value_21;
        this->entrySignal = _pf_script_state_checkpoint_->_pf_value_22;
        this->exitSignal = _pf_script_state_checkpoint_->_pf_value_23;
        this->holdExpired = _pf_script_state_checkpoint_->_pf_value_24;
        this->_var_initialized = _pf_script_state_checkpoint_->_pf_value_25;
        this->_ta_initialized_ = _pf_script_state_checkpoint_->_pf_value_26;
        this->_inputs_initialized_ = _pf_script_state_checkpoint_->_pf_value_27;
    }

    void commit_script_state() override {
        snapshot_script_state();
    }

    explicit GeneratedStrategy() : _ta_ema_1(17), _ta_ema_2(53), _ta_rsi_3(13), _ta_atr_4(15), openedAt(na<int>()) {
#if defined(PINEFORGE_HAS_EXPLICIT_PINE_EXECUTION_ADAPTER_V1)
        pineforge::source::PineStrategyHost::attach_pine_execution_adapter();
#elif defined(PINEFORGE_HAS_EXPLICIT_PINE_CAP_V1)
        pineforge::source::PineStrategyHost::enable_pine_intraday_cap();
#endif
        pineforge::source::PineStrategyConfig cfg{};
        cfg.initial_capital = 100000.0;
        cfg.default_qty_type = static_cast<int>(QtyType::FIXED);
        cfg.default_qty_value = 2.0;
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
        this->_ta_ema_1 = decltype(this->_ta_ema_1)(17);
        this->_precalc__ta_ema_1 = decltype(this->_precalc__ta_ema_1){};
        this->_ta_ema_2 = decltype(this->_ta_ema_2)(53);
        this->_precalc__ta_ema_2 = decltype(this->_precalc__ta_ema_2){};
        this->_ta_rsi_3 = decltype(this->_ta_rsi_3)(13);
        this->_precalc__ta_rsi_3 = decltype(this->_precalc__ta_rsi_3){};
        this->_ta_atr_4 = decltype(this->_ta_atr_4)(15);
        this->_precalc__ta_atr_4 = decltype(this->_precalc__ta_atr_4){};
        this->_ta_crossover_5 = decltype(this->_ta_crossover_5){};
        this->_ta_crossunder_6 = decltype(this->_ta_crossunder_6){};
        this->_use_precalc = false;
        this->openedAt = decltype(this->openedAt)(na<int>());
        this->_strat_position_size = decltype(this->_strat_position_size){};
        this->fastLength = 0;
        this->slowLength = 0;
        this->momentumLength = 0;
        this->entryMomentum = 0.0;
        this->exitMomentum = 0.0;
        this->confirmedOnly = false;
        this->maximumHoldBars = 0;
        this->atrLength = 0;
        this->stopAtr = 0.0;
        this->fastEma = 0.0;
        this->slowEma = 0.0;
        this->momentum = 0.0;
        this->atrValue = 0.0;
        this->signalBarReady = false;
        this->entrySignal = false;
        this->exitSignal = false;
        this->holdExpired = false;
        this->_var_initialized = false;
        this->_ta_initialized_ = false;
        this->_inputs_initialized_ = false;
        if (allow_precalculation) precalculate(bars, n);
    }

    void on_source_bar(const Bar& bar) override {
        if (history_advances_new_bar()) _strat_position_size.push(signed_position_size());
        else _strat_position_size.update(signed_position_size());
        if (!_var_initialized) {
            _var_initialized = true;
        } else {
        }
        if (!_inputs_initialized_) {
            fastLength = get_input_int("Fast EMA", 17);
            slowLength = get_input_int("Slow EMA", 53);
            momentumLength = get_input_int("RSI Length", 13);
            entryMomentum = get_input_double("Entry RSI", 54.0);
            exitMomentum = get_input_double("Exit RSI", 44.0);
            confirmedOnly = get_input_bool("Require Confirmed Bars", true);
            maximumHoldBars = get_input_int("Maximum Hold Bars", 192);
            atrLength = get_input_int("ATR Length", 15);
            stopAtr = get_input_double("Stop ATR", 2.3);
            _inputs_initialized_ = true;
        }
        if (!_ta_initialized_) {
            _ta_ema_1 = ta::EMA(get_input_int("Fast EMA", 17));
            _ta_ema_2 = ta::EMA(get_input_int("Slow EMA", 53));
            _ta_rsi_3 = ta::RSI(get_input_int("RSI Length", 13));
            _ta_atr_4 = ta::ATR(get_input_int("ATR Length", 15));
            _ta_initialized_ = true;
        }
        fastEma = (history_advances_new_bar() ? _ta_ema_1.compute(current_bar_.close) : _ta_ema_1.recompute(current_bar_.close));
        slowEma = (history_advances_new_bar() ? _ta_ema_2.compute(current_bar_.close) : _ta_ema_2.recompute(current_bar_.close));
        momentum = (history_advances_new_bar() ? _ta_rsi_3.compute(current_bar_.close) : _ta_rsi_3.recompute(current_bar_.close));
        atrValue = (history_advances_new_bar() ? _ta_atr_4.compute(current_bar_.high, current_bar_.low, current_bar_.close, prev_chart_close()) : _ta_atr_4.recompute(current_bar_.high, current_bar_.low, current_bar_.close, prev_chart_close()));
        signalBarReady = (!(confirmedOnly) || is_last_tick_);
        if ((([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()) && ([&]{ auto _pna_l = (_strat_position_size[1]); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && (_pfc_eq); }()))) {
            openedAt = pine_bar_index();
        }
        entrySignal = ((signalBarReady && (history_advances_new_bar() ? _ta_crossover_5.compute(fastEma, slowEma) : _ta_crossover_5.recompute(fastEma, slowEma))) && ([&]{ auto _pna_l = (momentum); auto _pna_r = (entryMomentum); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()));
        exitSignal = (signalBarReady && ((history_advances_new_bar() ? _ta_crossunder_6.compute(fastEma, slowEma) : _ta_crossunder_6.recompute(fastEma, slowEma)) || ([&]{ auto _pna_l = (momentum); auto _pna_r = (exitMomentum); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }())));
        holdExpired = ((([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()) && !(is_na(openedAt))) && ([&]{ auto _pna_l = ((pine_bar_index() - openedAt)); auto _pna_r = (maximumHoldBars); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) || _pfc_eq); }()));
        if (entrySignal) {
            strategy_entry(std::string("Lattice Long"), true, na<double>(), na<double>(), na<double>(), "");
        }
        if ((([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()) && (exitSignal || holdExpired))) {
            strategy_close(std::string("Lattice Long"), ((holdExpired) ? (std::string("Maximum hold")) : (std::string("Confirmed pivot"))), na<double>(), na<double>(), false, 180388626451ULL);
        }
        if (([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) {
            strategy_exit(std::string("Lattice Guard"), std::string("Lattice Long"), na<double>(), ((signed_position_size() == 0.0 ? na<double>() : position_entry_price_) - (atrValue * stopAtr)), na<double>(), na<double>(), na<double>(), 100.0, "", na<double>(), "", na<double>(), na<double>());
        }
        if (([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && (_pfc_eq); }())) {
            openedAt = na<int>();
        }
    }

    void precalculate(const Bar* bars, int n) {
        _use_precalc = false;
        if (n <= 0 || bars == nullptr) return;

        _precalc__ta_ema_1.resize(n);
        _precalc__ta_ema_2.resize(n);
        _precalc__ta_rsi_3.resize(n);
        _precalc__ta_atr_4.resize(n);

        _ta_ema_1 = ta::EMA(17);
        _ta_ema_2 = ta::EMA(53);
        _ta_rsi_3 = ta::RSI(13);
        _ta_atr_4 = ta::ATR(15);


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
            _precalc__ta_rsi_3[i] = _ta_rsi_3.compute(bars[i].close);
            _precalc__ta_atr_4[i] = _ta_atr_4.compute(bars[i].high, bars[i].low, bars[i].close, (i > 0 ? bars[i - 1].close : na<double>()));
        }

        _ta_ema_1 = ta::EMA(17);
        _ta_ema_2 = ta::EMA(53);
        _ta_rsi_3 = ta::RSI(13);
        _ta_atr_4 = ta::ATR(15);

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
