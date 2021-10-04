#!/bin/bash

echo -e "\033[1mGenerating Log2(x) for inputs x in [1, 2) \033[0m"
echo -e "\033[1mGenerating oracle file \033[0m"
mkdir -p intervals
cd GenerateOracleFiles
make -s Log2Small
./Log2Small intervals/Log2SmallOracle



