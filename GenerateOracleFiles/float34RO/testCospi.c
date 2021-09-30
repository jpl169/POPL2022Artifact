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
    fX.x = i;
    fX.x &= 0x7FFFFFFF;
    
    if (fX.x <= 950204803) {
      if (fX.x == 0) {
        if (roResult != 1.0) printf("uh oh\n");
        continue;
      }
      if (roResult != 0.99999998509883880615234375) printf("uh oh2\n");
      continue;
    }
    
    if (fX.x >= 0x4b000000) {
      if (fX.x >= 0x7F800000) {
        if (roResult != 0.0 / 0.0) printf("uh oh3\n");
        continue;
      }
      if (fX.x >= 0x4b800000) {
        if (roResult != 1.0) printf("uh oh4\n");
        continue;
      }
      
      // If x >= 2^23, then if x is even, then 1.0f
      if ((fX.x & 0x1) == 0) {
        if (roResult != 1.0) printf("uh oh5\n");
        continue;
      }
      
      // Otherwise, then -1.0f
      if (roResult != -1.0) printf("uh oh6\n");
    }
    
    // Range Reduction
    double xp = fX.f * 512.0;
    unsigned N = (unsigned)xp;
    unsigned N2 = N & 0xFF;
    unsigned I = (N >> 8) + 1;
    double R, cospiM, sinpiM;
    unsigned long s = (I & 0x2) ? 0x8000000000000000 : 0;
    R = fX.f - N * 0.001953125;
    
    if (R == 0 && N2 == 0) {
      if (I & 1l) {
        doubleX resx;
        resx.d = 1.0;
        resx.x |= s;
        if (roResult != resx.d) printf("uh oh7\n");
        continue;
      } else {
        if (roResult != 0.0) printf("uh oh8\n");
        continue;
      }
    }
    
    if (roResult >= 1.0) {
      printf("uh oh 9. This is a big problem\n");
    }

  }
  fclose(rof);
}
