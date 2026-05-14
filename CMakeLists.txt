# corpus/CMakeLists.txt — build one shared library per strategy.
#
# Gated by the top-level option PINEFORGE_BUILD_CORPUS_STRATEGIES (default
# OFF). When enabled, this iterates over every `corpus/*/*/generated.cpp`
# file and produces a corresponding `corpus/<cat>/<name>/strategy.so` (or
# `.dll` on Windows), linked statically against `libpineforge`.
#
# The .so files expose the C ABI declared in <pineforge/pineforge.h>; the
# scripts/run_strategy.py harness loads them via ctypes, pumps the
# reference OHLCV through, and writes engine_trades.csv next to each .so.
#
# The build is intentionally per-strategy: each .so is a self-contained
# unit tied to one strategy.pine. There is no cross-strategy code sharing.

file(GLOB STRATEGY_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
     ${CMAKE_CURRENT_SOURCE_DIR}/basic/*
     ${CMAKE_CURRENT_SOURCE_DIR}/community/*
     ${CMAKE_CURRENT_SOURCE_DIR}/parity-anomalies/*
     ${CMAKE_CURRENT_SOURCE_DIR}/validation/*
     ${CMAKE_CURRENT_SOURCE_DIR}/validation_barstate/*
     ${CMAKE_CURRENT_SOURCE_DIR}/validation_lower_tf/*
     ${CMAKE_CURRENT_SOURCE_DIR}/validation_magnifier/*
     ${CMAKE_CURRENT_SOURCE_DIR}/validation_na_chain/*
     ${CMAKE_CURRENT_SOURCE_DIR}/validation_oca/*
     ${CMAKE_CURRENT_SOURCE_DIR}/validation_process_orders/*
     ${CMAKE_CURRENT_SOURCE_DIR}/validation_pyramid/*
     ${CMAKE_CURRENT_SOURCE_DIR}/validation_recompute/*
     ${CMAKE_CURRENT_SOURCE_DIR}/validation_session/*
     ${CMAKE_CURRENT_SOURCE_DIR}/validation_ta_isolate/*
     ${CMAKE_CURRENT_SOURCE_DIR}/validation_typed_matrix/*
     ${CMAKE_CURRENT_SOURCE_DIR}/validation_varip/*)

set(_n_strategies 0)

foreach(STRAT_REL IN LISTS STRATEGY_DIRS)
    set(STRAT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${STRAT_REL})
    if(NOT IS_DIRECTORY ${STRAT_DIR})
        continue()
    endif()
    set(STRAT_CPP ${STRAT_DIR}/generated.cpp)
    if(NOT EXISTS ${STRAT_CPP})
        continue()
    endif()

    # Derive a unique target name from the relative path:
    #   corpus/basic/greedy → strategy_basic_greedy
    #   corpus/validation/01-macd-histogram → strategy_validation_01-macd-histogram
    string(REPLACE "/" "_" TARGET_NAME ${STRAT_REL})
    set(TARGET_NAME "strategy_${TARGET_NAME}")
    string(MAKE_C_IDENTIFIER ${TARGET_NAME} TARGET_NAME)

    add_library(${TARGET_NAME} SHARED ${STRAT_CPP})

    target_link_libraries(${TARGET_NAME} PRIVATE pineforge)

    # Force-load libpineforge so the c_abi.cpp symbols (pf_version_get,
    # strategy_set_trace_enabled) — which the codegen-emitted body never
    # references but the Python harness expects to find via dlsym — are
    # actually pulled into the final .so. macOS uses -Wl,-force_load;
    # ELF (Linux) uses -Wl,--whole-archive ... -Wl,--no-whole-archive.
    if(APPLE)
        target_link_options(${TARGET_NAME} PRIVATE
            "-Wl,-force_load,$<TARGET_FILE:pineforge>")
    elseif(UNIX)
        target_link_options(${TARGET_NAME} PRIVATE
            "-Wl,--whole-archive"
            "$<TARGET_FILE:pineforge>"
            "-Wl,--no-whole-archive")
    endif()

    # Generated .cpp files are auto-emitted; suppress a few warnings that
    # we'd never be able to fix in the codegen output:
    target_compile_options(${TARGET_NAME} PRIVATE
        -Wno-unused-but-set-variable
        -Wno-unused-variable
        -Wno-unused-parameter
        -Wno-sign-compare)

    # Place the produced .so next to its source — co-located with
    # strategy.pine, tv_trades.csv, engine_trades.csv.
    set_target_properties(${TARGET_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${STRAT_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${STRAT_DIR}
        OUTPUT_NAME strategy
        PREFIX "")

    math(EXPR _n_strategies "${_n_strategies} + 1")
endforeach()

message(STATUS "corpus: configured ${_n_strategies} strategy .so targets")

# Convenience aggregate target so users can `cmake --build build --target
# corpus_strategies` and get all of them in one go.
add_custom_target(corpus_strategies
    COMMENT "Built all corpus strategy .so files")

foreach(STRAT_REL IN LISTS STRATEGY_DIRS)
    set(STRAT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${STRAT_REL})
    if(NOT IS_DIRECTORY ${STRAT_DIR})
        continue()
    endif()
    if(NOT EXISTS ${STRAT_DIR}/generated.cpp)
        continue()
    endif()
    string(REPLACE "/" "_" TARGET_NAME ${STRAT_REL})
    set(TARGET_NAME "strategy_${TARGET_NAME}")
    string(MAKE_C_IDENTIFIER ${TARGET_NAME} TARGET_NAME)
    add_dependencies(corpus_strategies ${TARGET_NAME})
endforeach()
