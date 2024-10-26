mkdir -p ./build && 
cd ./build &&
cmake .. && 
cmake --build . --config Debug -- -j 10 && 
./tests/Tests && 
./iidx-skin-maker 
