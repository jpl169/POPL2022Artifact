#!/bin/bash

echo    "| function |    Using OurLibm    |    glibc dbl libm   |   Intel dbl libm    |"
echo    "----------------------------------------------------------------------------"
echo    "|          | rne rnn rnp rnz rna | rne rnn rnp rnz rna | rne rnn rnp rnz rna |"
echo -n "| ln(x)    |  "
./rlibm_Ofa/Log
echo -n "  |  "
./glibc_double/Log
echo -n "  |  "
./intel_double/Log
echo    "  |"

echo -n "| log2(x)  |  "
./rlibm_Ofa/Log2
echo -n "  |  "
./glibc_double/Log2
echo -n "  |  "
./intel_double/Log2
echo    "  |"

echo -n "| log10(x) |  "
./rlibm_Ofa/Log10
echo -n "  |  "
./glibc_double/Log10
echo -n "  |  "
./intel_double/Log10
echo    "  |"

echo -n "| exp(x)   |  "
./rlibm_Ofa/Exp
echo -n "  |  "
./glibc_double/Exp
echo -n "  |  "
./intel_double/Exp
echo    "  |"

echo -n "| exp2(x)  |  "
./rlibm_Ofa/Exp2
echo -n "  |  "
./glibc_double/Exp2
echo -n "  |  "
./intel_double/Exp2
echo    "  |"

echo -n "| exp10(x) |  "
./rlibm_Ofa/Exp10
echo -n "  |  "
./glibc_double/Exp10
echo -n "  |  "
./intel_double/Exp10
echo    "  |"

echo -n "| sinh(x)  |  "
./rlibm_Ofa/Sinh
echo -n "  |  "
./glibc_double/Sinh
echo -n "  |  "
./intel_double/Sinh
echo    "  |"

echo -n "| cosh(x)  |  "
./rlibm_Ofa/Cosh
echo -n "  |  "
./glibc_double/Cosh
echo -n "  |  "
./intel_double/Cosh
echo    "  |"

echo -n "| sinpi(x) |  "
./rlibm_Ofa/Sinpi
echo -n "  |  "
echo -n "                 "
echo -n "  |  "
./intel_double/Sinpi
echo    "  |"

echo -n "| cospi(x) |  "
./rlibm_Ofa/Cospi
echo -n "  |  "
echo -n "                 "
echo -n "  |  "
./intel_double/Cospi
echo    "  |"
