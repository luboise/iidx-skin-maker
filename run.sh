mkdir -p ./build && 
cd ./build &&
cmake .. && 
cmake --build . --config Debug -- -j 10 && 
./iidx-skin-maker 
