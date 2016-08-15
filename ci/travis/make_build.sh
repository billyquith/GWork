#!/usr/bin/env bash
# Prepare the build files for Travis to build

set -ev

cmake --version
mkdir build && cd build

function prepare_osx {
    cmake .. -GXcode -DBUILD_TEST=ON -DBUILD_SAMPLE=ON -DRENDER_NULL=ON
}

function prepare_linux {
    cmake .. -G "Unix Makefiles" -DBUILD_TEST=ON -DBUILD_SAMPLE=ON -DRENDER_NULL=ON
}

prepare_$TRAVIS_OS_NAME
