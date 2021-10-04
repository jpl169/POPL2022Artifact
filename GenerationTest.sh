#!/bin/bash

echo -e "\033[1mGenerating Log2(x) for inputs x in [1, 2) \033[0m"
echo -e "\033[1mGenerating oracle file \033[0m"
mkdir -p oracles
cd GenerateOracleFiles
make -s Log2Small
./Log2Small ../oracles/Log2SmallOracle
cd ..

mkdir -p intervals
cd IntervalGen
make -s Log2Small
./Log2Small ../oracles/Log2SmallOracle ../intervals/Log2SmallIntervals

