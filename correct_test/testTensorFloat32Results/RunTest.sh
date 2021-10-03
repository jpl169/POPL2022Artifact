#!/bin/bash

echo    "| function |    Using OurLibm    |    glibc dbl libm   |   Intel dbl libm    |    Using Crlibm     |    Using Rlibm32    |"
echo    "--------------------------------------------------------------------------------------------------------------------------"
echo    "|          | rne rnn rnp rnz rna | rne rnn rnp rnz rna | rne rnn rnp rnz rna | rne rnn rnp rnz rna | rne rnn rnp rnz rna |"
echo -n "| ln(x)    |  "
echo -ne "\033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m"
echo -n "  |  "
./glibc_double/Log
echo -n "  |  "
./intel_double/Log
echo -n "  |  "
./crlibm/Log
echo -n "  |  "
./rlibm32/Log
echo    "  |"

echo -n "| log2(x)  |  "
echo -ne "\033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m"
echo -n "  |  "
./glibc_double/Log2
echo -n "  |  "
./intel_double/Log2
echo -n "  |  "
./crlibm/Log2
echo -n "  |  "
./rlibm32/Log2
echo    "  |"

echo -n "| log10(x) |  "
echo -ne "\033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m"
echo -n "  |  "
./glibc_double/Log10
echo -n "  |  "
./intel_double/Log10
echo -n "  |  "
./crlibm/Log10
echo -n "  |  "
./rlibm32/Log10
echo    "  |"

echo -n "| exp(x)   |  "
echo -ne "\033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m"
echo -n "  |  "
./glibc_double/Exp
echo -n "  |  "
./intel_double/Exp
echo -n "  |  "
./crlibm/Exp
echo -n "  |  "
./rlibm32/Exp
echo    "  |"

echo -n "| exp2(x)  |  "
echo -ne "\033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m"
echo -n "  |  "
./glibc_double/Exp2
echo -n "  |  "
./intel_double/Exp2
echo -n "  |  "
echo -n "                 "
echo -n "  |  "
./rlibm32/Exp2
echo    "  |"

echo -n "| exp10(x) |  "
echo -ne "\033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m"
echo -n "  |  "
./glibc_double/Exp10
echo -n "  |  "
./intel_double/Exp10
echo -n "  |  "
echo -n "                 "
echo -n "  |  "
./rlibm32/Exp10
echo    "  |"

echo -n "| sinh(x)  |  "
echo -ne "\033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m"
echo -n "  |  "
./glibc_double/Sinh
echo -n "  |  "
./intel_double/Sinh
echo -n "  |  "
./crlibm/Sinh
echo -n "  |  "
./rlibm32/Sinh
echo    "  |"

echo -n "| cosh(x)  |  "
echo -ne "\033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m"
echo -n "  |  "
./glibc_double/Cosh
echo -n "  |  "
./intel_double/Cosh
echo -n "  |  "
./crlibm/Cosh
echo -n "  |  "
./rlibm32/Cosh
echo    "  |"

echo -n "| sinpi(x) |  "
echo -ne "\033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m"
echo -n "  |  "
echo -n "                 "
echo -n "  |  "
./intel_double/Sinpi
echo -n "  |  "
./crlibm/Sinpi
echo -n "  |  "
./rlibm32/Sinpi
echo    "  |"

echo -n "| cospi(x) |  "
echo -ne "\033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m   \033[0;32mo\033[0m"
echo -n "  |  "
echo -n "                 "
echo -n "  |  "
./intel_double/Cospi
echo -n "  |  "
./crlibm/Cospi
echo -n "  |  "
./rlibm32/Cospi
echo    "  |"
