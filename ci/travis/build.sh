#!/usr/bin/env bash
# Prepare the build files for Travis to build
# args: <C++ compiler> "<want opts>"

set -ev

uname -a
env

BUILD_OS=$TRAVIS_OS_NAME
WITH_OPTS="-DWITH_TESTS=ON -DWITH_SAMPLE=ON -DRENDER_NULL=ON $FEATURES"
RENDER_SAMPLE=GworkNullSample

cmake --version
echo "Using C++ compiler: CXX=$CXX, CC=$CC"
echo "Options for cmake generation: $WITH_OPTS"


function prepare_osx
{
    mkdir build
    pushd build
    cmake .. -GXcode $WITH_OPTS
    popd
}

function prepare_linux
{
    mkdir build
    pushd build
    # Travis doesn't pass on the COMPILER version so we'll use env CXX variable
    local ccmd="cmake .. -G \"Unix Makefiles\" $WITH_OPTS"
    echo "$ccmd"
    eval "$ccmd"
    popd
}


function build # (config)
{
    echo "==== Building config $1 ===="
    pushd build
    cmake --build . --target $RENDER_SAMPLE --config $1
    popd
}

function test_osx # (config)
{
    # OSX outputs to bin/CONFIG
    pushd bin/$1
    ./$RENDER_SAMPLE.app/Contents/MacOS/$RENDER_SAMPLE
    popd
}

function test_linux # (config)
{    
    # All Linux configs output to bin/
    pwd
    ls
    pushd bin
    ./$RENDER_SAMPLE
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
