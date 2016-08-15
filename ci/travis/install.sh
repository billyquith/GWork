#!/usr/bin/env bash
# Update the system and install the tools that Travis build requires

set -ev

echo "Lang:$LANG, OS:$TRAVIS_OS_NAME, CC:$CC"

if [[ "$TRAVIS_OS_NAME" == "osx" ]]; then
    brew update
    #if [[ -z `brew list | grep doxygen` ]]; then brew install doxygen; fi
    if [[ -z `brew list | grep cmake` ]]; then brew install cmake; fi
    brew outdated cmake || brew upgrade cmake
fi
  
# if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
#     #sudo apt-get -qq update
#     #sudo apt-get install -y cmake doxygen
# fi

CMAKE_ARGS="-DBUILD_TEST=ON -DBUILD_SAMPLE=ON -DRENDER_NULL=ON"
CMAKE_TARGET="GworkNullSample"
