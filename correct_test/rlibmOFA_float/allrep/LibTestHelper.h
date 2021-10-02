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

mpfr_t mval, mval200;
int default_emin, default_emax, new_emin, new_emax;

mpfr_rnd_t rnd_modes[5] = {MPFR_RNDN, MPFR_RNDD, MPFR_RNDU, MPFR_RNDZ, MPFR_RNDNA};
char* rnd_modes_string[5] = {"RNE", "RNN", "RNP", "RNZ", "RNA"};
enum RoundMode my_rnd_modes[5] = {RNE, RNN, RNP, RNZ, RNA};

float MpfrResult(float x, int numExpBit, unsigned bitlen, mpfr_rnd_t rnd) {
  if (rnd == MPFR_RNDNA) {
    unsigned sticky = 0;
    mpfr_set_emin(default_emin);
    mpfr_set_emax(default_emax);
    int exact = mpfr_set_d(mval200, x, MPFR_RNDZ);
    exact = __MPFR_ELEM__(mval200, mval200, MPFR_RNDZ);
    if (exact != 0) sticky = 1;
    double result = mpfr_get_d(mval200, MPFR_RNDZ);
    if (mpfr_cmp_d(mval200, result) != 0) sticky = 1;
    return RoundDoubleToFEN(result, numExpBit, bitlen, RNA, sticky);
  }
  
  mpfr_set_emin(new_emin);
  mpfr_set_emax(new_emax);
  int exact = mpfr_set_d(mval, x, MPFR_RNDZ);
  exact = mpfr_subnormalize(mval, exact, MPFR_RNDZ);

  exact = __MPFR_ELEM__(mval, mval, rnd);
  exact = mpfr_subnormalize(mval, exact, rnd);
  float result = mpfr_get_flt(mval, rnd);

  return result;
}

double roundToTensorfloat19(double d, mpfr_rnd_t rnd) {
  int exact = mpfr_set_d(mval, d, rnd);
  exact = mpfr_check_range(mval, exact, rnd);
  exact = mpfr_subnormalize(mval, exact, rnd);
  double result = mpfr_get_d(mval, rnd);
  return result;
}

float ConvertBinaryToFP(unsigned binary, int numExpBit, unsigned bitlen) {
  unsigned numMantissa = bitlen - numExpBit - 1;
  int bias = (1 << (numExpBit - 1)) - 1;
  
  unsigned signBit = binary & (1 << (bitlen - 1));
  unsigned mantissa = binary & ((1 << numMantissa) - 1);
  unsigned expBits = binary - signBit - mantissa;
  expBits >>= numMantissa;

  // First check for special cases:
  // zero:
  if (binary == 0 || binary == (1 << (bitlen - 1))) return 0.0;
  // infinity and nan:
  if (expBits == (1 << (unsigned)numExpBit) - 1) {
    if (mantissa == 0) {
      if (signBit == 0) return 1.0 / 0.0;
      else return -1.0 / 0.0;
    }

    return 0.0 / 0.0;
  }

  // Take care of denormal value
  if (expBits == 0) {
    int expVal = 1 - bias;

    // Get position of the first 1 from the left:
    unsigned mantissaCopy = mantissa;
    mantissaCopy |= mantissaCopy >> 16;
    mantissaCopy |= mantissaCopy >> 8;
    mantissaCopy |= mantissaCopy >> 4;
    mantissaCopy |= mantissaCopy >> 2;
    mantissaCopy |= mantissaCopy >> 1;
    int first1Pos = __builtin_popcount(mantissaCopy);
    unsigned numZeroInMantissa = numMantissa - first1Pos;

    expVal -= 1 + numZeroInMantissa;
    mantissa <<= (24 - first1Pos);
    mantissa &= 0x7FFFFF;

    floatX fX;
    fX.x = (signBit == 0) ? 0x0 : 0x80000000;
    fX.x |= mantissa;
    expVal += 127;
    unsigned floatExpBit = (unsigned)expVal << 23u;
    fX.x |= floatExpBit;
    return fX.f;
  }

  int expVal = (int)expBits - bias;
  expVal += 127;
  unsigned floatExpBit = (unsigned)expVal << 23u;
  mantissa <<= (23 - numMantissa);
  

  floatX fX;
  fX.x = (signBit == 0) ? 0x0 : 0x80000000;
  fX.x |= mantissa;
  fX.x |= floatExpBit;
  return fX.f;
}

unsigned long RunTestForExponent(int numExpBit) {
  unsigned long wrongResult = 0;

  for (unsigned bitlen = numExpBit + 2; bitlen <= numExpBit + 24; bitlen++) {
    int bias = (1 << (numExpBit - 1)) - 1;
    int emax = (1 << numExpBit) - 1 - bias;
    
    default_emin = mpfr_get_emin();
    default_emax = mpfr_get_emax();
    new_emin = 1 - bias - ((int)bitlen - 1 - numExpBit) + 1;
    new_emax = emax;
    
    mpfr_init2(mval, bitlen - numExpBit);
    mpfr_init2(mval200, bitlen - numExpBit + 1);
    
    unsigned long upperlimit = 1lu << (unsigned long)bitlen;
    // Run at most 64K at a time. That's still 5 * 22 * 7 * 64K = 50M tests
    unsigned step = (1u << (16u - bitlen));
    for (unsigned long count = 0x0; count < upperlimit; count += step) {
      float x = ConvertBinaryToFP((unsigned)count, numExpBit, bitlen);
      double res = __ELEM__(x);
      
      for (int rnd_index = 0; rnd_index < 5; rnd_index++) {
        double oracleResult = MpfrResult(x, numExpBit, bitlen, rnd_modes[rnd_index]);
        double roundedRes = RoundDoubleToFEN(res, numExpBit, bitlen,
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
    
    mpfr_clear(mval);
    mpfr_clear(mval200);
  }
  
  if (wrongResult == 0) printf("check\n");
  else printf("uh oh\n");

  return wrongResult;
}

void RunTest(char* logFile) {
  for (int i = 2; i <= 8; i++) {
    RunTestForExponent(i);
  }
}
