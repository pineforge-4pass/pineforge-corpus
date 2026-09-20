#include <pineforge/source/pine_strategy_host.hpp>
#include <pineforge/ta.hpp>
#include <pineforge/math.hpp>
#include <pineforge/series.hpp>
#include <pineforge/na.hpp>
#include <pineforge/map.hpp>
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

struct LayerInputs {
    int32_t __pf_id = -1;
};
inline bool is_na(const LayerInputs& _z) { return _z.__pf_id < 0; }

struct RsiLayer {
    int32_t __pf_id = -1;
};
inline bool is_na(const RsiLayer& _z) { return _z.__pf_id < 0; }

struct TrendLayer {
    int32_t __pf_id = -1;
};
inline bool is_na(const TrendLayer& _z) { return _z.__pf_id < 0; }

struct BbLayer {
    int32_t __pf_id = -1;
};
inline bool is_na(const BbLayer& _z) { return _z.__pf_id < 0; }

struct ScoreSnapshot {
    int32_t __pf_id = -1;
};
inline bool is_na(const ScoreSnapshot& _z) { return _z.__pf_id < 0; }

struct VolContext {
    int32_t __pf_id = -1;
};
inline bool is_na(const VolContext& _z) { return _z.__pf_id < 0; }

struct OhlcBar {
    int32_t __pf_id = -1;
};
inline bool is_na(const OhlcBar& _z) { return _z.__pf_id < 0; }

struct SessionScratch {
    int32_t __pf_id = -1;
};
inline bool is_na(const SessionScratch& _z) { return _z.__pf_id < 0; }

