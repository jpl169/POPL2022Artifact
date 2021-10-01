#!/bin/bash
cd crlibm
make -s
cd ../glibc_double
make -s
cd ../glibc_float
make -s
cd ../intel_double
make -s
cd ../intel_float
make -s
cd ../rlibm_Ofa
make -s
cd ../rlibm32
make -s
cd ..
