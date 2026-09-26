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
#include <charconv>
#include <system_error>
#include <stdexcept>
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


struct _PFTvFormatValue {
    enum class Kind { Number, Text } kind;
    double number = 0.0;
    std::string text;

    template <class T, std::enable_if_t<std::is_arithmetic_v<T> &&
                                        !std::is_same_v<T, bool>, int> = 0>
    _PFTvFormatValue(T value)
        : kind(Kind::Number),
          number(is_na(value) ? na<double>() : static_cast<double>(value)) {}
    _PFTvFormatValue(bool value)
        : kind(Kind::Text), text(value ? "true" : "false") {}
    _PFTvFormatValue(const std::string& value)
        : kind(Kind::Text), text(value) {}
    _PFTvFormatValue(const char* value)
        : kind(Kind::Text), text(value) {}
};

static std::string _pf_tv_trim(const std::string& value) {
    const size_t first = value.find_first_not_of(" \t");
    if (first == std::string::npos) return "";
    const size_t last = value.find_last_not_of(" \t");
    return value.substr(first, last - first + 1);
}

static void _pf_tv_increment_digits(std::string& digits) {
    for (size_t i = digits.size(); i > 0; --i) {
        if (digits[i - 1] != '9') {
            ++digits[i - 1];
            return;
        }
        digits[i - 1] = '0';
    }
    digits.insert(digits.begin(), '1');
}

static std::string _pf_tv_decimal(double value, int min_fraction,
                                  int max_fraction, bool grouping,
                                  int decimal_shift = 0) {
    if (std::isnan(value)) return "NaN";
    if (std::isinf(value)) return value < 0 ? "-Infinity" : "Infinity";
    max_fraction = std::max(0, std::min(max_fraction, 15));
    min_fraction = std::max(0, std::min(min_fraction, max_fraction));

    // C++17's no-precision to_chars gives the shortest round-trip decimal
    // spelling of this binary64 value. All subsequent scaling and rounding
    // operate on its digits, never on a floating-point intermediate.
    char buffer[128];
    const auto converted = std::to_chars(buffer, buffer + sizeof buffer, value);
    if (converted.ec != std::errc{})
        throw std::runtime_error("shortest-decimal conversion failed");
    std::string spelling(buffer, converted.ptr);
    const bool negative = !spelling.empty() && spelling[0] == '-';
    if (negative) spelling.erase(0, 1);

    const size_t exponent_pos = spelling.find_first_of("eE");
    const std::string mantissa = spelling.substr(0, exponent_pos);
    int exponent = 0;
    if (exponent_pos != std::string::npos) {
        size_t i = exponent_pos + 1;
        bool exponent_negative = false;
        if (i < spelling.size() && (spelling[i] == '+' || spelling[i] == '-')) {
            exponent_negative = spelling[i] == '-';
            ++i;
        }
        for (; i < spelling.size(); ++i)
            exponent = exponent * 10 + (spelling[i] - '0');
        if (exponent_negative) exponent = -exponent;
    }

    std::string digits;
    int decimal_point = 0;
    bool after_point = false;
    for (char ch : mantissa) {
        if (ch == '.') {
            after_point = true;
        } else {
            digits += ch;
            if (!after_point) ++decimal_point;
        }
    }
    decimal_point += exponent + decimal_shift;
    const size_t leading_zeroes = digits.find_first_not_of('0');
    if (leading_zeroes == std::string::npos) {
        digits = "0";
        decimal_point = 1;
    } else {
        digits.erase(0, leading_zeroes);
        decimal_point -= static_cast<int>(leading_zeroes);
    }

    // The retained digits form an integer in units of 10^-max_fraction.
    // The first discarded decimal digit decides a half-up tie exactly.
    std::string units;
    if (digits == "0") {
        units = "0";
    } else {
        const int keep = decimal_point + max_fraction;
        if (keep < 0) {
            units = "0";
        } else if (keep == 0) {
            units = digits[0] >= '5' ? "1" : "0";
        } else if (keep >= static_cast<int>(digits.size())) {
            units = digits;
            units.append(static_cast<size_t>(keep) - digits.size(), '0');
        } else {
            units = digits.substr(0, static_cast<size_t>(keep));
            if (digits[static_cast<size_t>(keep)] >= '5')
                _pf_tv_increment_digits(units);
        }
    }
    const bool nonzero = units.find_first_not_of('0') != std::string::npos;
    if (!nonzero) units = "0";
    if (units.size() <= static_cast<size_t>(max_fraction))
        units.insert(0, static_cast<size_t>(max_fraction) + 1 - units.size(), '0');
    const size_t integer_size = units.size() - static_cast<size_t>(max_fraction);
    std::string integer = units.substr(0, integer_size);
    std::string fraction = units.substr(integer_size);
    while (fraction.size() > static_cast<size_t>(min_fraction)
           && fraction.back() == '0') fraction.pop_back();
    if (grouping) {
        std::string grouped;
        for (size_t i = 0; i < integer_size; ++i) {
            if (i && (integer_size - i) % 3 == 0) grouped += ',';
            grouped += integer[i];
        }
        integer = grouped;
    }
    return (negative && nonzero ? "-" : "") + integer
        + (fraction.empty() ? "" : "." + fraction);
}

