#!/bin/bash

CURRENT_SOURCE_DIR=$(cd $(dirname "${BASH_SOURCE[0]}") && pwd)
cd "${CURRENT_SOURCE_DIR}"

mkdir -p build
cd build
cmake -DUSE_OPENCV=OFF ..
make
