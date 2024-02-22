#!/bin/bash

set -ex

[ -d build ] && rm -rf build
cmake -S . -B build
cmake --build build --verbose -j128
