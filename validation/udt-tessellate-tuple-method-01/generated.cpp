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
#include <deque>
#include <functional>
#include <limits>
#include <tuple>
#include <optional>
#include <type_traits>
#include <stdexcept>
#include <utility>
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

struct Tessellation {
    int32_t __pf_id = -1;
};
inline bool is_na(const Tessellation& _z) { return _z.__pf_id < 0; }

template <typename _PFValue>
struct _PFCheckpointTraits;

class _PFUdtUndoCoordinator {
    std::vector<std::function<void()>> _pf_undo_;
    uint64_t _pf_generation_ = 0;
    bool _pf_active_ = false;
public:
    struct Snapshot { uint64_t generation; };
    _PFUdtUndoCoordinator() = default;
    _PFUdtUndoCoordinator(
        const _PFUdtUndoCoordinator&) = delete;
    _PFUdtUndoCoordinator& operator=(
        const _PFUdtUndoCoordinator&) = delete;
    _PFUdtUndoCoordinator(
        _PFUdtUndoCoordinator&&) = delete;
    _PFUdtUndoCoordinator& operator=(
        _PFUdtUndoCoordinator&&) = delete;

    void reset_for_run() {
        _pf_undo_.clear();
        _pf_active_ = false;
        // Keep generation monotonic: an old token cannot become valid again.
    }

    Snapshot snapshot() {
        if (_pf_generation_ == std::numeric_limits<uint64_t>::max()) {
            _PF_ENGINE_INVARIANT("UDT checkpoint generation exhausted", std::overflow_error);
        }
        ++_pf_generation_;
        _pf_undo_.clear();
        _pf_active_ = true;
        return Snapshot{_pf_generation_};
    }
    uint64_t generation() const { return _pf_generation_; }
    bool active() const { return _pf_active_; }
    bool empty() const { return _pf_undo_.empty(); }
    void record(uint64_t generation, std::function<void()> undo) {
        if (!_pf_active_ || generation != _pf_generation_) {
            _PF_ENGINE_INVARIANT("invalid UDT undo generation", std::runtime_error);
        }
        _pf_undo_.push_back(std::move(undo));
    }
    void restore(const Snapshot& snapshot) {
        if (!_pf_active_ || snapshot.generation != _pf_generation_) {
            _PF_ENGINE_INVARIANT("invalid UDT coordinator checkpoint token", std::runtime_error);
        }
        for (auto entry = _pf_undo_.rbegin();
                entry != _pf_undo_.rend(); ++entry) (*entry)();
        _pf_undo_.clear();
    }
};

template <typename _PFHandle, typename _PFRecord>
class _PFUdtArena {
    using _PFRecordTraits = _PFCheckpointTraits<_PFRecord>;
    using _PFRecordSnapshot = typename _PFRecordTraits::snapshot_type;
    struct _PFSlot {
        _PFRecord value;
        uint64_t logged_generation = 0;
    };
    std::deque<_PFSlot> _pf_records_;
    _PFUdtUndoCoordinator* _pf_coordinator_;
    std::size_t _pf_checkpoint_size_ = 0;
    uint64_t _pf_checkpoint_generation_ = 0;
    bool _pf_checkpoint_active_ = false;

    void capture(std::size_t index) {
        if (!_pf_checkpoint_active_) return;
        auto& slot = _PF_INVARIANT_AT(_pf_records_, index);
        if (slot.logged_generation
                == _pf_checkpoint_generation_) return;
        auto snapshot = _PFRecordTraits::take(slot.value);
        _pf_coordinator_->record(_pf_checkpoint_generation_,
            [this, index, snapshot = std::move(snapshot)]() mutable {
                auto& restore_slot = _PF_INVARIANT_AT(_pf_records_, index);
                _PFRecordTraits::restore(restore_slot.value, snapshot);
                restore_slot.logged_generation = 0;
            });
        slot.logged_generation = _pf_checkpoint_generation_;
    }
public:
    struct Snapshot {
        uint64_t generation;
        std::size_t size;
    };

