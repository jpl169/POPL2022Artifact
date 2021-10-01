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

mpfr_t mval;
unsigned long totalWrongCount = 0;

double tensorfloat19exp(float x) {

  int exact = mpfr_set_d(mval, x, MPFR_RNDZ);
  if (exact != 0) {
    printf("uh oh... this value isn't exactly representable\n");
    printf("x = %.100e\n", x);
  }
  exact = mpfr_subnormalize(mval, exact, MPFR_RNDZ);
  if (exact != 0) {
    printf("uh oh... something going on with subnormal\n");
    printf("x = %.100e\n", x);
  }

  exact = mpfr_round_nearest_away(__MPFR_ELEM__, mval, mval);
  exact = mpfr_round_nearest_away(mpfr_check_range, mval, exact);
  exact = mpfr_round_nearest_away(mpfr_subnormalize, mval, exact);
  double result = mpfr_get_d(mval, MPFR_RNDZ);
  return result;
}

double roundToTensorfloat19(double d) {
  int exact = mpfr_round_nearest_away(mpfr_set_d, mval, d);
  exact = mpfr_round_nearest_away(mpfr_check_range, mval, exact);
  exact = mpfr_round_nearest_away(mpfr_subnormalize, mval, exact);
  double result = mpfr_get_d(mval, MPFR_RNDZ);
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
        
    mpfr_set_default_prec(bitlen - numExpBit);
    mpfr_set_emin(1 - bias - ((int)bitlen - 1 - numExpBit) + 1);
    mpfr_set_emax(emax);
    mpfr_init(mval);
    
    // 1000 0000 0000 0000 0000                                                                      
    unsigned long upperlimit = 1lu << (unsigned long)bitlen;
    for (unsigned long count = 0x0; count < upperlimit; count++) {
      float x = ConvertBinaryToFP((unsigned)count, numExpBit, bitlen);
      double res = __ELEM__(x);
      
      double oracleResult = tensorfloat19exp(x);
      double roundedRes = roundToTensorfloat19(res);

      if (oracleResult != oracleResult && roundedRes != roundedRes) continue;
      if (oracleResult != roundedRes && wrongResult < 10) {
        wrongResult++;
        fprintf(lfd, "x       = %.70e\n", x);
        fprintf(lfd, "bitlength = %u\n", bitlen);
        fprintf(lfd, "rounding mode = Round to nearest tie goes away\n");
        fprintf(lfd, "rlibm   = %.70e (%lx)\n", res, *(unsigned long*)&res);
        fprintf(lfd, "rounded = %.70e (%lx)\n", roundedRes, *(unsigned long*)&roundedRes);
        fprintf(lfd, "oracle  = %.70e (%lx)\n", oracleResult, *(unsigned long*)&oracleResult);
        fprintf(lfd, "\n");
        fflush(lfd);
      }
      
      
      if (count % 1000000 == 0) {
        printf("numExpBit: %1.d, bitlen: %2.u, count = %12.lu (wrong results: %lu/%lu)\r", numExpBit, bitlen, count, wrongResult, totalWrongCount);
        fflush(stdout);
      }
      
    }
    
    mpfr_clear(mval);
  }

  fprintf(lfd, "\nNumber Of Wrong Result for Expbit %d: %lu\n\n", numExpBit, wrongResult);
  printf("\nNumber Of Wrong Result for Expbit %d: %lu\n\n", numExpBit, wrongResult);
  fflush(lfd);
  return wrongResult;
}

void RunTest(char* logFile) {

  FILE* lfd = fopen(logFile, "w");
  // Exp bit for 8 has already been checked
  for (int i = 2; i < 8; i++) {
    totalWrongCount += RunTestForExponent(i, lfd);
  }

  printf("Total wrong count: %lu\n", totalWrongCount);

  fclose(lfd);
}
