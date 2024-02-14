#!/bin/bash
cd build || exit 1
make -j "$(nproc)" test

exit 1
