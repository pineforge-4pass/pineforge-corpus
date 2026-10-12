#include <pineforge/source/pine_strategy_host.hpp>
#include <pineforge/ta.hpp>
#if __has_include(<pineforge/checked_settings.hpp>)
#include <pineforge/checked_settings.hpp>
#endif
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


#ifdef PINEFORGE_HAS_RUN_FAILURE_CODES_V1
#define _PF_NO_DATA_STOP(function, call, line, english) ::pineforge::pine_no_data_stop(function, call, line, english)
#define _PF_OTHER_SYMBOL_STOP(function, symbol, call, line, english) ::pineforge::pine_other_symbol_stop(function, symbol, call, line, english)
#define _PF_ARRAY_STOP(reason, method, english) ::pineforge::pine_array_stop(reason, method, std::string(english).c_str())
#define _PF_COLLECTION_STOP(reason, object, english) ::pineforge::pine_collection_stop(object, reason, english)
#define _PF_NA_STOP(object, english) ::pineforge::pine_na_stop(object, english)
#define _PF_LIMIT_STOP(limit, max, english) ::pineforge::pine_limit_stop(limit, max, english)
#define _PF_UNSUPPORTED_STOP(reason, line, english) ::pineforge::pine_unsupported_stop(reason, line, english)
#define _PF_STRING_STOP(reason, english) ::pineforge::pine_string_stop(reason, english)
#define _PF_ENGINE_INVARIANT(english, legacy_type) ::pineforge::pine_engine_invariant(english)
#define _PF_INVARIANT_AT(container, index) _pf_invariant_at(container, index)
#define _PF_SETTING_FAILURE(strategy, entrypoint, message, reason) (strategy)->_pf_record_setting_failure(entrypoint, message, [] { return reason ? ::pineforge::RunFailureInfo(::pineforge::RunFailureCode::setting_rejected, {{"entrypoint", entrypoint}, {"reason", reason}}) : ::pineforge::RunFailureInfo(::pineforge::RunFailureCode::setting_rejected, {{"entrypoint", entrypoint}}); })
template <typename Container, typename Index>
decltype(auto) _pf_invariant_at(Container& container, Index index) {
    try { return container.at(index); }
    catch (const std::out_of_range& error) { ::pineforge::pine_engine_invariant(error.what()); }
}
#else
#define _PF_NO_DATA_STOP(function, call, line, english) pine_runtime_error(std::string(english))
#define _PF_OTHER_SYMBOL_STOP(function, symbol, call, line, english) pine_runtime_error(std::string(english))
#define _PF_ARRAY_STOP(reason, method, english) pine_runtime_error(english)
#define _PF_COLLECTION_STOP(reason, object, english) pine_runtime_error(english)
#define _PF_NA_STOP(object, english) throw std::runtime_error(english)
#define _PF_LIMIT_STOP(limit, max, english) throw std::length_error(english)
#define _PF_UNSUPPORTED_STOP(reason, line, english) pine_runtime_error(std::string(english))
#define _PF_STRING_STOP(reason, english) pine_runtime_error(std::string(english))
#define _PF_ENGINE_INVARIANT(english, legacy_type) throw legacy_type(english)
#define _PF_INVARIANT_AT(container, index) (container).at(index)
#define _PF_SETTING_FAILURE(strategy, entrypoint, message, reason) (strategy)->_pf_record_setting_failure(entrypoint, message)
#endif