    explicit _PFUdtArena(
            _PFUdtUndoCoordinator* coordinator)
            : _pf_coordinator_(coordinator) {
        if (!_pf_coordinator_)
            _PF_ENGINE_INVARIANT("UDT arena requires undo coordinator", std::invalid_argument);
    }
    _PFUdtArena(
        const _PFUdtArena&) = delete;
    _PFUdtArena& operator=(
        const _PFUdtArena&) = delete;
    _PFUdtArena(
        _PFUdtArena&&) = delete;
    _PFUdtArena& operator=(
        _PFUdtArena&&) = delete;

    void reset_for_run() {
        _pf_records_.clear();
        _pf_checkpoint_size_ = 0;
        _pf_checkpoint_generation_ = 0;
        _pf_checkpoint_active_ = false;
        // The arena remains attached to its original coordinator.
    }

    _PFHandle create(_PFRecord value) {
        if (_pf_records_.size() > static_cast<std::size_t>(
                std::numeric_limits<int32_t>::max())) {
            _PF_LIMIT_STOP("udt_objects", 2147483647LL, "UDT object-ID capacity exceeded");
        }
        const auto id = static_cast<int32_t>(_pf_records_.size());
        _pf_records_.push_back(_PFSlot{std::move(value), 0});
        return _PFHandle{id};
    }
    _PFHandle copy(_PFHandle value) {
        return create(static_cast<const _PFUdtArena&>(*this).get(value));
    }
    _PFRecord& get(_PFHandle value) {
        if (value.__pf_id < 0
                || static_cast<std::size_t>(value.__pf_id) >= _pf_records_.size()) {
            _PF_NA_STOP("udt_object", "UDT access on na or invalid object ID");
        }
        const auto index = static_cast<std::size_t>(value.__pf_id);
        capture(index);
        return _pf_records_[index].value;
    }
    const _PFRecord& get(_PFHandle value) const {
        if (value.__pf_id < 0
                || static_cast<std::size_t>(value.__pf_id) >= _pf_records_.size()) {
            _PF_NA_STOP("udt_object", "UDT access on na or invalid object ID");
        }
        return _pf_records_[static_cast<std::size_t>(value.__pf_id)].value;
    }
    const _PFRecord& read(_PFHandle value) const {
        return get(value);
    }
    std::size_t size() const { return _pf_records_.size(); }
    _PFRecord& record_at(std::size_t index) {
        capture(index);
        return _PF_INVARIANT_AT(_pf_records_, index).value;
    }
    const _PFRecord& record_at(std::size_t index) const {
        return _PF_INVARIANT_AT(_pf_records_, index).value;
    }
    Snapshot snapshot() {
        if (!_pf_coordinator_->active()) {
            _PF_ENGINE_INVARIANT("UDT coordinator checkpoint is not active", std::runtime_error);
        }
        _pf_checkpoint_generation_ = _pf_coordinator_->generation();
        _pf_checkpoint_size_ = _pf_records_.size();
        _pf_checkpoint_active_ = true;
        return Snapshot{_pf_checkpoint_generation_,
                        _pf_checkpoint_size_};
    }
    void restore(const Snapshot& snapshot) {
        if (!_pf_checkpoint_active_
                || snapshot.generation != _pf_checkpoint_generation_
                || snapshot.generation != _pf_coordinator_->generation()
                || snapshot.size != _pf_checkpoint_size_
                || _pf_records_.size() < snapshot.size
                || !_pf_coordinator_->empty()) {
            _PF_ENGINE_INVARIANT("invalid UDT checkpoint token", std::runtime_error);
        }
        _pf_records_.resize(snapshot.size);
    }
};

struct _PFUdtRecord_Tessellation {
    double center = 0.0;
    double span = 0.0;
};

template <typename _PFValue>
struct _PFCheckpointTraits {
    using snapshot_type = _PFValue;
    static snapshot_type take(const _PFValue& value) { return value; }
    static void restore(_PFValue& value, const snapshot_type& snapshot) {
        value = snapshot;
    }
};

