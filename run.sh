#!/bin/bash

BUILD_FOLDER="./build"

mkdir -p $BUILD_FOLDER && 
cd $BUILD_FOLDER &&
cmake .. && 
cd ../ &&
cmake --build $BUILD_FOLDER --config Debug -- -j 10 && 
cd ./bin/Linux/
./Tests && 
./iidx-skin-maker 
