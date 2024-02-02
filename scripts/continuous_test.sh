#!/bin/sh

######################################
# Build and run tests on file change #
######################################

PROJECT_ROOT="$(dirname "$0")/.."
BUILD_DIR="$PROJECT_ROOT/build"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit 1
cmake ..

find "$PROJECT_ROOT/engine" "$PROJECT_ROOT/tests" -iname "*.[hc]pp" | entr -sc "make -j$(nproc) test"