template <>
struct _PFCheckpointTraits<_PFUdtUndoCoordinator> {
    using coordinator_type = _PFUdtUndoCoordinator;
    using snapshot_type = typename coordinator_type::Snapshot;
    static snapshot_type take(coordinator_type& value) {
        return value.snapshot();
    }
    static void restore(coordinator_type& value,
                        const snapshot_type& snapshot) {
        value.restore(snapshot);
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

template <>
struct _PFCheckpointTraits<_PFUdtRecord_Tessellation> {
    struct snapshot_type {
        _PFCheckpointTraits<decltype(_PFUdtRecord_Tessellation::center)>::snapshot_type _pf_field_0;
        _PFCheckpointTraits<decltype(_PFUdtRecord_Tessellation::span)>::snapshot_type _pf_field_1;
    };
    static snapshot_type take(const _PFUdtRecord_Tessellation& value) {
        return snapshot_type{
            _PFCheckpointTraits<decltype(_PFUdtRecord_Tessellation::center)>::take(value.center),
            _PFCheckpointTraits<decltype(_PFUdtRecord_Tessellation::span)>::take(value.span),
        };
    }
    static void restore(_PFUdtRecord_Tessellation& value, const snapshot_type& snapshot) {
        _PFCheckpointTraits<decltype(_PFUdtRecord_Tessellation::center)>::restore(value.center, snapshot._pf_field_0);
        _PFCheckpointTraits<decltype(_PFUdtRecord_Tessellation::span)>::restore(value.span, snapshot._pf_field_1);
    }
};

template <>
struct _PFCheckpointTraits<_PFUdtArena<Tessellation, _PFUdtRecord_Tessellation>> {
    using arena_type = _PFUdtArena<Tessellation, _PFUdtRecord_Tessellation>;
    using snapshot_type = typename arena_type::Snapshot;
    static snapshot_type take(arena_type& value) {
        return value.snapshot();
    }
    static void restore(arena_type& value, const snapshot_type& snapshot) {
        value.restore(snapshot);
    }
};

class GeneratedStrategy : public pineforge::source::PineStrategyHost {
public:
    _PFUdtUndoCoordinator _pf_udt_undo;
    _PFUdtArena<Tessellation, _PFUdtRecord_Tessellation> _pf_udt_Tessellation{&_pf_udt_undo};
    ta::EMA _ta_ema_1;
    std::vector<double> _precalc__ta_ema_1;
    ta::ATR _ta_atr_2;
    std::vector<double> _precalc__ta_atr_2;
    ta::Crossover _ta_crossover_3;
    bool _use_precalc = false;
    int centerLength = 0;
    int spanLength = 0;
    double upperScale = 0.0;
    double lowerScale = 0.0;
    double maximumAtrLoss = 0.0;
    Tessellation tile = Tessellation{};
    double upperBoundary = 0.0;
    double lowerBoundary = 0.0;
    bool enterLong = false;
    bool channelEnded = false;
    bool riskExceeded = false;
    bool _ta_initialized_ = false;
    bool _inputs_initialized_ = false;

    struct _PFScriptState {
        _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_undo)>::snapshot_type _pf_value_0;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_Tessellation)>::snapshot_type _pf_value_1;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_ema_1)>::snapshot_type _pf_value_2;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_atr_2)>::snapshot_type _pf_value_3;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_crossover_3)>::snapshot_type _pf_value_4;
        _PFCheckpointTraits<decltype(GeneratedStrategy::centerLength)>::snapshot_type _pf_value_5;
        _PFCheckpointTraits<decltype(GeneratedStrategy::spanLength)>::snapshot_type _pf_value_6;
        _PFCheckpointTraits<decltype(GeneratedStrategy::upperScale)>::snapshot_type _pf_value_7;
        _PFCheckpointTraits<decltype(GeneratedStrategy::lowerScale)>::snapshot_type _pf_value_8;
        _PFCheckpointTraits<decltype(GeneratedStrategy::maximumAtrLoss)>::snapshot_type _pf_value_9;
        _PFCheckpointTraits<decltype(GeneratedStrategy::tile)>::snapshot_type _pf_value_10;
        _PFCheckpointTraits<decltype(GeneratedStrategy::upperBoundary)>::snapshot_type _pf_value_11;
        _PFCheckpointTraits<decltype(GeneratedStrategy::lowerBoundary)>::snapshot_type _pf_value_12;
        _PFCheckpointTraits<decltype(GeneratedStrategy::enterLong)>::snapshot_type _pf_value_13;
        _PFCheckpointTraits<decltype(GeneratedStrategy::channelEnded)>::snapshot_type _pf_value_14;
        _PFCheckpointTraits<decltype(GeneratedStrategy::riskExceeded)>::snapshot_type _pf_value_15;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_initialized_)>::snapshot_type _pf_value_16;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_inputs_initialized_)>::snapshot_type _pf_value_17;
    };
    static_assert(std::is_copy_constructible_v<_PFScriptState>, "generated Pine state must be deep-copy constructible");
    static_assert(std::is_copy_assignable_v<_PFScriptState>, "generated Pine state must be deep-copy assignable");
    std::optional<_PFScriptState> _pf_script_state_checkpoint_;

    void snapshot_script_state() override {
        _pf_script_state_checkpoint_.emplace(_PFScriptState{
            _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_undo)>::take(_pf_udt_undo),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_Tessellation)>::take(_pf_udt_Tessellation),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_ema_1)>::take(_ta_ema_1),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_atr_2)>::take(_ta_atr_2),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_crossover_3)>::take(_ta_crossover_3),
            _PFCheckpointTraits<decltype(GeneratedStrategy::centerLength)>::take(centerLength),
            _PFCheckpointTraits<decltype(GeneratedStrategy::spanLength)>::take(spanLength),
            _PFCheckpointTraits<decltype(GeneratedStrategy::upperScale)>::take(upperScale),
            _PFCheckpointTraits<decltype(GeneratedStrategy::lowerScale)>::take(lowerScale),
            _PFCheckpointTraits<decltype(GeneratedStrategy::maximumAtrLoss)>::take(maximumAtrLoss),
            _PFCheckpointTraits<decltype(GeneratedStrategy::tile)>::take(tile),
            _PFCheckpointTraits<decltype(GeneratedStrategy::upperBoundary)>::take(upperBoundary),
            _PFCheckpointTraits<decltype(GeneratedStrategy::lowerBoundary)>::take(lowerBoundary),
            _PFCheckpointTraits<decltype(GeneratedStrategy::enterLong)>::take(enterLong),
            _PFCheckpointTraits<decltype(GeneratedStrategy::channelEnded)>::take(channelEnded),
            _PFCheckpointTraits<decltype(GeneratedStrategy::riskExceeded)>::take(riskExceeded),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_initialized_)>::take(_ta_initialized_),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_inputs_initialized_)>::take(_inputs_initialized_),
        });
    }

    void restore_script_state() override {
        if (!_pf_script_state_checkpoint_) return;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_undo)>::restore(this->_pf_udt_undo, _pf_script_state_checkpoint_->_pf_value_0);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_Tessellation)>::restore(this->_pf_udt_Tessellation, _pf_script_state_checkpoint_->_pf_value_1);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_ema_1)>::restore(this->_ta_ema_1, _pf_script_state_checkpoint_->_pf_value_2);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_atr_2)>::restore(this->_ta_atr_2, _pf_script_state_checkpoint_->_pf_value_3);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_crossover_3)>::restore(this->_ta_crossover_3, _pf_script_state_checkpoint_->_pf_value_4);
        _PFCheckpointTraits<decltype(GeneratedStrategy::centerLength)>::restore(this->centerLength, _pf_script_state_checkpoint_->_pf_value_5);
        _PFCheckpointTraits<decltype(GeneratedStrategy::spanLength)>::restore(this->spanLength, _pf_script_state_checkpoint_->_pf_value_6);
        _PFCheckpointTraits<decltype(GeneratedStrategy::upperScale)>::restore(this->upperScale, _pf_script_state_checkpoint_->_pf_value_7);
        _PFCheckpointTraits<decltype(GeneratedStrategy::lowerScale)>::restore(this->lowerScale, _pf_script_state_checkpoint_->_pf_value_8);
        _PFCheckpointTraits<decltype(GeneratedStrategy::maximumAtrLoss)>::restore(this->maximumAtrLoss, _pf_script_state_checkpoint_->_pf_value_9);
        _PFCheckpointTraits<decltype(GeneratedStrategy::tile)>::restore(this->tile, _pf_script_state_checkpoint_->_pf_value_10);
        _PFCheckpointTraits<decltype(GeneratedStrategy::upperBoundary)>::restore(this->upperBoundary, _pf_script_state_checkpoint_->_pf_value_11);
        _PFCheckpointTraits<decltype(GeneratedStrategy::lowerBoundary)>::restore(this->lowerBoundary, _pf_script_state_checkpoint_->_pf_value_12);
        _PFCheckpointTraits<decltype(GeneratedStrategy::enterLong)>::restore(this->enterLong, _pf_script_state_checkpoint_->_pf_value_13);
        _PFCheckpointTraits<decltype(GeneratedStrategy::channelEnded)>::restore(this->channelEnded, _pf_script_state_checkpoint_->_pf_value_14);
        _PFCheckpointTraits<decltype(GeneratedStrategy::riskExceeded)>::restore(this->riskExceeded, _pf_script_state_checkpoint_->_pf_value_15);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_initialized_)>::restore(this->_ta_initialized_, _pf_script_state_checkpoint_->_pf_value_16);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_inputs_initialized_)>::restore(this->_inputs_initialized_, _pf_script_state_checkpoint_->_pf_value_17);
    }

    void commit_script_state() override {
        snapshot_script_state();
    }

    explicit GeneratedStrategy() : _ta_ema_1(42), _ta_atr_2(17) {
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
        cfg.default_qty_value = 2.0;
        cfg.pyramiding = 0;
        cfg.commission_type = static_cast<int>(CommissionType::PERCENT);
        cfg.commission_value = 0.05;
        cfg.slippage = 1;
        cfg.margin_long = 100.0;
        cfg.margin_short = 100.0;
        return cfg;
    }
    std::vector<::pineforge::checked_settings::Setting> _pf_settings_inputs() const {
        return {
            {"Center EMA Length", "int", ::pineforge::checked_settings::number(42), {}, 5, std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN(), 32, true, {}, "int"},
            {"ATR Span Length", "int", ::pineforge::checked_settings::number(17), {}, 2, std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN(), 32, true, {}, "int"},
            {"Upper Span", "float", ::pineforge::checked_settings::number(1.25), {}, 0.1, std::numeric_limits<double>::quiet_NaN(), 0.05, 32, true, {}, "float"},
            {"Lower Span", "float", ::pineforge::checked_settings::number(0.8), {}, 0.1, std::numeric_limits<double>::quiet_NaN(), 0.05, 32, true, {}, "float"},
            {"Maximum ATR Loss", "float", ::pineforge::checked_settings::number(2.5), {}, 0.5, std::numeric_limits<double>::quiet_NaN(), 0.1, 32, true, {}, "float"},
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
        _pf_document += ::pineforge::checked_settings::describe(_pf_inputs[0], ::pineforge::checked_settings::number(get_input_int("Center EMA Length", 42)));
        _pf_document += ',';
        _pf_document += ::pineforge::checked_settings::describe(_pf_inputs[1], ::pineforge::checked_settings::number(get_input_int("ATR Span Length", 17)));
        _pf_document += ',';
        _pf_document += ::pineforge::checked_settings::describe(_pf_inputs[2], ::pineforge::checked_settings::number(get_input_double("Upper Span", 1.25)));
        _pf_document += ',';
        _pf_document += ::pineforge::checked_settings::describe(_pf_inputs[3], ::pineforge::checked_settings::number(get_input_double("Lower Span", 0.8)));
        _pf_document += ',';
        _pf_document += ::pineforge::checked_settings::describe(_pf_inputs[4], ::pineforge::checked_settings::number(get_input_double("Maximum ATR Loss", 2.5)));
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
        this->_pf_udt_undo.reset_for_run();
        this->_pf_udt_Tessellation.reset_for_run();
        this->_ta_ema_1 = decltype(this->_ta_ema_1)(42);
        this->_precalc__ta_ema_1 = decltype(this->_precalc__ta_ema_1){};
        this->_ta_atr_2 = decltype(this->_ta_atr_2)(17);
        this->_precalc__ta_atr_2 = decltype(this->_precalc__ta_atr_2){};
        this->_ta_crossover_3 = decltype(this->_ta_crossover_3){};
        this->_use_precalc = false;
        this->centerLength = 0;
        this->spanLength = 0;
        this->upperScale = 0.0;
        this->lowerScale = 0.0;
        this->maximumAtrLoss = 0.0;
        this->tile = Tessellation{};
        this->upperBoundary = 0.0;
        this->lowerBoundary = 0.0;
        this->enterLong = false;
        this->channelEnded = false;
        this->riskExceeded = false;
        this->_ta_initialized_ = false;
        this->_inputs_initialized_ = false;
        if (allow_precalculation) precalculate(bars, n);
    }

    std::tuple<double, double> _udt_Tessellation_boundaries(Tessellation self, double upperScale, double lowerScale) {
        double upperBoundary = (_pf_udt_Tessellation.read(self).center + (_pf_udt_Tessellation.read(self).span * upperScale));
        double lowerBoundary = (_pf_udt_Tessellation.read(self).center - (_pf_udt_Tessellation.read(self).span * lowerScale));
        return std::make_tuple(upperBoundary, lowerBoundary);
    }

    void on_source_bar(const Bar& bar) override {
        if (!_inputs_initialized_) {
            centerLength = get_input_int("Center EMA Length", 42);
            spanLength = get_input_int("ATR Span Length", 17);
            upperScale = get_input_double("Upper Span", 1.25);
            lowerScale = get_input_double("Lower Span", 0.8);
            maximumAtrLoss = get_input_double("Maximum ATR Loss", 2.5);
            _inputs_initialized_ = true;
        }
        if (!_ta_initialized_) {
            _ta_ema_1 = ta::EMA(get_input_int("Center EMA Length", 42));
            _ta_atr_2 = ta::ATR(get_input_int("ATR Span Length", 17));
            _ta_initialized_ = true;
        }
        tile = _pf_udt_Tessellation.create(_PFUdtRecord_Tessellation{.center = (_use_precalc ? _precalc__ta_ema_1[bar_index_] : (history_advances_new_bar() ? _ta_ema_1.compute(current_bar_.close) : _ta_ema_1.recompute(current_bar_.close))), .span = (_use_precalc ? _precalc__ta_atr_2[bar_index_] : (history_advances_new_bar() ? _ta_atr_2.compute(current_bar_.high, current_bar_.low, current_bar_.close, prev_chart_close()) : _ta_atr_2.recompute(current_bar_.high, current_bar_.low, current_bar_.close, prev_chart_close())))});
        auto [upperBoundary, lowerBoundary] = _udt_Tessellation_boundaries(tile, upperScale, lowerScale);
        enterLong = (history_advances_new_bar() ? _ta_crossover_3.compute(current_bar_.close, upperBoundary) : _ta_crossover_3.recompute(current_bar_.close, upperBoundary));
        channelEnded = ([&]{ auto _pna_l = (current_bar_.close); auto _pna_r = (lowerBoundary); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }());
        riskExceeded = (([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()) && ([&]{ auto _pna_l = (current_bar_.close); auto _pna_r = (((signed_position_size() == 0.0 ? na<double>() : position_entry_price_) - (_pf_udt_Tessellation.read(tile).span * maximumAtrLoss))); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }()));
        if ((([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && (_pfc_eq); }()) && [&](){ auto _pf_bool_v = (enterLong); using _pf_bool_t = std::decay_t<decltype(_pf_bool_v)>; if constexpr (std::is_same_v<_pf_bool_t, bool>) { return _pf_bool_v; } else if constexpr (std::is_floating_point_v<_pf_bool_t> || std::is_integral_v<_pf_bool_t>) { return is_na(_pf_bool_v) ? false : (_pf_bool_v != 0); } else { return static_cast<bool>(_pf_bool_v); } }())) {
            strategy_entry(std::string("Tessellate Long"), true, na<double>(), na<double>(), na<double>(), "");
        } else
        if ((([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()) && (channelEnded || riskExceeded))) {
            strategy_close(std::string("Tessellate Long"), ((riskExceeded) ? (std::string("ATR risk")) : (std::string("Channel ended"))), na<double>(), na<double>(), false, 167503724563ULL);
        }
    }

    void precalculate(const Bar* bars, int n) {
        _use_precalc = false;
        if (n <= 0 || bars == nullptr) return;

        _precalc__ta_ema_1.resize(n);
        _precalc__ta_atr_2.resize(n);

        _ta_ema_1 = ta::EMA(get_input_int("Center EMA Length", 42));
        _ta_atr_2 = ta::ATR(get_input_int("ATR Span Length", 17));


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
            _precalc__ta_atr_2[i] = _ta_atr_2.compute(bars[i].high, bars[i].low, bars[i].close, (i > 0 ? bars[i - 1].close : na<double>()));
        }

        _ta_ema_1 = ta::EMA(get_input_int("Center EMA Length", 42));
        _ta_atr_2 = ta::ATR(get_input_int("ATR Span Length", 17));

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
            ::pineforge::checked_settings::receipt("{\"intrabar_persistence\":false,\"orders\":[\"entry:market\",\"strategy.close (unproven order shape)\"],\"requests\":[],\"version\":1}", json, capacity, required);
        });
    }
    uint32_t strategy_order_shapes_api_version(void) { return 1u; }
    int strategy_order_shapes_receipt(void* s, char* json, size_t capacity, size_t* required, char* error, size_t error_capacity) {
        if (required) *required = 0;
        return ::pineforge::checked_settings::boundary(error, error_capacity, [&] {
            ::pineforge::checked_settings::require(s != nullptr, "null strategy");
            ::pineforge::checked_settings::receipt("{\"calls\":[{\"call\":\"entry\",\"comment\":\"absent\",\"context\":\"straight\",\"direction\":\"long\",\"id\":\"literal\",\"limit\":\"absent\",\"oca_name\":\"absent\",\"oca_type\":\"absent\",\"qty\":\"absent\",\"qty_type\":\"absent\",\"site\":0,\"stop\":\"absent\"},{\"call\":\"close\",\"comment\":\"dynamic\",\"context\":\"straight\",\"id\":\"literal\",\"immediately\":\"absent\",\"qty\":\"absent\",\"qty_percent\":\"absent\",\"site\":1,\"target\":\"long\"}],\"entry_ids\":{\"long\":1,\"multi_site\":0,\"shared\":0,\"short\":0},\"host_reads\":[\"StreamPhase\",\"_src_close_\",\"_src_high_\",\"_src_hl2_\",\"_src_hlc3_\",\"_src_hlcc4_\",\"_src_low_\",\"_src_ohlc4_\",\"_src_open_\",\"_src_series_active_\",\"_src_volume_\",\"bar_index_\",\"commit_script_state\",\"config_\",\"configure_pine_strategy\",\"current_bar_\",\"fill_report\",\"get_input_double\",\"get_input_int\",\"history_advances_new_bar\",\"inputs_\",\"last_error\",\"last_error_\",\"on_source_bar\",\"override_\",\"position_entry_price_\",\"prepare_script_run\",\"prev_chart_close\",\"restore_script_state\",\"run\",\"script_bars_processed\",\"set_input\",\"set_magnifier_volume_weighted\",\"set_strategy_override\",\"signed_position_size\",\"snapshot_script_state\",\"strategy_close\",\"strategy_entry\",\"stream_phase_\"],\"process_orders_on_close\":false,\"settings\":{\"close_entries_rule\":\"FIFO\",\"commission_type\":\"percent\",\"commission_value\":0.05,\"default_qty_type\":\"fixed\",\"default_qty_value\":2.0,\"initial_capital\":100000.0,\"margin_long\":100.0,\"margin_short\":100.0,\"pyramiding\":0,\"slippage\":1},\"unmodeled\":[],\"version\":1}", json, capacity, required);
        });
    }
#endif
#endif
}
