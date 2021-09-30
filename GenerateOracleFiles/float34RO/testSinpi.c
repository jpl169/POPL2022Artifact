#include "stdlib.h"
#include "stdio.h"

typedef union {
  float f;
  unsigned x;
} floatX;

typedef union {
  double d;
  unsigned long x;
} doubleX;

int main(int argc, char** argv) {

  // argv[1]: filename of floating point 34 bit round to odd version
  // argv[2]: filename of Oracle result of 32-bit float

  if (argc != 2) {
    printf("Usage: %s <file of round to odd>\n", argv[0]);
    exit(0);
  }

  FILE* rof = fopen(argv[1], "r+");

  double roResult;
  float oracleResult, roundedResult;
  for (unsigned long i = 0; i < 0x100000000; i++) {
    fread(&roResult, sizeof(double), 1, rof);
    if (roResult != roResult) continue;
    
    floatX fX;
    fX.f = x;
    unsigned long s = (fX.x & 0x80000000) == 0 ? 0x0 : 0x8000000000000000;
    fX.x &= 0x7FFFFFFF;
    
    if (fX.x >= 0x4b000000) {
      if (fX.x >= 0x7F800000) {
        if (roResult != 0.0 / 0.0) {
          printf("uh oh\n");
        }
      }
      if (roResult != 0.0) {
        printf("uh oh2\n");
      }
    }
    
    double xp = fX.f * 512;
    unsigned N = (unsigned)xp;
    unsigned N2 = N & 0xFF;
    unsigned I = N >> 8;
    double R = fX.f - N * 0.001953125;
    if (I & 0x2) s ^= 0x8000000000000000;
    
    if (R == 0 && N2 == 0) {
      if (I & 0x1) {
        doubleX dX;
        dX.d = 1.0;
        dX.x |= s;
        if (dX.d != roResult) {
          printf("uh oh3\n");
        }
      } else {
        if (roResult != 0) {
          printf("uh oh4\n");
        }
      }
    }
    
    if (roResult >= 1.0) {
      printf("uh oh 5. This is a big problem\n");
    }

  }
  fclose(rof);
}
