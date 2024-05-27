#!/bin/bash
cd build || exit 1
cmake ..
ninja test

exit 1
