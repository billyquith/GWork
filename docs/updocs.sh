#!/usr/bin/env bash
# Update online documentation

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
ninja doc
cd ..
popd

rm -r $DOCDIR/*
cp -r $DOCOUT/* $DOCDIR

pushd $DOCDIR
git add *
git status
git commit -m 'update docs'
git push
popd

[ -d $PROJDIR ] && rm -rf $PROJDIR
[ -d $DOCDIR ] && rm -rf $DOCDIR

