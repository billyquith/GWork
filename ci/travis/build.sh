#!/usr/bin/env bash
# Prepare the build files for Travis to build
# args: <C++ compiler> "<want opts>"

set -ev

BUILD_OS=$TRAVIS_OS_NAME
#CPP_COMPILER=$1
WANT_OPTS="-DWANT_TESTS=ON -DWANT_SAMPLE=ON -DRENDER_NULL=ON $1"

cmake --version
echo "CXX=$CXX CC=$CC"
# echo "C++ compiler: $CPP_COMPILER"
echo "Options for cmake generation: $WANT_OPTS"


function prepare_osx
{
    mkdir build
    pushd build
    cmake .. -GXcode $WANT_OPTS
    popd
}

# map C++ compiler to C compiler
function cpp2c
{
    local comp=$1
    comp=${comp/clang\+\+/clang}
    comp=${comp/g\+\+/gcc}
    echo $comp
}

function prepare_linux
{
    mkdir build
    pushd build
    # Travis doesn't pass on the COMPILER version so we'll use env CXX variable
    local comp=$1
    local ccomp=$(cpp2c $comp)
    echo "Requesting C compiler: $ccomp, C++ compiler: $comp"
    local ccmd="CC=$ccomp CXX=$comp cmake .. -G ""Unix Makefiles"" $WANT_OPTS"
    echo "$ccmd"
    eval "$ccmd"
    popd
}


function build # (config)
{
    pushd build
    cmake --build . --target GworkNullSample --config $1
    popd
}

function test_osx # (config)
{
    pushd bin/$1
    ./GworkNullSample.app/Contents/MacOS/GworkNullSample
    popd
}

function test_linux # (config)
{
    pushd bin/$1
    ./GworkNullSample
    popd
}

function build_and_test # (config)
{
    build $1
    test_$BUILD_OS $1
}


prepare_$BUILD_OS "$@"

build_and_test Debug
build_and_test Release
