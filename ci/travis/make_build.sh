#!/usr/bin/env bash
# Prepare the build files for Travis to build

set -ev

cmake --version
mkdir build && cd build

function cpp2c {
    local comp=$1
    comp=$(echo $comp | sed 's/g\\+\\+/gcc/')
    comp=$(echo $comp | sed 's/clang\\+\\+/clang/')
    echo $comp
}

function prepare_osx {
    cmake .. -GXcode -DBUILD_TEST=ON -DBUILD_SAMPLE=ON -DRENDER_NULL=ON
}

function prepare_linux {
    # Travis doesn't pass on the COMPILER version so we'll use env CXX variable
    local comp=$1
    local ccomp=$(cpp2c $comp)
    echo "Requesting C compiler: $ccomp, C++ compiler: $comp"
    CC=$ccomp CXX=$comp cmake .. -G "Unix Makefiles" \
        -DBUILD_TEST=ON -DBUILD_SAMPLE=ON -DRENDER_NULL=ON
}

# args: <C++ compiler>
prepare_$TRAVIS_OS_NAME "$@"
