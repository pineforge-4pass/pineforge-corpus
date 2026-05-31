#!/usr/bin/env bash
# Standalone build for the SPX intraday-cap special probe. It lives at
# corpus/special-validation/ (one level deep) so run_corpus.sh's corpus/*/*/
# glob and the corpus CMake build do NOT pick it up; build it here against the
# engine's static lib instead.
#
# Prereq: the engine is built (build/lib/libpineforge.a exists):
#   cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DPINEFORGE_BUILD_TESTS=ON
#   cmake --build build -j
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ENGINE="$(cd "$HERE/../.." && pwd)"
LIB="$ENGINE/build/lib/libpineforge.a"
[ -f "$LIB" ] || { echo "missing $LIB — build the engine first" >&2; exit 1; }
clang++ -std=c++17 -O2 -shared \
    -Wno-unused-but-set-variable -Wno-unused-variable \
    -Wno-unused-parameter -Wno-sign-compare \
    -I"$ENGINE/include" \
    "$HERE/generated.cpp" \
    -Wl,-force_load,"$LIB" \
    -o "$HERE/strategy.dylib"
echo "built $HERE/strategy.dylib"
