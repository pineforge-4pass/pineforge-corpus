#include <pineforge/engine.hpp>
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
#include <memory>
#include <mutex>
#include <unordered_map>
#include <unordered_map>
#include <pineforge/color.hpp>
#include <pineforge/log.hpp>
#include <pineforge/str_utils.hpp>
#include <pineforge/session_time.hpp>

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
        {"AQUIS", "UK"},
        {"ARCA", "US"},
        {"ASX", "AU"},
        {"B3", "BR"},
        {"BINANCE", "GLOBAL"},
        {"BITMEX", "GLOBAL"},
        {"BMF", "BR"},
        {"BMFBOVESPA", "BR"},
        {"BSE", "IN"},
        {"BYBIT", "GLOBAL"},
        {"CBOE", "US"},
        {"CBOT", "US"},
        {"CME", "US"},
        {"CME_MINI", "US"},
        {"COINBASE", "US"},
        {"COMEX", "US"},
        {"DERIBIT", "GLOBAL"},
        {"EURONEXT", "EU"},
        {"HKEX", "HK"},
        {"JSE", "ZA"},
        {"KOSPI", "KR"},
        {"KRAKEN", "GLOBAL"},
        {"KRX", "KR"},
        {"LSE", "UK"},
        {"MOEX", "RU"},
        {"NASDAQ", "US"},
        {"NSE", "IN"},
        {"NYMEX", "US"},
        {"NYSE", "US"},
        {"OKX", "GLOBAL"},
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

class GeneratedStrategy : public BacktestEngine {
public:
    double _req_sec_0 = na<double>();
    double _req_sec_1 = na<double>();
    double _req_sec_2 = na<double>();
    std::unordered_map<std::string, Series<double>> _security_helper_series_;
    ta::EMA _ta_ema_1;
    ta::EMA _ta_ema_2;
    ta::EMA _ta_ema_3;
    ta::EMA _ta_ema_1_cs1;
    ta::EMA _ta_ema_2_cs1;
    ta::EMA _ta_ema_3_cs1;
    ta::EMA _ta_ema_1_cs2;
    ta::EMA _ta_ema_2_cs2;
    ta::EMA _ta_ema_3_cs2;
    ta::EMA _sec0__ta_ema_1_v0;
    ta::EMA _sec0__ta_ema_1_v1;
    ta::EMA _sec0__ta_ema_1_v2;
    ta::EMA _sec0__ta_ema_2_v0;
    ta::EMA _sec0__ta_ema_2_v1;
    ta::EMA _sec0__ta_ema_2_v2;
    ta::EMA _sec0__ta_ema_3_v0;
    ta::EMA _sec0__ta_ema_3_v1;
    ta::EMA _sec1__ta_ema_1_v0;
    ta::EMA _sec1__ta_ema_1_v1;
    ta::EMA _sec1__ta_ema_1_v2;
    ta::EMA _sec1__ta_ema_2_v0;
    ta::EMA _sec1__ta_ema_2_v1;
    ta::EMA _sec1__ta_ema_2_v2;
    ta::EMA _sec1__ta_ema_3_v0;
    ta::EMA _sec1__ta_ema_3_v1;
    ta::EMA _sec2__ta_ema_1_v0;
    ta::EMA _sec2__ta_ema_1_v1;
    ta::EMA _sec2__ta_ema_1_v2;
    ta::EMA _sec2__ta_ema_2_v0;
    ta::EMA _sec2__ta_ema_2_v1;
    ta::EMA _sec2__ta_ema_2_v2;
    ta::EMA _sec2__ta_ema_3_v0;
    ta::EMA _sec2__ta_ema_3_v1;
    double h1 = 0.0;
    double h2 = 0.0;
    double h3 = 0.0;
    bool allBull = false;
    bool allBear = false;
    bool _ta_initialized_ = false;
    bool _inputs_initialized_ = false;

