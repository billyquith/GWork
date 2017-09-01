#!/usr/bin/env bash

pushd ../build
cmake --build . --target doc

DOC=doc/html/index.html
case "$(uname -s)" in
  CYGWIN*) cygstart "$DOC";;
  *) open "$DOC";;
esac

popd
