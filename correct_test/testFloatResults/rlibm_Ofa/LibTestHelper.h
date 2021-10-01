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

mpfr_t mval;

mpfr_rnd_t rnd_modes[5] = {MPFR_RNDN, MPFR_RNDD, MPFR_RNDU, MPFR_RNDZ, MPFR_RNDNA};
char* rnd_modes_string[5] = {"RNE", "RNN", "RNP", "RNZ", "RNA"};
enum RoundMode my_rnd_modes[5] = {RNE, RNN, RNP, RNZ, RNA};

double MpfrResult(float x, mpfr_rnd_t rnd) {

  if (rnd == MPFR_RNDNA) {
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
  
  int exact = mpfr_set_d(mval, x, MPFR_RNDZ);
  /*
  if (exact != 0) {
    fprintf(lfd, "uh oh... this value isn't exactly representable\n");
    fprintf(lfd, "x = %.100e\n", x);
  }
   */
  exact = mpfr_subnormalize(mval, exact, MPFR_RNDZ);
  /*
  if (exact != 0) {
    fprintf(lfd, "uh oh... something going on with subnormal\n");
    fprintf(lfd, "x = %.100e\n", x);
  }
   */

  exact = __MPFR_ELEM__(mval, mval, rnd);
  exact = mpfr_check_range(mval, exact, rnd);
  exact = mpfr_subnormalize(mval, exact, rnd);
  double result = mpfr_get_d(mval, rnd);
  return result;
}

void RunTestForExponent() {
  unsigned long wrongCounts[5];
  for (int i = 0; i < 5; i++) wrongCounts[i] = 0;
      
  mpfr_set_default_prec(24);
  mpfr_set_emin(-148);
  mpfr_set_emax(128);
  mpfr_init(mval);
  
  floatX fx;
  
  for (unsigned long count = 0x0; count < 0x100000000; count += 0x100) {    
    fx.x = count;
    double res = __ELEM__(fx.f);
    
    for (int rnd_index = 0; rnd_index < 5; rnd_index++) {
      mpfr_rnd_t rnd = rnd_modes[rnd_index];
      double oracleResult = MpfrResult(fx.f, rnd);
      double roundedRes = RoundDoubleToF8N(res, 32, my_rnd_modes[rnd_index]);
      
      if (oracleResult != oracleResult && roundedRes != roundedRes) continue;
      if (oracleResult != roundedRes) {
        wrongCounts[rnd_index]++;
        
        /*
        if (wrongCounts[rnd_index] <= 5) {
          fprintf(lfd, "count  = %lu\n", count);
          fprintf(lfd, "rnd    = %s\n", rnd_modes_string[rnd_index]);
          fprintf(lfd, "x      = %.50e\n", fx.f);
          fprintf(lfd, "oracle = %.50e\n", oracleResult);
          fprintf(lfd, "test   = %.50e\n\n", roundedRes);
          
          printf("count  = %lu\n", count);
          printf("rnd    = %s\n", rnd_modes_string[rnd_index]);
          printf("x      = %.50e\n", fx.f);
          printf("oracle = %.50e\n", oracleResult);
          printf("test   = %.50e\n\n", roundedRes);
        }
         */
      }
    }
  }
  
  for (int i = 0; i < 5; i++) {
    if (wrongCounts[i] == 0) printf("\033[0;32mo\033[0m   ");
    else printf("\033[0;31mx\033[0m   ");
  }
  printf("\n");
  
  mpfr_clear(mval);
}

void RunTest(char* logFile) {
  RunTestForExponent();
}
