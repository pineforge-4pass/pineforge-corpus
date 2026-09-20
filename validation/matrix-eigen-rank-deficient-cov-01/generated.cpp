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
#include <pineforge/matrix.hpp>
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

template <typename _PFValue>
struct _PFCheckpointTraits {
    using snapshot_type = _PFValue;
    static snapshot_type take(const _PFValue& value) { return value; }
    static void restore(_PFValue& value, const snapshot_type& snapshot) {
        value = snapshot;
    }
};

template <>
struct _PFCheckpointTraits<PineMatrix> {
    using matrix_type = PineMatrix;
    using snapshot_type = std::optional<typename matrix_type::Snapshot>;
    static snapshot_type take(const matrix_type& value) {
        if (value.is_na()) return std::nullopt;
        return value.snapshot();
    }
    static void restore(matrix_type& value, const snapshot_type& snapshot) {
        if (!snapshot) {
            value = matrix_type{};
            return;
        }
        value.restore(*snapshot);
    }
};

template <typename _PFElement, typename _PFAllocator>
struct _PFCheckpointTraits<std::vector<_PFElement, _PFAllocator>> {
    using element_traits = _PFCheckpointTraits<_PFElement>;
    using element_snapshot = typename element_traits::snapshot_type;
    using snapshot_type = std::vector<element_snapshot>;
    static snapshot_type take(
            const std::vector<_PFElement, _PFAllocator>& value) {
        snapshot_type snapshot;
        snapshot.reserve(value.size());
        for (std::size_t index = 0; index < value.size(); ++index) {
            const _PFElement element = value[index];
            snapshot.push_back(element_traits::take(element));
        }
        return snapshot;
    }
    static void restore(
            std::vector<_PFElement, _PFAllocator>& value,
            const snapshot_type& snapshot) {
        value.clear();
        value.reserve(snapshot.size());
        for (const auto& element_snapshot_value : snapshot) {
            _PFElement element{};
            element_traits::restore(element, element_snapshot_value);
            value.push_back(element);
        }
    }
};

class GeneratedStrategy : public pineforge::source::PineStrategyHost {
public:
    ta::SMA _ta_sma_1;
    ta::SMA _ta_sma_1_cs1;
    ta::SMA _ta_sma_1_cs2;
    ta::EMA _ta_ema_2;
    std::vector<double> _precalc__ta_ema_2;
    ta::EMA _ta_ema_3;
    std::vector<double> _precalc__ta_ema_3;
    ta::Crossover _ta_crossover_4;
    ta::Crossunder _ta_crossunder_5;
    bool _use_precalc = false;
    PineMatrix cov3;
    int N = 0;
    double c1 = 0.0;
    double c2 = 0.0;
    double c3 = 0.0;
    double m1 = 0.0;
    double m2 = 0.0;
    double m3 = 0.0;
    double d1 = 0.0;
    double d2 = 0.0;
    double d3 = 0.0;
    bool warmedUp = false;
    std::vector<double> evals;
    double emin = 0.0;
    bool eigenOk = false;
    double emaFast = 0.0;
    double emaSlow = 0.0;
    bool baseEntry = false;
    bool baseExit = false;
    bool _var_initialized = false;
    bool _ta_initialized_ = false;
    bool _inputs_initialized_ = false;

