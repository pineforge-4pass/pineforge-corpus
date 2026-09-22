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
#ifdef PF_PINE_TIME_HAS_SESSION_DAY
#define PF_PINE_TIME_SESSION_DAY_ARGS(tz, sess) , tz, sess
#else
#define PF_PINE_TIME_SESSION_DAY_ARGS(tz, sess)
#endif
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
    ta::PivotHigh _ta_pivothigh_2;
    std::vector<double> _precalc__ta_pivothigh_2;
    ta::PivotLow _ta_pivotlow_3;
    std::vector<double> _precalc__ta_pivotlow_3;
    ta::RSI _ta_rsi_4;
    std::vector<double> _precalc__ta_rsi_4;
    ta::EMA _ta_ema_5;
    ta::SMA _ta_sma_6;
    std::vector<double> _precalc__ta_sma_6;
    ta::StdDev _ta_stdev_7;
    std::vector<double> _precalc__ta_stdev_7;
    math::Sum _ta_sum_8;
    ta::Change _ta_change_9;
    std::vector<double> _precalc__ta_change_9;
    ta::Change _ta_change_10;
    std::vector<double> _precalc__ta_change_10;
    ta::RMA _ta_rma_11;
    ta::RMA _ta_rma_12;
    ta::RMA _ta_rma_13;
    ta::RMA _ta_rma_14;
    bool _use_precalc = false;
    Series<double> _s_close;
    Series<double> _s_high;
    Series<double> _s_low;
    Series<double> _s_open;
    double last_ph;
    double last_pl;
    std::vector<double> z_top;
    std::vector<double> z_bot;
    std::vector<bool> z_isb;
    Series<bool> in_session;
    Series<double> rsi_smooth;
    int i_pivot = 0;
    double i_fvg_atr = 0.0;
    int i_rsi_len = 0;
    int i_vol_ma_len = 0;
    double i_vol_z = 0.0;
    int i_cd_sum = 0;
    int i_adx_len = 0;
    double i_adx_thr = 0.0;
    std::string i_session = std::string("");
    std::string i_tz = std::string("");
    double atr_v = 0.0;
    double ph_v = 0.0;
    double pl_v = 0.0;
    bool pivot_break_up = false;
    bool pivot_break_dn = false;
    bool bull_fvg_event = false;
    bool bear_fvg_event = false;
    double fvg_min_w = 0.0;
    bool in_bull_fvg = false;
    bool in_bear_fvg = false;
    double rsi_v = 0.0;
    bool rsi_div_bull = false;
    bool rsi_div_bear = false;
    double vol_ma = 0.0;
    double vol_std = 0.0;
    double vol_z = 0.0;
    bool vol_anom_bull = false;
    bool vol_anom_bear = false;
    double buy_vol = 0.0;
    double sell_vol = 0.0;
    double vol_d = 0.0;
    double cum_d = 0.0;
    bool cd_up = false;
    bool cd_dn = false;
    double up_mv = 0.0;
    double dn_mv = 0.0;
    double p_dm_v = 0.0;
    double m_dm_v = 0.0;
    double tr_smo = 0.0;
    double p_di_v = 0.0;
    double m_di_v = 0.0;
    double dx_v = 0.0;
    double adx_v = 0.0;
    bool trending_bull = false;
    bool trending_bear = false;
    bool long_setup = false;
    bool short_setup = false;
    bool _var_initialized = false;
    bool _ta_initialized_ = false;
    bool _inputs_initialized_ = false;

    struct _PFScriptState {
        decltype(GeneratedStrategy::_ta_atr_1) _pf_value_0;
        decltype(GeneratedStrategy::_ta_pivothigh_2) _pf_value_1;
        decltype(GeneratedStrategy::_ta_pivotlow_3) _pf_value_2;
        decltype(GeneratedStrategy::_ta_rsi_4) _pf_value_3;
        decltype(GeneratedStrategy::_ta_ema_5) _pf_value_4;
        decltype(GeneratedStrategy::_ta_sma_6) _pf_value_5;
        decltype(GeneratedStrategy::_ta_stdev_7) _pf_value_6;
        decltype(GeneratedStrategy::_ta_sum_8) _pf_value_7;
        decltype(GeneratedStrategy::_ta_change_9) _pf_value_8;
        decltype(GeneratedStrategy::_ta_change_10) _pf_value_9;
        decltype(GeneratedStrategy::_ta_rma_11) _pf_value_10;
        decltype(GeneratedStrategy::_ta_rma_12) _pf_value_11;
        decltype(GeneratedStrategy::_ta_rma_13) _pf_value_12;
        decltype(GeneratedStrategy::_ta_rma_14) _pf_value_13;
        decltype(GeneratedStrategy::_s_close) _pf_value_14;
        decltype(GeneratedStrategy::_s_high) _pf_value_15;
        decltype(GeneratedStrategy::_s_low) _pf_value_16;
        decltype(GeneratedStrategy::_s_open) _pf_value_17;
        decltype(GeneratedStrategy::last_ph) _pf_value_18;
        decltype(GeneratedStrategy::last_pl) _pf_value_19;
        decltype(GeneratedStrategy::z_top) _pf_value_20;
        decltype(GeneratedStrategy::z_bot) _pf_value_21;
        decltype(GeneratedStrategy::z_isb) _pf_value_22;
        decltype(GeneratedStrategy::in_session) _pf_value_23;
        decltype(GeneratedStrategy::rsi_smooth) _pf_value_24;
        decltype(GeneratedStrategy::i_pivot) _pf_value_25;
        decltype(GeneratedStrategy::i_fvg_atr) _pf_value_26;
        decltype(GeneratedStrategy::i_rsi_len) _pf_value_27;
        decltype(GeneratedStrategy::i_vol_ma_len) _pf_value_28;
        decltype(GeneratedStrategy::i_vol_z) _pf_value_29;
        decltype(GeneratedStrategy::i_cd_sum) _pf_value_30;
        decltype(GeneratedStrategy::i_adx_len) _pf_value_31;
        decltype(GeneratedStrategy::i_adx_thr) _pf_value_32;
        decltype(GeneratedStrategy::i_session) _pf_value_33;
        decltype(GeneratedStrategy::i_tz) _pf_value_34;
        decltype(GeneratedStrategy::atr_v) _pf_value_35;
        decltype(GeneratedStrategy::ph_v) _pf_value_36;
        decltype(GeneratedStrategy::pl_v) _pf_value_37;
        decltype(GeneratedStrategy::pivot_break_up) _pf_value_38;
        decltype(GeneratedStrategy::pivot_break_dn) _pf_value_39;
        decltype(GeneratedStrategy::bull_fvg_event) _pf_value_40;
        decltype(GeneratedStrategy::bear_fvg_event) _pf_value_41;
        decltype(GeneratedStrategy::fvg_min_w) _pf_value_42;
        decltype(GeneratedStrategy::in_bull_fvg) _pf_value_43;
        decltype(GeneratedStrategy::in_bear_fvg) _pf_value_44;
        decltype(GeneratedStrategy::rsi_v) _pf_value_45;
        decltype(GeneratedStrategy::rsi_div_bull) _pf_value_46;
        decltype(GeneratedStrategy::rsi_div_bear) _pf_value_47;
        decltype(GeneratedStrategy::vol_ma) _pf_value_48;
        decltype(GeneratedStrategy::vol_std) _pf_value_49;
        decltype(GeneratedStrategy::vol_z) _pf_value_50;
        decltype(GeneratedStrategy::vol_anom_bull) _pf_value_51;
        decltype(GeneratedStrategy::vol_anom_bear) _pf_value_52;
        decltype(GeneratedStrategy::buy_vol) _pf_value_53;
        decltype(GeneratedStrategy::sell_vol) _pf_value_54;
        decltype(GeneratedStrategy::vol_d) _pf_value_55;
        decltype(GeneratedStrategy::cum_d) _pf_value_56;
        decltype(GeneratedStrategy::cd_up) _pf_value_57;
        decltype(GeneratedStrategy::cd_dn) _pf_value_58;
        decltype(GeneratedStrategy::up_mv) _pf_value_59;
        decltype(GeneratedStrategy::dn_mv) _pf_value_60;
        decltype(GeneratedStrategy::p_dm_v) _pf_value_61;
        decltype(GeneratedStrategy::m_dm_v) _pf_value_62;
        decltype(GeneratedStrategy::tr_smo) _pf_value_63;
        decltype(GeneratedStrategy::p_di_v) _pf_value_64;
        decltype(GeneratedStrategy::m_di_v) _pf_value_65;
        decltype(GeneratedStrategy::dx_v) _pf_value_66;
        decltype(GeneratedStrategy::adx_v) _pf_value_67;
        decltype(GeneratedStrategy::trending_bull) _pf_value_68;
        decltype(GeneratedStrategy::trending_bear) _pf_value_69;
        decltype(GeneratedStrategy::long_setup) _pf_value_70;
        decltype(GeneratedStrategy::short_setup) _pf_value_71;
        decltype(GeneratedStrategy::_var_initialized) _pf_value_72;
        decltype(GeneratedStrategy::_ta_initialized_) _pf_value_73;
        decltype(GeneratedStrategy::_inputs_initialized_) _pf_value_74;
    };
    static_assert(std::is_copy_constructible_v<_PFScriptState>, "generated Pine state must be deep-copy constructible");
    static_assert(std::is_copy_assignable_v<_PFScriptState>, "generated Pine state must be deep-copy assignable");
    std::optional<_PFScriptState> _pf_script_state_checkpoint_;

    void snapshot_script_state() override {
        _pf_script_state_checkpoint_.emplace(_PFScriptState{
            _ta_atr_1,
            _ta_pivothigh_2,
            _ta_pivotlow_3,
            _ta_rsi_4,
            _ta_ema_5,
            _ta_sma_6,
            _ta_stdev_7,
            _ta_sum_8,
            _ta_change_9,
            _ta_change_10,
            _ta_rma_11,
            _ta_rma_12,
            _ta_rma_13,
            _ta_rma_14,
            _s_close,
            _s_high,
            _s_low,
            _s_open,
            last_ph,
            last_pl,
            z_top,
            z_bot,
            z_isb,
            in_session,
            rsi_smooth,
            i_pivot,
            i_fvg_atr,
            i_rsi_len,
            i_vol_ma_len,
            i_vol_z,
            i_cd_sum,
            i_adx_len,
            i_adx_thr,
            i_session,
            i_tz,
            atr_v,
            ph_v,
            pl_v,
            pivot_break_up,
            pivot_break_dn,
            bull_fvg_event,
            bear_fvg_event,
            fvg_min_w,
            in_bull_fvg,
            in_bear_fvg,
            rsi_v,
            rsi_div_bull,
            rsi_div_bear,
            vol_ma,
            vol_std,
            vol_z,
            vol_anom_bull,
            vol_anom_bear,
            buy_vol,
            sell_vol,
            vol_d,
            cum_d,
            cd_up,
            cd_dn,
            up_mv,
            dn_mv,
            p_dm_v,
            m_dm_v,
            tr_smo,
            p_di_v,
            m_di_v,
            dx_v,
            adx_v,
            trending_bull,
            trending_bear,
            long_setup,
            short_setup,
            _var_initialized,
            _ta_initialized_,
            _inputs_initialized_,
        });
    }

    void restore_script_state() override {
        if (!_pf_script_state_checkpoint_) return;
        this->_ta_atr_1 = _pf_script_state_checkpoint_->_pf_value_0;
        this->_ta_pivothigh_2 = _pf_script_state_checkpoint_->_pf_value_1;
        this->_ta_pivotlow_3 = _pf_script_state_checkpoint_->_pf_value_2;
        this->_ta_rsi_4 = _pf_script_state_checkpoint_->_pf_value_3;
        this->_ta_ema_5 = _pf_script_state_checkpoint_->_pf_value_4;
        this->_ta_sma_6 = _pf_script_state_checkpoint_->_pf_value_5;
        this->_ta_stdev_7 = _pf_script_state_checkpoint_->_pf_value_6;
        this->_ta_sum_8 = _pf_script_state_checkpoint_->_pf_value_7;
        this->_ta_change_9 = _pf_script_state_checkpoint_->_pf_value_8;
        this->_ta_change_10 = _pf_script_state_checkpoint_->_pf_value_9;
        this->_ta_rma_11 = _pf_script_state_checkpoint_->_pf_value_10;
        this->_ta_rma_12 = _pf_script_state_checkpoint_->_pf_value_11;
        this->_ta_rma_13 = _pf_script_state_checkpoint_->_pf_value_12;
        this->_ta_rma_14 = _pf_script_state_checkpoint_->_pf_value_13;
        this->_s_close = _pf_script_state_checkpoint_->_pf_value_14;
        this->_s_high = _pf_script_state_checkpoint_->_pf_value_15;
        this->_s_low = _pf_script_state_checkpoint_->_pf_value_16;
        this->_s_open = _pf_script_state_checkpoint_->_pf_value_17;
        this->last_ph = _pf_script_state_checkpoint_->_pf_value_18;
        this->last_pl = _pf_script_state_checkpoint_->_pf_value_19;
        this->z_top = _pf_script_state_checkpoint_->_pf_value_20;
        this->z_bot = _pf_script_state_checkpoint_->_pf_value_21;
        this->z_isb = _pf_script_state_checkpoint_->_pf_value_22;
        this->in_session = _pf_script_state_checkpoint_->_pf_value_23;
        this->rsi_smooth = _pf_script_state_checkpoint_->_pf_value_24;
        this->i_pivot = _pf_script_state_checkpoint_->_pf_value_25;
        this->i_fvg_atr = _pf_script_state_checkpoint_->_pf_value_26;
        this->i_rsi_len = _pf_script_state_checkpoint_->_pf_value_27;
        this->i_vol_ma_len = _pf_script_state_checkpoint_->_pf_value_28;
        this->i_vol_z = _pf_script_state_checkpoint_->_pf_value_29;
        this->i_cd_sum = _pf_script_state_checkpoint_->_pf_value_30;
        this->i_adx_len = _pf_script_state_checkpoint_->_pf_value_31;
        this->i_adx_thr = _pf_script_state_checkpoint_->_pf_value_32;
        this->i_session = _pf_script_state_checkpoint_->_pf_value_33;
        this->i_tz = _pf_script_state_checkpoint_->_pf_value_34;
        this->atr_v = _pf_script_state_checkpoint_->_pf_value_35;
        this->ph_v = _pf_script_state_checkpoint_->_pf_value_36;
        this->pl_v = _pf_script_state_checkpoint_->_pf_value_37;
        this->pivot_break_up = _pf_script_state_checkpoint_->_pf_value_38;
        this->pivot_break_dn = _pf_script_state_checkpoint_->_pf_value_39;
        this->bull_fvg_event = _pf_script_state_checkpoint_->_pf_value_40;
        this->bear_fvg_event = _pf_script_state_checkpoint_->_pf_value_41;
        this->fvg_min_w = _pf_script_state_checkpoint_->_pf_value_42;
        this->in_bull_fvg = _pf_script_state_checkpoint_->_pf_value_43;
        this->in_bear_fvg = _pf_script_state_checkpoint_->_pf_value_44;
        this->rsi_v = _pf_script_state_checkpoint_->_pf_value_45;
        this->rsi_div_bull = _pf_script_state_checkpoint_->_pf_value_46;
        this->rsi_div_bear = _pf_script_state_checkpoint_->_pf_value_47;
        this->vol_ma = _pf_script_state_checkpoint_->_pf_value_48;
        this->vol_std = _pf_script_state_checkpoint_->_pf_value_49;
        this->vol_z = _pf_script_state_checkpoint_->_pf_value_50;
        this->vol_anom_bull = _pf_script_state_checkpoint_->_pf_value_51;
        this->vol_anom_bear = _pf_script_state_checkpoint_->_pf_value_52;
        this->buy_vol = _pf_script_state_checkpoint_->_pf_value_53;
        this->sell_vol = _pf_script_state_checkpoint_->_pf_value_54;
        this->vol_d = _pf_script_state_checkpoint_->_pf_value_55;
        this->cum_d = _pf_script_state_checkpoint_->_pf_value_56;
        this->cd_up = _pf_script_state_checkpoint_->_pf_value_57;
        this->cd_dn = _pf_script_state_checkpoint_->_pf_value_58;
        this->up_mv = _pf_script_state_checkpoint_->_pf_value_59;
        this->dn_mv = _pf_script_state_checkpoint_->_pf_value_60;
        this->p_dm_v = _pf_script_state_checkpoint_->_pf_value_61;
        this->m_dm_v = _pf_script_state_checkpoint_->_pf_value_62;
        this->tr_smo = _pf_script_state_checkpoint_->_pf_value_63;
        this->p_di_v = _pf_script_state_checkpoint_->_pf_value_64;
        this->m_di_v = _pf_script_state_checkpoint_->_pf_value_65;
        this->dx_v = _pf_script_state_checkpoint_->_pf_value_66;
        this->adx_v = _pf_script_state_checkpoint_->_pf_value_67;
        this->trending_bull = _pf_script_state_checkpoint_->_pf_value_68;
        this->trending_bear = _pf_script_state_checkpoint_->_pf_value_69;
        this->long_setup = _pf_script_state_checkpoint_->_pf_value_70;
        this->short_setup = _pf_script_state_checkpoint_->_pf_value_71;
        this->_var_initialized = _pf_script_state_checkpoint_->_pf_value_72;
        this->_ta_initialized_ = _pf_script_state_checkpoint_->_pf_value_73;
        this->_inputs_initialized_ = _pf_script_state_checkpoint_->_pf_value_74;
    }

    void commit_script_state() override {
        snapshot_script_state();
    }

    explicit GeneratedStrategy() : _ta_atr_1(14), _ta_pivothigh_2(5, 5), _ta_pivotlow_3(5, 5), _ta_rsi_4(14), _ta_ema_5(3), _ta_sma_6(20), _ta_stdev_7(20), _ta_sum_8(10), _ta_rma_11(14), _ta_rma_12(14), _ta_rma_13(14), _ta_rma_14(14), last_ph(na<double>()), last_pl(na<double>()) {
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
        this->_ta_pivothigh_2 = decltype(this->_ta_pivothigh_2)(5, 5);
        this->_precalc__ta_pivothigh_2 = decltype(this->_precalc__ta_pivothigh_2){};
        this->_ta_pivotlow_3 = decltype(this->_ta_pivotlow_3)(5, 5);
        this->_precalc__ta_pivotlow_3 = decltype(this->_precalc__ta_pivotlow_3){};
        this->_ta_rsi_4 = decltype(this->_ta_rsi_4)(14);
        this->_precalc__ta_rsi_4 = decltype(this->_precalc__ta_rsi_4){};
        this->_ta_ema_5 = decltype(this->_ta_ema_5)(3);
        this->_ta_sma_6 = decltype(this->_ta_sma_6)(20);
        this->_precalc__ta_sma_6 = decltype(this->_precalc__ta_sma_6){};
        this->_ta_stdev_7 = decltype(this->_ta_stdev_7)(20);
        this->_precalc__ta_stdev_7 = decltype(this->_precalc__ta_stdev_7){};
        this->_ta_sum_8 = decltype(this->_ta_sum_8)(10);
        this->_ta_change_9 = decltype(this->_ta_change_9){};
        this->_precalc__ta_change_9 = decltype(this->_precalc__ta_change_9){};
        this->_ta_change_10 = decltype(this->_ta_change_10){};
        this->_precalc__ta_change_10 = decltype(this->_precalc__ta_change_10){};
        this->_ta_rma_11 = decltype(this->_ta_rma_11)(14);
        this->_ta_rma_12 = decltype(this->_ta_rma_12)(14);
        this->_ta_rma_13 = decltype(this->_ta_rma_13)(14);
        this->_ta_rma_14 = decltype(this->_ta_rma_14)(14);
        this->_use_precalc = false;
        this->_s_close = decltype(this->_s_close){};
        this->_s_high = decltype(this->_s_high){};
        this->_s_low = decltype(this->_s_low){};
        this->_s_open = decltype(this->_s_open){};
        this->last_ph = decltype(this->last_ph)(na<double>());
        this->last_pl = decltype(this->last_pl)(na<double>());
        this->z_top = decltype(this->z_top){};
        this->z_bot = decltype(this->z_bot){};
        this->z_isb = decltype(this->z_isb){};
        this->in_session = decltype(this->in_session){};
        this->rsi_smooth = decltype(this->rsi_smooth){};
        this->i_pivot = 0;
        this->i_fvg_atr = 0.0;
        this->i_rsi_len = 0;
        this->i_vol_ma_len = 0;
        this->i_vol_z = 0.0;
        this->i_cd_sum = 0;
        this->i_adx_len = 0;
        this->i_adx_thr = 0.0;
        this->i_session = std::string("");
        this->i_tz = std::string("");
        this->atr_v = 0.0;
        this->ph_v = 0.0;
        this->pl_v = 0.0;
        this->pivot_break_up = false;
        this->pivot_break_dn = false;
        this->bull_fvg_event = false;
        this->bear_fvg_event = false;
        this->fvg_min_w = 0.0;
        this->in_bull_fvg = false;
        this->in_bear_fvg = false;
        this->rsi_v = 0.0;
        this->rsi_div_bull = false;
        this->rsi_div_bear = false;
        this->vol_ma = 0.0;
        this->vol_std = 0.0;
        this->vol_z = 0.0;
        this->vol_anom_bull = false;
        this->vol_anom_bear = false;
        this->buy_vol = 0.0;
        this->sell_vol = 0.0;
        this->vol_d = 0.0;
        this->cum_d = 0.0;
        this->cd_up = false;
        this->cd_dn = false;
        this->up_mv = 0.0;
        this->dn_mv = 0.0;
        this->p_dm_v = 0.0;
        this->m_dm_v = 0.0;
        this->tr_smo = 0.0;
        this->p_di_v = 0.0;
        this->m_di_v = 0.0;
        this->dx_v = 0.0;
        this->adx_v = 0.0;
        this->trending_bull = false;
        this->trending_bear = false;
        this->long_setup = false;
        this->short_setup = false;
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
            z_top = std::vector<double>();
            z_bot = std::vector<double>();
            z_isb = std::vector<bool>();
            _var_initialized = true;
        } else {
        }
        if (!_inputs_initialized_) {
            i_pivot = get_input_int("Pivot strength", 5);
            i_fvg_atr = get_input_double("FVG min size (atr fraction)", 0.3);
            i_rsi_len = get_input_int("RSI length", 14);
            i_vol_ma_len = get_input_int("Volume MA length", 20);
            i_vol_z = get_input_double("Volume z threshold", 2.0);
            i_cd_sum = get_input_int("Cum-delta window", 10);
            i_adx_len = get_input_int("ADX length", 14);
            i_adx_thr = get_input_double("ADX trend threshold", 25);
            i_session = get_input_string("Active session", std::string("0800-1600"));
            i_tz = get_input_string("Timezone", std::string("America/New_York"));
            _inputs_initialized_ = true;
        }
        if (!_ta_initialized_) {
            _ta_pivothigh_2 = ta::PivotHigh(get_input_int("Pivot strength", 5), get_input_int("Pivot strength", 5));
            _ta_pivotlow_3 = ta::PivotLow(get_input_int("Pivot strength", 5), get_input_int("Pivot strength", 5));
            _ta_rsi_4 = ta::RSI(get_input_int("RSI length", 14));
            _ta_sma_6 = ta::SMA(get_input_int("Volume MA length", 20));
            _ta_sum_8 = math::Sum(get_input_int("Cum-delta window", 10));
            _ta_rma_11 = ta::RMA(get_input_int("ADX length", 14));
            _ta_rma_12 = ta::RMA(get_input_int("ADX length", 14));
            _ta_rma_13 = ta::RMA(get_input_int("ADX length", 14));
            _ta_rma_14 = ta::RMA(get_input_int("ADX length", 14));
            _ta_initialized_ = true;
        }
        atr_v = (history_advances_new_bar() ? _ta_atr_1.compute(current_bar_.high, current_bar_.low, current_bar_.close, prev_chart_close()) : _ta_atr_1.recompute(current_bar_.high, current_bar_.low, current_bar_.close, prev_chart_close()));
        ph_v = (history_advances_new_bar() ? _ta_pivothigh_2.compute(current_bar_.high) : _ta_pivothigh_2.recompute(current_bar_.high));
        pl_v = (history_advances_new_bar() ? _ta_pivotlow_3.compute(current_bar_.low) : _ta_pivotlow_3.recompute(current_bar_.low));
        if (!(is_na(ph_v))) {
            last_ph = ph_v;
        }
        if (!(is_na(pl_v))) {
            last_pl = pl_v;
        }
        pivot_break_up = (!(is_na(last_ph)) && ([&]{ auto _pna_l = (current_bar_.close); auto _pna_r = (last_ph); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()));
        pivot_break_dn = (!(is_na(last_pl)) && ([&]{ auto _pna_l = (current_bar_.close); auto _pna_r = (last_pl); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }()));
        bull_fvg_event = (([&]{ auto _pna_l = (current_bar_.low); auto _pna_r = (_s_high[2]); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()) && ([&]{ auto _pna_l = (_s_close[1]); auto _pna_r = (_s_open[1]); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()));
        bear_fvg_event = (([&]{ auto _pna_l = (current_bar_.high); auto _pna_r = (_s_low[2]); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }()) && ([&]{ auto _pna_l = (_s_close[1]); auto _pna_r = (_s_open[1]); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }()));
        fvg_min_w = (atr_v * i_fvg_atr);
        if ((bull_fvg_event && ([&]{ auto _pna_l = ((current_bar_.low - _s_high[2])); auto _pna_r = (fvg_min_w); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) || _pfc_eq); }()))) {
            z_top.push_back(current_bar_.low);
            z_bot.push_back(_s_high[2]);
            z_isb.push_back(true);
        }
        if ((bear_fvg_event && ([&]{ auto _pna_l = ((_s_low[2] - current_bar_.high)); auto _pna_r = (fvg_min_w); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) || _pfc_eq); }()))) {
            z_top.push_back(_s_low[2]);
            z_bot.push_back(current_bar_.high);
            z_isb.push_back(false);
        }
        while (([&]{ auto _pna_l = ((double)z_top.size()); auto _pna_r = (30); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) {
            [&](auto&& __pf_array){ if(__pf_array.empty()) pine_runtime_error("Cannot use shift() if array is empty."); using __pf_array_value_type=typename std::decay_t<decltype(__pf_array)>::value_type; __pf_array_value_type __pf_array_value=__pf_array.front(); __pf_array.erase(__pf_array.begin()); return __pf_array_value; }((z_top));
            [&](auto&& __pf_array){ if(__pf_array.empty()) pine_runtime_error("Cannot use shift() if array is empty."); using __pf_array_value_type=typename std::decay_t<decltype(__pf_array)>::value_type; __pf_array_value_type __pf_array_value=__pf_array.front(); __pf_array.erase(__pf_array.begin()); return __pf_array_value; }((z_bot));
            [&](auto&& __pf_array){ if(__pf_array.empty()) pine_runtime_error("Cannot use shift() if array is empty."); using __pf_array_value_type=typename std::decay_t<decltype(__pf_array)>::value_type; __pf_array_value_type __pf_array_value=__pf_array.front(); __pf_array.erase(__pf_array.begin()); return __pf_array_value; }((z_isb));
        }
        in_bull_fvg = false;
        in_bear_fvg = false;
        if (([&]{ auto _pna_l = ((double)z_top.size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) {
            int _for_start_0 = (0);
            int _for_end_0 = [&](){ double _pf_v = (double)((((double)z_top.size() - 1))); return is_na(_pf_v) ? na<int>() : (int)_pf_v; }();
            int _for_step_0 = (1);
            if (!is_na(_for_step_0) && _for_step_0 < 0) _for_step_0 = -_for_step_0;
            if (_for_step_0 == 0) _for_step_0 = 1;
            const bool _for_down_0 = (_for_start_0 > _for_end_0);
            for (int k = _for_start_0; !is_na(_for_start_0) && !is_na(_for_end_0) && !is_na(_for_step_0) && (_for_down_0 ? (k >= _for_end_0) : (k <= _for_end_0)); k += (_for_down_0 ? -_for_step_0 : _for_step_0), _for_end_0 = [&](){ double _pf_v = (double)((((double)z_top.size() - 1))); return is_na(_pf_v) ? na<int>() : (int)_pf_v; }()) {
                double t = [&](auto&& __pf_array)->decltype(auto){ return [&](auto&& __pf_raw_index_value)->decltype(auto){ using __pf_raw_index_type=std::decay_t<decltype(__pf_raw_index_value)>; if constexpr(!std::is_same_v<__pf_raw_index_type,bool>) { if(is_na(__pf_raw_index_value)) pine_runtime_error(std::string("Index na is out of bounds. Array size is ")+std::to_string((int64_t)__pf_array.size())); } if constexpr(std::is_floating_point_v<__pf_raw_index_type>) { if(!std::isfinite(__pf_raw_index_value)) { std::string __pf_raw_index_text=__pf_raw_index_value>0?"inf":"-inf"; pine_runtime_error(std::string("Index ")+__pf_raw_index_text+" is out of bounds. Array size is "+std::to_string((int64_t)__pf_array.size())); } long double __pf_raw_index_wide=(long double)__pf_raw_index_value; if(__pf_raw_index_wide<(long double)std::numeric_limits<int64_t>::min()||__pf_raw_index_wide>(long double)std::numeric_limits<int64_t>::max()) pine_runtime_error(std::string("Index ")+std::to_string((double)__pf_raw_index_value)+" is out of bounds. Array size is "+std::to_string((int64_t)__pf_array.size())); } int64_t __pf_raw_index=(int64_t)__pf_raw_index_value; int64_t __pf_array_size=(int64_t)__pf_array.size(); int64_t __pf_array_index=__pf_raw_index<0?__pf_raw_index+__pf_array_size:__pf_raw_index; if(__pf_array_index<0||__pf_array_index>=__pf_array_size) pine_runtime_error(std::string("Index ")+std::to_string(__pf_raw_index)+" is out of bounds. Array size is "+std::to_string(__pf_array_size)); if constexpr(std::is_lvalue_reference_v<decltype(__pf_array)>) return (__pf_array[(size_t)__pf_array_index]); else { using __pf_array_value_type=typename std::decay_t<decltype(__pf_array)>::value_type; return __pf_array_value_type(__pf_array[(size_t)__pf_array_index]); } }((k)); }((z_top));
                double b = [&](auto&& __pf_array)->decltype(auto){ return [&](auto&& __pf_raw_index_value)->decltype(auto){ using __pf_raw_index_type=std::decay_t<decltype(__pf_raw_index_value)>; if constexpr(!std::is_same_v<__pf_raw_index_type,bool>) { if(is_na(__pf_raw_index_value)) pine_runtime_error(std::string("Index na is out of bounds. Array size is ")+std::to_string((int64_t)__pf_array.size())); } if constexpr(std::is_floating_point_v<__pf_raw_index_type>) { if(!std::isfinite(__pf_raw_index_value)) { std::string __pf_raw_index_text=__pf_raw_index_value>0?"inf":"-inf"; pine_runtime_error(std::string("Index ")+__pf_raw_index_text+" is out of bounds. Array size is "+std::to_string((int64_t)__pf_array.size())); } long double __pf_raw_index_wide=(long double)__pf_raw_index_value; if(__pf_raw_index_wide<(long double)std::numeric_limits<int64_t>::min()||__pf_raw_index_wide>(long double)std::numeric_limits<int64_t>::max()) pine_runtime_error(std::string("Index ")+std::to_string((double)__pf_raw_index_value)+" is out of bounds. Array size is "+std::to_string((int64_t)__pf_array.size())); } int64_t __pf_raw_index=(int64_t)__pf_raw_index_value; int64_t __pf_array_size=(int64_t)__pf_array.size(); int64_t __pf_array_index=__pf_raw_index<0?__pf_raw_index+__pf_array_size:__pf_raw_index; if(__pf_array_index<0||__pf_array_index>=__pf_array_size) pine_runtime_error(std::string("Index ")+std::to_string(__pf_raw_index)+" is out of bounds. Array size is "+std::to_string(__pf_array_size)); if constexpr(std::is_lvalue_reference_v<decltype(__pf_array)>) return (__pf_array[(size_t)__pf_array_index]); else { using __pf_array_value_type=typename std::decay_t<decltype(__pf_array)>::value_type; return __pf_array_value_type(__pf_array[(size_t)__pf_array_index]); } }((k)); }((z_bot));
                bool s = [&](auto&& __pf_array)->decltype(auto){ return [&](auto&& __pf_raw_index_value)->decltype(auto){ using __pf_raw_index_type=std::decay_t<decltype(__pf_raw_index_value)>; if constexpr(!std::is_same_v<__pf_raw_index_type,bool>) { if(is_na(__pf_raw_index_value)) pine_runtime_error(std::string("Index na is out of bounds. Array size is ")+std::to_string((int64_t)__pf_array.size())); } if constexpr(std::is_floating_point_v<__pf_raw_index_type>) { if(!std::isfinite(__pf_raw_index_value)) { std::string __pf_raw_index_text=__pf_raw_index_value>0?"inf":"-inf"; pine_runtime_error(std::string("Index ")+__pf_raw_index_text+" is out of bounds. Array size is "+std::to_string((int64_t)__pf_array.size())); } long double __pf_raw_index_wide=(long double)__pf_raw_index_value; if(__pf_raw_index_wide<(long double)std::numeric_limits<int64_t>::min()||__pf_raw_index_wide>(long double)std::numeric_limits<int64_t>::max()) pine_runtime_error(std::string("Index ")+std::to_string((double)__pf_raw_index_value)+" is out of bounds. Array size is "+std::to_string((int64_t)__pf_array.size())); } int64_t __pf_raw_index=(int64_t)__pf_raw_index_value; int64_t __pf_array_size=(int64_t)__pf_array.size(); int64_t __pf_array_index=__pf_raw_index<0?__pf_raw_index+__pf_array_size:__pf_raw_index; if(__pf_array_index<0||__pf_array_index>=__pf_array_size) pine_runtime_error(std::string("Index ")+std::to_string(__pf_raw_index)+" is out of bounds. Array size is "+std::to_string(__pf_array_size)); if constexpr(std::is_lvalue_reference_v<decltype(__pf_array)>) return (__pf_array[(size_t)__pf_array_index]); else { using __pf_array_value_type=typename std::decay_t<decltype(__pf_array)>::value_type; return __pf_array_value_type(__pf_array[(size_t)__pf_array_index]); } }((k)); }((z_isb));
                if ((([&]{ auto _pna_l = (current_bar_.low); auto _pna_r = (t); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) || _pfc_eq); }()) && ([&]{ auto _pna_l = (current_bar_.high); auto _pna_r = (b); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) || _pfc_eq); }()))) {
                    if (s) {
                        in_bull_fvg = true;
                    } else {
                        in_bear_fvg = true;
                    }
                }
            }
        }
        rsi_v = (history_advances_new_bar() ? _ta_rsi_4.compute(current_bar_.close) : _ta_rsi_4.recompute(current_bar_.close));
        if (history_advances_new_bar()) rsi_smooth.push((history_advances_new_bar() ? _ta_ema_5.compute(rsi_v) : _ta_ema_5.recompute(rsi_v)));
        else rsi_smooth.update((history_advances_new_bar() ? _ta_ema_5.compute(rsi_v) : _ta_ema_5.recompute(rsi_v)));
        rsi_div_bull = ((([&]{ auto _pna_l = (rsi_smooth[0]); auto _pna_r = (40); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }()) && ([&]{ auto _pna_l = (current_bar_.close); auto _pna_r = (_s_close[5]); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) && ([&]{ auto _pna_l = (rsi_smooth[0]); auto _pna_r = (rsi_smooth[5]); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()));
        rsi_div_bear = ((([&]{ auto _pna_l = (rsi_smooth[0]); auto _pna_r = (60); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()) && ([&]{ auto _pna_l = (current_bar_.close); auto _pna_r = (_s_close[5]); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }())) && ([&]{ auto _pna_l = (rsi_smooth[0]); auto _pna_r = (rsi_smooth[5]); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }()));
        vol_ma = (history_advances_new_bar() ? _ta_sma_6.compute(current_bar_.volume) : _ta_sma_6.recompute(current_bar_.volume));
        vol_std = (history_advances_new_bar() ? _ta_stdev_7.compute(current_bar_.volume) : _ta_stdev_7.recompute(current_bar_.volume));
        vol_z = ((([&]{ auto _pna_l = (vol_std); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) ? (((double)((current_bar_.volume - vol_ma)) / (double)(vol_std))) : (0));
        vol_anom_bull = (([&]{ auto _pna_l = (std::abs(vol_z)); auto _pna_r = (i_vol_z); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()) && ([&]{ auto _pna_l = (current_bar_.close); auto _pna_r = (current_bar_.open); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()));
        vol_anom_bear = (([&]{ auto _pna_l = (std::abs(vol_z)); auto _pna_r = (i_vol_z); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()) && ([&]{ auto _pna_l = (current_bar_.close); auto _pna_r = (current_bar_.open); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }()));
        buy_vol = ((([&]{ auto _pna_l = (current_bar_.close); auto _pna_r = (current_bar_.open); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) ? (current_bar_.volume) : (((double)((current_bar_.volume * (current_bar_.close - current_bar_.low))) / (double)(((current_bar_.high - current_bar_.low) + 0.0001)))));
        sell_vol = ((([&]{ auto _pna_l = (current_bar_.close); auto _pna_r = (current_bar_.open); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }())) ? (current_bar_.volume) : (((double)((current_bar_.volume * (current_bar_.high - current_bar_.close))) / (double)(((current_bar_.high - current_bar_.low) + 0.0001)))));
        vol_d = (buy_vol - sell_vol);
        cum_d = (history_advances_new_bar() ? _ta_sum_8.compute(vol_d) : _ta_sum_8.recompute(vol_d));
        cd_up = ([&]{ auto _pna_l = (cum_d); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }());
        cd_dn = ([&]{ auto _pna_l = (cum_d); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }());
        up_mv = (history_advances_new_bar() ? _ta_change_9.compute(current_bar_.high) : _ta_change_9.recompute(current_bar_.high));
        dn_mv = (-(_use_precalc ? _precalc__ta_change_10[bar_index_] : (history_advances_new_bar() ? _ta_change_10.compute(current_bar_.low) : _ta_change_10.recompute(current_bar_.low))));
        p_dm_v = ((is_na(up_mv)) ? (na<double>()) : ((((([&]{ auto _pna_l = (up_mv); auto _pna_r = (dn_mv); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()) && ([&]{ auto _pna_l = (up_mv); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()))) ? (up_mv) : (0))));
        m_dm_v = ((is_na(dn_mv)) ? (na<double>()) : ((((([&]{ auto _pna_l = (dn_mv); auto _pna_r = (up_mv); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()) && ([&]{ auto _pna_l = (dn_mv); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()))) ? (dn_mv) : (0))));
        tr_smo = (history_advances_new_bar() ? _ta_rma_11.compute((std::isnan(_s_close[1]) ? (current_bar_.high - current_bar_.low) : std::max(current_bar_.high - current_bar_.low, std::max(std::abs(current_bar_.high - _s_close[1]), std::abs(current_bar_.low - _s_close[1]))))) : _ta_rma_11.recompute((std::isnan(_s_close[1]) ? (current_bar_.high - current_bar_.low) : std::max(current_bar_.high - current_bar_.low, std::max(std::abs(current_bar_.high - _s_close[1]), std::abs(current_bar_.low - _s_close[1]))))));
        p_di_v = ((([&]{ auto _pna_l = (tr_smo); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) ? (((double)((100 * (history_advances_new_bar() ? _ta_rma_12.compute(p_dm_v) : _ta_rma_12.recompute(p_dm_v)))) / (double)(tr_smo))) : (0));
        m_di_v = ((([&]{ auto _pna_l = (tr_smo); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) ? (((double)((100 * (history_advances_new_bar() ? _ta_rma_13.compute(m_dm_v) : _ta_rma_13.recompute(m_dm_v)))) / (double)(tr_smo))) : (0));
        dx_v = ((([&]{ auto _pna_l = ((p_di_v + m_di_v)); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) ? (((double)((100 * std::abs((p_di_v - m_di_v)))) / (double)((p_di_v + m_di_v)))) : (0));
        adx_v = (history_advances_new_bar() ? _ta_rma_14.compute(dx_v) : _ta_rma_14.recompute(dx_v));
        trending_bull = (([&]{ auto _pna_l = (adx_v); auto _pna_r = (i_adx_thr); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()) && ([&]{ auto _pna_l = (p_di_v); auto _pna_r = (m_di_v); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()));
        trending_bear = (([&]{ auto _pna_l = (adx_v); auto _pna_r = (i_adx_thr); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()) && ([&]{ auto _pna_l = (m_di_v); auto _pna_r = (p_di_v); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()));
        if (history_advances_new_bar()) in_session.push(!(is_na(pine_time(current_bar_.timestamp, script_tf_, i_session, i_tz, script_tf_ PF_PINE_TIME_SESSION_DAY_ARGS(syminfo_.timezone, syminfo_.session)))));
        else in_session.update(!(is_na(pine_time(current_bar_.timestamp, script_tf_, i_session, i_tz, script_tf_ PF_PINE_TIME_SESSION_DAY_ARGS(syminfo_.timezone, syminfo_.session)))));
        long_setup = ((((pivot_break_up && in_bull_fvg) && (vol_anom_bull || cd_up)) && trending_bull) && in_session[0]);
        short_setup = ((((pivot_break_dn && in_bear_fvg) && (vol_anom_bear || cd_dn)) && trending_bear) && in_session[0]);
        if ((long_setup && ([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) || _pfc_eq); }()))) {
            if (([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }())) {
                strategy_close(std::string("S"), std::string("flip flat"), na<double>(), na<double>(), false, 794568949783ULL);
            }
            strategy_entry(std::string("L"), true, na<double>(), na<double>(), na<double>(), std::string("vcp confluence long"));
        }
        if ((short_setup && ([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) || _pfc_eq); }()))) {
            if (([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) {
                strategy_close(std::string("L"), std::string("flip flat"), na<double>(), na<double>(), false, 816043786263ULL);
            }
            strategy_entry(std::string("S"), false, na<double>(), na<double>(), na<double>(), std::string("vcp confluence short"));
        }
        if (((!(in_session[0]) && in_session[1]) && ([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && (!_pfc_eq); }()))) {
            strategy_close("", std::string("session end"), na<double>(), na<double>(), false);
        }
    }

    void precalculate(const Bar* bars, int n) {
        _use_precalc = false;
        if (n <= 0 || bars == nullptr) return;

        _precalc__ta_atr_1.resize(n);
        _precalc__ta_pivothigh_2.resize(n);
        _precalc__ta_pivotlow_3.resize(n);
        _precalc__ta_rsi_4.resize(n);
        _precalc__ta_sma_6.resize(n);
        _precalc__ta_stdev_7.resize(n);
        _precalc__ta_change_9.resize(n);
        _precalc__ta_change_10.resize(n);

        _ta_atr_1 = ta::ATR(14);
        _ta_pivothigh_2 = ta::PivotHigh(5, 5);
        _ta_pivotlow_3 = ta::PivotLow(5, 5);
        _ta_rsi_4 = ta::RSI(14);
        _ta_sma_6 = ta::SMA(20);
        _ta_stdev_7 = ta::StdDev(20);
        _ta_change_9 = ta::Change();
        _ta_change_10 = ta::Change();

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
            _precalc__ta_pivothigh_2[i] = _ta_pivothigh_2.compute(bars[i].high);
            _precalc__ta_pivotlow_3[i] = _ta_pivotlow_3.compute(bars[i].low);
            _precalc__ta_rsi_4[i] = _ta_rsi_4.compute(bars[i].close);
            _precalc__ta_sma_6[i] = _ta_sma_6.compute(bars[i].volume);
            _precalc__ta_stdev_7[i] = _ta_stdev_7.compute(bars[i].volume);
            _precalc__ta_change_9[i] = _ta_change_9.compute(bars[i].high);
            _precalc__ta_change_10[i] = _ta_change_10.compute(bars[i].low);
        }

        _ta_atr_1 = ta::ATR(14);
        _ta_pivothigh_2 = ta::PivotHigh(5, 5);
        _ta_pivotlow_3 = ta::PivotLow(5, 5);
        _ta_rsi_4 = ta::RSI(14);
        _ta_sma_6 = ta::SMA(20);
        _ta_stdev_7 = ta::StdDev(20);
        _ta_change_9 = ta::Change();
        _ta_change_10 = ta::Change();
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
