#!/bin/bash

set -e # Fail on any error.
set -x # Display commands being run.

BUILD_ROOT=$PWD

cd github/cppdap

git submodule update --init

if [ "$BUILD_SYSTEM" == "cmake" ]; then
    mkdir build
    cd build

    build_and_run() {
        cmake .. -DCPPDAP_BUILD_EXAMPLES=1 -DCPPDAP_BUILD_TESTS=1 -DCPPDAP_WARNINGS_AS_ERRORS=1 $1
        make --jobs=$(nproc)

        ./cppdap-unittests
    }

    if [ "$BUILD_SANITIZER" == "asan" ]; then
        build_and_run "-DCPPDAP_ASAN=1"
    elif [ "$BUILD_SANITIZER" == "msan" ]; then
        build_and_run "-DCPPDAP_MSAN=1"
    elif [ "$BUILD_SANITIZER" == "tsan" ]; then
        build_and_run "-DCPPDAP_TSAN=1"
    else
        build_and_run
    fi
else
    echo "Unknown build system: $BUILD_SYSTEM"
    exit 1
fi