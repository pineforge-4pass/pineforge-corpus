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
#include <pineforge/drawing.hpp>
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
    ta::PivotHigh _ta_pivothigh_1;
    std::vector<double> _precalc__ta_pivothigh_1;
    ta::PivotLow _ta_pivotlow_2;
    std::vector<double> _precalc__ta_pivotlow_2;
    ta::EMA _ta_ema_3;
    std::vector<double> _precalc__ta_ema_3;
    ta::EMA _ta_ema_4;
    std::vector<double> _precalc__ta_ema_4;
    ta::EMA _ta_ema_5;
    std::vector<double> _precalc__ta_ema_5;
    ta::RSI _ta_rsi_6;
    std::vector<double> _precalc__ta_rsi_6;
    ta::Highest _ta_highest_7;
    std::vector<double> _precalc__ta_highest_7;
    ta::Lowest _ta_lowest_8;
    std::vector<double> _precalc__ta_lowest_8;
    bool _use_precalc = false;
    double last_ph;
    double last_pl;
    int last_ph_x;
    int last_pl_x;
    double prev_ph_y;
    int prev_ph_x;
    double prev_pl_y;
    int prev_pl_x;
    Series<double> hi_now;
    Series<double> lo_now;
    int i_pivot = 0;
    int i_ema_fast = 0;
    int i_ema_mid = 0;
    int i_ema_slow = 0;
    int i_rsi_len = 0;
    double i_rsi_lo = 0.0;
    double i_rsi_hi = 0.0;
    int i_break_len = 0;
    double i_rr = 0.0;
    double ph = 0.0;
    double pl = 0.0;
    double cur_x = 0.0;
    double ema_fast = 0.0;
    double ema_mid = 0.0;
    double ema_slow = 0.0;
    bool stack_bull = false;
    bool stack_bear = false;
    bool trend_bull = false;
    bool trend_bear = false;
    double r = 0.0;
    bool mom_bull = false;
    bool mom_bear = false;
    double break_hi = 0.0;
    double break_lo = 0.0;
    bool struct_bull = false;
    bool struct_bear = false;
    bool gate_long = false;
    bool gate_short = false;
    bool go_long = false;
    bool go_short = false;
    double entry_px = 0.0;
    double sl_px = 0.0;
    double tp_px = 0.0;
    DrawingArena<LineRec> _pf_lines_{500};
    DrawingArena<BoxRec> _pf_boxes_{50};
    DrawingArena<LabelRec> _pf_labels_{50};
    DrawingArena<LinefillRec> _pf_linefills_{50};
    bool _var_initialized = false;
    bool _ta_initialized_ = false;
    bool _inputs_initialized_ = false;

    struct _PFScriptState {
        decltype(GeneratedStrategy::_ta_pivothigh_1) _pf_value_0;
        decltype(GeneratedStrategy::_ta_pivotlow_2) _pf_value_1;
        decltype(GeneratedStrategy::_ta_ema_3) _pf_value_2;
        decltype(GeneratedStrategy::_ta_ema_4) _pf_value_3;
        decltype(GeneratedStrategy::_ta_ema_5) _pf_value_4;
        decltype(GeneratedStrategy::_ta_rsi_6) _pf_value_5;
        decltype(GeneratedStrategy::_ta_highest_7) _pf_value_6;
        decltype(GeneratedStrategy::_ta_lowest_8) _pf_value_7;
        decltype(GeneratedStrategy::last_ph) _pf_value_8;
        decltype(GeneratedStrategy::last_pl) _pf_value_9;
        decltype(GeneratedStrategy::last_ph_x) _pf_value_10;
        decltype(GeneratedStrategy::last_pl_x) _pf_value_11;
        decltype(GeneratedStrategy::prev_ph_y) _pf_value_12;
        decltype(GeneratedStrategy::prev_ph_x) _pf_value_13;
        decltype(GeneratedStrategy::prev_pl_y) _pf_value_14;
        decltype(GeneratedStrategy::prev_pl_x) _pf_value_15;
        decltype(GeneratedStrategy::hi_now) _pf_value_16;
        decltype(GeneratedStrategy::lo_now) _pf_value_17;
        decltype(GeneratedStrategy::i_pivot) _pf_value_18;
        decltype(GeneratedStrategy::i_ema_fast) _pf_value_19;
        decltype(GeneratedStrategy::i_ema_mid) _pf_value_20;
        decltype(GeneratedStrategy::i_ema_slow) _pf_value_21;
        decltype(GeneratedStrategy::i_rsi_len) _pf_value_22;
        decltype(GeneratedStrategy::i_rsi_lo) _pf_value_23;
        decltype(GeneratedStrategy::i_rsi_hi) _pf_value_24;
        decltype(GeneratedStrategy::i_break_len) _pf_value_25;
        decltype(GeneratedStrategy::i_rr) _pf_value_26;
        decltype(GeneratedStrategy::ph) _pf_value_27;
        decltype(GeneratedStrategy::pl) _pf_value_28;
        decltype(GeneratedStrategy::cur_x) _pf_value_29;
        decltype(GeneratedStrategy::ema_fast) _pf_value_30;
        decltype(GeneratedStrategy::ema_mid) _pf_value_31;
        decltype(GeneratedStrategy::ema_slow) _pf_value_32;
        decltype(GeneratedStrategy::stack_bull) _pf_value_33;
        decltype(GeneratedStrategy::stack_bear) _pf_value_34;
        decltype(GeneratedStrategy::trend_bull) _pf_value_35;
        decltype(GeneratedStrategy::trend_bear) _pf_value_36;
        decltype(GeneratedStrategy::r) _pf_value_37;
        decltype(GeneratedStrategy::mom_bull) _pf_value_38;
        decltype(GeneratedStrategy::mom_bear) _pf_value_39;
        decltype(GeneratedStrategy::break_hi) _pf_value_40;
        decltype(GeneratedStrategy::break_lo) _pf_value_41;
        decltype(GeneratedStrategy::struct_bull) _pf_value_42;
        decltype(GeneratedStrategy::struct_bear) _pf_value_43;
        decltype(GeneratedStrategy::gate_long) _pf_value_44;
        decltype(GeneratedStrategy::gate_short) _pf_value_45;
        decltype(GeneratedStrategy::go_long) _pf_value_46;
        decltype(GeneratedStrategy::go_short) _pf_value_47;
        decltype(GeneratedStrategy::entry_px) _pf_value_48;
        decltype(GeneratedStrategy::sl_px) _pf_value_49;
        decltype(GeneratedStrategy::tp_px) _pf_value_50;
        decltype(GeneratedStrategy::_pf_lines_) _pf_value_51;
        decltype(GeneratedStrategy::_pf_boxes_) _pf_value_52;
        decltype(GeneratedStrategy::_pf_labels_) _pf_value_53;
        decltype(GeneratedStrategy::_pf_linefills_) _pf_value_54;
        decltype(GeneratedStrategy::_var_initialized) _pf_value_55;
        decltype(GeneratedStrategy::_ta_initialized_) _pf_value_56;
        decltype(GeneratedStrategy::_inputs_initialized_) _pf_value_57;
    };
    static_assert(std::is_copy_constructible_v<_PFScriptState>, "generated Pine state must be deep-copy constructible");
    static_assert(std::is_copy_assignable_v<_PFScriptState>, "generated Pine state must be deep-copy assignable");
    std::optional<_PFScriptState> _pf_script_state_checkpoint_;

    void snapshot_script_state() override {
        _pf_script_state_checkpoint_.emplace(_PFScriptState{
            _ta_pivothigh_1,
            _ta_pivotlow_2,
            _ta_ema_3,
            _ta_ema_4,
            _ta_ema_5,
            _ta_rsi_6,
            _ta_highest_7,
            _ta_lowest_8,
            last_ph,
            last_pl,
            last_ph_x,
            last_pl_x,
            prev_ph_y,
            prev_ph_x,
            prev_pl_y,
            prev_pl_x,
            hi_now,
            lo_now,
            i_pivot,
            i_ema_fast,
            i_ema_mid,
            i_ema_slow,
            i_rsi_len,
            i_rsi_lo,
            i_rsi_hi,
            i_break_len,
            i_rr,
            ph,
            pl,
            cur_x,
            ema_fast,
            ema_mid,
            ema_slow,
            stack_bull,
            stack_bear,
            trend_bull,
            trend_bear,
            r,
            mom_bull,
            mom_bear,
            break_hi,
            break_lo,
            struct_bull,
            struct_bear,
            gate_long,
            gate_short,
            go_long,
            go_short,
            entry_px,
            sl_px,
            tp_px,
            _pf_lines_,
            _pf_boxes_,
            _pf_labels_,
            _pf_linefills_,
            _var_initialized,
            _ta_initialized_,
            _inputs_initialized_,
        });
    }

    void restore_script_state() override {
        if (!_pf_script_state_checkpoint_) return;
        this->_ta_pivothigh_1 = _pf_script_state_checkpoint_->_pf_value_0;
        this->_ta_pivotlow_2 = _pf_script_state_checkpoint_->_pf_value_1;
        this->_ta_ema_3 = _pf_script_state_checkpoint_->_pf_value_2;
        this->_ta_ema_4 = _pf_script_state_checkpoint_->_pf_value_3;
        this->_ta_ema_5 = _pf_script_state_checkpoint_->_pf_value_4;
        this->_ta_rsi_6 = _pf_script_state_checkpoint_->_pf_value_5;
        this->_ta_highest_7 = _pf_script_state_checkpoint_->_pf_value_6;
        this->_ta_lowest_8 = _pf_script_state_checkpoint_->_pf_value_7;
        this->last_ph = _pf_script_state_checkpoint_->_pf_value_8;
        this->last_pl = _pf_script_state_checkpoint_->_pf_value_9;
        this->last_ph_x = _pf_script_state_checkpoint_->_pf_value_10;
        this->last_pl_x = _pf_script_state_checkpoint_->_pf_value_11;
        this->prev_ph_y = _pf_script_state_checkpoint_->_pf_value_12;
        this->prev_ph_x = _pf_script_state_checkpoint_->_pf_value_13;
        this->prev_pl_y = _pf_script_state_checkpoint_->_pf_value_14;
        this->prev_pl_x = _pf_script_state_checkpoint_->_pf_value_15;
        this->hi_now = _pf_script_state_checkpoint_->_pf_value_16;
        this->lo_now = _pf_script_state_checkpoint_->_pf_value_17;
        this->i_pivot = _pf_script_state_checkpoint_->_pf_value_18;
        this->i_ema_fast = _pf_script_state_checkpoint_->_pf_value_19;
        this->i_ema_mid = _pf_script_state_checkpoint_->_pf_value_20;
        this->i_ema_slow = _pf_script_state_checkpoint_->_pf_value_21;
        this->i_rsi_len = _pf_script_state_checkpoint_->_pf_value_22;
        this->i_rsi_lo = _pf_script_state_checkpoint_->_pf_value_23;
        this->i_rsi_hi = _pf_script_state_checkpoint_->_pf_value_24;
        this->i_break_len = _pf_script_state_checkpoint_->_pf_value_25;
        this->i_rr = _pf_script_state_checkpoint_->_pf_value_26;
        this->ph = _pf_script_state_checkpoint_->_pf_value_27;
        this->pl = _pf_script_state_checkpoint_->_pf_value_28;
        this->cur_x = _pf_script_state_checkpoint_->_pf_value_29;
        this->ema_fast = _pf_script_state_checkpoint_->_pf_value_30;
        this->ema_mid = _pf_script_state_checkpoint_->_pf_value_31;
        this->ema_slow = _pf_script_state_checkpoint_->_pf_value_32;
        this->stack_bull = _pf_script_state_checkpoint_->_pf_value_33;
        this->stack_bear = _pf_script_state_checkpoint_->_pf_value_34;
        this->trend_bull = _pf_script_state_checkpoint_->_pf_value_35;
        this->trend_bear = _pf_script_state_checkpoint_->_pf_value_36;
        this->r = _pf_script_state_checkpoint_->_pf_value_37;
        this->mom_bull = _pf_script_state_checkpoint_->_pf_value_38;
        this->mom_bear = _pf_script_state_checkpoint_->_pf_value_39;
        this->break_hi = _pf_script_state_checkpoint_->_pf_value_40;
        this->break_lo = _pf_script_state_checkpoint_->_pf_value_41;
        this->struct_bull = _pf_script_state_checkpoint_->_pf_value_42;
        this->struct_bear = _pf_script_state_checkpoint_->_pf_value_43;
        this->gate_long = _pf_script_state_checkpoint_->_pf_value_44;
        this->gate_short = _pf_script_state_checkpoint_->_pf_value_45;
        this->go_long = _pf_script_state_checkpoint_->_pf_value_46;
        this->go_short = _pf_script_state_checkpoint_->_pf_value_47;
        this->entry_px = _pf_script_state_checkpoint_->_pf_value_48;
        this->sl_px = _pf_script_state_checkpoint_->_pf_value_49;
        this->tp_px = _pf_script_state_checkpoint_->_pf_value_50;
        this->_pf_lines_ = _pf_script_state_checkpoint_->_pf_value_51;
        this->_pf_boxes_ = _pf_script_state_checkpoint_->_pf_value_52;
        this->_pf_labels_ = _pf_script_state_checkpoint_->_pf_value_53;
        this->_pf_linefills_ = _pf_script_state_checkpoint_->_pf_value_54;
        this->_var_initialized = _pf_script_state_checkpoint_->_pf_value_55;
        this->_ta_initialized_ = _pf_script_state_checkpoint_->_pf_value_56;
        this->_inputs_initialized_ = _pf_script_state_checkpoint_->_pf_value_57;
    }

    void commit_script_state() override {
        snapshot_script_state();
    }

    explicit GeneratedStrategy() : _ta_pivothigh_1(5, 5), _ta_pivotlow_2(5, 5), _ta_ema_3(21), _ta_ema_4(55), _ta_ema_5(200), _ta_rsi_6(14), _ta_highest_7(20), _ta_lowest_8(20), last_ph(na<double>()), last_pl(na<double>()), last_ph_x(na<int>()), last_pl_x(na<int>()), prev_ph_y(na<double>()), prev_ph_x(na<int>()), prev_pl_y(na<double>()), prev_pl_x(na<int>()) {
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
        this->_ta_pivothigh_1 = decltype(this->_ta_pivothigh_1)(5, 5);
        this->_precalc__ta_pivothigh_1 = decltype(this->_precalc__ta_pivothigh_1){};
        this->_ta_pivotlow_2 = decltype(this->_ta_pivotlow_2)(5, 5);
        this->_precalc__ta_pivotlow_2 = decltype(this->_precalc__ta_pivotlow_2){};
        this->_ta_ema_3 = decltype(this->_ta_ema_3)(21);
        this->_precalc__ta_ema_3 = decltype(this->_precalc__ta_ema_3){};
        this->_ta_ema_4 = decltype(this->_ta_ema_4)(55);
        this->_precalc__ta_ema_4 = decltype(this->_precalc__ta_ema_4){};
        this->_ta_ema_5 = decltype(this->_ta_ema_5)(200);
        this->_precalc__ta_ema_5 = decltype(this->_precalc__ta_ema_5){};
        this->_ta_rsi_6 = decltype(this->_ta_rsi_6)(14);
        this->_precalc__ta_rsi_6 = decltype(this->_precalc__ta_rsi_6){};
        this->_ta_highest_7 = decltype(this->_ta_highest_7)(20);
        this->_precalc__ta_highest_7 = decltype(this->_precalc__ta_highest_7){};
        this->_ta_lowest_8 = decltype(this->_ta_lowest_8)(20);
        this->_precalc__ta_lowest_8 = decltype(this->_precalc__ta_lowest_8){};
        this->_use_precalc = false;
        this->last_ph = decltype(this->last_ph)(na<double>());
        this->last_pl = decltype(this->last_pl)(na<double>());
        this->last_ph_x = decltype(this->last_ph_x)(na<int>());
        this->last_pl_x = decltype(this->last_pl_x)(na<int>());
        this->prev_ph_y = decltype(this->prev_ph_y)(na<double>());
        this->prev_ph_x = decltype(this->prev_ph_x)(na<int>());
        this->prev_pl_y = decltype(this->prev_pl_y)(na<double>());
        this->prev_pl_x = decltype(this->prev_pl_x)(na<int>());
        this->hi_now = decltype(this->hi_now){};
        this->lo_now = decltype(this->lo_now){};
        this->i_pivot = 0;
        this->i_ema_fast = 0;
        this->i_ema_mid = 0;
        this->i_ema_slow = 0;
        this->i_rsi_len = 0;
        this->i_rsi_lo = 0.0;
        this->i_rsi_hi = 0.0;
        this->i_break_len = 0;
        this->i_rr = 0.0;
        this->ph = 0.0;
        this->pl = 0.0;
        this->cur_x = 0.0;
        this->ema_fast = 0.0;
        this->ema_mid = 0.0;
        this->ema_slow = 0.0;
        this->stack_bull = false;
        this->stack_bear = false;
        this->trend_bull = false;
        this->trend_bear = false;
        this->r = 0.0;
        this->mom_bull = false;
        this->mom_bear = false;
        this->break_hi = 0.0;
        this->break_lo = 0.0;
        this->struct_bull = false;
        this->struct_bear = false;
        this->gate_long = false;
        this->gate_short = false;
        this->go_long = false;
        this->go_short = false;
        this->entry_px = 0.0;
        this->sl_px = 0.0;
        this->tp_px = 0.0;
        this->_pf_lines_ = decltype(this->_pf_lines_){500};
        this->_pf_boxes_ = decltype(this->_pf_boxes_){50};
        this->_pf_labels_ = decltype(this->_pf_labels_){50};
        this->_pf_linefills_ = decltype(this->_pf_linefills_){50};
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
            i_pivot = get_input_int("Pivot strength", 5);
            i_ema_fast = get_input_int("EMA fast", 21);
            i_ema_mid = get_input_int("EMA mid", 55);
            i_ema_slow = get_input_int("EMA slow", 200);
            i_rsi_len = get_input_int("RSI length", 14);
            i_rsi_lo = get_input_double("RSI long threshold", 55);
            i_rsi_hi = get_input_double("RSI short threshold", 45);
            i_break_len = get_input_int("Breakout window", 20);
            i_rr = get_input_double("Reward:risk ratio", 2.0);
            _inputs_initialized_ = true;
        }
        if (!_ta_initialized_) {
            _ta_pivothigh_1 = ta::PivotHigh(get_input_int("Pivot strength", 5), get_input_int("Pivot strength", 5));
            _ta_pivotlow_2 = ta::PivotLow(get_input_int("Pivot strength", 5), get_input_int("Pivot strength", 5));
            _ta_ema_3 = ta::EMA(get_input_int("EMA fast", 21));
            _ta_ema_4 = ta::EMA(get_input_int("EMA mid", 55));
            _ta_ema_5 = ta::EMA(get_input_int("EMA slow", 200));
            _ta_rsi_6 = ta::RSI(get_input_int("RSI length", 14));
            _ta_highest_7 = ta::Highest(get_input_int("Breakout window", 20));
            _ta_lowest_8 = ta::Lowest(get_input_int("Breakout window", 20));
            _ta_initialized_ = true;
        }
        ph = (history_advances_new_bar() ? _ta_pivothigh_1.compute(current_bar_.high) : _ta_pivothigh_1.recompute(current_bar_.high));
        pl = (history_advances_new_bar() ? _ta_pivotlow_2.compute(current_bar_.low) : _ta_pivotlow_2.recompute(current_bar_.low));
        if (!(is_na(ph))) {
            cur_x = (pine_bar_index() - i_pivot);
            if ((!(is_na(last_ph)) && !(is_na(last_ph_x)))) {
                pf_line_new(_pf_lines_, [&](){ auto _pf_v = (last_ph_x); return is_na(_pf_v) ? na<int64_t>() : (int64_t)_pf_v; }(), (double)(last_ph), [&](){ auto _pf_v = (cur_x); return is_na(_pf_v) ? na<int64_t>() : (int64_t)_pf_v; }(), (double)(ph), XLoc::bar_index, false, false);
            }
            prev_ph_y = last_ph;
            prev_ph_x = last_ph_x;
            last_ph = ph;
            last_ph_x = [&](){ auto _pf_v = (cur_x); return is_na(_pf_v) ? na<int>() : (int)_pf_v; }();
        }
        if (!(is_na(pl))) {
            cur_x = (pine_bar_index() - i_pivot);
            if ((!(is_na(last_pl)) && !(is_na(last_pl_x)))) {
                pf_line_new(_pf_lines_, [&](){ auto _pf_v = (last_pl_x); return is_na(_pf_v) ? na<int64_t>() : (int64_t)_pf_v; }(), (double)(last_pl), [&](){ auto _pf_v = (cur_x); return is_na(_pf_v) ? na<int64_t>() : (int64_t)_pf_v; }(), (double)(pl), XLoc::bar_index, false, false);
            }
            prev_pl_y = last_pl;
            prev_pl_x = last_pl_x;
            last_pl = pl;
            last_pl_x = [&](){ auto _pf_v = (cur_x); return is_na(_pf_v) ? na<int>() : (int)_pf_v; }();
        }
        ema_fast = (history_advances_new_bar() ? _ta_ema_3.compute(current_bar_.close) : _ta_ema_3.recompute(current_bar_.close));
        ema_mid = (history_advances_new_bar() ? _ta_ema_4.compute(current_bar_.close) : _ta_ema_4.recompute(current_bar_.close));
        ema_slow = (history_advances_new_bar() ? _ta_ema_5.compute(current_bar_.close) : _ta_ema_5.recompute(current_bar_.close));
        stack_bull = (([&]{ auto _pna_l = (ema_fast); auto _pna_r = (ema_mid); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()) && ([&]{ auto _pna_l = (ema_mid); auto _pna_r = (ema_slow); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()));
        stack_bear = (([&]{ auto _pna_l = (ema_fast); auto _pna_r = (ema_mid); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }()) && ([&]{ auto _pna_l = (ema_mid); auto _pna_r = (ema_slow); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }()));
        trend_bull = ([&]{ auto _pna_l = (ema_fast); auto _pna_r = (ema_mid); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }());
        trend_bear = ([&]{ auto _pna_l = (ema_fast); auto _pna_r = (ema_mid); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }());
        r = (history_advances_new_bar() ? _ta_rsi_6.compute(current_bar_.close) : _ta_rsi_6.recompute(current_bar_.close));
        mom_bull = ([&]{ auto _pna_l = (r); auto _pna_r = (i_rsi_lo); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }());
        mom_bear = ([&]{ auto _pna_l = (r); auto _pna_r = (i_rsi_hi); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }());
        if (history_advances_new_bar()) hi_now.push((history_advances_new_bar() ? _ta_highest_7.compute(current_bar_.high) : _ta_highest_7.recompute(current_bar_.high)));
        else hi_now.update((history_advances_new_bar() ? _ta_highest_7.compute(current_bar_.high) : _ta_highest_7.recompute(current_bar_.high)));
        if (history_advances_new_bar()) lo_now.push((history_advances_new_bar() ? _ta_lowest_8.compute(current_bar_.low) : _ta_lowest_8.recompute(current_bar_.low)));
        else lo_now.update((history_advances_new_bar() ? _ta_lowest_8.compute(current_bar_.low) : _ta_lowest_8.recompute(current_bar_.low)));
        break_hi = hi_now[1];
        break_lo = lo_now[1];
        struct_bull = ([&]{ auto _pna_l = (current_bar_.close); auto _pna_r = (break_hi); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }());
        struct_bear = ([&]{ auto _pna_l = (current_bar_.close); auto _pna_r = (break_lo); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }());
        gate_long = ((trend_bull && mom_bull) && struct_bull);
        gate_short = ((trend_bear && mom_bear) && struct_bear);
        go_long = ((stack_bull && gate_long) && !(is_na(last_pl)));
        go_short = ((stack_bear && gate_short) && !(is_na(last_ph)));
        if ((go_long && ([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) || _pfc_eq); }()))) {
            strategy_entry(std::string("L"), true, na<double>(), na<double>(), 1, std::string("integ trend long"), "", 0, -1);
        }
        if ((go_short && ([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) || _pfc_eq); }()))) {
            strategy_entry(std::string("S"), false, na<double>(), na<double>(), 1, std::string("integ trend short"), "", 0, -1);
        }
        entry_px = (signed_position_size() == 0.0 ? na<double>() : position_entry_price_);
        if ((([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()) && !(is_na(last_pl)))) {
            sl_px = last_pl;
            tp_px = (entry_px + ((entry_px - last_pl) * i_rr));
            strategy_exit(std::string("Brk"), std::string("L"), tp_px, sl_px, na<double>(), na<double>(), na<double>(), 100.0, "", na<double>(), "", na<double>(), na<double>());
        }
        if ((([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }()) && !(is_na(last_ph)))) {
            sl_px = last_ph;
            tp_px = (entry_px - ((last_ph - entry_px) * i_rr));
            strategy_exit(std::string("Brk"), std::string("S"), tp_px, sl_px, na<double>(), na<double>(), na<double>(), 100.0, "", na<double>(), "", na<double>(), na<double>());
        }
    }

    void precalculate(const Bar* bars, int n) {
        _use_precalc = false;
        if (n <= 0 || bars == nullptr) return;

        _precalc__ta_pivothigh_1.resize(n);
        _precalc__ta_pivotlow_2.resize(n);
        _precalc__ta_ema_3.resize(n);
        _precalc__ta_ema_4.resize(n);
        _precalc__ta_ema_5.resize(n);
        _precalc__ta_rsi_6.resize(n);
        _precalc__ta_highest_7.resize(n);
        _precalc__ta_lowest_8.resize(n);

        _ta_pivothigh_1 = ta::PivotHigh(get_input_int("Pivot strength", 5), get_input_int("Pivot strength", 5));
        _ta_pivotlow_2 = ta::PivotLow(get_input_int("Pivot strength", 5), get_input_int("Pivot strength", 5));
        _ta_ema_3 = ta::EMA(get_input_int("EMA fast", 21));
        _ta_ema_4 = ta::EMA(get_input_int("EMA mid", 55));
        _ta_ema_5 = ta::EMA(get_input_int("EMA slow", 200));
        _ta_rsi_6 = ta::RSI(get_input_int("RSI length", 14));
        _ta_highest_7 = ta::Highest(get_input_int("Breakout window", 20));
        _ta_lowest_8 = ta::Lowest(get_input_int("Breakout window", 20));


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
            _precalc__ta_pivothigh_1[i] = _ta_pivothigh_1.compute(bars[i].high);
            _precalc__ta_pivotlow_2[i] = _ta_pivotlow_2.compute(bars[i].low);
            _precalc__ta_ema_3[i] = _ta_ema_3.compute(bars[i].close);
            _precalc__ta_ema_4[i] = _ta_ema_4.compute(bars[i].close);
            _precalc__ta_ema_5[i] = _ta_ema_5.compute(bars[i].close);
            _precalc__ta_rsi_6[i] = _ta_rsi_6.compute(bars[i].close);
            _precalc__ta_highest_7[i] = _ta_highest_7.compute(bars[i].high);
            _precalc__ta_lowest_8[i] = _ta_lowest_8.compute(bars[i].low);
        }

        _ta_pivothigh_1 = ta::PivotHigh(get_input_int("Pivot strength", 5), get_input_int("Pivot strength", 5));
        _ta_pivotlow_2 = ta::PivotLow(get_input_int("Pivot strength", 5), get_input_int("Pivot strength", 5));
        _ta_ema_3 = ta::EMA(get_input_int("EMA fast", 21));
        _ta_ema_4 = ta::EMA(get_input_int("EMA mid", 55));
        _ta_ema_5 = ta::EMA(get_input_int("EMA slow", 200));
        _ta_rsi_6 = ta::RSI(get_input_int("RSI length", 14));
        _ta_highest_7 = ta::Highest(get_input_int("Breakout window", 20));
        _ta_lowest_8 = ta::Lowest(get_input_int("Breakout window", 20));

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