struct GateState {
    int32_t __pf_id = -1;
};
inline bool is_na(const GateState& _z) { return _z.__pf_id < 0; }

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
            throw std::overflow_error("UDT checkpoint generation exhausted");
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
            throw std::runtime_error("invalid UDT undo generation");
        }
        _pf_undo_.push_back(std::move(undo));
    }
    void restore(const Snapshot& snapshot) {
        if (!_pf_active_ || snapshot.generation != _pf_generation_) {
            throw std::runtime_error("invalid UDT coordinator checkpoint token");
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
        auto& slot = _pf_records_.at(index);
        if (slot.logged_generation
                == _pf_checkpoint_generation_) return;
        auto snapshot = _PFRecordTraits::take(slot.value);
        _pf_coordinator_->record(_pf_checkpoint_generation_,
            [this, index, snapshot = std::move(snapshot)]() mutable {
                auto& restore_slot = _pf_records_.at(index);
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
            throw std::invalid_argument("UDT arena requires undo coordinator");
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
            throw std::length_error("UDT object-ID capacity exceeded");
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
            throw std::runtime_error("UDT access on na or invalid object ID");
        }
        const auto index = static_cast<std::size_t>(value.__pf_id);
        capture(index);
        return _pf_records_[index].value;
    }
    const _PFRecord& get(_PFHandle value) const {
        if (value.__pf_id < 0
                || static_cast<std::size_t>(value.__pf_id) >= _pf_records_.size()) {
            throw std::runtime_error("UDT access on na or invalid object ID");
        }
        return _pf_records_[static_cast<std::size_t>(value.__pf_id)].value;
    }
    const _PFRecord& read(_PFHandle value) const {
        return get(value);
    }
    std::size_t size() const { return _pf_records_.size(); }
    _PFRecord& record_at(std::size_t index) {
        capture(index);
        return _pf_records_.at(index).value;
    }
    const _PFRecord& record_at(std::size_t index) const {
        return _pf_records_.at(index).value;
    }
    Snapshot snapshot() {
        if (!_pf_coordinator_->active()) {
            throw std::runtime_error("UDT coordinator checkpoint is not active");
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
            throw std::runtime_error("invalid UDT checkpoint token");
        }
        _pf_records_.resize(snapshot.size);
    }
};

struct _PFUdtRecord_LayerInputs {
    int64_t rsi_len = 0;
    int64_t ma_len = 0;
    int64_t bb_len = 0;
    double bb_mult = 0.0;
};

struct _PFUdtRecord_RsiLayer {
    double value = 0.0;
    int64_t contrib = 0;
};

struct _PFUdtRecord_TrendLayer {
    double ema = 0.0;
    int64_t contrib = 0;
};

struct _PFUdtRecord_BbLayer {
    double mid = 0.0;
    double upper = 0.0;
    double lower = 0.0;
    int64_t contrib = 0;
};

struct _PFUdtRecord_ScoreSnapshot {
    int64_t rsi_c = 0;
    int64_t trend_c = 0;
    int64_t bb_c = 0;
};

struct _PFUdtRecord_VolContext {
    double atr = 0.0;
    double atr_ma = 0.0;
    double ratio = 0.0;
};

struct _PFUdtRecord_OhlcBar {
    double o = 0.0;
    double h = 0.0;
    double l = 0.0;
    double c = 0.0;
};

struct _PFUdtRecord_SessionScratch {
    bool in_session = false;
    int64_t bar_streak = 0;
};

struct _PFUdtRecord_GateState {
    bool vol_ok = false;
    bool map_ok = false;
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

template <typename _PFKey, typename _PFValue>
struct _PFCheckpointTraits<PineMap<_PFKey, _PFValue>> {
    using map_type = PineMap<_PFKey, _PFValue>;
    static_assert(map_type::snapshot_supported,
                  "generated map checkpoints require primitive map values");
    using snapshot_type = std::optional<typename map_type::Snapshot>;
    static snapshot_type take(const map_type& value) {
        if (value.is_na()) return std::nullopt;
        return value.snapshot();
    }
    static void restore(map_type& value, const snapshot_type& snapshot) {
        if (!snapshot) {
            value = map_type{};
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

template <>
struct _PFCheckpointTraits<_PFUdtRecord_LayerInputs> {
    struct snapshot_type {
        _PFCheckpointTraits<decltype(_PFUdtRecord_LayerInputs::rsi_len)>::snapshot_type _pf_field_0;
        _PFCheckpointTraits<decltype(_PFUdtRecord_LayerInputs::ma_len)>::snapshot_type _pf_field_1;
        _PFCheckpointTraits<decltype(_PFUdtRecord_LayerInputs::bb_len)>::snapshot_type _pf_field_2;
        _PFCheckpointTraits<decltype(_PFUdtRecord_LayerInputs::bb_mult)>::snapshot_type _pf_field_3;
    };
    static snapshot_type take(const _PFUdtRecord_LayerInputs& value) {
        return snapshot_type{
            _PFCheckpointTraits<decltype(_PFUdtRecord_LayerInputs::rsi_len)>::take(value.rsi_len),
            _PFCheckpointTraits<decltype(_PFUdtRecord_LayerInputs::ma_len)>::take(value.ma_len),
            _PFCheckpointTraits<decltype(_PFUdtRecord_LayerInputs::bb_len)>::take(value.bb_len),
            _PFCheckpointTraits<decltype(_PFUdtRecord_LayerInputs::bb_mult)>::take(value.bb_mult),
        };
    }
    static void restore(_PFUdtRecord_LayerInputs& value, const snapshot_type& snapshot) {
        _PFCheckpointTraits<decltype(_PFUdtRecord_LayerInputs::rsi_len)>::restore(value.rsi_len, snapshot._pf_field_0);
        _PFCheckpointTraits<decltype(_PFUdtRecord_LayerInputs::ma_len)>::restore(value.ma_len, snapshot._pf_field_1);
        _PFCheckpointTraits<decltype(_PFUdtRecord_LayerInputs::bb_len)>::restore(value.bb_len, snapshot._pf_field_2);
        _PFCheckpointTraits<decltype(_PFUdtRecord_LayerInputs::bb_mult)>::restore(value.bb_mult, snapshot._pf_field_3);
    }
};

template <>
struct _PFCheckpointTraits<_PFUdtArena<LayerInputs, _PFUdtRecord_LayerInputs>> {
    using arena_type = _PFUdtArena<LayerInputs, _PFUdtRecord_LayerInputs>;
    using snapshot_type = typename arena_type::Snapshot;
    static snapshot_type take(arena_type& value) {
        return value.snapshot();
    }
    static void restore(arena_type& value, const snapshot_type& snapshot) {
        value.restore(snapshot);
    }
};

template <>
struct _PFCheckpointTraits<_PFUdtRecord_RsiLayer> {
    struct snapshot_type {
        _PFCheckpointTraits<decltype(_PFUdtRecord_RsiLayer::value)>::snapshot_type _pf_field_0;
        _PFCheckpointTraits<decltype(_PFUdtRecord_RsiLayer::contrib)>::snapshot_type _pf_field_1;
    };
    static snapshot_type take(const _PFUdtRecord_RsiLayer& value) {
        return snapshot_type{
            _PFCheckpointTraits<decltype(_PFUdtRecord_RsiLayer::value)>::take(value.value),
            _PFCheckpointTraits<decltype(_PFUdtRecord_RsiLayer::contrib)>::take(value.contrib),
        };
    }
    static void restore(_PFUdtRecord_RsiLayer& value, const snapshot_type& snapshot) {
        _PFCheckpointTraits<decltype(_PFUdtRecord_RsiLayer::value)>::restore(value.value, snapshot._pf_field_0);
        _PFCheckpointTraits<decltype(_PFUdtRecord_RsiLayer::contrib)>::restore(value.contrib, snapshot._pf_field_1);
    }
};

template <>
struct _PFCheckpointTraits<_PFUdtArena<RsiLayer, _PFUdtRecord_RsiLayer>> {
    using arena_type = _PFUdtArena<RsiLayer, _PFUdtRecord_RsiLayer>;
    using snapshot_type = typename arena_type::Snapshot;
    static snapshot_type take(arena_type& value) {
        return value.snapshot();
    }
    static void restore(arena_type& value, const snapshot_type& snapshot) {
        value.restore(snapshot);
    }
};

template <>
struct _PFCheckpointTraits<_PFUdtRecord_TrendLayer> {
    struct snapshot_type {
        _PFCheckpointTraits<decltype(_PFUdtRecord_TrendLayer::ema)>::snapshot_type _pf_field_0;
        _PFCheckpointTraits<decltype(_PFUdtRecord_TrendLayer::contrib)>::snapshot_type _pf_field_1;
    };
    static snapshot_type take(const _PFUdtRecord_TrendLayer& value) {
        return snapshot_type{
            _PFCheckpointTraits<decltype(_PFUdtRecord_TrendLayer::ema)>::take(value.ema),
            _PFCheckpointTraits<decltype(_PFUdtRecord_TrendLayer::contrib)>::take(value.contrib),
        };
    }
    static void restore(_PFUdtRecord_TrendLayer& value, const snapshot_type& snapshot) {
        _PFCheckpointTraits<decltype(_PFUdtRecord_TrendLayer::ema)>::restore(value.ema, snapshot._pf_field_0);
        _PFCheckpointTraits<decltype(_PFUdtRecord_TrendLayer::contrib)>::restore(value.contrib, snapshot._pf_field_1);
    }
};

template <>
struct _PFCheckpointTraits<_PFUdtArena<TrendLayer, _PFUdtRecord_TrendLayer>> {
    using arena_type = _PFUdtArena<TrendLayer, _PFUdtRecord_TrendLayer>;
    using snapshot_type = typename arena_type::Snapshot;
    static snapshot_type take(arena_type& value) {
        return value.snapshot();
    }
    static void restore(arena_type& value, const snapshot_type& snapshot) {
        value.restore(snapshot);
    }
};

template <>
struct _PFCheckpointTraits<_PFUdtRecord_BbLayer> {
    struct snapshot_type {
        _PFCheckpointTraits<decltype(_PFUdtRecord_BbLayer::mid)>::snapshot_type _pf_field_0;
        _PFCheckpointTraits<decltype(_PFUdtRecord_BbLayer::upper)>::snapshot_type _pf_field_1;
        _PFCheckpointTraits<decltype(_PFUdtRecord_BbLayer::lower)>::snapshot_type _pf_field_2;
        _PFCheckpointTraits<decltype(_PFUdtRecord_BbLayer::contrib)>::snapshot_type _pf_field_3;
    };
    static snapshot_type take(const _PFUdtRecord_BbLayer& value) {
        return snapshot_type{
            _PFCheckpointTraits<decltype(_PFUdtRecord_BbLayer::mid)>::take(value.mid),
            _PFCheckpointTraits<decltype(_PFUdtRecord_BbLayer::upper)>::take(value.upper),
            _PFCheckpointTraits<decltype(_PFUdtRecord_BbLayer::lower)>::take(value.lower),
            _PFCheckpointTraits<decltype(_PFUdtRecord_BbLayer::contrib)>::take(value.contrib),
        };
    }
    static void restore(_PFUdtRecord_BbLayer& value, const snapshot_type& snapshot) {
        _PFCheckpointTraits<decltype(_PFUdtRecord_BbLayer::mid)>::restore(value.mid, snapshot._pf_field_0);
        _PFCheckpointTraits<decltype(_PFUdtRecord_BbLayer::upper)>::restore(value.upper, snapshot._pf_field_1);
        _PFCheckpointTraits<decltype(_PFUdtRecord_BbLayer::lower)>::restore(value.lower, snapshot._pf_field_2);
        _PFCheckpointTraits<decltype(_PFUdtRecord_BbLayer::contrib)>::restore(value.contrib, snapshot._pf_field_3);
    }
};

template <>
struct _PFCheckpointTraits<_PFUdtArena<BbLayer, _PFUdtRecord_BbLayer>> {
    using arena_type = _PFUdtArena<BbLayer, _PFUdtRecord_BbLayer>;
    using snapshot_type = typename arena_type::Snapshot;
    static snapshot_type take(arena_type& value) {
        return value.snapshot();
    }
    static void restore(arena_type& value, const snapshot_type& snapshot) {
        value.restore(snapshot);
    }
};

template <>
struct _PFCheckpointTraits<_PFUdtRecord_ScoreSnapshot> {
    struct snapshot_type {
        _PFCheckpointTraits<decltype(_PFUdtRecord_ScoreSnapshot::rsi_c)>::snapshot_type _pf_field_0;
        _PFCheckpointTraits<decltype(_PFUdtRecord_ScoreSnapshot::trend_c)>::snapshot_type _pf_field_1;
        _PFCheckpointTraits<decltype(_PFUdtRecord_ScoreSnapshot::bb_c)>::snapshot_type _pf_field_2;
    };
    static snapshot_type take(const _PFUdtRecord_ScoreSnapshot& value) {
        return snapshot_type{
            _PFCheckpointTraits<decltype(_PFUdtRecord_ScoreSnapshot::rsi_c)>::take(value.rsi_c),
            _PFCheckpointTraits<decltype(_PFUdtRecord_ScoreSnapshot::trend_c)>::take(value.trend_c),
            _PFCheckpointTraits<decltype(_PFUdtRecord_ScoreSnapshot::bb_c)>::take(value.bb_c),
        };
    }
    static void restore(_PFUdtRecord_ScoreSnapshot& value, const snapshot_type& snapshot) {
        _PFCheckpointTraits<decltype(_PFUdtRecord_ScoreSnapshot::rsi_c)>::restore(value.rsi_c, snapshot._pf_field_0);
        _PFCheckpointTraits<decltype(_PFUdtRecord_ScoreSnapshot::trend_c)>::restore(value.trend_c, snapshot._pf_field_1);
        _PFCheckpointTraits<decltype(_PFUdtRecord_ScoreSnapshot::bb_c)>::restore(value.bb_c, snapshot._pf_field_2);
    }
};

template <>
struct _PFCheckpointTraits<_PFUdtArena<ScoreSnapshot, _PFUdtRecord_ScoreSnapshot>> {
    using arena_type = _PFUdtArena<ScoreSnapshot, _PFUdtRecord_ScoreSnapshot>;
    using snapshot_type = typename arena_type::Snapshot;
    static snapshot_type take(arena_type& value) {
        return value.snapshot();
    }
    static void restore(arena_type& value, const snapshot_type& snapshot) {
        value.restore(snapshot);
    }
};

template <>
struct _PFCheckpointTraits<_PFUdtRecord_VolContext> {
    struct snapshot_type {
        _PFCheckpointTraits<decltype(_PFUdtRecord_VolContext::atr)>::snapshot_type _pf_field_0;
        _PFCheckpointTraits<decltype(_PFUdtRecord_VolContext::atr_ma)>::snapshot_type _pf_field_1;
        _PFCheckpointTraits<decltype(_PFUdtRecord_VolContext::ratio)>::snapshot_type _pf_field_2;
    };
    static snapshot_type take(const _PFUdtRecord_VolContext& value) {
        return snapshot_type{
            _PFCheckpointTraits<decltype(_PFUdtRecord_VolContext::atr)>::take(value.atr),
            _PFCheckpointTraits<decltype(_PFUdtRecord_VolContext::atr_ma)>::take(value.atr_ma),
            _PFCheckpointTraits<decltype(_PFUdtRecord_VolContext::ratio)>::take(value.ratio),
        };
    }
    static void restore(_PFUdtRecord_VolContext& value, const snapshot_type& snapshot) {
        _PFCheckpointTraits<decltype(_PFUdtRecord_VolContext::atr)>::restore(value.atr, snapshot._pf_field_0);
        _PFCheckpointTraits<decltype(_PFUdtRecord_VolContext::atr_ma)>::restore(value.atr_ma, snapshot._pf_field_1);
        _PFCheckpointTraits<decltype(_PFUdtRecord_VolContext::ratio)>::restore(value.ratio, snapshot._pf_field_2);
    }
};

template <>
struct _PFCheckpointTraits<_PFUdtArena<VolContext, _PFUdtRecord_VolContext>> {
    using arena_type = _PFUdtArena<VolContext, _PFUdtRecord_VolContext>;
    using snapshot_type = typename arena_type::Snapshot;
    static snapshot_type take(arena_type& value) {
        return value.snapshot();
    }
    static void restore(arena_type& value, const snapshot_type& snapshot) {
        value.restore(snapshot);
    }
};

template <>
struct _PFCheckpointTraits<_PFUdtRecord_OhlcBar> {
    struct snapshot_type {
        _PFCheckpointTraits<decltype(_PFUdtRecord_OhlcBar::o)>::snapshot_type _pf_field_0;
        _PFCheckpointTraits<decltype(_PFUdtRecord_OhlcBar::h)>::snapshot_type _pf_field_1;
        _PFCheckpointTraits<decltype(_PFUdtRecord_OhlcBar::l)>::snapshot_type _pf_field_2;
        _PFCheckpointTraits<decltype(_PFUdtRecord_OhlcBar::c)>::snapshot_type _pf_field_3;
    };
    static snapshot_type take(const _PFUdtRecord_OhlcBar& value) {
        return snapshot_type{
            _PFCheckpointTraits<decltype(_PFUdtRecord_OhlcBar::o)>::take(value.o),
            _PFCheckpointTraits<decltype(_PFUdtRecord_OhlcBar::h)>::take(value.h),
            _PFCheckpointTraits<decltype(_PFUdtRecord_OhlcBar::l)>::take(value.l),
            _PFCheckpointTraits<decltype(_PFUdtRecord_OhlcBar::c)>::take(value.c),
        };
    }
    static void restore(_PFUdtRecord_OhlcBar& value, const snapshot_type& snapshot) {
        _PFCheckpointTraits<decltype(_PFUdtRecord_OhlcBar::o)>::restore(value.o, snapshot._pf_field_0);
        _PFCheckpointTraits<decltype(_PFUdtRecord_OhlcBar::h)>::restore(value.h, snapshot._pf_field_1);
        _PFCheckpointTraits<decltype(_PFUdtRecord_OhlcBar::l)>::restore(value.l, snapshot._pf_field_2);
        _PFCheckpointTraits<decltype(_PFUdtRecord_OhlcBar::c)>::restore(value.c, snapshot._pf_field_3);
    }
};

template <>
struct _PFCheckpointTraits<_PFUdtArena<OhlcBar, _PFUdtRecord_OhlcBar>> {
    using arena_type = _PFUdtArena<OhlcBar, _PFUdtRecord_OhlcBar>;
    using snapshot_type = typename arena_type::Snapshot;
    static snapshot_type take(arena_type& value) {
        return value.snapshot();
    }
    static void restore(arena_type& value, const snapshot_type& snapshot) {
        value.restore(snapshot);
    }
};

template <>
struct _PFCheckpointTraits<_PFUdtRecord_SessionScratch> {
    struct snapshot_type {
        _PFCheckpointTraits<decltype(_PFUdtRecord_SessionScratch::in_session)>::snapshot_type _pf_field_0;
        _PFCheckpointTraits<decltype(_PFUdtRecord_SessionScratch::bar_streak)>::snapshot_type _pf_field_1;
    };
    static snapshot_type take(const _PFUdtRecord_SessionScratch& value) {
        return snapshot_type{
            _PFCheckpointTraits<decltype(_PFUdtRecord_SessionScratch::in_session)>::take(value.in_session),
            _PFCheckpointTraits<decltype(_PFUdtRecord_SessionScratch::bar_streak)>::take(value.bar_streak),
        };
    }
    static void restore(_PFUdtRecord_SessionScratch& value, const snapshot_type& snapshot) {
        _PFCheckpointTraits<decltype(_PFUdtRecord_SessionScratch::in_session)>::restore(value.in_session, snapshot._pf_field_0);
        _PFCheckpointTraits<decltype(_PFUdtRecord_SessionScratch::bar_streak)>::restore(value.bar_streak, snapshot._pf_field_1);
    }
};

template <>
struct _PFCheckpointTraits<_PFUdtArena<SessionScratch, _PFUdtRecord_SessionScratch>> {
    using arena_type = _PFUdtArena<SessionScratch, _PFUdtRecord_SessionScratch>;
    using snapshot_type = typename arena_type::Snapshot;
    static snapshot_type take(arena_type& value) {
        return value.snapshot();
    }
    static void restore(arena_type& value, const snapshot_type& snapshot) {
        value.restore(snapshot);
    }
};

template <>
struct _PFCheckpointTraits<_PFUdtRecord_GateState> {
    struct snapshot_type {
        _PFCheckpointTraits<decltype(_PFUdtRecord_GateState::vol_ok)>::snapshot_type _pf_field_0;
        _PFCheckpointTraits<decltype(_PFUdtRecord_GateState::map_ok)>::snapshot_type _pf_field_1;
    };
    static snapshot_type take(const _PFUdtRecord_GateState& value) {
        return snapshot_type{
            _PFCheckpointTraits<decltype(_PFUdtRecord_GateState::vol_ok)>::take(value.vol_ok),
            _PFCheckpointTraits<decltype(_PFUdtRecord_GateState::map_ok)>::take(value.map_ok),
        };
    }
    static void restore(_PFUdtRecord_GateState& value, const snapshot_type& snapshot) {
        _PFCheckpointTraits<decltype(_PFUdtRecord_GateState::vol_ok)>::restore(value.vol_ok, snapshot._pf_field_0);
        _PFCheckpointTraits<decltype(_PFUdtRecord_GateState::map_ok)>::restore(value.map_ok, snapshot._pf_field_1);
    }
};

template <>
struct _PFCheckpointTraits<_PFUdtArena<GateState, _PFUdtRecord_GateState>> {
    using arena_type = _PFUdtArena<GateState, _PFUdtRecord_GateState>;
    using snapshot_type = typename arena_type::Snapshot;
    static snapshot_type take(arena_type& value) {
        return value.snapshot();
    }
    static void restore(arena_type& value, const snapshot_type& snapshot) {
        value.restore(snapshot);
    }
};

const int Aggression_conservative = 0;
const int Aggression_balanced = 1;
const int Aggression_aggressive = 2;
static const std::string Aggression_str_values[] = {std::string("Conservative"), std::string("Balanced"), std::string("Aggressive")};

class GeneratedStrategy : public pineforge::source::PineStrategyHost {
public:
    _PFUdtUndoCoordinator _pf_udt_undo;
    _PFUdtArena<LayerInputs, _PFUdtRecord_LayerInputs> _pf_udt_LayerInputs{&_pf_udt_undo};
    _PFUdtArena<RsiLayer, _PFUdtRecord_RsiLayer> _pf_udt_RsiLayer{&_pf_udt_undo};
    _PFUdtArena<TrendLayer, _PFUdtRecord_TrendLayer> _pf_udt_TrendLayer{&_pf_udt_undo};
    _PFUdtArena<BbLayer, _PFUdtRecord_BbLayer> _pf_udt_BbLayer{&_pf_udt_undo};
    _PFUdtArena<ScoreSnapshot, _PFUdtRecord_ScoreSnapshot> _pf_udt_ScoreSnapshot{&_pf_udt_undo};
    _PFUdtArena<VolContext, _PFUdtRecord_VolContext> _pf_udt_VolContext{&_pf_udt_undo};
    _PFUdtArena<OhlcBar, _PFUdtRecord_OhlcBar> _pf_udt_OhlcBar{&_pf_udt_undo};
    _PFUdtArena<SessionScratch, _PFUdtRecord_SessionScratch> _pf_udt_SessionScratch{&_pf_udt_undo};
    _PFUdtArena<GateState, _PFUdtRecord_GateState> _pf_udt_GateState{&_pf_udt_undo};
    ta::RSI _ta_rsi_1;
    std::vector<double> _precalc__ta_rsi_1;
    ta::EMA _ta_ema_2;
    std::vector<double> _precalc__ta_ema_2;
    ta::BB _ta_bb_3;
    std::vector<ta::BBResult> _precalc__ta_bb_3;
    ta::ATR _ta_atr_4;
    std::vector<double> _precalc__ta_atr_4;
    ta::SMA _ta_sma_5;
    ta::Crossover _ta_crossover_6;
    bool _use_precalc = false;
    Series<double> _s_close;
    LayerInputs cfg;
    double thr_long;
    double thr_short;
    PineMap<std::string, double> gateMap;
    RsiLayer rsiL;
    TrendLayer trL;
    BbLayer bbL;
    ScoreSnapshot snap;
    VolContext vol;
    OhlcBar curBar;
    SessionScratch sess;
    GateState gates;
    int prevScore;
    int rsiLen = 0;
    int maLen = 0;
    int bbLen = 0;
    double bbMult = 0.0;
    double profile = 0.0;
    double rsiVal = 0.0;
    double emaVal = 0.0;
    double bbMid = 0.0;
    double bbUpper = 0.0;
    double bbLower = 0.0;
    double atrVal = 0.0;
    double atrMa = 0.0;
    double atrRatio = 0.0;
    double score = 0.0;
    bool longCond = false;
    bool shortCond = false;
    double exitL = 0.0;
    double exitS = 0.0;
    bool _var_initialized = false;
    bool _ta_initialized_ = false;
    bool _inputs_initialized_ = false;

    struct _PFScriptState {
        _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_undo)>::snapshot_type _pf_value_0;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_LayerInputs)>::snapshot_type _pf_value_1;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_RsiLayer)>::snapshot_type _pf_value_2;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_TrendLayer)>::snapshot_type _pf_value_3;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_BbLayer)>::snapshot_type _pf_value_4;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_ScoreSnapshot)>::snapshot_type _pf_value_5;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_VolContext)>::snapshot_type _pf_value_6;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_OhlcBar)>::snapshot_type _pf_value_7;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_SessionScratch)>::snapshot_type _pf_value_8;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_GateState)>::snapshot_type _pf_value_9;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_rsi_1)>::snapshot_type _pf_value_10;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_ema_2)>::snapshot_type _pf_value_11;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_bb_3)>::snapshot_type _pf_value_12;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_atr_4)>::snapshot_type _pf_value_13;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_sma_5)>::snapshot_type _pf_value_14;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_crossover_6)>::snapshot_type _pf_value_15;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_s_close)>::snapshot_type _pf_value_16;
        _PFCheckpointTraits<decltype(GeneratedStrategy::cfg)>::snapshot_type _pf_value_17;
        _PFCheckpointTraits<decltype(GeneratedStrategy::thr_long)>::snapshot_type _pf_value_18;
        _PFCheckpointTraits<decltype(GeneratedStrategy::thr_short)>::snapshot_type _pf_value_19;
        _PFCheckpointTraits<decltype(GeneratedStrategy::gateMap)>::snapshot_type _pf_value_20;
        _PFCheckpointTraits<decltype(GeneratedStrategy::rsiL)>::snapshot_type _pf_value_21;
        _PFCheckpointTraits<decltype(GeneratedStrategy::trL)>::snapshot_type _pf_value_22;
        _PFCheckpointTraits<decltype(GeneratedStrategy::bbL)>::snapshot_type _pf_value_23;
        _PFCheckpointTraits<decltype(GeneratedStrategy::snap)>::snapshot_type _pf_value_24;
        _PFCheckpointTraits<decltype(GeneratedStrategy::vol)>::snapshot_type _pf_value_25;
        _PFCheckpointTraits<decltype(GeneratedStrategy::curBar)>::snapshot_type _pf_value_26;
        _PFCheckpointTraits<decltype(GeneratedStrategy::sess)>::snapshot_type _pf_value_27;
        _PFCheckpointTraits<decltype(GeneratedStrategy::gates)>::snapshot_type _pf_value_28;
        _PFCheckpointTraits<decltype(GeneratedStrategy::prevScore)>::snapshot_type _pf_value_29;
        _PFCheckpointTraits<decltype(GeneratedStrategy::rsiLen)>::snapshot_type _pf_value_30;
        _PFCheckpointTraits<decltype(GeneratedStrategy::maLen)>::snapshot_type _pf_value_31;
        _PFCheckpointTraits<decltype(GeneratedStrategy::bbLen)>::snapshot_type _pf_value_32;
        _PFCheckpointTraits<decltype(GeneratedStrategy::bbMult)>::snapshot_type _pf_value_33;
        _PFCheckpointTraits<decltype(GeneratedStrategy::profile)>::snapshot_type _pf_value_34;
        _PFCheckpointTraits<decltype(GeneratedStrategy::rsiVal)>::snapshot_type _pf_value_35;
        _PFCheckpointTraits<decltype(GeneratedStrategy::emaVal)>::snapshot_type _pf_value_36;
        _PFCheckpointTraits<decltype(GeneratedStrategy::bbMid)>::snapshot_type _pf_value_37;
        _PFCheckpointTraits<decltype(GeneratedStrategy::bbUpper)>::snapshot_type _pf_value_38;
        _PFCheckpointTraits<decltype(GeneratedStrategy::bbLower)>::snapshot_type _pf_value_39;
        _PFCheckpointTraits<decltype(GeneratedStrategy::atrVal)>::snapshot_type _pf_value_40;
        _PFCheckpointTraits<decltype(GeneratedStrategy::atrMa)>::snapshot_type _pf_value_41;
        _PFCheckpointTraits<decltype(GeneratedStrategy::atrRatio)>::snapshot_type _pf_value_42;
        _PFCheckpointTraits<decltype(GeneratedStrategy::score)>::snapshot_type _pf_value_43;
        _PFCheckpointTraits<decltype(GeneratedStrategy::longCond)>::snapshot_type _pf_value_44;
        _PFCheckpointTraits<decltype(GeneratedStrategy::shortCond)>::snapshot_type _pf_value_45;
        _PFCheckpointTraits<decltype(GeneratedStrategy::exitL)>::snapshot_type _pf_value_46;
        _PFCheckpointTraits<decltype(GeneratedStrategy::exitS)>::snapshot_type _pf_value_47;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_var_initialized)>::snapshot_type _pf_value_48;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_initialized_)>::snapshot_type _pf_value_49;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_inputs_initialized_)>::snapshot_type _pf_value_50;
    };
    static_assert(std::is_copy_constructible_v<_PFScriptState>, "generated Pine state must be deep-copy constructible");
    static_assert(std::is_copy_assignable_v<_PFScriptState>, "generated Pine state must be deep-copy assignable");
    std::optional<_PFScriptState> _pf_script_state_checkpoint_;

    void snapshot_script_state() override {
        _pf_script_state_checkpoint_.emplace(_PFScriptState{
            _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_undo)>::take(_pf_udt_undo),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_LayerInputs)>::take(_pf_udt_LayerInputs),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_RsiLayer)>::take(_pf_udt_RsiLayer),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_TrendLayer)>::take(_pf_udt_TrendLayer),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_BbLayer)>::take(_pf_udt_BbLayer),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_ScoreSnapshot)>::take(_pf_udt_ScoreSnapshot),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_VolContext)>::take(_pf_udt_VolContext),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_OhlcBar)>::take(_pf_udt_OhlcBar),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_SessionScratch)>::take(_pf_udt_SessionScratch),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_GateState)>::take(_pf_udt_GateState),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_rsi_1)>::take(_ta_rsi_1),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_ema_2)>::take(_ta_ema_2),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_bb_3)>::take(_ta_bb_3),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_atr_4)>::take(_ta_atr_4),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_sma_5)>::take(_ta_sma_5),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_crossover_6)>::take(_ta_crossover_6),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_s_close)>::take(_s_close),
            _PFCheckpointTraits<decltype(GeneratedStrategy::cfg)>::take(cfg),
            _PFCheckpointTraits<decltype(GeneratedStrategy::thr_long)>::take(thr_long),
            _PFCheckpointTraits<decltype(GeneratedStrategy::thr_short)>::take(thr_short),
            _PFCheckpointTraits<decltype(GeneratedStrategy::gateMap)>::take(gateMap),
            _PFCheckpointTraits<decltype(GeneratedStrategy::rsiL)>::take(rsiL),
            _PFCheckpointTraits<decltype(GeneratedStrategy::trL)>::take(trL),
            _PFCheckpointTraits<decltype(GeneratedStrategy::bbL)>::take(bbL),
            _PFCheckpointTraits<decltype(GeneratedStrategy::snap)>::take(snap),
            _PFCheckpointTraits<decltype(GeneratedStrategy::vol)>::take(vol),
            _PFCheckpointTraits<decltype(GeneratedStrategy::curBar)>::take(curBar),
            _PFCheckpointTraits<decltype(GeneratedStrategy::sess)>::take(sess),
            _PFCheckpointTraits<decltype(GeneratedStrategy::gates)>::take(gates),
            _PFCheckpointTraits<decltype(GeneratedStrategy::prevScore)>::take(prevScore),
            _PFCheckpointTraits<decltype(GeneratedStrategy::rsiLen)>::take(rsiLen),
            _PFCheckpointTraits<decltype(GeneratedStrategy::maLen)>::take(maLen),
            _PFCheckpointTraits<decltype(GeneratedStrategy::bbLen)>::take(bbLen),
            _PFCheckpointTraits<decltype(GeneratedStrategy::bbMult)>::take(bbMult),
            _PFCheckpointTraits<decltype(GeneratedStrategy::profile)>::take(profile),
            _PFCheckpointTraits<decltype(GeneratedStrategy::rsiVal)>::take(rsiVal),
            _PFCheckpointTraits<decltype(GeneratedStrategy::emaVal)>::take(emaVal),
            _PFCheckpointTraits<decltype(GeneratedStrategy::bbMid)>::take(bbMid),
            _PFCheckpointTraits<decltype(GeneratedStrategy::bbUpper)>::take(bbUpper),
            _PFCheckpointTraits<decltype(GeneratedStrategy::bbLower)>::take(bbLower),
            _PFCheckpointTraits<decltype(GeneratedStrategy::atrVal)>::take(atrVal),
            _PFCheckpointTraits<decltype(GeneratedStrategy::atrMa)>::take(atrMa),
            _PFCheckpointTraits<decltype(GeneratedStrategy::atrRatio)>::take(atrRatio),
            _PFCheckpointTraits<decltype(GeneratedStrategy::score)>::take(score),
            _PFCheckpointTraits<decltype(GeneratedStrategy::longCond)>::take(longCond),
            _PFCheckpointTraits<decltype(GeneratedStrategy::shortCond)>::take(shortCond),
            _PFCheckpointTraits<decltype(GeneratedStrategy::exitL)>::take(exitL),
            _PFCheckpointTraits<decltype(GeneratedStrategy::exitS)>::take(exitS),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_var_initialized)>::take(_var_initialized),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_initialized_)>::take(_ta_initialized_),
            _PFCheckpointTraits<decltype(GeneratedStrategy::_inputs_initialized_)>::take(_inputs_initialized_),
        });
    }

    void restore_script_state() override {
        if (!_pf_script_state_checkpoint_) return;
        _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_undo)>::restore(this->_pf_udt_undo, _pf_script_state_checkpoint_->_pf_value_0);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_LayerInputs)>::restore(this->_pf_udt_LayerInputs, _pf_script_state_checkpoint_->_pf_value_1);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_RsiLayer)>::restore(this->_pf_udt_RsiLayer, _pf_script_state_checkpoint_->_pf_value_2);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_TrendLayer)>::restore(this->_pf_udt_TrendLayer, _pf_script_state_checkpoint_->_pf_value_3);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_BbLayer)>::restore(this->_pf_udt_BbLayer, _pf_script_state_checkpoint_->_pf_value_4);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_ScoreSnapshot)>::restore(this->_pf_udt_ScoreSnapshot, _pf_script_state_checkpoint_->_pf_value_5);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_VolContext)>::restore(this->_pf_udt_VolContext, _pf_script_state_checkpoint_->_pf_value_6);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_OhlcBar)>::restore(this->_pf_udt_OhlcBar, _pf_script_state_checkpoint_->_pf_value_7);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_SessionScratch)>::restore(this->_pf_udt_SessionScratch, _pf_script_state_checkpoint_->_pf_value_8);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_pf_udt_GateState)>::restore(this->_pf_udt_GateState, _pf_script_state_checkpoint_->_pf_value_9);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_rsi_1)>::restore(this->_ta_rsi_1, _pf_script_state_checkpoint_->_pf_value_10);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_ema_2)>::restore(this->_ta_ema_2, _pf_script_state_checkpoint_->_pf_value_11);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_bb_3)>::restore(this->_ta_bb_3, _pf_script_state_checkpoint_->_pf_value_12);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_atr_4)>::restore(this->_ta_atr_4, _pf_script_state_checkpoint_->_pf_value_13);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_sma_5)>::restore(this->_ta_sma_5, _pf_script_state_checkpoint_->_pf_value_14);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_crossover_6)>::restore(this->_ta_crossover_6, _pf_script_state_checkpoint_->_pf_value_15);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_s_close)>::restore(this->_s_close, _pf_script_state_checkpoint_->_pf_value_16);
        _PFCheckpointTraits<decltype(GeneratedStrategy::cfg)>::restore(this->cfg, _pf_script_state_checkpoint_->_pf_value_17);
        _PFCheckpointTraits<decltype(GeneratedStrategy::thr_long)>::restore(this->thr_long, _pf_script_state_checkpoint_->_pf_value_18);
        _PFCheckpointTraits<decltype(GeneratedStrategy::thr_short)>::restore(this->thr_short, _pf_script_state_checkpoint_->_pf_value_19);
        _PFCheckpointTraits<decltype(GeneratedStrategy::gateMap)>::restore(this->gateMap, _pf_script_state_checkpoint_->_pf_value_20);
        _PFCheckpointTraits<decltype(GeneratedStrategy::rsiL)>::restore(this->rsiL, _pf_script_state_checkpoint_->_pf_value_21);
        _PFCheckpointTraits<decltype(GeneratedStrategy::trL)>::restore(this->trL, _pf_script_state_checkpoint_->_pf_value_22);
        _PFCheckpointTraits<decltype(GeneratedStrategy::bbL)>::restore(this->bbL, _pf_script_state_checkpoint_->_pf_value_23);
        _PFCheckpointTraits<decltype(GeneratedStrategy::snap)>::restore(this->snap, _pf_script_state_checkpoint_->_pf_value_24);
        _PFCheckpointTraits<decltype(GeneratedStrategy::vol)>::restore(this->vol, _pf_script_state_checkpoint_->_pf_value_25);
        _PFCheckpointTraits<decltype(GeneratedStrategy::curBar)>::restore(this->curBar, _pf_script_state_checkpoint_->_pf_value_26);
        _PFCheckpointTraits<decltype(GeneratedStrategy::sess)>::restore(this->sess, _pf_script_state_checkpoint_->_pf_value_27);
        _PFCheckpointTraits<decltype(GeneratedStrategy::gates)>::restore(this->gates, _pf_script_state_checkpoint_->_pf_value_28);
        _PFCheckpointTraits<decltype(GeneratedStrategy::prevScore)>::restore(this->prevScore, _pf_script_state_checkpoint_->_pf_value_29);
        _PFCheckpointTraits<decltype(GeneratedStrategy::rsiLen)>::restore(this->rsiLen, _pf_script_state_checkpoint_->_pf_value_30);
        _PFCheckpointTraits<decltype(GeneratedStrategy::maLen)>::restore(this->maLen, _pf_script_state_checkpoint_->_pf_value_31);
        _PFCheckpointTraits<decltype(GeneratedStrategy::bbLen)>::restore(this->bbLen, _pf_script_state_checkpoint_->_pf_value_32);
        _PFCheckpointTraits<decltype(GeneratedStrategy::bbMult)>::restore(this->bbMult, _pf_script_state_checkpoint_->_pf_value_33);
        _PFCheckpointTraits<decltype(GeneratedStrategy::profile)>::restore(this->profile, _pf_script_state_checkpoint_->_pf_value_34);
        _PFCheckpointTraits<decltype(GeneratedStrategy::rsiVal)>::restore(this->rsiVal, _pf_script_state_checkpoint_->_pf_value_35);
        _PFCheckpointTraits<decltype(GeneratedStrategy::emaVal)>::restore(this->emaVal, _pf_script_state_checkpoint_->_pf_value_36);
        _PFCheckpointTraits<decltype(GeneratedStrategy::bbMid)>::restore(this->bbMid, _pf_script_state_checkpoint_->_pf_value_37);
        _PFCheckpointTraits<decltype(GeneratedStrategy::bbUpper)>::restore(this->bbUpper, _pf_script_state_checkpoint_->_pf_value_38);
        _PFCheckpointTraits<decltype(GeneratedStrategy::bbLower)>::restore(this->bbLower, _pf_script_state_checkpoint_->_pf_value_39);
        _PFCheckpointTraits<decltype(GeneratedStrategy::atrVal)>::restore(this->atrVal, _pf_script_state_checkpoint_->_pf_value_40);
        _PFCheckpointTraits<decltype(GeneratedStrategy::atrMa)>::restore(this->atrMa, _pf_script_state_checkpoint_->_pf_value_41);
        _PFCheckpointTraits<decltype(GeneratedStrategy::atrRatio)>::restore(this->atrRatio, _pf_script_state_checkpoint_->_pf_value_42);
        _PFCheckpointTraits<decltype(GeneratedStrategy::score)>::restore(this->score, _pf_script_state_checkpoint_->_pf_value_43);
        _PFCheckpointTraits<decltype(GeneratedStrategy::longCond)>::restore(this->longCond, _pf_script_state_checkpoint_->_pf_value_44);
        _PFCheckpointTraits<decltype(GeneratedStrategy::shortCond)>::restore(this->shortCond, _pf_script_state_checkpoint_->_pf_value_45);
        _PFCheckpointTraits<decltype(GeneratedStrategy::exitL)>::restore(this->exitL, _pf_script_state_checkpoint_->_pf_value_46);
        _PFCheckpointTraits<decltype(GeneratedStrategy::exitS)>::restore(this->exitS, _pf_script_state_checkpoint_->_pf_value_47);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_var_initialized)>::restore(this->_var_initialized, _pf_script_state_checkpoint_->_pf_value_48);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_ta_initialized_)>::restore(this->_ta_initialized_, _pf_script_state_checkpoint_->_pf_value_49);
        _PFCheckpointTraits<decltype(GeneratedStrategy::_inputs_initialized_)>::restore(this->_inputs_initialized_, _pf_script_state_checkpoint_->_pf_value_50);
    }

    void commit_script_state() override {
        snapshot_script_state();
    }

    explicit GeneratedStrategy() : _ta_rsi_1(14), _ta_ema_2(20), _ta_bb_3(20, 2.0), _ta_atr_4(14), _ta_sma_5(20), thr_long(2), thr_short(-2), prevScore(na<int>()) {
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
        this->_pf_udt_undo.reset_for_run();
        this->_pf_udt_LayerInputs.reset_for_run();
        this->_pf_udt_RsiLayer.reset_for_run();
        this->_pf_udt_TrendLayer.reset_for_run();
        this->_pf_udt_BbLayer.reset_for_run();
        this->_pf_udt_ScoreSnapshot.reset_for_run();
        this->_pf_udt_VolContext.reset_for_run();
        this->_pf_udt_OhlcBar.reset_for_run();
        this->_pf_udt_SessionScratch.reset_for_run();
        this->_pf_udt_GateState.reset_for_run();
        this->_ta_rsi_1 = decltype(this->_ta_rsi_1)(14);
        this->_precalc__ta_rsi_1 = decltype(this->_precalc__ta_rsi_1){};
        this->_ta_ema_2 = decltype(this->_ta_ema_2)(20);
        this->_precalc__ta_ema_2 = decltype(this->_precalc__ta_ema_2){};
        this->_ta_bb_3 = decltype(this->_ta_bb_3)(20, 2.0);
        this->_precalc__ta_bb_3 = decltype(this->_precalc__ta_bb_3){};
        this->_ta_atr_4 = decltype(this->_ta_atr_4)(14);
        this->_precalc__ta_atr_4 = decltype(this->_precalc__ta_atr_4){};
        this->_ta_sma_5 = decltype(this->_ta_sma_5)(20);
        this->_ta_crossover_6 = decltype(this->_ta_crossover_6){};
        this->_use_precalc = false;
        this->_s_close = decltype(this->_s_close){};
        this->cfg = decltype(this->cfg){};
        this->thr_long = decltype(this->thr_long)(2);
        this->thr_short = decltype(this->thr_short)(-2);
        this->gateMap = decltype(this->gateMap){};
        this->rsiL = decltype(this->rsiL){};
        this->trL = decltype(this->trL){};
        this->bbL = decltype(this->bbL){};
        this->snap = decltype(this->snap){};
        this->vol = decltype(this->vol){};
        this->curBar = decltype(this->curBar){};
        this->sess = decltype(this->sess){};
        this->gates = decltype(this->gates){};
        this->prevScore = decltype(this->prevScore)(na<int>());
        this->rsiLen = 0;
        this->maLen = 0;
        this->bbLen = 0;
        this->bbMult = 0.0;
        this->profile = 0.0;
        this->rsiVal = 0.0;
        this->emaVal = 0.0;
        this->bbMid = 0.0;
        this->bbUpper = 0.0;
        this->bbLower = 0.0;
        this->atrVal = 0.0;
        this->atrMa = 0.0;
        this->atrRatio = 0.0;
        this->score = 0.0;
        this->longCond = false;
        this->shortCond = false;
        this->exitL = 0.0;
        this->exitS = 0.0;
        this->_var_initialized = false;
        this->_ta_initialized_ = false;
        this->_inputs_initialized_ = false;
        if (allow_precalculation) precalculate(bars, n);
    }

    void on_source_bar(const Bar& bar) override {
        if (history_advances_new_bar()) _s_close.push(current_bar_.close);
        else _s_close.update(current_bar_.close);
        if (!_var_initialized) {
            cfg = _pf_udt_LayerInputs.create(_PFUdtRecord_LayerInputs{.rsi_len = (int64_t)(rsiLen), .ma_len = (int64_t)(maLen), .bb_len = (int64_t)(bbLen), .bb_mult = bbMult});
            gateMap = PineMap<std::string, double>::new_();
            rsiL = _pf_udt_RsiLayer.create(_PFUdtRecord_RsiLayer{.value = na<double>(), .contrib = (int64_t)(0)});
            trL = _pf_udt_TrendLayer.create(_PFUdtRecord_TrendLayer{.ema = na<double>(), .contrib = (int64_t)(0)});
            bbL = _pf_udt_BbLayer.create(_PFUdtRecord_BbLayer{.mid = na<double>(), .upper = na<double>(), .lower = na<double>(), .contrib = (int64_t)(0)});
            snap = _pf_udt_ScoreSnapshot.create(_PFUdtRecord_ScoreSnapshot{.rsi_c = (int64_t)(0), .trend_c = (int64_t)(0), .bb_c = (int64_t)(0)});
            vol = _pf_udt_VolContext.create(_PFUdtRecord_VolContext{.atr = na<double>(), .atr_ma = na<double>(), .ratio = 1.0});
            curBar = _pf_udt_OhlcBar.create(_PFUdtRecord_OhlcBar{.o = na<double>(), .h = na<double>(), .l = na<double>(), .c = na<double>()});
            sess = _pf_udt_SessionScratch.create(_PFUdtRecord_SessionScratch{.in_session = true, .bar_streak = (int64_t)(0)});
            gates = _pf_udt_GateState.create(_PFUdtRecord_GateState{.vol_ok = true, .map_ok = true});
            _var_initialized = true;
        } else {
        }
        if (!_inputs_initialized_) {
            rsiLen = get_input_int("RSI Length", 14);
            maLen = get_input_int("MA Length", 20);
            bbLen = get_input_int("BB Length", 20);
            bbMult = get_input_double("BB Mult", 2.0);
            profile = get_input_int("Stress profile", Aggression_balanced);
            _inputs_initialized_ = true;
        }
        if (!_ta_initialized_) {
            _ta_rsi_1 = ta::RSI(get_input_int("RSI Length", 14));
            _ta_ema_2 = ta::EMA(get_input_int("MA Length", 20));
            _ta_bb_3 = ta::BB(get_input_int("BB Length", 20), get_input_double("BB Mult", 2.0));
            _ta_initialized_ = true;
        }
        if ((bar_index_ == 0)) {
            [&](auto&& __pf_map_receiver_0)->decltype(auto){ return [&](auto&& __pf_map_param_arg_0)->decltype(auto){ return [&](auto&& __pf_map_param_arg_1)->decltype(auto){ return __pf_map_receiver_0.put(__pf_map_param_arg_0, __pf_map_param_arg_1); }((2.0)); }((std::string("long"))); }((gateMap));
            [&](auto&& __pf_map_receiver_1)->decltype(auto){ return [&](auto&& __pf_map_param_arg_2)->decltype(auto){ return [&](auto&& __pf_map_param_arg_3)->decltype(auto){ return __pf_map_receiver_1.put(__pf_map_param_arg_2, __pf_map_param_arg_3); }(((-2.0))); }((std::string("short"))); }((gateMap));
            [&](auto&& __pf_map_receiver_2)->decltype(auto){ return [&](auto&& __pf_map_param_arg_4)->decltype(auto){ return [&](auto&& __pf_map_param_arg_5)->decltype(auto){ return __pf_map_receiver_2.put(__pf_map_param_arg_4, __pf_map_param_arg_5); }((0.0)); }((std::string("exit_long"))); }((gateMap));
            [&](auto&& __pf_map_receiver_3)->decltype(auto){ return [&](auto&& __pf_map_param_arg_6)->decltype(auto){ return [&](auto&& __pf_map_param_arg_7)->decltype(auto){ return __pf_map_receiver_3.put(__pf_map_param_arg_6, __pf_map_param_arg_7); }((0.0)); }((std::string("exit_short"))); }((gateMap));
        }
        auto __switch_val_0 = profile;
        if (__switch_val_0 == Aggression_conservative) {
            thr_long = ([&]{ auto _nz_v = ([&](auto&& __pf_map_receiver_4)->decltype(auto){ return [&](auto&& __pf_map_param_arg_8)->decltype(auto){ return __pf_map_receiver_4.get(__pf_map_param_arg_8); }((std::string("long"))); }((gateMap))); return is_na(_nz_v) ? (2.0) : _nz_v; }());
            thr_short = ([&]{ auto _nz_v = ([&](auto&& __pf_map_receiver_5)->decltype(auto){ return [&](auto&& __pf_map_param_arg_9)->decltype(auto){ return __pf_map_receiver_5.get(__pf_map_param_arg_9); }((std::string("short"))); }((gateMap))); return is_na(_nz_v) ? ((-2.0)) : _nz_v; }());
        }
        else if (__switch_val_0 == Aggression_balanced) {
            thr_long = ([&]{ auto _nz_v = ([&](auto&& __pf_map_receiver_6)->decltype(auto){ return [&](auto&& __pf_map_param_arg_10)->decltype(auto){ return __pf_map_receiver_6.get(__pf_map_param_arg_10); }((std::string("long"))); }((gateMap))); return is_na(_nz_v) ? (2.0) : _nz_v; }());
            thr_short = ([&]{ auto _nz_v = ([&](auto&& __pf_map_receiver_7)->decltype(auto){ return [&](auto&& __pf_map_param_arg_11)->decltype(auto){ return __pf_map_receiver_7.get(__pf_map_param_arg_11); }((std::string("short"))); }((gateMap))); return is_na(_nz_v) ? ((-2.0)) : _nz_v; }());
        }
        else if (__switch_val_0 == Aggression_aggressive) {
            thr_long = ([&]{ auto _nz_v = ([&](auto&& __pf_map_receiver_8)->decltype(auto){ return [&](auto&& __pf_map_param_arg_12)->decltype(auto){ return __pf_map_receiver_8.get(__pf_map_param_arg_12); }((std::string("long"))); }((gateMap))); return is_na(_nz_v) ? (2.0) : _nz_v; }());
            thr_short = ([&]{ auto _nz_v = ([&](auto&& __pf_map_receiver_9)->decltype(auto){ return [&](auto&& __pf_map_param_arg_13)->decltype(auto){ return __pf_map_receiver_9.get(__pf_map_param_arg_13); }((std::string("short"))); }((gateMap))); return is_na(_nz_v) ? ((-2.0)) : _nz_v; }());
        }
        else {
            thr_long = ([&]{ auto _nz_v = ([&](auto&& __pf_map_receiver_10)->decltype(auto){ return [&](auto&& __pf_map_param_arg_14)->decltype(auto){ return __pf_map_receiver_10.get(__pf_map_param_arg_14); }((std::string("long"))); }((gateMap))); return is_na(_nz_v) ? (2.0) : _nz_v; }());
            thr_short = ([&]{ auto _nz_v = ([&](auto&& __pf_map_receiver_11)->decltype(auto){ return [&](auto&& __pf_map_param_arg_15)->decltype(auto){ return __pf_map_receiver_11.get(__pf_map_param_arg_15); }((std::string("short"))); }((gateMap))); return is_na(_nz_v) ? ((-2.0)) : _nz_v; }());
        }
        rsiVal = (history_advances_new_bar() ? _ta_rsi_1.compute(current_bar_.close) : _ta_rsi_1.recompute(current_bar_.close));
        emaVal = (history_advances_new_bar() ? _ta_ema_2.compute(current_bar_.close) : _ta_ema_2.recompute(current_bar_.close));
        auto _result__ta_bb_3 = (history_advances_new_bar() ? _ta_bb_3.compute(current_bar_.close) : _ta_bb_3.recompute(current_bar_.close));
        double bbMid = _result__ta_bb_3.middle;
        double bbUpper = _result__ta_bb_3.upper;
        double bbLower = _result__ta_bb_3.lower;
        atrVal = (history_advances_new_bar() ? _ta_atr_4.compute(current_bar_.high, current_bar_.low, current_bar_.close, prev_chart_close()) : _ta_atr_4.recompute(current_bar_.high, current_bar_.low, current_bar_.close, prev_chart_close()));
        atrMa = (history_advances_new_bar() ? _ta_sma_5.compute(atrVal) : _ta_sma_5.recompute(atrVal));
        atrRatio = ((([&]{ auto _pna_l = (atrMa); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) ? (((double)(atrVal) / (double)(atrMa))) : (1.0));
        _pf_udt_LayerInputs.get(cfg).rsi_len = rsiLen;
        _pf_udt_LayerInputs.get(cfg).ma_len = maLen;
        _pf_udt_LayerInputs.get(cfg).bb_len = bbLen;
        _pf_udt_LayerInputs.get(cfg).bb_mult = bbMult;
        _pf_udt_RsiLayer.get(rsiL).value = rsiVal;
        _pf_udt_RsiLayer.get(rsiL).contrib = ((([&]{ auto _pna_l = (rsiVal); auto _pna_r = (50); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) ? (1) : (((([&]{ auto _pna_l = (rsiVal); auto _pna_r = (50); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }())) ? ((-1)) : (0))));
        _pf_udt_TrendLayer.get(trL).ema = emaVal;
        _pf_udt_TrendLayer.get(trL).contrib = ((([&]{ auto _pna_l = (current_bar_.close); auto _pna_r = (emaVal); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) ? (1) : ((-1)));
        _pf_udt_BbLayer.get(bbL).mid = bbMid;
        _pf_udt_BbLayer.get(bbL).upper = bbUpper;
        _pf_udt_BbLayer.get(bbL).lower = bbLower;
        _pf_udt_BbLayer.get(bbL).contrib = ((([&]{ auto _pna_l = (current_bar_.close); auto _pna_r = (bbMid); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }())) ? (1) : ((-1)));
        _pf_udt_VolContext.get(vol).atr = atrVal;
        _pf_udt_VolContext.get(vol).atr_ma = atrMa;
        _pf_udt_VolContext.get(vol).ratio = atrRatio;
        _pf_udt_OhlcBar.get(curBar).o = current_bar_.open;
        _pf_udt_OhlcBar.get(curBar).h = current_bar_.high;
        _pf_udt_OhlcBar.get(curBar).l = current_bar_.low;
        _pf_udt_OhlcBar.get(curBar).c = current_bar_.close;
        _pf_udt_GateState.get(gates).vol_ok = ([&]{ auto _pna_l = (_pf_udt_VolContext.read(vol).ratio); auto _pna_r = (25.0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }());
        _pf_udt_GateState.get(gates).map_ok = ([&](auto&& __pf_map_receiver_12)->decltype(auto){ return [&](auto&& __pf_map_param_arg_16)->decltype(auto){ return __pf_map_receiver_12.contains(__pf_map_param_arg_16); }((std::string("long"))); }((gateMap)) && [&](auto&& __pf_map_receiver_13)->decltype(auto){ return [&](auto&& __pf_map_param_arg_17)->decltype(auto){ return __pf_map_receiver_13.contains(__pf_map_param_arg_17); }((std::string("short"))); }((gateMap)));
        score = ((_pf_udt_RsiLayer.read(rsiL).contrib + _pf_udt_TrendLayer.read(trL).contrib) + _pf_udt_BbLayer.read(bbL).contrib);
        _pf_udt_ScoreSnapshot.get(snap).rsi_c = _pf_udt_RsiLayer.read(rsiL).contrib;
        _pf_udt_ScoreSnapshot.get(snap).trend_c = _pf_udt_TrendLayer.read(trL).contrib;
        _pf_udt_ScoreSnapshot.get(snap).bb_c = _pf_udt_BbLayer.read(bbL).contrib;
        _pf_udt_SessionScratch.get(sess).in_session = true;
        _pf_udt_SessionScratch.get(sess).bar_streak = (((([&]{ auto _pna_l = (pine_bar_index()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()) && ([&]{ auto _pna_l = (current_bar_.close); auto _pna_r = (_s_close[1]); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && (_pfc_eq); }()))) ? ((_pf_udt_SessionScratch.read(sess).bar_streak + 1)) : (0));
        longCond = ((((_pf_udt_GateState.read(gates).vol_ok && _pf_udt_GateState.read(gates).map_ok) && ([&]{ auto _pna_l = (score); auto _pna_r = (thr_long); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) || _pfc_eq); }())) && !(is_na(prevScore))) && ([&]{ auto _pna_l = (prevScore); auto _pna_r = (thr_long); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }()));
        shortCond = ((((_pf_udt_GateState.read(gates).vol_ok && _pf_udt_GateState.read(gates).map_ok) && ([&]{ auto _pna_l = (score); auto _pna_r = (thr_short); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) || _pfc_eq); }())) && !(is_na(prevScore))) && ([&]{ auto _pna_l = (prevScore); auto _pna_r = (thr_short); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()));
        if (longCond) {
            strategy_entry(std::string("Long"), true, na<double>(), na<double>(), na<double>(), "");
        }
        if (shortCond) {
            strategy_entry(std::string("Short"), false, na<double>(), na<double>(), na<double>(), "");
        }
        exitL = ([&]{ auto _nz_v = ([&](auto&& __pf_map_receiver_14)->decltype(auto){ return [&](auto&& __pf_map_param_arg_18)->decltype(auto){ return __pf_map_receiver_14.get(__pf_map_param_arg_18); }((std::string("exit_long"))); }((gateMap))); return is_na(_nz_v) ? (0.0) : _nz_v; }());
        exitS = ([&]{ auto _nz_v = ([&](auto&& __pf_map_receiver_15)->decltype(auto){ return [&](auto&& __pf_map_param_arg_19)->decltype(auto){ return __pf_map_receiver_15.get(__pf_map_param_arg_19); }((std::string("exit_short"))); }((gateMap))); return is_na(_nz_v) ? (0.0) : _nz_v; }());
        if ((([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) && !_pfc_eq); }()) && ([&]{ auto _pna_l = (score); auto _pna_r = (exitL); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) || _pfc_eq); }()))) {
            strategy_close(std::string("Long"), "", na<double>(), na<double>(), false, 755914244115ULL);
        }
        if ((([&]{ auto _pna_l = (signed_position_size()); auto _pna_r = (0); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l < _pfc_r) && !_pfc_eq); }()) && ([&]{ auto _pna_l = (score); auto _pna_r = (exitS); double _pfc_l = static_cast<double>(_pna_l); double _pfc_r = static_cast<double>(_pna_r); bool _pfc_eq = (_pfc_l == _pfc_r) || (std::isfinite(_pfc_l) && std::isfinite(_pfc_r) && std::fabs(_pfc_l - _pfc_r) <= 1e-10); return !is_na(_pna_l) && !is_na(_pna_r) && ((_pfc_l > _pfc_r) || _pfc_eq); }()))) {
            strategy_close(std::string("Short"), "", na<double>(), na<double>(), false, 764504178707ULL);
        }
        prevScore = score;
    }

    void precalculate(const Bar* bars, int n) {
        _use_precalc = false;
        if (n <= 0 || bars == nullptr) return;

        _precalc__ta_rsi_1.resize(n);
        _precalc__ta_ema_2.resize(n);
        _precalc__ta_bb_3.resize(n);
        _precalc__ta_atr_4.resize(n);

        _ta_rsi_1 = ta::RSI(14);
        _ta_ema_2 = ta::EMA(20);
        _ta_bb_3 = ta::BB(20, 2.0);
        _ta_atr_4 = ta::ATR(14);

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
            _precalc__ta_rsi_1[i] = _ta_rsi_1.compute(bars[i].close);
            _precalc__ta_ema_2[i] = _ta_ema_2.compute(bars[i].close);
            _precalc__ta_bb_3[i] = _ta_bb_3.compute(bars[i].close);
            _precalc__ta_atr_4[i] = _ta_atr_4.compute(bars[i].high, bars[i].low, bars[i].close, (i > 0 ? bars[i - 1].close : na<double>()));
        }

        _ta_rsi_1 = ta::RSI(14);
        _ta_ema_2 = ta::EMA(20);
        _ta_bb_3 = ta::BB(20, 2.0);
        _ta_atr_4 = ta::ATR(14);
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
