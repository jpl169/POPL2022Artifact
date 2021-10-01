#!/bin/bash

echo    "| function |    Using OurLibm    |    glibc dbl libm   |   Intel dbl libm    |"
echo    "----------------------------------------------------------------------------"
echo    "|          | rne rnn rnp rnz rna | rne rnn rnp rnz rna | rne rnn rnp rnz rna |"
echo -n "| ln(x)    | "
./rlibm_Ofa/Log
echo -n "  | "
./glibc_double/Log
echo -n "  | "
./intel_double/Log
echo    "  |"
