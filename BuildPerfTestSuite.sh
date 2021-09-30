#!/bin/bash

echo -e "\033[1mBuilding Test Script For Float Results \033[0m"
cd overhead_test/float/crlibm
make -s clean
make -s -f Makefile_gcc10
cd ../glibc_double
make -s clean
make -s -f Makefile_gcc10
cd ../glibc_float
make -s clean
make -s -f Makefile_gcc10
cd ../glibc_rlibm_generic
make -s clean
make -s -f Makefile_gcc10
cd ../glibc_rlibm32
make -s clean
make -s -f Makefile_gcc10
cd ../intel_double
make -s clean
make -s
cd ../intel_float
make -s clean
make -s
cd ../intel_rlibm_generic
make -s clean
make -s
cd ../intel_rlibm32
make -s clean
make -s
cd ../../..

echo -e "\033[1mBuilding Test Script For Bfloat16 Results \033[0m"
cd overhead_test/bfloat16/crlibm
make -s clean
make -s -f Makefile_gcc10
cd ../glibc_double
make -s clean
make -s -f Makefile_gcc10
cd ../glibc_float
make -s clean
make -s -f Makefile_gcc10
cd ../glibc_rlibm
make -s clean
make -s -f Makefile_gcc10
cd ../glibc_rlibm_generic
make -s clean
make -s -f Makefile_gcc10
cd ../glibc_rlibm32
make -s clean
make -s -f Makefile_gcc10
cd ../intel_double
make -s clean
make -s
cd ../intel_float
make -s clean
make -s
cd ../intel_rlibm
make -s clean
make -s
cd ../intel_rlibm_generic
make -s clean
make -s
cd ../intel_rlibm32
make -s clean
make -s
cd ../../..
