#!/usr/bin/env bash

DOCDIR=../build_docs

if [ ! -d $DOCDIR ]; then
    mkdir $DOCDIR
    pushd $DOCDIR
    cmake -GNinja -DRENDER_NULL=ON ..
    popd
fi

pushd $DOCDIR
cmake --build . --target gwork_docs

DOC=doc/html/index.html
case "$(uname -s)" in
  CYGWIN*) cygstart "$DOC";;
  *) open "$DOC";;
esac

popd
