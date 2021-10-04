#!/bin/bash

echo -e "\033[1mGenerating Log2(x) for inputs x in [1, 2) \033[0m"
echo -e "\033[1mGenerating oracle file \033[0m"
cd smallGenerationTest

mkdir -p oracles
cd GenerateOracleFiles
make -s clean
make -s Log2Small
./Log2Small ../oracles/Log2SmallOracle
cd ..

mkdir -p intervals
cd IntervalGen
make -s clean
make -s Log2Small
./Log2Small ../intervals/Log2SmallIntervals ../oracles/Log2SmallOracle 
cd ..

cd functiongen
make -s clean
make -s Log2Small
./Log2Small ../intervals/Log2SmallIntervals log2Small.log ../include/float34RO_headers/log2Small.h 0