static std::string _pf_tv_pattern(double value, const std::string& pattern) {
    if (!std::isfinite(value)) return _pf_tv_decimal(value, 0, 0, false);
    const std::string fmt = _pf_tv_trim(pattern);
    const size_t dot = fmt.find('.');
    const std::string integer = fmt.substr(0, dot);
    const std::string fraction = dot == std::string::npos ? "" : fmt.substr(dot + 1);
    const int max_fraction = static_cast<int>(std::count_if(
        fraction.begin(), fraction.end(), [](char c) { return c == '0' || c == '#'; }));
    const int min_fraction = static_cast<int>(std::count(fraction.begin(), fraction.end(), '0'));
    const bool grouping = integer.find(',') != std::string::npos;
    const bool percent = fmt.find('%') != std::string::npos;
    return _pf_tv_decimal(value, min_fraction, max_fraction, grouping,
                          percent ? 2 : 0)
        + (percent ? "%" : "");
}

static std::string pine_str_tostring_tv(double value,
                                        const std::string& format_mode = "",
                                        double mintick = 0.0) {
    if (std::isnan(value)) return "NaN";
    if (std::isinf(value)) return value < 0 ? "-Infinity" : "Infinity";
    if (format_mode == "mintick")
        return pine_str_tostring(value, format_mode, mintick);
    if (format_mode == "percent")
        return _pf_tv_pattern(value, "#.##") + "%";
    if (format_mode == "volume") {
        const double magnitude = std::fabs(value);
        int decimal_shift = 0;
        const char* unit = "";
        if (magnitude >= 1.0e12) { decimal_shift = -12; unit = "T"; }
        else if (magnitude >= 1.0e9) { decimal_shift = -9; unit = "B"; }
        else if (magnitude >= 1.0e6) { decimal_shift = -6; unit = "M"; }
        else if (magnitude >= 1.0e3) { decimal_shift = -3; unit = "K"; }
        return _pf_tv_decimal(value, 0, decimal_shift ? 2 : 0, false,
                              decimal_shift) + unit;
    }
    return _pf_tv_pattern(value, format_mode.empty() ? "#.##########" : format_mode);
}

template <class T, std::enable_if_t<std::is_integral_v<T> &&
                                    !std::is_same_v<T, bool>, int> = 0>
static std::string pine_str_tostring_tv(T value,
                                        const std::string& format_mode = "",
                                        double mintick = 0.0) {
    return pine_str_tostring_tv(
        is_na(value) ? na<double>() : static_cast<double>(value),
        format_mode, mintick);
}

