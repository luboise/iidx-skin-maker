#!/bin/bash

BUILD_FOLDER="windows"

BUILD_FOLDER=build-windows
TOOLCHAIN_FILENAME=windows-toolchain.cmake

mkdir -p ./$BUILD_FOLDER && 
cd ./$BUILD_FOLDER &&

export PATH=/opt/msvc/bin/x64:$PATH

cmake -DCMAKE_TOOLCHAIN_FILE=../windows-toolchain.cmake -DCMAKE_BUILD_TYPE=Release .. && 
cmake --build . --config Release -- -j 10 && 
echo "Successfully built for Windows."
