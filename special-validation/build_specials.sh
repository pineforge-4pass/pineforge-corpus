#!/usr/bin/env bash
# build_specials.sh — build + run + verify every categorized special-validation
# probe (special-validation/<category>/<probe>/). These live OUTSIDE the
# corpus/*/*/ glob so scripts/run_corpus.sh never touches them — they use
# non-default instruments (licensed OHLCV, gitignored) or document an engine
# limitation, and are reproduced on demand here.
#
# Prereq: engine built (build/lib/libpineforge.a) + each probe's local data
# (ohlcv_*.csv + tv_trades.csv) regenerated per its README (see make_feed.py).
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"   # corpus/special-validation
ENGINE="$(cd "$HERE/../.." && pwd)"                     # engine root
LIB="$ENGINE/build/lib/libpineforge.a"
EIGEN="${EIGEN3_INCLUDE_DIR:-/opt/homebrew/include/eigen3}"
[ -f "$LIB" ] || { echo "missing $LIB — build the engine first" >&2; exit 1; }
for probe in "$HERE"/*/*/; do
    [ -f "$probe/generated.cpp" ] || continue
    name="$(basename "$probe")"
    clang++ -std=c++17 -O2 -fPIC -shared \
        -I"$ENGINE/include" -I"$EIGEN" \
        "$probe/generated.cpp" -Wl,-force_load,"$LIB" \
        -o "$probe/strategy.dylib"
    if [ -f "$probe/tv_trades.csv" ]; then
        python3 "$ENGINE/scripts/run_strategy.py" "$probe" --so-name strategy.dylib >/dev/null 2>&1 || true
        printf '%-44s ' "$name"
        python3 "$ENGINE/scripts/verify_corpus.py" "$probe" 2>/dev/null | grep -oE '\-> [a-z_]+' | tail -1 || echo "(no verdict)"
    else
        printf '%-44s (no tv_trades.csv — add your TV export per README)\n' "$name"
    fi
done
