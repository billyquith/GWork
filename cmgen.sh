#!/bin/bash

# GEN="-G Ninja"
GEN="-G Xcode"
# DBFLAGS="--debug-output"
# DBFLAGS="--trace"
# RENDER=-DWANT_RENDERER_SDL2=ON
# RENDER="-DWANT_RENDERER_SFML2=ON"
RENDER="-DWANT_RENDERER_ALLEGRO5=ON"

[ ! -d build ] && mkdir build
cd build
echo $(pwd)
CMD="cmake $GEN $DBFLAGS $RENDER .."
echo $CMD
$CMD

