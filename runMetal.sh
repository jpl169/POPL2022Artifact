#!/bin/bash

cd external/metalibm/
make -s clean
make -s

cd ../../overhead_test/float/metalibm_double_avx2_O3_flags/
make -s -f Makefile_gcc10
./runAll.sh
cd ../metalibm_float_avx2_O3_flags/
make -s -f Makefile_gcc10
./runAll.sh
