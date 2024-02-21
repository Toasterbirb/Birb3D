#!/bin/sh

PROJECT_ROOT="$(dirname "$0")/.."

SHELL_SCRIPTS="$(find "$PROJECT_ROOT/scripts" "$PROJECT_ROOT/engine" -iname "*.sh")"
for SCRIPT in $SHELL_SCRIPTS
do
	shellcheck -s sh -S style "$SCRIPT"
done

cppcheck --include="$PROJECT_ROOT/engine/core/include/Profiling.hpp" -q --enable=warning,style,performance,portability --check-level=exhaustive "$PROJECT_ROOT/engine" "$PROJECT_ROOT/tests" "$PROJECT_ROOT/demos"