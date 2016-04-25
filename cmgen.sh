#!/bin/bash

# GEN="-G Ninja"
GEN="-G Xcode"
# DBFLAGS="--debug-output"
# DBFLAGS="--trace"
# RENDER=-DRENDER_SDL2=ON
# RENDER="-DRENDER_SFML2=ON"
RENDER="-DRENDER_ALLEGRO5=ON"

[ ! -d build ] && mkdir build
cd build
echo $(pwd)
CMD="cmake $GEN $DBFLAGS $RENDER .."
echo $CMD
$CMD

