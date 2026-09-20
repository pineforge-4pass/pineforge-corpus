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
    double _req_sec_0 = na<double>();
    double _req_sec_1 = na<double>();
    double _req_sec_2 = na<double>();
    double _req_sec_3 = na<double>();
    std::unordered_map<std::string, Series<double>> _security_helper_series_;
    ta::RSI _ta_rsi_1;
    ta::EMA _ta_ema_2;
    ta::EMA _ta_ema_3;
    ta::EMA _ta_ema_4;
    ta::SMA _ta_sma_5;
    bool _use_precalc = false;
    ta::RSI _sec0__ta_rsi_1;
    ta::EMA _sec1__ta_ema_2_v0;
    ta::EMA _sec1__ta_ema_2_v1;
    ta::EMA _sec1__ta_ema_2_v2;
    ta::EMA _sec1__ta_ema_3_v0;
    ta::EMA _sec1__ta_ema_3_v1;
    ta::EMA _sec1__ta_ema_3_v2;
    ta::EMA _sec1__ta_ema_4_v0;
    ta::EMA _sec1__ta_ema_4_v1;
    ta::SMA _sec2__ta_sma_5;
    double htfRsi = 0.0;
    double htfHist = 0.0;
    double htfBbM = 0.0;
    double htfClose = 0.0;
    bool bullAgree = false;
    bool bearAgree = false;
    bool _ta_initialized_ = false;
    bool _inputs_initialized_ = false;

    struct _PFScriptState {
        decltype(GeneratedStrategy::_req_sec_0) _pf_value_0;
        decltype(GeneratedStrategy::_req_sec_1) _pf_value_1;
        decltype(GeneratedStrategy::_req_sec_2) _pf_value_2;
        decltype(GeneratedStrategy::_req_sec_3) _pf_value_3;
        decltype(GeneratedStrategy::_security_helper_series_) _pf_value_4;
        decltype(GeneratedStrategy::_ta_rsi_1) _pf_value_5;
        decltype(GeneratedStrategy::_ta_ema_2) _pf_value_6;
        decltype(GeneratedStrategy::_ta_ema_3) _pf_value_7;
        decltype(GeneratedStrategy::_ta_ema_4) _pf_value_8;
        decltype(GeneratedStrategy::_ta_sma_5) _pf_value_9;
        decltype(GeneratedStrategy::_sec0__ta_rsi_1) _pf_value_10;
        decltype(GeneratedStrategy::_sec1__ta_ema_2_v0) _pf_value_11;
        decltype(GeneratedStrategy::_sec1__ta_ema_2_v1) _pf_value_12;
        decltype(GeneratedStrategy::_sec1__ta_ema_2_v2) _pf_value_13;
        decltype(GeneratedStrategy::_sec1__ta_ema_3_v0) _pf_value_14;
        decltype(GeneratedStrategy::_sec1__ta_ema_3_v1) _pf_value_15;
        decltype(GeneratedStrategy::_sec1__ta_ema_3_v2) _pf_value_16;
        decltype(GeneratedStrategy::_sec1__ta_ema_4_v0) _pf_value_17;
        decltype(GeneratedStrategy::_sec1__ta_ema_4_v1) _pf_value_18;
        decltype(GeneratedStrategy::_sec2__ta_sma_5) _pf_value_19;
        decltype(GeneratedStrategy::htfRsi) _pf_value_20;
        decltype(GeneratedStrategy::htfHist) _pf_value_21;
        decltype(GeneratedStrategy::htfBbM) _pf_value_22;
        decltype(GeneratedStrategy::htfClose) _pf_value_23;
        decltype(GeneratedStrategy::bullAgree) _pf_value_24;
        decltype(GeneratedStrategy::bearAgree) _pf_value_25;
        decltype(GeneratedStrategy::_ta_initialized_) _pf_value_26;
        decltype(GeneratedStrategy::_inputs_initialized_) _pf_value_27;
    };
    static_assert(std::is_copy_constructible_v<_PFScriptState>, "generated Pine state must be deep-copy constructible");
    static_assert(std::is_copy_assignable_v<_PFScriptState>, "generated Pine state must be deep-copy assignable");
    std::optional<_PFScriptState> _pf_script_state_checkpoint_;

    void snapshot_script_state() override {
        _pf_script_state_checkpoint_.emplace(_PFScriptState{
            _req_sec_0,
            _req_sec_1,
            _req_sec_2,
            _req_sec_3,
            _security_helper_series_,
            _ta_rsi_1,
            _ta_ema_2,
            _ta_ema_3,
            _ta_ema_4,
            _ta_sma_5,
            _sec0__ta_rsi_1,
            _sec1__ta_ema_2_v0,
            _sec1__ta_ema_2_v1,
            _sec1__ta_ema_2_v2,
            _sec1__ta_ema_3_v0,
            _sec1__ta_ema_3_v1,
            _sec1__ta_ema_3_v2,
            _sec1__ta_ema_4_v0,
            _sec1__ta_ema_4_v1,
            _sec2__ta_sma_5,
            htfRsi,
            htfHist,
            htfBbM,
            htfClose,
            bullAgree,
            bearAgree,
            _ta_initialized_,
            _inputs_initialized_,
        });
    }

    void restore_script_state() override {
        if (!_pf_script_state_checkpoint_) return;
        this->_req_sec_0 = _pf_script_state_checkpoint_->_pf_value_0;
        this->_req_sec_1 = _pf_script_state_checkpoint_->_pf_value_1;
        this->_req_sec_2 = _pf_script_state_checkpoint_->_pf_value_2;
        this->_req_sec_3 = _pf_script_state_checkpoint_->_pf_value_3;
        this->_security_helper_series_ = _pf_script_state_checkpoint_->_pf_value_4;
        this->_ta_rsi_1 = _pf_script_state_checkpoint_->_pf_value_5;
        this->_ta_ema_2 = _pf_script_state_checkpoint_->_pf_value_6;
        this->_ta_ema_3 = _pf_script_state_checkpoint_->_pf_value_7;
        this->_ta_ema_4 = _pf_script_state_checkpoint_->_pf_value_8;
        this->_ta_sma_5 = _pf_script_state_checkpoint_->_pf_value_9;
        this->_sec0__ta_rsi_1 = _pf_script_state_checkpoint_->_pf_value_10;
        this->_sec1__ta_ema_2_v0 = _pf_script_state_checkpoint_->_pf_value_11;
        this->_sec1__ta_ema_2_v1 = _pf_script_state_checkpoint_->_pf_value_12;
        this->_sec1__ta_ema_2_v2 = _pf_script_state_checkpoint_->_pf_value_13;
        this->_sec1__ta_ema_3_v0 = _pf_script_state_checkpoint_->_pf_value_14;
        this->_sec1__ta_ema_3_v1 = _pf_script_state_checkpoint_->_pf_value_15;
        this->_sec1__ta_ema_3_v2 = _pf_script_state_checkpoint_->_pf_value_16;
        this->_sec1__ta_ema_4_v0 = _pf_script_state_checkpoint_->_pf_value_17;
        this->_sec1__ta_ema_4_v1 = _pf_script_state_checkpoint_->_pf_value_18;
        this->_sec2__ta_sma_5 = _pf_script_state_checkpoint_->_pf_value_19;
        this->htfRsi = _pf_script_state_checkpoint_->_pf_value_20;
        this->htfHist = _pf_script_state_checkpoint_->_pf_value_21;
        this->htfBbM = _pf_script_state_checkpoint_->_pf_value_22;
        this->htfClose = _pf_script_state_checkpoint_->_pf_value_23;
        this->bullAgree = _pf_script_state_checkpoint_->_pf_value_24;
        this->bearAgree = _pf_script_state_checkpoint_->_pf_value_25;
        this->_ta_initialized_ = _pf_script_state_checkpoint_->_pf_value_26;
        this->_inputs_initialized_ = _pf_script_state_checkpoint_->_pf_value_27;
    }

    void commit_script_state() override {
        snapshot_script_state();
    }

    explicit GeneratedStrategy() : _ta_rsi_1(14), _ta_ema_2(12), _ta_ema_3(26), _ta_ema_4(9), _ta_sma_5(20), _sec0__ta_rsi_1(14), _sec1__ta_ema_2_v0(12), _sec1__ta_ema_2_v1(12), _sec1__ta_ema_2_v2(12), _sec1__ta_ema_3_v0(26), _sec1__ta_ema_3_v1(26), _sec1__ta_ema_3_v2(26), _sec1__ta_ema_4_v0(9), _sec1__ta_ema_4_v1(9), _sec2__ta_sma_5(20) {
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
        register_security_eval(0, "60", input_tf_, false, false);
        register_security_eval(1, "60", input_tf_, false, false);
        register_security_eval(2, "60", input_tf_, false, false);
        register_security_eval(3, "60", input_tf_, false, false);
    }

