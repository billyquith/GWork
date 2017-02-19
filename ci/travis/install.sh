#!/usr/bin/env bash
# Update the system and install the tools that Travis build requires

set -ev

echo "Lang:$LANG, OS:$TRAVIS_OS_NAME, CC:$CC"


function get_ponder {
    git clone https://github.com/billyquith/ponder.git
    cd ponder
    mkdir build && cd build
    cmake .. -G $1
    cmake --build . --target install
    cd ../..
}


function install_osx {
    brew update
    if [[ -z `brew list | grep doxygen` ]]; then brew install doxygen; fi
    if [[ -z `brew list | grep cmake` ]]; then brew install cmake; fi
    brew outdated cmake || brew upgrade cmake

    if [[ "$1" == "reflect" ]]; then
        brew install lua  # currently Lua 5.2
        get_ponder Xcode
    fi
}
  
function install_linux {
    # sudo apt-get -qq update
    # sudo apt-get install -y cmake doxygen
    echo "See .travis.yml for packages/addons install/update"
}

install_$TRAVIS_OS_NAME "$@"
