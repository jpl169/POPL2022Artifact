#!/bin/bash

cd overhead_test/glibc_rlibm_generic
./runAll.sh
cd ../glibc_double
./runAll.sh
cd ../glibc_float
./runAll.sh
cd ../crlibm
./runAll.sh
cd ../glibc_rlibm32
./runAll.sh
cd ../intel_rlibm_generic
./runAll.sh
cd ../intel_double
./runAll.sh
cd ../intel_float
./runAll.sh
cd ../../..
