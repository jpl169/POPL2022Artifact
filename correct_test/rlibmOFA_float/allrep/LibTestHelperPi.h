#define _GNU_SOURCE
#include <stdio.h>
#include "float34RO_math.h"
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "math.h"
#include "mpfr.h"
#include "rounding.h"

mpfr_t mval;
int IsSpecialCase(float, double*);

mpfr_rnd_t rnd_modes[5] = {MPFR_RNDN, MPFR_RNDD, MPFR_RNDU, MPFR_RNDZ, MPFR_RNDNA};
char* rnd_modes_string[5] = {"RNE", "RNN", "RNP", "RNZ", "RNA"};
enum RoundMode my_rnd_modes[5] = {RNE, RNN, RNP, RNZ, RNA};

double MpfrResult(float x, int* sticky) {
  double specCaseRes;
  if (IsSpecialCase(x, &specCaseRes)) {
    *sticky = 0;
    return specCaseRes;
  }
  
  double reduced = fmod(x, 2.0);
  mpfr_const_pi(mval, MPFR_RNDZ);
  mpfr_mul_d(mval, mval, reduced, MPFR_RNDZ);
  
  // Don't need to track exact, because it's guaranteed to be not exact as long
  // as the input is not a special case input.
  __MPFR_ELEM__(mval, mval, MPFR_RNDZ);
  *sticky = 1;
  return mpfr_get_d(mval, MPFR_RNDZ);
}

double roundToTensorfloat19(double d, mpfr_rnd_t rnd) {
  int exact = mpfr_set_d(mval, d, rnd);
  exact = mpfr_check_range(mval, exact, rnd);
  exact = mpfr_subnormalize(mval, exact, rnd);
  double result = mpfr_get_d(mval, rnd);
  return result;
}

unsigned long RunTestForExponent(int numExpBit) {
  unsigned long wrongResult = 0;
  mpfr_init2(mval, 500);

  for (unsigned bitlen = numExpBit + 2; bitlen <= numExpBit + 24; bitlen++) {
    int bias = (1 << (numExpBit - 1)) - 1;
    int emax = (1 << numExpBit) - 1 - bias;
    
    unsigned long upperlimit = 1lu << (unsigned long)bitlen;
    // Run at most 64K at a time. That's still 5 * 22 * 7 * 64K = 50M tests
    
    unsigned step = (bitlen > 16) ? (1u << (bitlen - 16u)) : 1u;
    for (unsigned long count = 0x0; count < upperlimit; count += step) {
      float x = ConvertBinToFP((unsigned)count, numExpBit, bitlen);
      double res = __ELEM__(x);
      int sticky;
      double orc = MpfrResult(x, &sticky);
      
      for (int rnd_index = 0; rnd_index < 5; rnd_index++) {
        float oracleResult = RoundDoubleToF8NWithSticky(orc, 32, rmode, sticky);
        float roundedRes = RoundDoubleToFEN(res, numExpBit, bitlen,
                                             my_rnd_modes[rnd_index], 0);
	
        if (oracleResult != oracleResult && roundedRes != roundedRes) continue;
        if (oracleResult != roundedRes && wrongResult < 10) {
            wrongResult++;
          
          if (wrongResult <= 5) {
            printf("count  = %lu\n", count);
            printf("rnd    = %s\n", rnd_modes_string[rnd_index]);
            printf("x      = %.50e\n", x);
            printf("oracle = %.50e\n", oracleResult);
            printf("res    = %.100e\n", res);
            printf("res    = %lx\n", *(unsigned long*)&res);
            printf("test   = %.50e\n", roundedRes);
            printf("res    = %x\n\n", *(unsigned*)&roundedRes);
          }
        }
      }
    }
  }
  
  if (wrongResult == 0) printf("check\n");
  else printf("uh oh\n");
  
  mpfr_clear(mval);

  return wrongResult;
}

void RunTest(char* logFile) {
  for (int i = 2; i <= 8; i++) {
    RunTestForExponent(i);
  }
}
