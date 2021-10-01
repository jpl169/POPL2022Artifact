#define _GNU_SOURCE
#include <stdio.h>
#include "mathimf.h"
#include <x86intrin.h>
#include <stdlib.h>

typedef union {
    double d;
    unsigned long x;
} doubleX;

typedef union {
    float f;
    unsigned int x;
} floatX;

void RunTest(const char* filename) {
  unsigned long count = 0;
  unsigned long totalTime = 0;
  unsigned long someCount = 0;
  unsigned long t1, t2;
  unsigned int dummy;

  double dres;
  float x, res;
  floatX xbase;
  
  for (count = 0x0; count < 0x100000000; count++) {
    xbase.x = count;
    x = xbase.f;

    // Make sure t2 > t1.
    do {
      t1 = __rdtscp(&dummy);
      dres = __ELEM__(x);
      res = (float)dres;
      t2 = __rdtscp(&dummy);
    } while(t2 <= t1);
      
    totalTime += (t2 - t1);

    // Add check to make sure that program uses double math library
    if (dres == (double)res) someCount++;
    if (res == 0.0f) someCount++;
  }
  
  FILE* f = fopen(filename, "w");
  fprintf(f, "%lu, %lu", totalTime, someCount);
  fclose(f);
}
