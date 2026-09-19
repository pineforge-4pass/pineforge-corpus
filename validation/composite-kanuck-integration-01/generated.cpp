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
    std::vector<double> _precalc__ta_sum_1;
    ta::SMA _ta_sma_2;
    std::vector<double> _precalc__ta_sma_2;
    ta::RSI _ta_rsi_3;
    ta::Crossover _ta_crossover_4;
    ta::Crossunder _ta_crossunder_5;
    bool _use_precalc = false;
    Series<double> _s_close{500};
    Series<double> kama{500};
    int tick_counter;
    int i_kama_len = 0;
    int i_kama_fast = 0;
    int i_kama_slow = 0;
    int i_step = 0;
    int i_sma_len = 0;
    int i_rsi_offset = 0;
    int i_deep_lag = 0;
    double change_n = 0.0;
    double vol_sum = 0.0;
    double er = 0.0;
    double fast_sc = 0.0;
    double slow_sc = 0.0;
    double sc = 0.0;
    bool gate = false;
    double long_sma = 0.0;
    double deep_rsi = 0.0;
    double deep_close = 0.0;
    bool deep_ready = false;
    bool kama_up = false;
    bool kama_down = false;
    bool go_long = false;
    bool go_short = false;
    bool _var_initialized = false;
    bool _ta_initialized_ = false;
    bool _inputs_initialized_ = false;

    struct _PFScriptState {
        decltype(GeneratedStrategy::_ta_sum_1) _pf_value_0;
        decltype(GeneratedStrategy::_ta_sma_2) _pf_value_1;
        decltype(GeneratedStrategy::_ta_rsi_3) _pf_value_2;
        decltype(GeneratedStrategy::_ta_crossover_4) _pf_value_3;
        decltype(GeneratedStrategy::_ta_crossunder_5) _pf_value_4;
        decltype(GeneratedStrategy::_s_close) _pf_value_5;
        decltype(GeneratedStrategy::kama) _pf_value_6;
        decltype(GeneratedStrategy::tick_counter) _pf_value_7;
        decltype(GeneratedStrategy::i_kama_len) _pf_value_8;
        decltype(GeneratedStrategy::i_kama_fast) _pf_value_9;
        decltype(GeneratedStrategy::i_kama_slow) _pf_value_10;
        decltype(GeneratedStrategy::i_step) _pf_value_11;
        decltype(GeneratedStrategy::i_sma_len) _pf_value_12;
        decltype(GeneratedStrategy::i_rsi_offset) _pf_value_13;
        decltype(GeneratedStrategy::i_deep_lag) _pf_value_14;
        decltype(GeneratedStrategy::change_n) _pf_value_15;
        decltype(GeneratedStrategy::vol_sum) _pf_value_16;
        decltype(GeneratedStrategy::er) _pf_value_17;
        decltype(GeneratedStrategy::fast_sc) _pf_value_18;
        decltype(GeneratedStrategy::slow_sc) _pf_value_19;
        decltype(GeneratedStrategy::sc) _pf_value_20;
        decltype(GeneratedStrategy::gate) _pf_value_21;
        decltype(GeneratedStrategy::long_sma) _pf_value_22;
        decltype(GeneratedStrategy::deep_rsi) _pf_value_23;
        decltype(GeneratedStrategy::deep_close) _pf_value_24;
        decltype(GeneratedStrategy::deep_ready) _pf_value_25;
        decltype(GeneratedStrategy::kama_up) _pf_value_26;
        decltype(GeneratedStrategy::kama_down) _pf_value_27;
        decltype(GeneratedStrategy::go_long) _pf_value_28;
        decltype(GeneratedStrategy::go_short) _pf_value_29;
        decltype(GeneratedStrategy::_var_initialized) _pf_value_30;
        decltype(GeneratedStrategy::_ta_initialized_) _pf_value_31;
        decltype(GeneratedStrategy::_inputs_initialized_) _pf_value_32;
    };
    static_assert(std::is_copy_constructible_v<_PFScriptState>, "generated Pine state must be deep-copy constructible");
    static_assert(std::is_copy_assignable_v<_PFScriptState>, "generated Pine state must be deep-copy assignable");
    std::optional<_PFScriptState> _pf_script_state_checkpoint_;

    void snapshot_script_state() override {
        _pf_script_state_checkpoint_.emplace(_PFScriptState{
            _ta_sum_1,
            _ta_sma_2,
            _ta_rsi_3,
            _ta_crossover_4,
            _ta_crossunder_5,
            _s_close,
            kama,
            tick_counter,
            i_kama_len,
            i_kama_fast,
            i_kama_slow,
            i_step,
            i_sma_len,
            i_rsi_offset,
            i_deep_lag,
            change_n,
            vol_sum,
            er,
            fast_sc,
            slow_sc,
            sc,
            gate,
            long_sma,
            deep_rsi,
            deep_close,
            deep_ready,
            kama_up,
            kama_down,
            go_long,
            go_short,
            _var_initialized,
            _ta_initialized_,
            _inputs_initialized_,
        });
    }

    void restore_script_state() override {
        if (!_pf_script_state_checkpoint_) return;
        this->_ta_sum_1 = _pf_script_state_checkpoint_->_pf_value_0;
        this->_ta_sma_2 = _pf_script_state_checkpoint_->_pf_value_1;
        this->_ta_rsi_3 = _pf_script_state_checkpoint_->_pf_value_2;
        this->_ta_crossover_4 = _pf_script_state_checkpoint_->_pf_value_3;
        this->_ta_crossunder_5 = _pf_script_state_checkpoint_->_pf_value_4;
        this->_s_close = _pf_script_state_checkpoint_->_pf_value_5;
        this->kama = _pf_script_state_checkpoint_->_pf_value_6;
        this->tick_counter = _pf_script_state_checkpoint_->_pf_value_7;
        this->i_kama_len = _pf_script_state_checkpoint_->_pf_value_8;
        this->i_kama_fast = _pf_script_state_checkpoint_->_pf_value_9;
        this->i_kama_slow = _pf_script_state_checkpoint_->_pf_value_10;
        this->i_step = _pf_script_state_checkpoint_->_pf_value_11;
        this->i_sma_len = _pf_script_state_checkpoint_->_pf_value_12;
        this->i_rsi_offset = _pf_script_state_checkpoint_->_pf_value_13;
        this->i_deep_lag = _pf_script_state_checkpoint_->_pf_value_14;
        this->change_n = _pf_script_state_checkpoint_->_pf_value_15;
        this->vol_sum = _pf_script_state_checkpoint_->_pf_value_16;
        this->er = _pf_script_state_checkpoint_->_pf_value_17;
        this->fast_sc = _pf_script_state_checkpoint_->_pf_value_18;
        this->slow_sc = _pf_script_state_checkpoint_->_pf_value_19;
        this->sc = _pf_script_state_checkpoint_->_pf_value_20;
        this->gate = _pf_script_state_checkpoint_->_pf_value_21;
        this->long_sma = _pf_script_state_checkpoint_->_pf_value_22;
        this->deep_rsi = _pf_script_state_checkpoint_->_pf_value_23;
        this->deep_close = _pf_script_state_checkpoint_->_pf_value_24;
        this->deep_ready = _pf_script_state_checkpoint_->_pf_value_25;
        this->kama_up = _pf_script_state_checkpoint_->_pf_value_26;
        this->kama_down = _pf_script_state_checkpoint_->_pf_value_27;
        this->go_long = _pf_script_state_checkpoint_->_pf_value_28;
        this->go_short = _pf_script_state_checkpoint_->_pf_value_29;
        this->_var_initialized = _pf_script_state_checkpoint_->_pf_value_30;
        this->_ta_initialized_ = _pf_script_state_checkpoint_->_pf_value_31;
        this->_inputs_initialized_ = _pf_script_state_checkpoint_->_pf_value_32;
    }

    void commit_script_state() override {
        snapshot_script_state();
    }

    explicit GeneratedStrategy() : _ta_sum_1(14), _ta_sma_2(400), _ta_rsi_3(14), tick_counter(0) {
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
        this->_precalc__ta_sum_1 = decltype(this->_precalc__ta_sum_1){};
        this->_ta_sma_2 = decltype(this->_ta_sma_2)(400);
        this->_precalc__ta_sma_2 = decltype(this->_precalc__ta_sma_2){};
        this->_ta_rsi_3 = decltype(this->_ta_rsi_3)(14);
        this->_ta_crossover_4 = decltype(this->_ta_crossover_4){};
        this->_ta_crossunder_5 = decltype(this->_ta_crossunder_5){};
        this->_use_precalc = false;
        this->_s_close = decltype(this->_s_close){500};
        this->kama = decltype(this->kama){500};
        this->tick_counter = decltype(this->tick_counter)(0);
        this->i_kama_len = 0;
        this->i_kama_fast = 0;
        this->i_kama_slow = 0;
        this->i_step = 0;
        this->i_sma_len = 0;
        this->i_rsi_offset = 0;
        this->i_deep_lag = 0;
        this->change_n = 0.0;
        this->vol_sum = 0.0;
        this->er = 0.0;
        this->fast_sc = 0.0;
        this->slow_sc = 0.0;
        this->sc = 0.0;
        this->gate = false;
        this->long_sma = 0.0;
        this->deep_rsi = 0.0;
        this->deep_close = 0.0;
        this->deep_ready = false;
        this->kama_up = false;
        this->kama_down = false;
        this->go_long = false;
        this->go_short = false;
        this->_var_initialized = false;
        this->_ta_initialized_ = false;
        this->_inputs_initialized_ = false;
        if (allow_precalculation) precalculate(bars, n);
    }

    void on_source_bar(const Bar& bar) override {
        if (history_advances_new_bar()) _s_close.push(current_bar_.close);
        else _s_close.update(current_bar_.close);
        if (!_var_initialized) {
            kama.push(na<double>());
            _var_initialized = true;
        } else {
            if (history_advances_new_bar()) kama.push(kama[0]);
            else kama.update(kama[0]);
        }
        if (!_inputs_initialized_) {
            i_kama_len = get_input_int("KAMA length", 14);
            i_kama_fast = get_input_int("KAMA fast end", 2);
            i_kama_slow = get_input_int("KAMA slow end", 30);
            i_step = get_input_int("Tick-counter modulo", 2);
            i_sma_len = get_input_int("Long SMA length", 400);
            i_rsi_offset = get_input_int("RSI source offset", 200);
            i_deep_lag = get_input_int("Close-reference lag", 450);
            _inputs_initialized_ = true;
        }
        if (!_ta_initialized_) {
            _ta_sum_1 = math::Sum(get_input_int("KAMA length", 14));
            _ta_sma_2 = ta::SMA(get_input_int("Long SMA length", 400));
            _ta_initialized_ = true;
        }
        change_n = std::abs((current_bar_.close - _s_close[i_kama_len]));
        vol_sum = (history_advances_new_bar() ? _ta_sum_1.compute(std::abs((current_bar_.close - _s_close[1]))) : _ta_sum_1.recompute(std::abs((current_bar_.close - _s_close[1]))));
        er = ((([&]{ auto _pna_l = (vol_sum); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) ? (((double)(change_n) / (double)(vol_sum))) : (0.0));
        fast_sc = ((double)(2.0) / (double)((i_kama_fast + 1)));
        slow_sc = ((double)(2.0) / (double)((i_kama_slow + 1)));
        sc = std::pow(((er * (fast_sc - slow_sc)) + slow_sc), 2);
        kama.update(((is_na(kama[1])) ? (current_bar_.close) : ((kama[1] + (sc * (current_bar_.close - kama[1]))))));
        tick_counter += 1;
        gate = ([&]{ auto _pna_l = (std::fmod((double)(tick_counter), (double)(i_step))); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && (_pfc_eq); }());
        long_sma = (history_advances_new_bar() ? _ta_sma_2.compute(current_bar_.close) : _ta_sma_2.recompute(current_bar_.close));
        deep_rsi = (history_advances_new_bar() ? _ta_rsi_3.compute(_s_close[i_rsi_offset]) : _ta_rsi_3.recompute(_s_close[i_rsi_offset]));
        deep_close = _s_close[i_deep_lag];
        deep_ready = ((!(is_na(long_sma)) && !(is_na(deep_rsi))) && !(is_na(deep_close)));
        kama_up = (history_advances_new_bar() ? _ta_crossover_4.compute(current_bar_.close, kama[0]) : _ta_crossover_4.recompute(current_bar_.close, kama[0]));
        kama_down = (history_advances_new_bar() ? _ta_crossunder_5.compute(current_bar_.close, kama[0]) : _ta_crossunder_5.recompute(current_bar_.close, kama[0]));
        go_long = ((deep_ready && kama_up) && gate);
        go_short = ((deep_ready && kama_down) && gate);
        if ((go_long && ([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) || _pfc_eq); }()))) {
            strategy_entry(std::string("L"), true, na<double>(), na<double>(), 1, std::string("integ long"), "", 0, -1);
        }
        if ((go_short && ([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) || _pfc_eq); }()))) {
            strategy_entry(std::string("S"), false, na<double>(), na<double>(), 1, std::string("integ short"), "", 0, -1);
        }
    }

    void precalculate(const Bar* bars, int n) {
        _use_precalc = false;
        if (n <= 0 || bars == nullptr) return;

        _precalc__ta_sum_1.resize(n);
        _precalc__ta_sma_2.resize(n);

        _ta_sum_1 = math::Sum(14);
        _ta_sma_2 = ta::SMA(400);

        _s_close.clear();

        for (int i = 0; i < n; ++i) {
            _s_close.push(bars[i].close);
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
            _precalc__ta_sum_1[i] = _ta_sum_1.compute(std::abs((bars[i].close - _s_close[1])));
            _precalc__ta_sma_2[i] = _ta_sma_2.compute(bars[i].close);
        }

        _ta_sum_1 = math::Sum(14);
        _ta_sma_2 = ta::SMA(400);
        _s_close.clear();

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
