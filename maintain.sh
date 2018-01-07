#!/usr/bin/env bash
# Maintenance script

CM_GEN="-GXcode"
CM_OPTS="-DWANT_TESTS=ON -DWANT_SAMPLES=ON"
CM_REFLECT="-DWANT_REFLECTION=ON"
CM_PROJ=gwork.xcodeproj
BUILD_LOG_NAME=maintain_build.txt
BUILD_LOG=../$BUILD_LOG_NAME


# Get dependencies
function get_deps
{    
    pushd "source"
    [ ! -d deps ] || mkdir deps
    cd deps
    git clone --depth=3 https://github.com/billyquith/ponder.git ponder
    pushd ponder
    git checkout -b dev/gwork
    popd
    popd
}

function ensure_project #(renderer, build_dir)
{
    local BDIR=build_$2
    echo "Project: Renderer $1 in $BDIR"
    
    if [ ! -d $BDIR ]; then
        mkdir $BDIR
    fi
    
    pushd $BDIR
    cmake .. $CM_GEN -DRENDER_$1=ON $CM_OPTS $CM_REFLECT
    popd
}

function open_project #(renderer, build_dir)
{
    pushd build_$2
    open $CM_PROJ
    popd
}

function build_project #(renderer, build_dir)
{
    pushd build_$2
    echo "===================================================================" >> $BUILD_LOG
    echo "==== RENDER_${1} ====" >> $BUILD_LOG
    echo "===================================================================" >> $BUILD_LOG
    cmake --build . | tee -a $BUILD_LOG
    popd
}

function run_project #(renderer, build_dir)
{
    pushd build_$2
    cmake 
    popd
}

function apply_projects
{
    $1 ALLEGRO5 al
    $1 OPENGL gl
    $1 NULL null
    $1 SDL2 sdl
    $1 SFML2 sfml
    $1 SW sw
}

function build_docs
{
    pushd docs
    ./mkdocs.sh
    popd
}

function build_all
{
    echo "Maintain build log:" > $BUILD_LOG
    apply_projects build_project
    grep "error:" $BUILD_LOG_NAME > maintain_build_errors.txt
    echo
    echo "ERRORS:"
    echo
    cat maintain_build_errors.txt
}

function show_help
{
    echo "Usage: maintain.sh --deps -p(roject) -o(pen) -b(uild) -r(un) --docs"
}

while getopts "pobrdh" OPT; do
    case $OPT in
    --deps) get_deps
        ;;
    p)  apply_projects ensure_project
        ;;
    o)  apply_projects open_project
        ;;
    b)  build_all
        ;;
    r)  apply_projects run_project
        ;;
    --docs)  build_docs
        ;;
    h)  show_help
        ;;
    *)  echo "Unknown argument"    
        show_help
        ;;
    esac
done