static std::string _pf_tv_number_style(double value, const std::string& style) {
    if (!std::isfinite(value)) return _pf_tv_decimal(value, 0, 0, false);
    const std::string fmt = _pf_tv_trim(style);
    if (fmt.empty()) return _pf_tv_pattern(value, "#,###.###");
    if (fmt == "integer") return _pf_tv_pattern(value, "#,###");
    if (fmt == "percent") return _pf_tv_decimal(value, 0, 0, true, 2) + "%";
    if (fmt == "currency") {
        const std::string digits = _pf_tv_decimal(value, 2, 2, true);
        return digits[0] == '-' ? "-$" + digits.substr(1) : "$" + digits;
    }
    return _pf_tv_pattern(value, fmt);
}

static std::string pine_str_format_tv(
    const std::string& format_string,
    const std::vector<_PFTvFormatValue>& args) {
    std::string result;
    bool quoted = false;
    for (size_t i = 0; i < format_string.size();) {
        const char c = format_string[i];
        if (c == '\'') {
            if (i + 1 < format_string.size() && format_string[i + 1] == '\'') {
                result += '\'';
                i += 2;
            } else {
                quoted = !quoted;
                ++i;
            }
            continue;
        }
        if (c != '{' || quoted) {
            result += c;
            ++i;
            continue;
        }
        const size_t end = format_string.find('}', i + 1);
        if (end == std::string::npos) {
            result += format_string.substr(i);
            break;
        }
        const std::string inside = format_string.substr(i + 1, end - i - 1);
        const size_t first_comma = inside.find(',');
        const std::string index_text = _pf_tv_trim(inside.substr(0, first_comma));
        if (index_text.empty() || !std::all_of(index_text.begin(), index_text.end(),
                                               [](char ch) { return ch >= '0' && ch <= '9'; })) {
            result += format_string.substr(i, end - i + 1);
            i = end + 1;
            continue;
        }
        const size_t index = static_cast<size_t>(std::stoul(index_text));
        if (index >= args.size()) {
            result += format_string.substr(i, end - i + 1);
            i = end + 1;
            continue;
        }
        const auto& arg = args[index];
        if (arg.kind == _PFTvFormatValue::Kind::Text) {
            result += arg.text;
        } else if (first_comma == std::string::npos) {
            result += _pf_tv_number_style(arg.number, "");
        } else {
            const size_t second_comma = inside.find(',', first_comma + 1);
            const std::string type = _pf_tv_trim(inside.substr(
                first_comma + 1, second_comma - first_comma - 1));
            if (type == "number") {
                const std::string style = second_comma == std::string::npos
                    ? "" : inside.substr(second_comma + 1);
                result += _pf_tv_number_style(arg.number, style);
            } else {
                result += format_string.substr(i, end - i + 1);
            }
        }
        i = end + 1;
    }
    return result;
}


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
    ta::SMA _ta_sma_1;
    std::vector<double> _precalc__ta_sma_1;
    ta::Crossover _ta_crossover_2;
    ta::Crossunder _ta_crossunder_3;
    bool _use_precalc = false;
    Line lv;
    Line lv2;
    Linefill lf;
    double sma = 0.0;
    Label lbl = Label{};
    double level = 0.0;
    bool eUp = false;
    bool eDn = false;
    DrawingArena<LineRec> _pf_lines_{50};
    DrawingArena<BoxRec> _pf_boxes_{50};
    DrawingArena<LabelRec> _pf_labels_{50};
    DrawingArena<LinefillRec> _pf_linefills_{50};
    bool _var_initialized = false;
    bool _ta_initialized_ = false;
    bool _inputs_initialized_ = false;

    struct _PFScriptState {
        decltype(GeneratedStrategy::_ta_sma_1) _pf_value_0;
        decltype(GeneratedStrategy::_ta_crossover_2) _pf_value_1;
        decltype(GeneratedStrategy::_ta_crossunder_3) _pf_value_2;
        decltype(GeneratedStrategy::lv) _pf_value_3;
        decltype(GeneratedStrategy::lv2) _pf_value_4;
        decltype(GeneratedStrategy::lf) _pf_value_5;
        decltype(GeneratedStrategy::sma) _pf_value_6;
        decltype(GeneratedStrategy::lbl) _pf_value_7;
        decltype(GeneratedStrategy::level) _pf_value_8;
        decltype(GeneratedStrategy::eUp) _pf_value_9;
        decltype(GeneratedStrategy::eDn) _pf_value_10;
        decltype(GeneratedStrategy::_pf_lines_) _pf_value_11;
        decltype(GeneratedStrategy::_pf_boxes_) _pf_value_12;
        decltype(GeneratedStrategy::_pf_labels_) _pf_value_13;
        decltype(GeneratedStrategy::_pf_linefills_) _pf_value_14;
        decltype(GeneratedStrategy::_var_initialized) _pf_value_15;
        decltype(GeneratedStrategy::_ta_initialized_) _pf_value_16;
        decltype(GeneratedStrategy::_inputs_initialized_) _pf_value_17;
    };
    static_assert(std::is_copy_constructible_v<_PFScriptState>, "generated Pine state must be deep-copy constructible");
    static_assert(std::is_copy_assignable_v<_PFScriptState>, "generated Pine state must be deep-copy assignable");
    std::optional<_PFScriptState> _pf_script_state_checkpoint_;

    void snapshot_script_state() override {
        _pf_script_state_checkpoint_.emplace(_PFScriptState{
            _ta_sma_1,
            _ta_crossover_2,
            _ta_crossunder_3,
            lv,
            lv2,
            lf,
            sma,
            lbl,
            level,
            eUp,
            eDn,
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
        this->_ta_sma_1 = _pf_script_state_checkpoint_->_pf_value_0;
        this->_ta_crossover_2 = _pf_script_state_checkpoint_->_pf_value_1;
        this->_ta_crossunder_3 = _pf_script_state_checkpoint_->_pf_value_2;
        this->lv = _pf_script_state_checkpoint_->_pf_value_3;
        this->lv2 = _pf_script_state_checkpoint_->_pf_value_4;
        this->lf = _pf_script_state_checkpoint_->_pf_value_5;
        this->sma = _pf_script_state_checkpoint_->_pf_value_6;
        this->lbl = _pf_script_state_checkpoint_->_pf_value_7;
        this->level = _pf_script_state_checkpoint_->_pf_value_8;
        this->eUp = _pf_script_state_checkpoint_->_pf_value_9;
        this->eDn = _pf_script_state_checkpoint_->_pf_value_10;
        this->_pf_lines_ = _pf_script_state_checkpoint_->_pf_value_11;
        this->_pf_boxes_ = _pf_script_state_checkpoint_->_pf_value_12;
        this->_pf_labels_ = _pf_script_state_checkpoint_->_pf_value_13;
        this->_pf_linefills_ = _pf_script_state_checkpoint_->_pf_value_14;
        this->_var_initialized = _pf_script_state_checkpoint_->_pf_value_15;
        this->_ta_initialized_ = _pf_script_state_checkpoint_->_pf_value_16;
        this->_inputs_initialized_ = _pf_script_state_checkpoint_->_pf_value_17;
    }

    void commit_script_state() override {
        snapshot_script_state();
    }

    explicit GeneratedStrategy() : _ta_sma_1(14) {
#if defined(PINEFORGE_HAS_EXPLICIT_PINE_EXECUTION_ADAPTER_V1)
        pineforge::source::PineStrategyHost::attach_pine_execution_adapter();
#elif defined(PINEFORGE_HAS_EXPLICIT_PINE_CAP_V1)
        pineforge::source::PineStrategyHost::enable_pine_intraday_cap();
#endif
        pineforge::source::PineStrategyConfig cfg{};
        cfg.default_qty_type = static_cast<int>(QtyType::FIXED);
        cfg.default_qty_value = 1.0;
        cfg.pyramiding = 0;
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
        this->_ta_sma_1 = decltype(this->_ta_sma_1)(14);
        this->_precalc__ta_sma_1 = decltype(this->_precalc__ta_sma_1){};
        this->_ta_crossover_2 = decltype(this->_ta_crossover_2){};
        this->_ta_crossunder_3 = decltype(this->_ta_crossunder_3){};
        this->_use_precalc = false;
        this->lv = decltype(this->lv){};
        this->lv2 = decltype(this->lv2){};
        this->lf = decltype(this->lf){};
        this->sma = 0.0;
        this->lbl = Label{};
        this->level = 0.0;
        this->eUp = false;
        this->eDn = false;
        this->_pf_lines_ = decltype(this->_pf_lines_){50};
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
        sma = (history_advances_new_bar() ? _ta_sma_1.compute(current_bar_.close) : _ta_sma_1.recompute(current_bar_.close));
        if (is_na(lv)) {
            lv = pf_line_new(_pf_lines_, (int64_t)(pine_bar_index()), (double)(sma), (int64_t)((pine_bar_index() + 1)), (double)(sma), XLoc::bar_index, true, true);
            lv2 = pf_line_new(_pf_lines_, (int64_t)(pine_bar_index()), (double)(sma), (int64_t)((pine_bar_index() + 1)), (double)(sma), XLoc::bar_index, false, false);
            lf = pf_linefill_new(_pf_linefills_, lv, lv2);
        } else {
            pf_line_set_y1(_pf_lines_, lv, (double)(sma));
            pf_line_set_y2(_pf_lines_, lv, (double)(sma));
            pf_noop(lv, pine_color::new_color(pine_color::blue, [&](){ auto _pf_color_v = (50); using _pf_color_t = std::decay_t<decltype(_pf_color_v)>; if constexpr (std::is_same_v<_pf_color_t, bool>) return _pf_color_v ? 1 : 0; else return is_na(_pf_color_v) ? 100 : (int)_pf_color_v; }()));
            pf_noop(lv, 2);
            pf_noop(lv, 0);
        }
        lbl = pf_label_new(_pf_labels_, (int64_t)(pine_bar_index()), (double)(current_bar_.high), pine_str_tostring_tv(sma, std::string(), syminfo_mintick_), XLoc::bar_index, YLoc::price);
        pf_label_set_text(_pf_labels_, lbl, std::string("x"));
        level = pf_line_get_y2(_pf_lines_, lv);
        eUp = (history_advances_new_bar() ? _ta_crossover_2.compute(current_bar_.close, level) : _ta_crossover_2.recompute(current_bar_.close, level));
        eDn = (history_advances_new_bar() ? _ta_crossunder_3.compute(current_bar_.close, level) : _ta_crossunder_3.recompute(current_bar_.close, level));
        if ([&](){ auto _pf_bool_v = (eUp); using _pf_bool_t = std::decay_t<decltype(_pf_bool_v)>; if constexpr (std::is_same_v<_pf_bool_t, bool>) { return _pf_bool_v; } else if constexpr (std::is_floating_point_v<_pf_bool_t> || std::is_integral_v<_pf_bool_t>) { return is_na(_pf_bool_v) ? false : (_pf_bool_v != 0); } else { return static_cast<bool>(_pf_bool_v); } }()) {
            strategy_entry(std::string("L"), true, na<double>(), na<double>(), na<double>(), "");
        }
        if ((([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()) && [&](){ auto _pf_bool_v = (eDn); using _pf_bool_t = std::decay_t<decltype(_pf_bool_v)>; if constexpr (std::is_same_v<_pf_bool_t, bool>) { return _pf_bool_v; } else if constexpr (std::is_floating_point_v<_pf_bool_t> || std::is_integral_v<_pf_bool_t>) { return is_na(_pf_bool_v) ? false : (_pf_bool_v != 0); } else { return static_cast<bool>(_pf_bool_v); } }())) {
            strategy_close(std::string("L"), "", na<double>(), na<double>(), false, 107374182419ULL);
        }
    }

    void precalculate(const Bar* bars, int n) {
        _use_precalc = false;
        if (n <= 0 || bars == nullptr) return;

        _precalc__ta_sma_1.resize(n);

        _ta_sma_1 = ta::SMA(14);


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
            _precalc__ta_sma_1[i] = _ta_sma_1.compute(bars[i].close);
        }

        _ta_sma_1 = ta::SMA(14);

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
