#!/bin/bash

cd overhead_test/glibc_rlibm_generic
make -s clean
make -s
./runAll.sh
cd ../glibc_double
make -s clean
make -s
./runAll.sh
cd ../glibc_float
make -s clean
make -s
./runAll.sh
cd ../crlibm
make -s clean
make -s
./runAll.sh
cd ../glibc_rlibm32
make -s clean
make -s
./runAll.sh
cd ../intel_rlibm_generic
make -s clean
make -s
./runAll.sh
cd ../intel_double
make -s clean
make -s
./runAll.sh
cd ../intel_float
make -s clean
make -s
./runAll.sh
cd ../../..
