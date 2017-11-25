#!/usr/bin/env bash
# Update the system and install the tools that Travis build requires

set -ev

echo "Lang:$LANG, OS:$TRAVIS_OS_NAME, CC:$CC"


function install_ponder
{
    git clone --depth=3 https://github.com/billyquith/ponder.git
    cd ponder
    mkdir build && cd build
    cmake .. -G "Unix Makefiles"
    cmake --build . --target install
    cd ../..
}

function install_lua53
{
    # https://www.lua.org/download.html
    curl -R -O http://www.lua.org/ftp/lua-5.3.4.tar.gz
    tar zxf lua-5.3.4.tar.gz
    cd lua-5.3.4
    
    # From Lua makefile: Convenience platforms targets.
    # PLATS= aix bsd c89 freebsd generic linux macosx mingw posix solaris
    
    make $1 test
    make install
}


# $1 - "+what +we +want"
function setup_osx
{
    echo "Requested: $1"
    
    # if [[ -z `brew list --versions cmake` ]]; then
    #     brew install cmake
    # else
    #     # Update cmake to latest version (will also update brew)
    #     brew outdated cmake || brew upgrade cmake
    # fi
    #
    # if [[ -z `echo $1 | grep "+doxygen"` ]]; then
    #     if [[ -z `brew list --versions doxygen` ]]; then
    #         brew install doxygen
    #     fi
    # fi

    if [[ -z `echo $1 | grep "+lua"` ]]; then
        #brew install lua@5.3
        install_lua53 macosx
    fi

    if [[ -z `echo $1 | grep "+ponder"` ]]; then
        install_ponder
    fi
}

function setup_linux
{
    # sudo apt-get -qq update
    # sudo apt-get install -y cmake doxygen
    echo "See .travis.yml for packages/addons install/update"

    if [[ -z `echo $1 | grep "+lua"` ]]; then
        install_lua53 linux
    fi    
}

setup_$TRAVIS_OS_NAME "$@"
