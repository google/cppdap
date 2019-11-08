#!/bin/bash

set -e # Fail on any error.
set -x # Display commands being run.

BUILD_ROOT=$PWD

cd github/cppdap

git submodule update --init

if [ "$BUILD_SYSTEM" == "cmake" ]; then
    mkdir build
    cd build

    cmake .. -DCPPDAP_BUILD_EXAMPLES=1 -DCPPDAP_BUILD_TESTS=1 -DCPPDAP_WARNINGS_AS_ERRORS=1
    make -j$(sysctl -n hw.logicalcpu)

    ./cppdap-unittests
else
    echo "Unknown build system: $BUILD_SYSTEM"
    exit 1
fi