#define __GEN_ELEM__ rlibm34_log2
#define	__ELEM__ rlibm_log2_internal
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "math.h"
#include "float34RO_math.h"
#include "rounding.h"
#include "float_math.h"
#include "float_headers/constants.h"
#include "float_headers/Log2.h"

double rlibm_log2_internal(float x) {
    floatX fix, fit;
    fix.f = x;
    int m = 0;
    
    if (fix.x < 0x800000 || fix.x >= 0x7F800000) {
        if ((fix.x & 0x7FFFFFFF) == 0) { // log(+/-0) = -infty
            fix.x = 0xFF800000;
            return fix.f;
        }
        
        if (fix.x > 0x7FFFFFFF) { // Log(-val) = NaN
            return (x - x) / 0;
            
        }
        
        if (fix.x >= 0x7F800000) {
            return x + x;
        }
        
        fix.f *= 8.388608e+06;
        m -= 23;
    }
    
    m += fix.x >> 23;
    m -= 127;
    fix.x &= 0x007FFFFF;
    fix.x |= 0x3F800000;
    
    fit.x = fix.x & 0x007F0000;
    int FIndex = fit.x >> 16;
    fit.x |= 0x3F800000;
    
    double f = fix.f - fit.f;
    f *= log_oneByF[FIndex];
    
    double y = C4;
    y *= f;
    y += C3;
    y *= f;
    y += C2;
    y *= f;
    y += C1;
    y *= f;
    y += C0;
    y *= f;
    
    return y + log2_lut[FIndex] + m;
}

char* rnd_modes_string[5] = {"RNE", "RNN", "RNP", "RNZ", "RNA"};
enum RoundMode my_rnd_modes[5] = {RNE, RNN, RNP, RNZ, RNA};

void RunTestForExponent(FILE* lfd) {
  unsigned long wrongCounts[5];
  for (int i = 0; i < 5; i++) wrongCounts[i] = 0;
  
  floatX fx;
  
  for (unsigned long count = 0x0; count < 0x100000000; count++) {
    if (count % 1000000 == 0) {
      printf("count = %lu (%lu)\r", count, wrongCounts[3]);
      fflush(stdout);
    }
    
    fx.x = count;
    double orc = __GEN_ELEM__(fx.f);
    double res = __ELEM__(fx.f);
    
    for (int rnd_index = 0; rnd_index < 5; rnd_index++) {
      double oracleResult = RoundDoubleToF8N(orc, 32, my_rnd_modes[rnd_index]);
      double roundedRes = RoundDoubleToF8N(res, 32, my_rnd_modes[rnd_index]);
      
      if (oracleResult != oracleResult && roundedRes != roundedRes) continue;
      if (oracleResult != roundedRes) {
	wrongCounts[rnd_index]++;
	if (rnd_index == 3) {
	  printf("count  = %lu\n", count);
          printf("rnd    = %s\n", rnd_modes_string[rnd_index]);
          printf("x      = %.50e\n", fx.f);
	  
          printf("orc    = %.50e\n", orc);
          printf("res    = %.50e\n\n", res);
          printf("oracle = %.50e\n", oracleResult);
          printf("test   = %.50e\n\n", roundedRes);
	}
      }
    }
  }
  
  for (int i = 0; i < 5; i++) {
    fprintf(lfd, "Number of Wrong Results for %s rounding mode: %lu\n",
            rnd_modes_string[i],
            wrongCounts[i]);
  }
}

void RunTest(char* logFile) {

  FILE* lfd = fopen(logFile, "w");
  RunTestForExponent(lfd);
  fclose(lfd);
}

int main(int argc, char** argv) {

    RunTest("Log2LogFile.txt");
    return 0;
}
