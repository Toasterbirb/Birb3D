#!/bin/bash
cd build || exit 1
cmake ..
make -j "$(nproc)" test

exit 1