// TradingView's drawing lifetime. A deleted or collected drawing reads like a
// na handle: every getter returns na, every setter does nothing and na() is
// true. A new line, box or label that makes its kind's live count reach
// max_<kind>_count + 6 deletes the oldest drawings of that kind that no var,
// varip or history-read variable holds, until max_<kind>_count remain; one
// held only by an array, a map, an object field, a local or a plain non-var
// variable is collectable. Linefills are never collected.
constexpr int _PF_DRAWING_UNBOUNDED = 2147483647;
template <class T> struct _PFDrawingArg { using type = T; };
template <class R, class Rec, class H, class... A>
inline R _pf_drawing_get(R (*get)(DrawingArena<Rec>&, H, A...),
                         typename _PFDrawingArg<DrawingArena<Rec>&>::type arena,
                         typename _PFDrawingArg<H>::type h,
                         typename _PFDrawingArg<A>::type... args) {
    return arena.alive(h.id) ? get(arena, h, args...) : na<R>();
}
template <class Rec, class H, class... A>
inline void _pf_drawing_set(void (*set)(DrawingArena<Rec>&, H, A...),
                            typename _PFDrawingArg<DrawingArena<Rec>&>::type arena,
                            typename _PFDrawingArg<H>::type h,
                            typename _PFDrawingArg<A>::type... args) {
    if (arena.alive(h.id)) set(arena, h, args...);
}
template <class Rec, class H>
inline bool _pf_drawing_na(const DrawingArena<Rec>& arena, H h) {
    return !arena.alive(h.id);
}
template <class Rec>
inline void _pf_collect_drawings(DrawingArena<Rec>& arena, int keep,
                                 const int32_t* pins, std::size_t pin_count) {
    const std::size_t excess = arena.order().size() - static_cast<std::size_t>(keep);
    std::vector<int32_t> doomed;
    for (int32_t id : arena.order()) {
        if (doomed.size() == excess) break;
        if (std::find(pins, pins + pin_count, id) == pins + pin_count) doomed.push_back(id);
    }
    for (int32_t id : doomed) arena.erase(id);
}


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
        _PF_ENGINE_INVARIANT("shortest-decimal conversion failed", std::runtime_error);
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
        size_t index = 0;
        for (char _pf_digit : index_text) {
            const size_t _pf_value = static_cast<size_t>(_pf_digit - '0');
            if (index > (std::numeric_limits<size_t>::max() - _pf_value) / 10) {
                _PF_STRING_STOP("format_index_overflow", "stoul");
            }
            index = index * 10 + _pf_value;
        }
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
    DrawingArena<LineRec> _pf_lines_{_PF_DRAWING_UNBOUNDED};
    DrawingArena<BoxRec> _pf_boxes_{_PF_DRAWING_UNBOUNDED};
    DrawingArena<LabelRec> _pf_labels_{_PF_DRAWING_UNBOUNDED};
    DrawingArena<LinefillRec> _pf_linefills_{_PF_DRAWING_UNBOUNDED};
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
        cfg.initial_capital = 100000.0;
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
    bool _pf_setting_failed_ = false;
    std::string _pf_setting_failure_;
#ifdef PINEFORGE_HAS_RUN_FAILURE_CODES_V1
    ::pineforge::RunFailureInfo _pf_setting_failure_info_{::pineforge::RunFailureCode::none, {}};
#ifdef PF_SETTINGS_API_VERSION
    using _pf_setting_error_base = ::pineforge::checked_settings::LatchedSettingsFailure;
#else
    using _pf_setting_error_base = std::runtime_error;
#endif
    struct _pf_latched_setting_error : _pf_setting_error_base, ::pineforge::RunFailureInfo {
        _pf_latched_setting_error(const std::string& text, const ::pineforge::RunFailureInfo& info)
            : _pf_setting_error_base(text), ::pineforge::RunFailureInfo(info) {}
    };
#endif
    void _pf_record_failure(const char* entrypoint, const char* message) noexcept {
#ifdef PINEFORGE_HAS_RUN_FAILURE_CODES_V1
        try { ::pineforge::note_run_failure_unknown(*this, entrypoint); } catch (...) {}
#else
        try { last_error_ = entrypoint; last_error_ += ": "; last_error_ += message; } catch (...) {}
#endif
    }
    void _pf_record_failure(const char* entrypoint, const std::exception& error) noexcept {
#ifdef PINEFORGE_HAS_RUN_FAILURE_CODES_V1
        try { ::pineforge::note_run_failure(*this, entrypoint, error); } catch (...) {}
#else
        _pf_record_failure(entrypoint, error.what());
#endif
    }
