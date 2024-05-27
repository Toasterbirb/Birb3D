#!/bin/bash
cd build || exit 1
cmake ..
make -j "$(nproc)" test
ninja test

exit 1
