#!/bin/bash

cd overhead_test/glibc_rlibm_generic
make -s clean
make -s
./runAll.sh
make -s clean
cd ../glibc_double
make -s clean
make -s
./runAll.sh
make -s clean
cd ../glibc_float
make -s clean
make -s
./runAll.sh
make -s clean
cd ../crlibm
make -s clean
make -s
./runAll.sh
make -s clean
cd ../glibc_rlibm32
make -s clean
make -s
./runAll.sh
make -s clean
cd ../intel_rlibm_generic
make -s clean
make -s
./runAll.sh
make -s clean
cd ../intel_double
make -s clean
make -s
./runAll.sh
make -s clean
cd ../intel_float
make -s clean
make -s
./runAll.sh
make -s clean
cd ../..
