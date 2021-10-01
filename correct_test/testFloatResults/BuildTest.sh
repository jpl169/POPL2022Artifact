#!/bin/bash
cd ../rlibm_Ofa
make clean
make -s
cd ../glibc_double
make clean
make -s
cd ../intel_double
make clean
make -s
cd crlibm
make clean
make -s
cd ../rlibm32
make clean
make -s
cd ..
