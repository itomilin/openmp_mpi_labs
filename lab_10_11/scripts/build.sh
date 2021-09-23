#!/bin/bash

CMAKE_OPT=$1

if [[ -d ./build ]]; then
    rm -Rf ./build/*
    cd build
else
    mkdir ./build && cd ./build
fi

cmake $CMAKE_OPT ..
make -j3

