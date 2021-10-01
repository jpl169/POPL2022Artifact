#define __MPFR_ELEM__ mpfr_log10
#define	__ELEM__ rlibm_log10
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

mpfr_rnd_t rnd_modes[5] = {MPFR_RNDN, MPFR_RNDD, MPFR_RNDU, MPFR_RNDZ, MPFR_RNDA};

double tensorfloat19exp(float x, mpfr_rnd_t rnd, FILE* lfd) {

int exact = mpfr_set_d(mval, x, MPFR_RNDZ);
 if (exact != 0) {
   fprintf(lfd, "uh oh... this value isn't exactly representable\n");
   fprintf(lfd, "x = %.100e\n", x);
   exit(0);
 }
 exact = mpfr_subnormalize(mval, exact, MPFR_RNDZ);
 if (exact != 0) {
   fprintf(lfd, "uh oh... something going on with subnormal\n");
   fprintf(lfd, "x = %.100e\n", x);
   exit(0);
 }
 
 exact = __MPFR_ELEM__(mval, mval, rnd);
 exact = mpfr_check_range(mval, exact, rnd);
 exact = mpfr_subnormalize(mval, exact, rnd);
 double result = mpfr_get_d(mval, rnd);
 
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
  int bias = (1 << (numExpBit) - 1) - 1;
  
  unsigned signBit = binary & (1 << (bitlen - 1));
  unsigned mantissa = binary & ((1 << numMantissa) - 1);
  unsigned expBits = binary - signBit - mantissa;
  expBits >>= numMantissa;
  
  if (binary == 0 || binary == (1 << (bitlen - 1))) return 0.0;

  if (expBits == (1 << (unsigned)numExpBit) - 1) {
    if (mantissa == 0) {
      if (signBit == 0) return 1.0 / 0.0;
      else return -1.0 / 0.0;
    }

    return 0.0 / 0.0;
  }

  if (expBits == 0) {
    int expVal = 1 - bias;
    
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

unsigned long RunTestForExponent(unsigned bitlen, int numExpBit, FILE* lfd, char* resFile) {
  unsigned long wrongResult = 0;

  fprintf(lfd, "Checking for numExpBit = %d\n", numExpBit);
  fflush(lfd);
  int bias = (1 << (numExpBit - 1)) - 1;
  int emax = (1 << numExpBit) - 1 - bias;
  
  mpfr_set_default_prec(bitlen - numExpBit);
  mpfr_set_emin(2 - bias - (int)bitlen + numExpBit);
  mpfr_set_emax(emax);
  mpfr_init(mval);
  
  unsigned long upperlimit = 1lu << (unsigned long)bitlen;
  for (unsigned long count = 0x0; count < upperlimit; count++) {
    float x = ConvertBinaryToFP((unsigned)count, numExpBit, bitlen);
    double res = __ELEM__(x);
    
    for (int rnd_index = 0; rnd_index < 5; rnd_index++) {
      mpfr_rnd_t rnd = rnd_modes[rnd_index];
      double oracleResult = tensorfloat19exp(x, rnd, lfd);
      double roundedRes = roundToTensorfloat19(res, rnd);
      
      if (oracleResult != oracleResult && roundedRes != roundedRes) continue;
      if (oracleResult != roundedRes && wrongResult < 10) {
	wrongResult++;
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

    if (count % 3000000 == 0) {
      fprintf(lfd, "numExpBit: %1.d, bitlen: %2.u, count = %12.lu (wrong results: %lu/%lu)\r", numExpBit, bitlen, count, wrongResult, totalWrongCount);
      fflush(stdout);
    }
    
  }

  mpfr_clear(mval);

  FILE* rfd = fopen(resFile, "w");
  if (wrongResult > 0) {
    fprintf(lfd, "\nNumber Of Wrong Result for Expbit %d: %lu\n\n", numExpBit, wrongResult);
  }
  fclose(rfd);

  fprintf(lfd, "\nNumber Of Wrong Result for Expbit %d: %lu\n\n", numExpBit, wrongResult);
  fflush(lfd);
  return wrongResult;
}

void RunTest(unsigned bitlen, int expbit, char* logFile, char* resFile) {
  
  FILE* lfd = fopen(logFile, "w");
  RunTestForExponent(bitlen, expbit, lfd, resFile);
  
  fclose(lfd);
}



int main(int argc, char** argv) {

  if (argc != 5) {
    printf("Usage: %s <log filename> <result filename> <bitlen> <expbit>\n", argv[0]);
    exit(0);
  }
  
  RunTest(atoi(argv[3]), atoi(argv[4]), argv[1], argv[2]);
    return 0;
}
