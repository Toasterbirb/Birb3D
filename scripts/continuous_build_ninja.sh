#!/bin/sh

######################################
# Build and run tests on file change #
######################################

JOB_COUNT="$1"
[ -z "$JOB_COUNT" ] && echo "Defaulting to $(nproc) threads" && JOB_COUNT="$(nproc)"

PROJECT_ROOT="$(dirname "$0")/.."
BUILD_DIR="$PROJECT_ROOT/build"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit 1
cmake .. -G Ninja

find "$PROJECT_ROOT/engine" "$PROJECT_ROOT/demos" "$PROJECT_ROOT/tests" | entr -sc "ninja -j $JOB_COUNT"
