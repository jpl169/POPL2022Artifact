#!/bin/bash
cd crlibm
./runAll.sh
cd ../glibc_double
./runAll.sh
cd ../glibc_float
./runAll.sh
cd ../intel_double
./runAll.sh
cd ../intel_float
./runAll.sh
cd ../rlibm_Ofa
./runAll.sh
cd ../rlibm32
./runAll.sh
cd ..
