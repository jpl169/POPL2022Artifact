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

void RunTest(char* FileName) {

  struct stat s;
  unsigned long count = 0;
  unsigned long wrongResult = 0;
  
  float x;
  float res;
  floatX xbase;

  int fd = open(FileName, O_RDONLY);

  // Get Size of oracle file
  int status = fstat(fd, &s);
  size_t file_size = s.st_size;

  // MMap oracle file
  float* oracle = mmap(0, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (oracle == MAP_FAILED) {
    printf("mmap %s failed: %s\n", FileName, strerror(errno));
    exit(0);
  }
  
  for (count = 0x0; count < 0x100000000; count++) {
    xbase.x = count;
    x = xbase.f;

    res = (float)__ELEM__(x);

    float oracleResult = oracle[count];

    if (oracleResult != oracleResult && res != res) continue;
    if (oracleResult != res) {
      wrongResult++;
      printf("x       = %.70e\n", x);
      printf("rlibm   = %.70e (%x)\n", res, *(unsigned*)&res);
      printf("oracle  = %.70e (%x)\n", oracleResult, *(unsigned*)&oracleResult);
      printf("\n\n");
    }

    
    if (count % 1000000 == 0) {
      printf("count = %lu (%lu)\r", count, wrongResult);
      fflush(stdout);
    }
    
  }

  // Un-mmap oracle file
  munmap(oracle, file_size);
  close(fd);
  printf("rlibm  wrong result: %lu\n", wrongResult);
}
