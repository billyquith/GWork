#!/usr/bin/env bash
# Maintenance script to compile Gwork on all UNIX support platforms (OSX, Linux)

set -e

case $(uname -s) in
Darwin) CM_GEN="-GXcode" ;;
*) echo "Unsupported OS"; exit 1 ;;
esac
CM_OPTS="-DWITH_TESTS=ON -DWITH_SAMPLE=ON"
CM_REFLECT="-DWITH_REFLECTION=ON"
CM_PROJ=gwork.xcodeproj
BUILD_LOG_NAME=maintain_build.txt
BUILD_LOG=../$BUILD_LOG_NAME


# Get dependencies
function get_deps
{
    echo "Nothing to get"
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

function clean_project #(renderer, build_dir)
{
    pushd build_$2
    echo "Cleaning $1"
    cmake --build . --target clean
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
    $1 IRRLICHT irr
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
    local BERR=maintain_build_errors.txt
    echo "Build errors:" > $BERR

    echo "Maintain build log:" > $BUILD_LOG
    apply_projects build_project

    echo "===================================================================" >> $BERR
    grep "error:" $BUILD_LOG_NAME >> $BERR
    echo
    echo "ERRORS:"
    echo
    cat maintain_build_errors.txt
}

function clean_all
{
    apply_projects clean_project
}

function show_help
{
    cat <<EOM
Usage: maintain.sh -hpobr [--deps] [--docs]
    -d(ocs)         Generate docs
    -g(get)         Get/build up project dependecies
    -h(elp)
    -p(roject)      Create projects
    -o(pen)         Open projects in editor
    -b(uild)        Build all
    -r(un)          Run all
    -c(lean)        Clean all build artifacts

EOM
}

while getopts "pobcrdgh" OPT; do
    case $OPT in
    p)  apply_projects ensure_project
        ;;
    o)  apply_projects open_project
        ;;
    b)  build_all
        ;;
    c)  clean_all
        ;;
    r)  apply_projects run_project
        ;;
    d)  build_docs
        ;;
    g)  get_deps
        ;;
    h)  show_help
        ;;
    *)  echo "Unknown argument"
        show_help
        ;;
    esac
done
