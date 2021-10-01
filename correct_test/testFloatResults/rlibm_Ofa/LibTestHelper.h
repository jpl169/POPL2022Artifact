#define _GNU_SOURCE
#include <stdio.h>
#include "float34RO_math.h"
#include "rounding.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "math.h"
#include "mpfr.h"

mpfr_t mval, mval200;

mpfr_rnd_t rnd_modes[5] = {MPFR_RNDN, MPFR_RNDD, MPFR_RNDU, MPFR_RNDZ, MPFR_RNDNA};
char* rnd_modes_string[5] = {"RNE", "RNN", "RNP", "RNZ", "RNA"};
enum RoundMode my_rnd_modes[5] = {RNE, RNN, RNP, RNZ, RNA};

float MpfrResult(float x, mpfr_rnd_t rnd) {
  if (rnd == MPFR_RNDNA) {
    int exact = mpfr_set_d(mval200, x, MPFR_RNDZ);
    
    exact = __MPFR_ELEM__(mval200, mval200, MPFR_RNDZ);
    exact = mpfr_check_range(mval200, exact, MPFR_RNDZ);
    exact = mpfr_subnormalize(mval200, exact, MPFR_RNDZ);
    double result = mpfr_get_d(mval200, MPFR_RNDZ);
    return RoundDoubleToF8N(result, 32, RNA);
  }
  
  int exact = mpfr_set_d(mval, x, MPFR_RNDZ);
  exact = mpfr_subnormalize(mval, exact, MPFR_RNDZ);

  exact = __MPFR_ELEM__(mval, mval, rnd);
  exact = mpfr_subnormalize(mval, exact, rnd);
  float result = mpfr_get_flt(mval, rnd);
  return result;
}

void RunTestForExponent() {
  unsigned long wrongCounts[5];
  for (int i = 0; i < 5; i++) wrongCounts[i] = 0;
      
  mpfr_set_default_prec(24);
  mpfr_set_emin(-148);
  mpfr_set_emax(128);
  mpfr_init2(mval, 24);
  mpfr_init2(mval200, 200);
  
  floatX fx;
  
  for (unsigned long count = 0x0; count < 0x100000000; count += 0x100) {
    fx.x = count;
    double res = __ELEM__(fx.f);
    
    for (int rnd_index = 0; rnd_index < 5; rnd_index++) {
      mpfr_rnd_t rnd = rnd_modes[rnd_index];
      float oracleResult = MpfrResult(fx.f, rnd);
      
      if (count == 3266227968) {
        printf("input       [%s] = %.100e\n", rnd_modes_string[rnd_index], fx.f);
        printf("oracleResult[%s] = %.100e\n", rnd_modes_string[rnd_index], oracleResult);
      }
      
      float roundedRes = RoundDoubleToF8N(res, 32, my_rnd_modes[rnd_index]);
      
      if (oracleResult != oracleResult && roundedRes != roundedRes) continue;
      if (oracleResult != roundedRes) {
        wrongCounts[rnd_index]++;
        
        if (wrongCounts[rnd_index] <= 5) {
          printf("count  = %lu\n", count);
          printf("rnd    = %s\n", rnd_modes_string[rnd_index]);
          printf("x      = %.50e\n", fx.f);
          printf("oracle = %.50e\n", oracleResult);
          printf("res    = %.100e\n", res);
          printf("res    = %lx\n", *(unsigned long*)&res);
          printf("test   = %.50e\n\n", roundedRes);
          printf("res    = %x\n", *(unsigned*)&roundedRes);
        }
      }
    }
  }
  
  for (int i = 0; i < 5; i++) {
    if (wrongCounts[i] == 0) printf("\033[0;32mo\033[0m");
    else printf("\033[0;31mx\033[0m");
    if (i < 4) printf("   ");
  }
  
  mpfr_clear(mval);
}

void RunTest(char* logFile) {
  RunTestForExponent();
}
