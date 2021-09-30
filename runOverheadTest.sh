#!/bin/bash

echo -e "\033[1mBfloat16 Results \033[0m"
cd overhead_test/bfloat16/crlibm
./runAll.sh
cd ../glibc_double
./runAll.sh
cd ../glibc_float
./runAll.sh
cd ../glibc_rlibm
./runAll.sh
cd ../glibc_rlibm_generic
./runAll.sh
cd ../glibc_rlibm32
./runAll.sh
cd ../intel_double
./runAll.sh
cd ../intel_float
./runAll.sh
cd ../intel_rlibm
./runAll.sh
cd ../intel_rlibm_generic
./runAll.sh
cd ../intel_rlibm32
./runAll.sh
cd ../../..

echo -e "\033[1mPosit16 Results \033[0m"
cd overhead_test/posit16/crlibm
./runAll.sh
cd ../glibc_double
./runAll.sh
cd ../glibc_float
./runAll.sh
cd ../glibc_rlibm
./runAll.sh
cd ../glibc_rlibm_generic
./runAll.sh
cd ../glibc_rlibm32
./runAll.sh
cd ../intel_double
./runAll.sh
cd ../intel_float
./runAll.sh
cd ../intel_rlibm
./runAll.sh
cd ../intel_rlibm_generic
./runAll.sh
cd ../intel_rlibm32
./runAll.sh
cd ../softposit_math
./runAll.sh
cd ../../..

echo -e "\033[1mFloat Results \033[0m"
cd overhead_test/float/crlibm
./runAll.sh
cd ../glibc_double
./runAll.sh
cd ../glibc_float
./runAll.sh
cd ../glibc_rlibm_generic
./runAll.sh
cd ../glibc_rlibm32
./runAll.sh
cd ../intel_double
./runAll.sh
cd ../intel_float
./runAll.sh
cd ../intel_rlibm_generic
./runAll.sh
cd ../intel_rlibm32
./runAll.sh
cd ../../..

echo -e "\033[1mPosit32 Results \033[0m"
cd overhead_test/posit32/crlibm
./runAll.sh
cd ../glibc_double
./runAll.sh
cd ../glibc_rlibm_generic
./runAll.sh
cd ../glibc_rlibm32
./runAll.sh
cd ../intel_double
./runAll.sh
cd ../intel_rlibm_generic
./runAll.sh
cd ../intel_rlibm32
./runAll.sh
cd ../../..
