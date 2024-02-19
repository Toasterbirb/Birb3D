#!/bin/sh

PROJECT_ROOT="$(dirname "$0")/.."

cppcheck --include="$PROJECT_ROOT/engine/core/include/Profiling.hpp" -q --enable=warning,style,performance,portability --check-level=exhaustive "$PROJECT_ROOT/engine" "$PROJECT_ROOT/tests" "$PROJECT_ROOT/demos"
