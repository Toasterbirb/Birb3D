#!/bin/sh

############################
# Cross-compile the engine #
############################

PROJECT_ROOT="$(dirname "$0")/.."
BUILD_DIRECTORY="$PROJECT_ROOT/build_win"

mkdir -p "$BUILD_DIRECTORY"
cd "$BUILD_DIRECTORY" || exit 1
cmake .. -DBIRB_WINDOWS=ON -DBIRB_PROFILER=OFF -DCMAKE_TOOLCHAIN_FILE=../x86_64-w64-mingw32.cmake -DCMAKE_CXX_FLAGS="-std=c++20"
make -j "$(nproc)"