#ifdef PINEFORGE_HAS_RUN_FAILURE_CODES_V1
    template <typename FailureInfo>
    void _pf_record_setting_failure(const char* entrypoint, const char* message, FailureInfo failure_info) noexcept {
#else
    void _pf_record_setting_failure(const char* entrypoint, const char* message) noexcept {
#endif
        if (!_pf_setting_failed_) {
            _pf_setting_failed_ = true;
            try { _pf_setting_failure_ = entrypoint; _pf_setting_failure_ += ": "; _pf_setting_failure_ += message; } catch (...) {}
#ifdef PINEFORGE_HAS_RUN_FAILURE_CODES_V1
            try { _pf_setting_failure_info_ = failure_info(); }
            catch (...) { _pf_setting_failure_info_ = ::pineforge::RunFailureInfo(::pineforge::RunFailureCode::out_of_memory, {}); }
#endif
        }
        _pf_refuse_failed_setting(nullptr);
    }
    void _pf_require_settings_ok() const {
#ifdef PINEFORGE_HAS_RUN_FAILURE_CODES_V1
        if (_pf_setting_failed_) throw _pf_latched_setting_error(_pf_setting_failure_.empty() ? "legacy strategy setter failed" : _pf_setting_failure_, _pf_setting_failure_info_);
#elif defined(PF_SETTINGS_API_VERSION)
        if (_pf_setting_failed_) throw ::pineforge::checked_settings::LatchedSettingsFailure(_pf_setting_failure_.empty() ? "legacy strategy setter failed" : _pf_setting_failure_);
#else
        if (_pf_setting_failed_) throw std::runtime_error(_pf_setting_failure_.empty() ? "legacy strategy setter failed" : _pf_setting_failure_);
#endif
    }
    bool _pf_refuse_failed_setting(ReportC* out) noexcept {
        if (!_pf_setting_failed_) return false;
#ifdef PINEFORGE_HAS_RUN_FAILURE_CODES_V1
        try { ::pineforge::note_run_failure(*this, _pf_setting_failure_.empty() ? "legacy strategy setter failed" : _pf_setting_failure_, _pf_setting_failure_info_.run_failure()); } catch (...) {}
#else
        try { last_error_ = _pf_setting_failure_.empty() ? "legacy strategy setter failed" : _pf_setting_failure_; } catch (...) {}
#endif
        if (out) *out = ReportC{};
        return true;
    }
#ifdef PF_SETTINGS_API_VERSION
    static pineforge::source::PineStrategyConfig _pf_settings_declared_config() {
        pineforge::source::PineStrategyConfig cfg{};
        cfg.initial_capital = 100000.0;
        cfg.default_qty_type = static_cast<int>(QtyType::FIXED);
        cfg.default_qty_value = 1.0;
        cfg.pyramiding = 0;
        return cfg;
    }
    std::vector<::pineforge::checked_settings::Setting> _pf_settings_inputs() const {
        return {
        };
    }
    std::vector<::pineforge::checked_settings::Setting> _pf_settings_overrides() const {
        const double _pf_nan = std::numeric_limits<double>::quiet_NaN();
        const auto _pf_defaults = _pf_settings_declared_config();
        return {
            {"initial_capital", "float", ::pineforge::checked_settings::number(_pf_defaults.initial_capital), {}, 0.0},
            {"commission_value", "float", ::pineforge::checked_settings::number(_pf_defaults.commission_value), {}, 0.0},
            {"default_qty_value", "float", ::pineforge::checked_settings::number(_pf_defaults.default_qty_value), {}, 0.0},
            {"pyramiding", "int", ::pineforge::checked_settings::number(_pf_defaults.pyramiding), {}, 0.0},
            {"slippage", "int", ::pineforge::checked_settings::number(_pf_defaults.slippage), {}, 0.0},
            {"process_orders_on_close", "bool", ::pineforge::checked_settings::number(_pf_defaults.process_orders_on_close), {}, _pf_nan},
            {"calc_on_order_fills", "bool", ::pineforge::checked_settings::number(_pf_defaults.calc_on_order_fills), {}, _pf_nan},
            {"close_entries_rule", "string", _pf_close_entries_rule_word(_pf_defaults.close_entries_rule_any), {"FIFO", "ANY"}, _pf_nan},
            {"default_qty_type", "string", _pf_default_qty_type_word(_pf_defaults.default_qty_type), {"fixed", "percent_of_equity", "cash"}, _pf_nan},
            {"commission_type", "string", _pf_commission_type_word(_pf_defaults.commission_type), {"percent", "cash_per_order", "cash_per_contract"}, _pf_nan},
        };
    }
    static std::string _pf_close_entries_rule_word(int _pf_value) {
        if (_pf_value == 0) return "FIFO";
        if (_pf_value == 1) return "ANY";
        return "invalid";
    }
    static std::string _pf_default_qty_type_word(int _pf_value) {
        if (_pf_value == 0) return "fixed";
        if (_pf_value == 1) return "percent_of_equity";
        if (_pf_value == 2) return "cash";
        return "invalid";
    }
    static std::string _pf_commission_type_word(int _pf_value) {
        if (_pf_value == 0) return "percent";
        if (_pf_value == 1) return "cash_per_order";
        if (_pf_value == 2) return "cash_per_contract";
        return "invalid";
    }
    void _pf_set_input_checked(const std::string& _pf_key, const std::string& _pf_value) {
        if (_pf_refuse_failed_setting(nullptr)) throw ::pineforge::checked_settings::Error{PF_SETTINGS_RUN_FAILED, last_error_.c_str()};
        ::pineforge::checked_settings::require(script_bars_processed() == 0 && stream_phase_ == StreamPhase::IDLE, "settings are frozen after execution begins", PF_SETTINGS_UNSUPPORTED);
        const auto _pf_inputs = _pf_settings_inputs();
        const ::pineforge::checked_settings::Setting* _pf_match = nullptr;
        for (const auto& _pf_input : _pf_inputs) {
            if (_pf_input.name != _pf_key) continue;
            ::pineforge::checked_settings::require(_pf_match == nullptr, "ambiguous input key", PF_SETTINGS_UNSUPPORTED);
            _pf_match = &_pf_input;
        }
        ::pineforge::checked_settings::require(_pf_match != nullptr, "unknown input key");
        const auto _pf_canonical = ::pineforge::checked_settings::validate(*_pf_match, _pf_value);
        set_input(_pf_key, _pf_canonical);
        ::pineforge::checked_settings::require(inputs_.count(_pf_key) && inputs_.at(_pf_key) == _pf_canonical, "input was not installed", PF_SETTINGS_UNSUPPORTED);
    }
    void _pf_set_override_checked(const std::string& _pf_key, const std::string& _pf_value) {
        if (_pf_refuse_failed_setting(nullptr)) throw ::pineforge::checked_settings::Error{PF_SETTINGS_RUN_FAILED, last_error_.c_str()};
        ::pineforge::checked_settings::require(script_bars_processed() == 0 && stream_phase_ == StreamPhase::IDLE, "settings are frozen after execution begins", PF_SETTINGS_UNSUPPORTED);
        for (const auto& _pf_override : _pf_settings_overrides()) {
            if (_pf_override.name != _pf_key) continue;
            auto _pf_alias = _pf_value;
            if (_pf_key == "close_entries_rule") {
                if (_pf_value == "0" || _pf_value == "fifo") _pf_alias = "FIFO";
                if (_pf_value == "1" || _pf_value == "any") _pf_alias = "ANY";
            }
            if (_pf_key == "default_qty_type") {
                if (_pf_value == "0" || _pf_value == "strategy.fixed") _pf_alias = "fixed";
                if (_pf_value == "1" || _pf_value == "strategy.percent_of_equity") _pf_alias = "percent_of_equity";
                if (_pf_value == "2" || _pf_value == "strategy.cash") _pf_alias = "cash";
            }
            if (_pf_key == "commission_type") {
                if (_pf_value == "0" || _pf_value == "strategy.commission.percent") _pf_alias = "percent";
                if (_pf_value == "1" || _pf_value == "strategy.commission.cash_per_order") _pf_alias = "cash_per_order";
                if (_pf_value == "2" || _pf_value == "strategy.commission.cash_per_contract") _pf_alias = "cash_per_contract";
            }
            const auto _pf_canonical = ::pineforge::checked_settings::validate(_pf_override, _pf_alias);
            set_strategy_override(_pf_key, _pf_canonical);
            return;
        }
        throw ::pineforge::checked_settings::Error{PF_SETTINGS_INVALID_ARGUMENT, "unknown override key"};
    }
    std::string _pf_settings_receipt() const {
        ::pineforge::checked_settings::require(!_pf_setting_failed_, _pf_setting_failure_.empty() ? "legacy strategy setter failed" : _pf_setting_failure_.c_str(), PF_SETTINGS_RUN_FAILED);
        std::string _pf_document = "{\"version\":1,\"inputs\":[";
        const auto _pf_inputs = _pf_settings_inputs();
        _pf_document += "],\"overrides\":[";
        const auto _pf_overrides = _pf_settings_overrides();
        _pf_document += ::pineforge::checked_settings::describe(_pf_overrides[0], ::pineforge::checked_settings::number((std::isnan(override_.initial_capital) ? config_.initial_capital : override_.initial_capital)));
        _pf_document += ',';
        _pf_document += ::pineforge::checked_settings::describe(_pf_overrides[1], ::pineforge::checked_settings::number((std::isnan(override_.commission_value) ? config_.commission_value : override_.commission_value)));
        _pf_document += ',';
        _pf_document += ::pineforge::checked_settings::describe(_pf_overrides[2], ::pineforge::checked_settings::number((std::isnan(override_.default_qty_value) ? config_.default_qty_value : override_.default_qty_value)));
        _pf_document += ',';
        _pf_document += ::pineforge::checked_settings::describe(_pf_overrides[3], ::pineforge::checked_settings::number((override_.pyramiding < 0 ? config_.pyramiding : override_.pyramiding)));
        _pf_document += ',';
        _pf_document += ::pineforge::checked_settings::describe(_pf_overrides[4], ::pineforge::checked_settings::number((override_.slippage < 0 ? config_.slippage : override_.slippage)));
        _pf_document += ',';
        _pf_document += ::pineforge::checked_settings::describe(_pf_overrides[5], ::pineforge::checked_settings::number(static_cast<bool>((override_.process_orders_on_close < 0 ? config_.process_orders_on_close : override_.process_orders_on_close))));
        _pf_document += ',';
        _pf_document += ::pineforge::checked_settings::describe(_pf_overrides[6], ::pineforge::checked_settings::number(static_cast<bool>((override_.calc_on_order_fills < 0 ? config_.calc_on_order_fills : override_.calc_on_order_fills))));
        _pf_document += ',';
        _pf_document += ::pineforge::checked_settings::describe(_pf_overrides[7], _pf_close_entries_rule_word((override_.close_entries_rule < 0 ? config_.close_entries_rule_any : override_.close_entries_rule)));
        _pf_document += ',';
        _pf_document += ::pineforge::checked_settings::describe(_pf_overrides[8], _pf_default_qty_type_word((override_.default_qty_type < 0 ? config_.default_qty_type : override_.default_qty_type)));
        _pf_document += ',';
        _pf_document += ::pineforge::checked_settings::describe(_pf_overrides[9], _pf_commission_type_word((override_.commission_type < 0 ? config_.commission_type : override_.commission_type)));
        return _pf_document + "]}";
    }
#endif

#ifndef PINEFORGE_HAS_SCRIPT_RUN_PREPARE_V1
#error "Generated lifecycle reset requires a matching PineForge engine; rebuild with script-run preparation support"
#endif
    void prepare_script_run(const Bar* bars, int n, bool allow_precalculation) override {
        _pf_require_settings_ok();
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
        this->_pf_lines_ = decltype(this->_pf_lines_){_PF_DRAWING_UNBOUNDED};
        this->_pf_boxes_ = decltype(this->_pf_boxes_){_PF_DRAWING_UNBOUNDED};
        this->_pf_labels_ = decltype(this->_pf_labels_){_PF_DRAWING_UNBOUNDED};
        this->_pf_linefills_ = decltype(this->_pf_linefills_){_PF_DRAWING_UNBOUNDED};
        this->_var_initialized = false;
        this->_ta_initialized_ = false;
        this->_inputs_initialized_ = false;
        if (allow_precalculation) precalculate(bars, n);
    }

    Line _pf_collect_lines_(Line _pf_new) {
        if ((int)this->_pf_lines_.order().size() >= 56) {
            const int32_t _pf_held[] = {_pf_new.id, this->lv.id, this->lv2.id};
            _pf_collect_drawings(this->_pf_lines_, 50, _pf_held,
                                 sizeof(_pf_held) / sizeof(_pf_held[0]));
        }
        return _pf_new;
    }
    Box _pf_collect_boxes_(Box _pf_new) {
        if ((int)this->_pf_boxes_.order().size() >= 56) {
            const int32_t _pf_held[] = {_pf_new.id};
            _pf_collect_drawings(this->_pf_boxes_, 50, _pf_held,
                                 sizeof(_pf_held) / sizeof(_pf_held[0]));
        }
        return _pf_new;
    }
    Label _pf_collect_labels_(Label _pf_new) {
        if ((int)this->_pf_labels_.order().size() >= 56) {
            const int32_t _pf_held[] = {_pf_new.id};
            _pf_collect_drawings(this->_pf_labels_, 50, _pf_held,
                                 sizeof(_pf_held) / sizeof(_pf_held[0]));
        }
        return _pf_new;
    }

    void on_source_bar(const Bar& bar) override {
        if (!_var_initialized) {
            _var_initialized = true;
        } else {
        }
        sma = (history_advances_new_bar() ? _ta_sma_1.compute(current_bar_.close) : _ta_sma_1.recompute(current_bar_.close));
        if (_pf_drawing_na(_pf_lines_, lv)) {
            lv = _pf_collect_lines_(pf_line_new(_pf_lines_, (int64_t)(pine_bar_index()), (double)(sma), (int64_t)((pine_bar_index() + 1)), (double)(sma), XLoc::bar_index, true, true));
            lv2 = _pf_collect_lines_(pf_line_new(_pf_lines_, (int64_t)(pine_bar_index()), (double)(sma), (int64_t)((pine_bar_index() + 1)), (double)(sma), XLoc::bar_index, false, false));
            lf = pf_linefill_new(_pf_linefills_, lv, lv2);
        } else {
            _pf_drawing_set(pf_line_set_y1, _pf_lines_, lv, (double)(sma));
            _pf_drawing_set(pf_line_set_y2, _pf_lines_, lv, (double)(sma));
            pf_noop(lv, pine_color::new_color(pine_color::blue, [&](){ auto _pf_color_v = (50); using _pf_color_t = std::decay_t<decltype(_pf_color_v)>; if constexpr (std::is_same_v<_pf_color_t, bool>) return _pf_color_v ? 1 : 0; else return is_na(_pf_color_v) ? 100 : (int)_pf_color_v; }()));
            pf_noop(lv, 2);
            pf_noop(lv, 0);
        }
        lbl = _pf_collect_labels_(pf_label_new(_pf_labels_, (int64_t)(pine_bar_index()), (double)(current_bar_.high), pine_str_tostring_tv(sma, std::string(), syminfo_mintick_), XLoc::bar_index, YLoc::price));
        _pf_drawing_set(pf_label_set_text, _pf_labels_, lbl, std::string("x"));
        level = _pf_drawing_get(pf_line_get_y2, _pf_lines_, lv);
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
#if defined(PINEFORGE_HAS_RUN_FAILURE_CODES_V1) && defined(PF_SETTINGS_API_VERSION)
    int strategy_create_checked(const char* params_json, void** out, char* error, size_t error_capacity);
#endif
    void* strategy_create(const char* params_json) {
#if defined(PINEFORGE_HAS_RUN_FAILURE_CODES_V1) && defined(PF_SETTINGS_API_VERSION)
        void* out = nullptr;
        strategy_create_checked(nullptr, &out, nullptr, 0);
        return out;
#else
        try { return new GeneratedStrategy(); } catch (...) { return nullptr; }
#endif
    }
    void run_backtest(void* s, Bar* bars, int n, ReportC* out) {
        try {
        auto* strat = static_cast<GeneratedStrategy*>(s);
#ifndef PF_SETTINGS_API_VERSION
        if (strat->_pf_refuse_failed_setting(out)) return;
#endif
        strat->run(bars, n);
        if (!strat->_pf_refuse_failed_setting(out)) strat->fill_report(out);
        } catch (const std::exception& _pf_error) {
            if (out) *out = ReportC{};
            if (s) static_cast<GeneratedStrategy*>(s)->_pf_record_failure("run_backtest", _pf_error);
        } catch (...) {
            if (out) *out = ReportC{};
            if (s) static_cast<GeneratedStrategy*>(s)->_pf_record_failure("run_backtest", "unknown C++ exception");
        }
    }
    static void _pf_run_backtest_full_impl(void* s, Bar* bars, int n,
                           const char* input_tf, const char* script_tf,
                           int bar_magnifier, int magnifier_samples,
                           int magnifier_dist,
                           ReportC* out) {
        auto* strat = static_cast<GeneratedStrategy*>(s);
#ifndef PF_SETTINGS_API_VERSION
        if (strat->_pf_refuse_failed_setting(out)) return;
#endif
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
        if (!strat->_pf_refuse_failed_setting(out)) strat->fill_report(out);
    }
    void run_backtest_full(void* s, Bar* bars, int n, const char* input_tf, const char* script_tf,
                           int bar_magnifier, int magnifier_samples, int magnifier_dist, ReportC* out) {
        try { _pf_run_backtest_full_impl(s, bars, n, input_tf, script_tf, bar_magnifier, magnifier_samples, magnifier_dist, out); }
        catch (const std::exception& _pf_error) { if (out) *out = ReportC{}; if (s) static_cast<GeneratedStrategy*>(s)->_pf_record_failure("run_backtest_full", _pf_error); }
        catch (...) { if (out) *out = ReportC{}; if (s) static_cast<GeneratedStrategy*>(s)->_pf_record_failure("run_backtest_full", "unknown C++ exception"); }
    }
    void strategy_free(void* s) {
        try { delete static_cast<GeneratedStrategy*>(s); } catch (...) {}
    }
    void report_free(ReportC* report) {
        try { BacktestEngine::free_report(report); } catch (...) {}
    }
    void strategy_set_input(void* s, const char* key, const char* value) {
        if (!s || !key || !value) return;
        try { static_cast<GeneratedStrategy*>(s)->set_input(key, value); }
        catch (const std::exception& _pf_error) { _PF_SETTING_FAILURE(static_cast<GeneratedStrategy*>(s), "strategy_set_input", _pf_error.what(), nullptr); }
        catch (...) { _PF_SETTING_FAILURE(static_cast<GeneratedStrategy*>(s), "strategy_set_input", "unknown C++ exception", nullptr); }
    }
    void strategy_set_override(void* s, const char* key, const char* value) {
        if (!s || !key || !value) return;
        try { static_cast<GeneratedStrategy*>(s)->set_strategy_override(key, value); }
#ifdef PINEFORGE_HAS_RUN_FAILURE_CODES_V1
        catch (const std::invalid_argument& _pf_error) { _PF_SETTING_FAILURE(static_cast<GeneratedStrategy*>(s), "strategy_set_override", _pf_error.what(), "unparseable_value"); }
        catch (const std::out_of_range& _pf_error) { _PF_SETTING_FAILURE(static_cast<GeneratedStrategy*>(s), "strategy_set_override", _pf_error.what(), "unparseable_value"); }
#endif
        catch (const std::exception& _pf_error) { _PF_SETTING_FAILURE(static_cast<GeneratedStrategy*>(s), "strategy_set_override", _pf_error.what(), nullptr); }
        catch (...) { _PF_SETTING_FAILURE(static_cast<GeneratedStrategy*>(s), "strategy_set_override", "unknown C++ exception", nullptr); }
    }
    void strategy_set_magnifier_volume_weighted(void* s, int on) {
        if (!s) return;
        try { static_cast<GeneratedStrategy*>(s)->set_magnifier_volume_weighted(on != 0); }
        catch (const std::exception& _pf_error) { _PF_SETTING_FAILURE(static_cast<GeneratedStrategy*>(s), "strategy_set_magnifier_volume_weighted", _pf_error.what(), nullptr); }
        catch (...) { _PF_SETTING_FAILURE(static_cast<GeneratedStrategy*>(s), "strategy_set_magnifier_volume_weighted", "unknown C++ exception", nullptr); }
    }
#ifdef PF_SETTINGS_API_VERSION
    uint32_t strategy_settings_api_version(void) { return PF_SETTINGS_API_VERSION; }
    int strategy_create_checked(const char* params_json, void** out, char* error, size_t error_capacity) {
        if (out) *out = nullptr;
        return ::pineforge::checked_settings::boundary(error, error_capacity, [&] {
            ::pineforge::checked_settings::require(out != nullptr, "strategy output pointer is null");
            ::pineforge::checked_settings::require(!params_json || !*params_json, "params_json is reserved; use checked setters", PF_SETTINGS_UNSUPPORTED);
            *out = new GeneratedStrategy();
        });
    }
    int strategy_set_input_checked(void* s, const char* key, const char* value, char* error, size_t error_capacity) {
        return ::pineforge::checked_settings::boundary(error, error_capacity, [&] {
            ::pineforge::checked_settings::require(s && key && value, "null strategy, key or value");
            static_cast<GeneratedStrategy*>(s)->_pf_set_input_checked(key, value);
        });
    }
    int strategy_set_override_checked(void* s, const char* key, const char* value, char* error, size_t error_capacity) {
        return ::pineforge::checked_settings::boundary(error, error_capacity, [&] {
            ::pineforge::checked_settings::require(s && key && value, "null strategy, key or value");
            static_cast<GeneratedStrategy*>(s)->_pf_set_override_checked(key, value);
        });
    }
    int strategy_get_effective_settings(void* s, char* json, size_t capacity, size_t* required, char* error, size_t error_capacity) {
        if (required) *required = 0;
        return ::pineforge::checked_settings::boundary(error, error_capacity, [&] {
            ::pineforge::checked_settings::require(s != nullptr, "null strategy");
            ::pineforge::checked_settings::receipt(static_cast<GeneratedStrategy*>(s)->_pf_settings_receipt(), json, capacity, required);
        });
    }
    int run_backtest_full_checked(void* s, Bar* bars, int n, const char* input_tf, const char* script_tf, int bar_magnifier, int magnifier_samples, int magnifier_dist, ReportC* out, char* error, size_t error_capacity) {
        return ::pineforge::checked_settings::boundary(error, error_capacity, [&] {
            ::pineforge::checked_settings::require(s && out && n >= 0 && (n == 0 || bars), "invalid batch arguments");
            _pf_run_backtest_full_impl(s, bars, n, input_tf, script_tf, bar_magnifier, magnifier_samples, magnifier_dist, out);
            const auto& _pf_error = static_cast<GeneratedStrategy*>(s)->last_error();
            ::pineforge::checked_settings::require(_pf_error.empty(), _pf_error.c_str(), PF_SETTINGS_RUN_FAILED);
        });
    }
#endif
#ifdef PF_SETTINGS_API_VERSION
#ifdef PF_CAPABILITIES_API_VERSION
    uint32_t strategy_capabilities_api_version(void) { return PF_CAPABILITIES_API_VERSION; }
    int strategy_capabilities_receipt(void* s, char* json, size_t capacity, size_t* required, char* error, size_t error_capacity) {
        if (required) *required = 0;
        return ::pineforge::checked_settings::boundary(error, error_capacity, [&] {
            ::pineforge::checked_settings::require(s != nullptr, "null strategy");
            ::pineforge::checked_settings::receipt("{\"declarations\":{\"backtest_fill_limits_assumption\":0,\"calc_on_every_history_tick\":false,\"calc_on_every_tick\":false,\"calc_on_order_fills\":false,\"currency\":\"currency.NONE\",\"dynamic_requests\":true,\"fill_orders_on_standard_ohlc\":false,\"process_orders_on_close\":false,\"timeframe\":\"\",\"timeframe_gaps\":true,\"use_bar_magnifier\":false},\"requests\":[],\"requirements\":{\"auxiliary_security_feeds\":false,\"fx_curve\":false,\"historical_probe_overrides\":false,\"intrabar_persistence\":false,\"native_security_feeds\":false,\"recorded_series\":false},\"unresolved\":[],\"version\":1}", json, capacity, required);
        });
    }
    uint32_t strategy_confirmed_bar_api_version(void) { return 1u; }
    int strategy_confirmed_bar_receipt(void* s, char* json, size_t capacity, size_t* required, char* error, size_t error_capacity) {
        if (required) *required = 0;
        return ::pineforge::checked_settings::boundary(error, error_capacity, [&] {
            ::pineforge::checked_settings::require(s != nullptr, "null strategy");
            ::pineforge::checked_settings::receipt("{\"intrabar_persistence\":false,\"orders\":[\"close:market\",\"entry:market\"],\"requests\":[],\"version\":1}", json, capacity, required);
        });
    }
    uint32_t strategy_order_shapes_api_version(void) { return 1u; }
    int strategy_order_shapes_receipt(void* s, char* json, size_t capacity, size_t* required, char* error, size_t error_capacity) {
        if (required) *required = 0;
        return ::pineforge::checked_settings::boundary(error, error_capacity, [&] {
            ::pineforge::checked_settings::require(s != nullptr, "null strategy");
            ::pineforge::checked_settings::receipt("{\"calls\":[{\"call\":\"entry\",\"comment\":\"absent\",\"context\":\"straight\",\"direction\":\"long\",\"id\":\"literal\",\"limit\":\"absent\",\"oca_name\":\"absent\",\"oca_type\":\"absent\",\"qty\":\"absent\",\"qty_type\":\"absent\",\"site\":0,\"stop\":\"absent\"},{\"call\":\"close\",\"comment\":\"absent\",\"context\":\"straight\",\"id\":\"literal\",\"immediately\":\"absent\",\"qty\":\"absent\",\"qty_percent\":\"absent\",\"site\":1,\"target\":\"long\"}],\"entry_ids\":{\"long\":1,\"multi_site\":0,\"shared\":0,\"short\":0},\"host_reads\":[\"StreamPhase\",\"_src_close_\",\"_src_high_\",\"_src_hl2_\",\"_src_hlc3_\",\"_src_hlcc4_\",\"_src_low_\",\"_src_ohlc4_\",\"_src_open_\",\"_src_series_active_\",\"_src_volume_\",\"commit_script_state\",\"config_\",\"configure_pine_strategy\",\"current_bar_\",\"fill_report\",\"history_advances_new_bar\",\"inputs_\",\"last_error\",\"last_error_\",\"on_source_bar\",\"override_\",\"pine_bar_index\",\"prepare_script_run\",\"restore_script_state\",\"run\",\"script_bars_processed\",\"set_input\",\"set_magnifier_volume_weighted\",\"set_strategy_override\",\"signed_position_size\",\"snapshot_script_state\",\"strategy_close\",\"strategy_entry\",\"stream_phase_\",\"syminfo_mintick_\"],\"process_orders_on_close\":false,\"settings\":{\"close_entries_rule\":\"FIFO\",\"commission_type\":\"percent\",\"commission_value\":0.0,\"default_qty_type\":\"fixed\",\"default_qty_value\":1.0,\"initial_capital\":100000.0,\"margin_long\":100.0,\"margin_short\":100.0,\"pyramiding\":0,\"slippage\":0},\"unmodeled\":[],\"version\":1}", json, capacity, required);
        });
    }
#endif
#endif
}