#ifndef PINEFORGE_HAS_SCRIPT_RUN_PREPARE_V1
#error "Generated lifecycle reset requires a matching PineForge engine; rebuild with script-run preparation support"
#endif
    void prepare_script_run(const Bar* bars, int n, bool allow_precalculation) override {
        _pf_script_state_checkpoint_.reset();
        this->_req_sec_0 = na<double>();
        this->_req_sec_1 = na<double>();
        this->_req_sec_2 = na<double>();
        this->_req_sec_3 = na<double>();
        this->_security_helper_series_ = decltype(this->_security_helper_series_){};
        this->_ta_rsi_1 = decltype(this->_ta_rsi_1)(14);
        this->_ta_ema_2 = decltype(this->_ta_ema_2)(12);
        this->_ta_ema_3 = decltype(this->_ta_ema_3)(26);
        this->_ta_ema_4 = decltype(this->_ta_ema_4)(9);
        this->_ta_sma_5 = decltype(this->_ta_sma_5)(20);
        this->_use_precalc = false;
        this->_sec0__ta_rsi_1 = decltype(this->_sec0__ta_rsi_1)(14);
        this->_sec1__ta_ema_2_v0 = decltype(this->_sec1__ta_ema_2_v0)(12);
        this->_sec1__ta_ema_2_v1 = decltype(this->_sec1__ta_ema_2_v1)(12);
        this->_sec1__ta_ema_2_v2 = decltype(this->_sec1__ta_ema_2_v2)(12);
        this->_sec1__ta_ema_3_v0 = decltype(this->_sec1__ta_ema_3_v0)(26);
        this->_sec1__ta_ema_3_v1 = decltype(this->_sec1__ta_ema_3_v1)(26);
        this->_sec1__ta_ema_3_v2 = decltype(this->_sec1__ta_ema_3_v2)(26);
        this->_sec1__ta_ema_4_v0 = decltype(this->_sec1__ta_ema_4_v0)(9);
        this->_sec1__ta_ema_4_v1 = decltype(this->_sec1__ta_ema_4_v1)(9);
        this->_sec2__ta_sma_5 = decltype(this->_sec2__ta_sma_5)(20);
        this->htfRsi = 0.0;
        this->htfHist = 0.0;
        this->htfBbM = 0.0;
        this->htfClose = 0.0;
        this->bullAgree = false;
        this->bearAgree = false;
        this->_ta_initialized_ = false;
        this->_inputs_initialized_ = false;
        (void)bars; (void)n; (void)allow_precalculation;
    }

    double f_rsi_cs0(double src, int len) {
        return (history_advances_new_bar() ? _ta_rsi_1.compute(src) : _ta_rsi_1.recompute(src));
    }

    double f_macd_hist_cs0(double src, int fast, int slow, int sig) {
        double macdLine = ((history_advances_new_bar() ? _ta_ema_2.compute(src) : _ta_ema_2.recompute(src)) - (history_advances_new_bar() ? _ta_ema_3.compute(src) : _ta_ema_3.recompute(src)));
        double sigLine = (history_advances_new_bar() ? _ta_ema_4.compute(macdLine) : _ta_ema_4.recompute(macdLine));
        return (macdLine - sigLine);
    }

    double f_bb_mid_cs0(double src, int len) {
        return (history_advances_new_bar() ? _ta_sma_5.compute(src) : _ta_sma_5.recompute(src));
    }

    void on_source_bar(const Bar& bar) override {
        htfRsi = _req_sec_0;
        htfHist = _req_sec_1;
        htfBbM = _req_sec_2;
        htfClose = _req_sec_3;
        bullAgree = ((([&]{ auto _pna_l = (htfRsi); auto _pna_r = (55.0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()) && ([&]{ auto _pna_l = (htfHist); auto _pna_r = (0.0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) && ([&]{ auto _pna_l = (htfClose); auto _pna_r = (htfBbM); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()));
        bearAgree = ((([&]{ auto _pna_l = (htfRsi); auto _pna_r = (45.0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }()) || ([&]{ auto _pna_l = (htfHist); auto _pna_r = (0.0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }())) || ([&]{ auto _pna_l = (htfClose); auto _pna_r = (htfBbM); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }()));
        if ((bullAgree && ([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && (_pfc_eq); }()))) {
            strategy_entry(std::string("L"), true, na<double>(), na<double>(), 1, std::string("entry long"), "", 0, -1);
        }
        if ((bearAgree && ([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()))) {
            strategy_close(std::string("L"), std::string("exit long"), na<double>(), na<double>(), false, 219043332115ULL);
        }
    }


    void _eval_security_0(const Bar& bar, bool is_complete) {
        auto _secval_0 = security_series_slot_is_new(0) ? _sec0__ta_rsi_1.compute(bar.close) : _sec0__ta_rsi_1.recompute(bar.close);
        _req_sec_0 = _secval_0;
    }

    void _eval_security_1(const Bar& bar, bool is_complete) {
        double _sec1_f_macd_hist_1_macdLine = ((security_series_slot_is_new(1) ? _sec1__ta_ema_2_v2.compute(bar.close) : _sec1__ta_ema_2_v2.recompute(bar.close)) - (security_series_slot_is_new(1) ? _sec1__ta_ema_3_v2.compute(bar.close) : _sec1__ta_ema_3_v2.recompute(bar.close)));
        double _sec1_f_macd_hist_2_sigLine = (security_series_slot_is_new(1) ? _sec1__ta_ema_4_v1.compute(_sec1_f_macd_hist_1_macdLine) : _sec1__ta_ema_4_v1.recompute(_sec1_f_macd_hist_1_macdLine));
        _req_sec_1 = (_sec1_f_macd_hist_1_macdLine - _sec1_f_macd_hist_2_sigLine);
    }

    void _eval_security_2(const Bar& bar, bool is_complete) {
        auto _secval_4 = security_series_slot_is_new(2) ? _sec2__ta_sma_5.compute(bar.close) : _sec2__ta_sma_5.recompute(bar.close);
        _req_sec_2 = _secval_4;
    }

    void _eval_security_3(const Bar& bar, bool is_complete) {
        _req_sec_3 = bar.close;
    }

    void evaluate_security(int sec_id, const Bar& bar, bool is_complete) override {
        switch (sec_id) {
            case 0: _eval_security_0(bar, is_complete); break;
            case 1: _eval_security_1(bar, is_complete); break;
            case 2: _eval_security_2(bar, is_complete); break;
            case 3: _eval_security_3(bar, is_complete); break;
        }
    }
    void clear_security(int sec_id) override {
        switch (sec_id) {
            case 0: _req_sec_0 = na<double>(); break;
            case 1: _req_sec_1 = na<double>(); break;
            case 2: _req_sec_2 = na<double>(); break;
            case 3: _req_sec_3 = na<double>(); break;
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
