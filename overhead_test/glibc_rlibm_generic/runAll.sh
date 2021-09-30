#!/bin/bash
echo -e "--- FLOAT WITH GCC RLIBM GENERIC ---"

echo -e "Log"
taskset -c 12 ./Log

echo -e "Log2"
taskset -c 12 ./Log2

echo -e "Log10"
taskset -c 12 ./Log10

echo -e "Exp"
taskset -c 12 ./Exp

echo -e "Exp2"
taskset -c 12 ./Exp2

echo -e "Exp10"
taskset -c 12 ./Exp10

echo -e "Sinh"
taskset -c 12 ./Sinh

echo -e "Cosh"
taskset -c 12 ./Cosh

echo -e "Sinpi"
taskset -c 12 ./Sinpi

echo -e "Cospi"
taskset -c 12 ./Cospi
