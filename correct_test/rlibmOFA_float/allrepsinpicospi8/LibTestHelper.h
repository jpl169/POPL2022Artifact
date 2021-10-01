#define _GNU_SOURCE
#include <stdio.h>
#include "float34RO_math.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "math.h"
#include "mpfr.h"

int IsSpecialCase(float, double*);

mpfr_t mvalInput, mvalRes;
unsigned long totalWrongCount = 0;

mpfr_exp_t default_emin, default_emax, curr_emin, curr_emax;

mpfr_rnd_t rnd_modes[4] = {MPFR_RNDN, MPFR_RNDD, MPFR_RNDU, MPFR_RNDZ};

void ChangeMPFRParameter() {
  mpfr_set_emin(curr_emin);
  mpfr_set_emax(curr_emax);
}

void RestoreMPFRParameter() {
  mpfr_set_emin(default_emin);
  mpfr_set_emax(default_emax);
}

double tensorfloat19exp(float x, mpfr_rnd_t rnd) {
  double specCaseRes;
  if (IsSpecialCase(x, &specCaseRes)) return specCaseRes;

  double reduced = fmod(x, 2.0);
  mpfr_const_pi(mvalInput, MPFR_RNDZ);
  mpfr_mul_d(mvalInput, mvalInput, reduced, MPFR_RNDZ);
  
  int exact = __MPFR_ELEM__(mvalRes, mvalInput, rnd);
  
  ChangeMPFRParameter();
  
  exact = mpfr_check_range(mvalRes, exact, rnd);
  exact = mpfr_subnormalize(mvalRes, exact, rnd);
  
  RestoreMPFRParameter();
  
  double result = mpfr_get_d(mvalRes, rnd);

  return result;
}

double roundToTensorfloat19(double d, mpfr_rnd_t rnd) {
  ChangeMPFRParameter();
  
  int exact = mpfr_set_d(mvalRes, d, rnd);
  exact = mpfr_check_range(mvalRes, exact, rnd);
  exact = mpfr_subnormalize(mvalRes, exact, rnd);
  
  RestoreMPFRParameter();
  
  double result = mpfr_get_d(mvalRes, rnd);
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

// 19bits
// 1 sign bit, 8 exponent bits, 10 mantissa bits
// 11 precision bits
// bias = 2^{7} - 1 = 127
// 2^{8} - 1 - 127 = 255 - 127 = 128
// emax = 128
// 1 - bias = 1 - 127 = -126
// smallest value: 2^(-126 - 10) = 2^(-136) = 1/2 * 2^emin
// emin = -135


// double
// 1 sign bit, 11 exponent bits, 52 mantissa bits
// 53 precision bits
// bias = 2^{10} - 1 = 1023
// 2^{11} - 1 - 1023 = 2047 - 1023 = 1024
// emax = 1024
// 1 - bias = 1 - 1023 = -1022
// smallest value: 2^(-1022 - 52) = 2^(-1074) = 1/2 * 2^emin
// emin = -1073

unsigned long RunTestForExponent(int numExpBit, FILE* lfd) {
  unsigned long wrongResult = 0;
  
  

  fprintf(lfd, "Checking for numExpBit = %d\n", numExpBit);
  fprintf(lfd, "bitlength from %u to %u\n", numExpBit + 2, numExpBit + 24);
  fflush(lfd);
  for (unsigned bitlen = numExpBit + 2; bitlen <= numExpBit + 24; bitlen++) {
    int bias = (1 << (numExpBit - 1)) - 1;
    int emax = (1 << numExpBit) - 1 - bias;
    
    curr_emin = 1 - bias - ((int)bitlen - 1 - numExpBit) + 1;
    curr_emax = emax;

    mpfr_init2(mvalRes, bitlen - numExpBit);
    
    // 1000 0000 0000 0000 0000                                                                      
    unsigned long upperlimit = 1lu << (unsigned long)bitlen;
    for (unsigned long count = 0x0; count < upperlimit; count++) {
      float x = ConvertBinaryToFP((unsigned)count, numExpBit, bitlen);
      double res = __ELEM__(x);
      
      for (int rnd_index = 0; rnd_index < 4; rnd_index++) {
        mpfr_rnd_t rnd = rnd_modes[rnd_index];
        double oracleResult = tensorfloat19exp(x, rnd);
        double roundedRes = roundToTensorfloat19(res, rnd);
	
        if (oracleResult != oracleResult && roundedRes != roundedRes) continue;
        if (oracleResult != roundedRes) {
          wrongResult++;
          if (wrongResult < 10) {
            fprintf(lfd, "x       = %.70e\n", x);
            fprintf(lfd, "bitlength = %u\n", bitlen);
            fprintf(lfd, "rounding mode index = %i\n", rnd_index);
            fprintf(lfd, "rlibm   = %.70e (%lx)\n", res, *(unsigned long*)&res);
            fprintf(lfd, "rounded = %.70e (%lx)\n", roundedRes, *(unsigned long*)&roundedRes);
            fprintf(lfd, "oracle  = %.70e (%lx)\n", oracleResult, *(unsigned long*)&oracleResult);
            fprintf(lfd, "\n");
            fflush(lfd);
          }
        }
      }
      
      
      if (count % 1000000 == 0) {
        printf("numExpBit: %1.d, bitlen: %2.u, count = %12.lu (wrong results: %lu/%lu)\r", numExpBit, bitlen, count, wrongResult, totalWrongCount);
        fflush(stdout);
      }
      
    }
    
    mpfr_clear(mvalRes);
  }

  fprintf(lfd, "\nNumber Of Wrong Result for Expbit %d: %lu\n\n", numExpBit, wrongResult);
  printf("\nNumber Of Wrong Result for Expbit %d: %lu\n\n", numExpBit, wrongResult);
  fflush(lfd);
  return wrongResult;
}

void RunTest(char* logFile) {
  
  mpfr_init2(mvalInput, 1000);
  
  default_emin = mpfr_get_emin();
  default_emax = mpfr_get_emax();

  FILE* lfd = fopen(logFile, "w");
  // Exp bit for 8 has already been checked
  totalWrongCount += RunTestForExponent(8, lfd);

  printf("Total wrong count: %lu\n", totalWrongCount);

  fclose(lfd);
  mpfr_clear(mvalInput);
}
