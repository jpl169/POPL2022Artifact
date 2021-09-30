#include "stdlib.h"
#include "stdio.h"

int main(int argc, char** argv) {

  // argv[1]: filename of floating point 34 bit round to odd version
  // argv[2]: filename of Oracle result of 32-bit float

  if (argc != 3) {
    printf("Usage: %s <file of round to odd> <file of oracle>", argv[0]);
    exit(0);
  }

  FILE* rof = fopen(argv[1], "r+");
  FILE* oraclef = fopen(argv[2], "r+");

  double roResult;
  float oracleResult, roundedResult;
  for (unsigned long i = 0; i < 0x100000000; i++) {
    fread(&roResult, sizeof(double), 1, rof);
    fread(&oracleResult, sizeof(float), 1, oraclef);
    roundedResult = (float)roResult;

    if (roundedResult != roundedResult && oracleResult != oracleResult) continue;
    if (roundedResult != oracleResult) {
      printf("uh oh\n");
      printf("%lu\n", i);
      printf("ro      = %.100e (%lx)\n", roResult, *(unsigned long*)&roResult);
      printf("rounded = %.100e (%x)\n", roundedResult, *(unsigned*)&roundedResult);
      printf("oracle  = %.100e (%x)\n", oracleResult, *(unsigned*)&oracleResult);

    }
  }
}
