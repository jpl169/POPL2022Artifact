#define _GNU_SOURCE
#include <stdio.h>
#include "crlibm.h"
#include "float34RO_math.h"
#include "rounding.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

char* rnd_modes_string[5] = {"RNE", "RNN", "RNP", "RNZ", "RNA"};
enum RoundMode my_rnd_modes[5] = {RNE, RNN, RNP, RNZ, RNA};
double res[5] = {0, 0, 0, 0, 0};

void RunTestForExponent(FILE* lfd) {
  unsigned long wrongCounts[5] = {0, 0, 0, 0, 0};
  
  floatX fx;
  
  for (unsigned long count = 0x0; count < 0x100000000; count++) {
    if (count % 1000000 == 0) {
      printf("count = %lx (%ld, %ld, %ld)\r", wrongCounts[1], wrongCounts[2], wrongCounts[3]);
      fflush(stdout);
    }
    
    fx.x = count;
    double orc = __GEN_ELEM__(fx.f);
    res[1] = __ELEM_RD__(fx.f);
    res[2] = __ELEM_RU__(fx.f);
    res[3] = __ELEM_RZ__(fx.f);

    for (int rnd_index = 1; rnd_index < 4; rnd_index++) {
      double oracleResult = RoundDoubleToF8N(orc, 32, my_rnd_modes[rnd_index]);
      double roundedRes = RoundDoubleToF8N(res[rnd_index], 32, my_rnd_modes[rnd_index]);
      
      if (oracleResult != oracleResult && roundedRes != roundedRes) continue;
      if (oracleResult != roundedRes) {
        wrongCounts[rnd_index]++;
        if (wrongCounts[rnd_index] <= 5) {
          printf("count  = %lu\n", count);
          printf("rnd    = %s\n", rnd_modes_string[rnd_index]);
          printf("x      = %.50e\n", fx.f);
          printf("orc    = %.50e\n", orc);
          printf("res    = %.50e\n\n", res[rnd_index]);
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
