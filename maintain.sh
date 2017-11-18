#!/usr/bin/env bash
# Maintenance script

CM_GEN="-GXcode"
CM_REFLECT="-DWANT_REFLECTION=ON"
CM_PROJ=gwork.xcodeproj

function project #(renderer, build_dir)
{
    local BDIR=build_$2
    echo "Project: renderer $1 in $BDIR"
    
    if [ ! -d $BDIR ]; then
        mkdir $BDIR
    fi
    
    pushd $BDIR
    cmake .. $CM_GEN -DRENDER_$1=ON $CM_REFLECT        
    open $CM_PROJ
    popd
}

project ALLEGRO5 al
project OPENGL gl
project NULL null
project SDL2 sdl
project SFML2 sfml
project SW sw