    explicit GeneratedStrategy() : _ta_ema_1(12), _ta_ema_2(26), _ta_ema_3(9), _ta_ema_1_cs1(12), _ta_ema_2_cs1(26), _ta_ema_3_cs1(9), _ta_ema_1_cs2(12), _ta_ema_2_cs2(26), _ta_ema_3_cs2(9), _sec0__ta_ema_1_v0(12), _sec0__ta_ema_1_v1(12), _sec0__ta_ema_1_v2(12), _sec0__ta_ema_2_v0(26), _sec0__ta_ema_2_v1(26), _sec0__ta_ema_2_v2(26), _sec0__ta_ema_3_v0(9), _sec0__ta_ema_3_v1(9), _sec1__ta_ema_1_v0(12), _sec1__ta_ema_1_v1(12), _sec1__ta_ema_1_v2(12), _sec1__ta_ema_2_v0(26), _sec1__ta_ema_2_v1(26), _sec1__ta_ema_2_v2(26), _sec1__ta_ema_3_v0(9), _sec1__ta_ema_3_v1(9), _sec2__ta_ema_1_v0(12), _sec2__ta_ema_1_v1(12), _sec2__ta_ema_1_v2(12), _sec2__ta_ema_2_v0(26), _sec2__ta_ema_2_v1(26), _sec2__ta_ema_2_v2(26), _sec2__ta_ema_3_v0(9), _sec2__ta_ema_3_v1(9) {
        initial_capital_ = 1000000.0;
        default_qty_type_ = QtyType::FIXED;
        default_qty_value_ = 1.0;
        pyramiding_ = 1;
        commission_type_ = CommissionType::PERCENT;
        commission_value_ = 0.0;
        slippage_ = 0;
        script_has_strategy_close_ = true;
    }

    void set_strategy_override(const std::string& key, const std::string& value) {
        if (key == "initial_capital") { initial_capital_ = std::stod(value); return; }
        if (key == "commission_value") { commission_value_ = std::stod(value); return; }
        if (key == "default_qty_value") { default_qty_value_ = std::stod(value); return; }
        if (key == "pyramiding") { pyramiding_ = std::stoi(value); return; }
        if (key == "slippage") { slippage_ = std::stoi(value); return; }
        if (key == "process_orders_on_close") { process_orders_on_close_ = (value == "true" || value == "1"); return; }
        if (key == "close_entries_rule") { close_entries_rule_any_ = (value == "ANY" || value == "any" || value == "1"); return; }
        if (key == "default_qty_type") {
            if (value == "fixed" || value == "strategy.fixed" || value == "0") default_qty_type_ = QtyType::FIXED;
            else if (value == "percent_of_equity" || value == "strategy.percent_of_equity" || value == "1") default_qty_type_ = QtyType::PERCENT_OF_EQUITY;
            else if (value == "cash" || value == "strategy.cash" || value == "2") default_qty_type_ = QtyType::CASH;
            return;
        }
        if (key == "commission_type") {
            if (value == "percent" || value == "strategy.commission.percent" || value == "0") commission_type_ = CommissionType::PERCENT;
            else if (value == "cash_per_order" || value == "strategy.commission.cash_per_order" || value == "1") commission_type_ = CommissionType::CASH_PER_ORDER;
            else if (value == "cash_per_contract" || value == "strategy.commission.cash_per_contract" || value == "2") commission_type_ = CommissionType::CASH_PER_CONTRACT;
            return;
        }
    }

    void configure_security_evaluators() override {
        security_eval_states_.clear();
        register_security_eval(0, "60", input_tf_, false, false);
        register_security_eval(1, "240", input_tf_, false, false);
        register_security_eval(2, "D", input_tf_, false, false);
    }

    double f_macd_hist_cs0(double src, int fast, int slow, int sig) {
        double macd_val = ((is_first_tick_ ? _ta_ema_1.compute(src) : _ta_ema_1.recompute(src)) - (is_first_tick_ ? _ta_ema_2.compute(src) : _ta_ema_2.recompute(src)));
        double macd_sig = (is_first_tick_ ? _ta_ema_3.compute(macd_val) : _ta_ema_3.recompute(macd_val));
        return (macd_val - macd_sig);
    }

    double f_macd_hist_cs1(double src, int fast, int slow, int sig) {
        double macd_val = ((is_first_tick_ ? _ta_ema_1_cs1.compute(src) : _ta_ema_1_cs1.recompute(src)) - (is_first_tick_ ? _ta_ema_2_cs1.compute(src) : _ta_ema_2_cs1.recompute(src)));
        double macd_sig = (is_first_tick_ ? _ta_ema_3_cs1.compute(macd_val) : _ta_ema_3_cs1.recompute(macd_val));
        return (macd_val - macd_sig);
    }

