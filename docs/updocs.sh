#!/usr/bin/env bash
# Update online documentation

set -e

PROJDIR=tmp-proj
DOCDIR=tmp-docs
DOCOUT=$PROJDIR/build/doc/html

[ -d $PROJDIR ] && rm -rf $PROJDIR
[ -d $DOCDIR ] && rm -rf $DOCDIR

git clone https://github.com/billyquith/gwork.git $PROJDIR -b gwork --depth=1
git clone https://github.com/billyquith/gwork.git $DOCDIR -b gh-pages --depth=1

pushd $PROJDIR
mkdir build && cd build
cmake .. -G Ninja -DRENDER_NULL=ON
cmake --build . --target gwork_docs
cd ..
popd

[ -d $DOCDIR] && rm -r $DOCDIR/*
cp -r $DOCOUT/* $DOCDIR

pushd $DOCDIR
git add --all --verbose .
git status
if [[ "$1" == "-d" ]]; then exit 0; fi
read -p "Commit message: " MESSAGE
git commit -m "${MESSAGE:-"Update docs"}"
git push
popd

[ -d $PROJDIR ] && rm -rf $PROJDIR
[ -d $DOCDIR ] && rm -rf $DOCDIR
