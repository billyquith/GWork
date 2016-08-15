#!/usr/bin/env bash
# Prepare the build files for Travis to build

set -ev

cmake --version
echo "CMAKE_ARGS:$CMAKE_ARGS"
mkdir build && cd build

if [[ "$TRAVIS_OS_NAME" == "osx" ]]; then
    cmake .. -GXcode $CMAKE_ARGS
fi

if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
    cmake .. -G "Unix Makefiles" $CMAKE_ARGS
fi
