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

mpfr_rnd_t rnd_modes[5] = {MPFR_RNDN, MPFR_RNDD, MPFR_RNDU, MPFR_RNDZ, MPFR_RNDA};

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
double tensorfloat19exp(float x, mpfr_rnd_t rnd) {

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


void RunTest(char* logFile) {

  FILE* lfd = fopen(logFile, "w");
  
  unsigned long wrongResult = 0;
    
  for (unsigned bitlen = 10; bitlen < 32; bitlen++) {
    
    mpfr_set_default_prec(bitlen - 8);
    mpfr_set_emin(-126 - (int)bitlen + 10);
    mpfr_set_emax(128);
    mpfr_init(mval);
    
    // 1000 0000 0000 0000 0000
    unsigned long upperlimit = 1lu << (unsigned long)bitlen;
    for (unsigned long count = 0x0; count < upperlimit; count++) {
      floatX xbase;
      xbase.x = count << (32lu - (unsigned long)bitlen);
      float x = xbase.f;
      
      double res = __ELEM__(x);
      
      for (int rnd_index = 0; rnd_index < 5; rnd_index++) {
	mpfr_rnd_t rnd = rnd_modes[rnd_index];
	double oracleResult = tensorfloat19exp(x, rnd);
	double roundedRes = roundToTensorfloat19(res, rnd);
	
	// This means both resutls are NaN
	if (oracleResult != oracleResult && roundedRes != roundedRes) continue;
	
	if (oracleResult != roundedRes) {
	  wrongResult++;
	  fprintf(lfd, "x       = %.70e\n", x);
	  fprintf(lfd, "bitlength = %u\n", bitlen);
	  fprintf(lfd, "rounding mode index = %i\n", rnd_index);
	  fprintf(lfd, "rlibm   = %.70e (%lx)\n", res, *(unsigned long*)&res);
	  fprintf(lfd, "rounded = %.70e (%lx)\n", roundedRes, *(unsigned long*)&roundedRes);
	  fprintf(lfd, "oracle  = %.70e (%lx)\n", oracleResult, *(unsigned long*)&oracleResult);
	  fprintf(lfd, "\n");
	}
      }
      
      
      if (count % 1000000 == 0) {
	printf("bitlen: %2.u, count = %12.lu (%lu)\r", bitlen, count, wrongResult);
	fflush(stdout);
      }
      
    }
    
    mpfr_clear(mval);
  }
  
  printf("\n\nrlibm  wrong result: %lu\n", wrongResult);
  fclose(lfd);
}
