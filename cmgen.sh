#!/bin/bash

rm CMakeCache.txt
rm -r CMakeFiles
rm *.cmake
rm gwen/*.cmake
rm *.ninja
rm -r build/
rm gwen/lib*.a
rm -r gwork.*/      # Xcode

GEN="-G Ninja"
# GEN="-G Xcode"
# DBFLAGS="--debug-output"
# DBFLAGS="--trace"
# RENDER=-DRENDER_SDL2=ON
RENDER="-DRENDER_SFML2=ON"
# RENDER="-DRENDER_ALLEGRO5=ON"

CMD="cmake $GEN $DBFLAGS $RENDER"
echo $CMD
$CMD

