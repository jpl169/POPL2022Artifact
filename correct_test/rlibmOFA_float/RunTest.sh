#!/bin/bash

echo -e "Testing correctness of Log(x) for various FP representations"
./allrep/Log
echo -e "Testing correctness of Log2(x) for various FP representations"
./allrep/Log2
echo -e "Testing correctness of Log10(x) for various FP representations"
./allrep/Log10
echo -e "Testing correctness of Exp(x) for various FP representations"
./allrep/Exp
echo -e "Testing correctness of Exp2(x) for various FP representations"
./allrep/Exp2
echo -e "Testing correctness of Exp10(x) for various FP representations"
./allrep/Exp10
echo -e "Testing correctness of Sinh(x) for various FP representations"
./allrep/Sinh
echo -e "Testing correctness of Cosh(x) for various FP representations"
./allrep/Cosh
echo -e "Testing correctness of Sinpi(x) for various FP representations"
./allrep/Sinpi
echo -e "Testing correctness of Cospi(x) for various FP representations"
./allrep/Cospi
