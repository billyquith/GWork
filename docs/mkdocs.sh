#!/usr/bin/env bash

pushd ../build
cmake --build . --target doc
open doc/html/index.html
popd