    struct _PFScriptState {
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_sma_1)>::snapshot_type _pf_value_0;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_sma_1_cs1)>::snapshot_type _pf_value_1;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_sma_1_cs2)>::snapshot_type _pf_value_2;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_ema_2)>::snapshot_type _pf_value_3;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_ema_3)>::snapshot_type _pf_value_4;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_crossover_4)>::snapshot_type _pf_value_5;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_crossunder_5)>::snapshot_type _pf_value_6;
        _PFCheckpointTraits<decltype(GeneratedStrategy::cov3)>::snapshot_type _pf_value_7;
        _PFCheckpointTraits<decltype(GeneratedStrategy::N)>::snapshot_type _pf_value_8;
        _PFCheckpointTraits<decltype(GeneratedStrategy::c1)>::snapshot_type _pf_value_9;
        _PFCheckpointTraits<decltype(GeneratedStrategy::c2)>::snapshot_type _pf_value_10;
        _PFCheckpointTraits<decltype(GeneratedStrategy::c3)>::snapshot_type _pf_value_11;
        _PFCheckpointTraits<decltype(GeneratedStrategy::m1)>::snapshot_type _pf_value_12;
        _PFCheckpointTraits<decltype(GeneratedStrategy::m2)>::snapshot_type _pf_value_13;
        _PFCheckpointTraits<decltype(GeneratedStrategy::m3)>::snapshot_type _pf_value_14;
        _PFCheckpointTraits<decltype(GeneratedStrategy::d1)>::snapshot_type _pf_value_15;
        _PFCheckpointTraits<decltype(GeneratedStrategy::d2)>::snapshot_type _pf_value_16;
        _PFCheckpointTraits<decltype(GeneratedStrategy::d3)>::snapshot_type _pf_value_17;
        _PFCheckpointTraits<decltype(GeneratedStrategy::warmedUp)>::snapshot_type _pf_value_18;
        _PFCheckpointTraits<decltype(GeneratedStrategy::evals)>::snapshot_type _pf_value_19;
        _PFCheckpointTraits<decltype(GeneratedStrategy::emin)>::snapshot_type _pf_value_20;
        _PFCheckpointTraits<decltype(GeneratedStrategy::eigenOk)>::snapshot_type _pf_value_21;
        _PFCheckpointTraits<decltype(GeneratedStrategy::emaFast)>::snapshot_type _pf_value_22;
        _PFCheckpointTraits<decltype(GeneratedStrategy::emaSlow)>::snapshot_type _pf_value_23;
        _PFCheckpointTraits<decltype(GeneratedStrategy::baseEntry)>::snapshot_type _pf_value_24;
        _PFCheckpointTraits<decltype(GeneratedStrategy::baseExit)>::snapshot_type _pf_value_25;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_var_initialized)>::snapshot_type _pf_value_26;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_initialized_)>::snapshot_type _pf_value_27;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_inputs_initialized_)>::snapshot_type _pf_value_28;
    };
    static_assert(std::is_copy_constructible_v<_PFScriptState>, "generated Pine state must be deep-copy constructible");
    static_assert(std::is_copy_assignable_v<_PFScriptState>, "generated Pine state must be deep-copy assignable");
    std::optional<_PFScriptState> _pf_script_state_checkpoint_;

    void snapshot_script_state() override {
        _pf_script_state_checkpoint_.emplace(_PFScriptState{
            _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_sma_1)>::take(_ta_sma_1),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_sma_1_cs1)>::take(_ta_sma_1_cs1),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_sma_1_cs2)>::take(_ta_sma_1_cs2),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_ema_2)>::take(_ta_ema_2),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_ema_3)>::take(_ta_ema_3),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_crossover_4)>::take(_ta_crossover_4),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_crossunder_5)>::take(_ta_crossunder_5),
            _PFCheckpointTraits<decltype(GeneratedStrategy::cov3)>::take(cov3),
            _PFCheckpointTraits<decltype(GeneratedStrategy::N)>::take(N),
            _PFCheckpointTraits<decltype(GeneratedStrategy::c1)>::take(c1),
            _PFCheckpointTraits<decltype(GeneratedStrategy::c2)>::take(c2),
            _PFCheckpointTraits<decltype(GeneratedStrategy::c3)>::take(c3),
            _PFCheckpointTraits<decltype(GeneratedStrategy::m1)>::take(m1),
            _PFCheckpointTraits<decltype(GeneratedStrategy::m2)>::take(m2),
            _PFCheckpointTraits<decltype(GeneratedStrategy::m3)>::take(m3),
            _PFCheckpointTraits<decltype(GeneratedStrategy::d1)>::take(d1),
            _PFCheckpointTraits<decltype(GeneratedStrategy::d2)>::take(d2),
            _PFCheckpointTraits<decltype(GeneratedStrategy::d3)>::take(d3),
            _PFCheckpointTraits<decltype(GeneratedStrategy::warmedUp)>::take(warmedUp),
            _PFCheckpointTraits<decltype(GeneratedStrategy::evals)>::take(evals),
            _PFCheckpointTraits<decltype(GeneratedStrategy::emin)>::take(emin),
            _PFCheckpointTraits<decltype(GeneratedStrategy::eigenOk)>::take(eigenOk),
            _PFCheckpointTraits<decltype(GeneratedStrategy::emaFast)>::take(emaFast),
            _PFCheckpointTraits<decltype(GeneratedStrategy::emaSlow)>::take(emaSlow),
            _PFCheckpointTraits<decltype(GeneratedStrategy::baseEntry)>::take(baseEntry),
            _PFCheckpointTraits<decltype(GeneratedStrategy::baseExit)>::take(baseExit),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_var_initialized)>::take(_var_initialized),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_initialized_)>::take(_ta_initialized_),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_inputs_initialized_)>::take(_inputs_initialized_),
        });
    }

    void restore_script_state() override {
        if (!_pf_script_state_checkpoint_) return;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_sma_1)>::restore(this->_ta_sma_1, _pf_script_state_checkpoint_->_pf_value_0);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_sma_1_cs1)>::restore(this->_ta_sma_1_cs1, _pf_script_state_checkpoint_->_pf_value_1);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_sma_1_cs2)>::restore(this->_ta_sma_1_cs2, _pf_script_state_checkpoint_->_pf_value_2);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_ema_2)>::restore(this->_ta_ema_2, _pf_script_state_checkpoint_->_pf_value_3);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_ema_3)>::restore(this->_ta_ema_3, _pf_script_state_checkpoint_->_pf_value_4);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_crossover_4)>::restore(this->_ta_crossover_4, _pf_script_state_checkpoint_->_pf_value_5);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_crossunder_5)>::restore(this->_ta_crossunder_5, _pf_script_state_checkpoint_->_pf_value_6);
        _PFCheckpointTraits<decltype(GeneratedStrategy::cov3)>::restore(this->cov3, _pf_script_state_checkpoint_->_pf_value_7);
        _PFCheckpointTraits<decltype(GeneratedStrategy::N)>::restore(this->N, _pf_script_state_checkpoint_->_pf_value_8);
        _PFCheckpointTraits<decltype(GeneratedStrategy::c1)>::restore(this->c1, _pf_script_state_checkpoint_->_pf_value_9);
        _PFCheckpointTraits<decltype(GeneratedStrategy::c2)>::restore(this->c2, _pf_script_state_checkpoint_->_pf_value_10);
        _PFCheckpointTraits<decltype(GeneratedStrategy::c3)>::restore(this->c3, _pf_script_state_checkpoint_->_pf_value_11);
        _PFCheckpointTraits<decltype(GeneratedStrategy::m1)>::restore(this->m1, _pf_script_state_checkpoint_->_pf_value_12);
        _PFCheckpointTraits<decltype(GeneratedStrategy::m2)>::restore(this->m2, _pf_script_state_checkpoint_->_pf_value_13);
        _PFCheckpointTraits<decltype(GeneratedStrategy::m3)>::restore(this->m3, _pf_script_state_checkpoint_->_pf_value_14);
        _PFCheckpointTraits<decltype(GeneratedStrategy::d1)>::restore(this->d1, _pf_script_state_checkpoint_->_pf_value_15);
        _PFCheckpointTraits<decltype(GeneratedStrategy::d2)>::restore(this->d2, _pf_script_state_checkpoint_->_pf_value_16);
        _PFCheckpointTraits<decltype(GeneratedStrategy::d3)>::restore(this->d3, _pf_script_state_checkpoint_->_pf_value_17);
        _PFCheckpointTraits<decltype(GeneratedStrategy::warmedUp)>::restore(this->warmedUp, _pf_script_state_checkpoint_->_pf_value_18);
        _PFCheckpointTraits<decltype(GeneratedStrategy::evals)>::restore(this->evals, _pf_script_state_checkpoint_->_pf_value_19);
        _PFCheckpointTraits<decltype(GeneratedStrategy::emin)>::restore(this->emin, _pf_script_state_checkpoint_->_pf_value_20);
        _PFCheckpointTraits<decltype(GeneratedStrategy::eigenOk)>::restore(this->eigenOk, _pf_script_state_checkpoint_->_pf_value_21);
        _PFCheckpointTraits<decltype(GeneratedStrategy::emaFast)>::restore(this->emaFast, _pf_script_state_checkpoint_->_pf_value_22);
        _PFCheckpointTraits<decltype(GeneratedStrategy::emaSlow)>::restore(this->emaSlow, _pf_script_state_checkpoint_->_pf_value_23);
        _PFCheckpointTraits<decltype(GeneratedStrategy::baseEntry)>::restore(this->baseEntry, _pf_script_state_checkpoint_->_pf_value_24);
        _PFCheckpointTraits<decltype(GeneratedStrategy::baseExit)>::restore(this->baseExit, _pf_script_state_checkpoint_->_pf_value_25);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_var_initialized)>::restore(this->_var_initialized, _pf_script_state_checkpoint_->_pf_value_26);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_initialized_)>::restore(this->_ta_initialized_, _pf_script_state_checkpoint_->_pf_value_27);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_inputs_initialized_)>::restore(this->_inputs_initialized_, _pf_script_state_checkpoint_->_pf_value_28);
    }

    void commit_script_state() override {
        snapshot_script_state();
    }

    explicit GeneratedStrategy() : _ta_sma_1(32), _ta_sma_1_cs1(32), _ta_sma_1_cs2(32), _ta_ema_2(9), _ta_ema_3(21) {
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
        this->_ta_sma_1 = decltype(this->_ta_sma_1)(32);
        this->_ta_sma_1_cs1 = decltype(this->_ta_sma_1_cs1)(32);
        this->_ta_sma_1_cs2 = decltype(this->_ta_sma_1_cs2)(32);
        this->_ta_ema_2 = decltype(this->_ta_ema_2)(9);
        this->_precalc__ta_ema_2 = decltype(this->_precalc__ta_ema_2){};
        this->_ta_ema_3 = decltype(this->_ta_ema_3)(21);
        this->_precalc__ta_ema_3 = decltype(this->_precalc__ta_ema_3){};
        this->_ta_crossover_4 = decltype(this->_ta_crossover_4){};
        this->_ta_crossunder_5 = decltype(this->_ta_crossunder_5){};
        this->_use_precalc = false;
        this->cov3 = decltype(this->cov3){};
        this->N = 0;
        this->c1 = 0.0;
        this->c2 = 0.0;
        this->c3 = 0.0;
        this->m1 = 0.0;
        this->m2 = 0.0;
        this->m3 = 0.0;
        this->d1 = 0.0;
        this->d2 = 0.0;
        this->d3 = 0.0;
        this->warmedUp = false;
        this->evals = decltype(this->evals){};
        this->emin = 0.0;
        this->eigenOk = false;
        this->emaFast = 0.0;
        this->emaSlow = 0.0;
        this->baseEntry = false;
        this->baseExit = false;
        this->_var_initialized = false;
        this->_ta_initialized_ = false;
        this->_inputs_initialized_ = false;
        if (allow_precalculation) precalculate(bars, n);
    }

    double mean_cs0(double src, int64_t len) {
        return (history_advances_new_bar() ? _ta_sma_1.compute(src) : _ta_sma_1.recompute(src));
    }

    double mean_cs1(double src, int64_t len) {
        return (history_advances_new_bar() ? _ta_sma_1_cs1.compute(src) : _ta_sma_1_cs1.recompute(src));
    }

    double mean_cs2(double src, int64_t len) {
        return (history_advances_new_bar() ? _ta_sma_1_cs2.compute(src) : _ta_sma_1_cs2.recompute(src));
    }

    void on_source_bar(const Bar& bar) override {
        if (!_var_initialized) {
            cov3 = PineMatrix::new_(3, 3, 0.0);
            _var_initialized = true;
        } else {
        }
        N = 32;
        c1 = current_bar_.close;
        c2 = (current_bar_.close * 2.0);
        c3 = (current_bar_.close * 3.0);
        m1 = mean_cs0(c1, 32);
        m2 = mean_cs1(c2, 32);
        m3 = mean_cs2(c3, 32);
        d1 = ([&]{ auto _nz_v = ((c1 - m1)); return is_na(_nz_v) ? (0.0) : _nz_v; }());
        d2 = ([&]{ auto _nz_v = ((c2 - m2)); return is_na(_nz_v) ? (0.0) : _nz_v; }());
        d3 = ([&]{ auto _nz_v = ((c3 - m3)); return is_na(_nz_v) ? (0.0) : _nz_v; }());
        cov3.set((int)(0), (int)(0), (d1 * d1));
        cov3.set((int)(0), (int)(1), (d1 * d2));
        cov3.set((int)(0), (int)(2), (d1 * d3));
        cov3.set((int)(1), (int)(0), (d2 * d1));
        cov3.set((int)(1), (int)(1), (d2 * d2));
        cov3.set((int)(1), (int)(2), (d2 * d3));
        cov3.set((int)(2), (int)(0), (d3 * d1));
        cov3.set((int)(2), (int)(1), (d3 * d2));
        cov3.set((int)(2), (int)(2), (d3 * d3));
        warmedUp = ([&]{ auto _pna_l = (pine_bar_index()); auto _pna_r = (32); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) || _pfc_eq); }());
        evals = ((warmedUp) ? (cov3.eigenvalues()) : (std::vector<double>((size_t)(0), 0.0)));
        emin = ((([&]{ auto _pna_l = ((double)evals.size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) ? ((evals.empty()?na<double>():*std::min_element(evals.begin(),evals.end()))) : (na<double>()));
        eigenOk = (!(is_na(emin)) && ([&]{ auto _pna_l = (emin); auto _pna_r = (1e-09); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()));
        emaFast = (history_advances_new_bar() ? _ta_ema_2.compute(current_bar_.close) : _ta_ema_2.recompute(current_bar_.close));
        emaSlow = (history_advances_new_bar() ? _ta_ema_3.compute(current_bar_.close) : _ta_ema_3.recompute(current_bar_.close));
        baseEntry = (history_advances_new_bar() ? _ta_crossover_4.compute(emaFast, emaSlow) : _ta_crossover_4.recompute(emaFast, emaSlow));
        baseExit = (history_advances_new_bar() ? _ta_crossunder_5.compute(emaFast, emaSlow) : _ta_crossunder_5.recompute(emaFast, emaSlow));
        if ((baseEntry && ([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && (_pfc_eq); }()))) {
            if (eigenOk) {
                strategy_entry(std::string("L_eig"), true, na<double>(), na<double>(), 1, std::string("entry long eig-ok"), "", 0, -1);
            } else {
                strategy_entry(std::string("L_fb"), true, na<double>(), na<double>(), 1, std::string("entry long fallback"), "", 0, -1);
            }
        }
        if ((baseExit && ([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()))) {
            strategy_close("", std::string("exit"), na<double>(), na<double>(), false);
        }
    }

    void precalculate(const Bar* bars, int n) {
        _use_precalc = false;
        if (n <= 0 || bars == nullptr) return;

        _precalc__ta_ema_2.resize(n);
        _precalc__ta_ema_3.resize(n);

        _ta_ema_2 = ta::EMA(9);
        _ta_ema_3 = ta::EMA(21);


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
            _precalc__ta_ema_2[i] = _ta_ema_2.compute(bars[i].close);
            _precalc__ta_ema_3[i] = _ta_ema_3.compute(bars[i].close);
        }

        _ta_ema_2 = ta::EMA(9);
        _ta_ema_3 = ta::EMA(21);

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