    double f_macd_hist_cs2(double src, int fast, int slow, int sig) {
        double macd_val = ((is_first_tick_ ? _ta_ema_1_cs2.compute(src) : _ta_ema_1_cs2.recompute(src)) - (is_first_tick_ ? _ta_ema_2_cs2.compute(src) : _ta_ema_2_cs2.recompute(src)));
        double macd_sig = (is_first_tick_ ? _ta_ema_3_cs2.compute(macd_val) : _ta_ema_3_cs2.recompute(macd_val));
        return (macd_val - macd_sig);
    }

    void on_bar(const Bar& bar) override {
        h1 = _req_sec_0;
        h2 = _req_sec_1;
        h3 = _req_sec_2;
        allBull = (((h1 > 0) && (h2 > 0)) && (h3 > 0));
        allBear = (((h1 < 0) && (h2 < 0)) && (h3 < 0));
        if ((allBull && (signed_position_size() <= 0))) {
            if ((signed_position_size() < 0)) {
                strategy_close(std::string("S"), std::string("flip to long"), na<double>(), na<double>(), false);
            }
            strategy_entry(std::string("L"), true, na<double>(), na<double>(), na<double>(), std::string("3-TF MACD bull"));
        }
        if ((allBear && (signed_position_size() >= 0))) {
            if ((signed_position_size() > 0)) {
                strategy_close(std::string("L"), std::string("flip to short"), na<double>(), na<double>(), false);
            }
            strategy_entry(std::string("S"), false, na<double>(), na<double>(), na<double>(), std::string("3-TF MACD bear"));
        }
    }

    void _eval_security_0(const Bar& bar, bool is_complete) {
        double _sec0_f_macd_hist_1_macd_val = ((is_complete ? _sec0__ta_ema_1_v2.compute(bar.close) : _sec0__ta_ema_1_v2.recompute(bar.close)) - (is_complete ? _sec0__ta_ema_2_v2.compute(bar.close) : _sec0__ta_ema_2_v2.recompute(bar.close)));
        double _sec0_f_macd_hist_2_macd_sig = (is_complete ? _sec0__ta_ema_3_v1.compute(_sec0_f_macd_hist_1_macd_val) : _sec0__ta_ema_3_v1.recompute(_sec0_f_macd_hist_1_macd_val));
        _req_sec_0 = (_sec0_f_macd_hist_1_macd_val - _sec0_f_macd_hist_2_macd_sig);
    }

    void _eval_security_1(const Bar& bar, bool is_complete) {
        double _sec1_f_macd_hist_3_macd_val = ((is_complete ? _sec1__ta_ema_1_v2.compute(bar.close) : _sec1__ta_ema_1_v2.recompute(bar.close)) - (is_complete ? _sec1__ta_ema_2_v2.compute(bar.close) : _sec1__ta_ema_2_v2.recompute(bar.close)));
        double _sec1_f_macd_hist_4_macd_sig = (is_complete ? _sec1__ta_ema_3_v1.compute(_sec1_f_macd_hist_3_macd_val) : _sec1__ta_ema_3_v1.recompute(_sec1_f_macd_hist_3_macd_val));
        _req_sec_1 = (_sec1_f_macd_hist_3_macd_val - _sec1_f_macd_hist_4_macd_sig);
    }

    void _eval_security_2(const Bar& bar, bool is_complete) {
        double _sec2_f_macd_hist_5_macd_val = ((is_complete ? _sec2__ta_ema_1_v2.compute(bar.close) : _sec2__ta_ema_1_v2.recompute(bar.close)) - (is_complete ? _sec2__ta_ema_2_v2.compute(bar.close) : _sec2__ta_ema_2_v2.recompute(bar.close)));
        double _sec2_f_macd_hist_6_macd_sig = (is_complete ? _sec2__ta_ema_3_v1.compute(_sec2_f_macd_hist_5_macd_val) : _sec2__ta_ema_3_v1.recompute(_sec2_f_macd_hist_5_macd_val));
        _req_sec_2 = (_sec2_f_macd_hist_5_macd_val - _sec2_f_macd_hist_6_macd_sig);
    }

    void evaluate_security(int sec_id, const Bar& bar, bool is_complete) override {
        switch (sec_id) {
            case 0: _eval_security_0(bar, is_complete); break;
            case 1: _eval_security_1(bar, is_complete); break;
            case 2: _eval_security_2(bar, is_complete); break;
        }
    }
    void clear_security(int sec_id) override {
        switch (sec_id) {
            case 0: _req_sec_0 = na<double>(); break;
            case 1: _req_sec_1 = na<double>(); break;
            case 2: _req_sec_2 = na<double>(); break;
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
