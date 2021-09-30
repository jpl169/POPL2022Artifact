#include "stdlib.h"
#include "stdio.h"
#include "math.h"

#define MAXVAL 3.40282361850336062550457001444955389952e+38
#define PI 3.141592653589793115997963468544185161590576171875
#define MAXm1VAL 3.40282356779733661637539395458142568448e+38

typedef union {
  float f;
  unsigned x;
} floatX;

typedef union {
  double d;
  unsigned long x;
} doubleX;

double RoundToFloat34RO(double val) {
  // Take care of special cases
  if (val == 0.0) return 0.0;
  if (val == 1.0 / 0.0) return val;
  if (val == -1.0 / 0.0) return val;
  if (val > 0) {
    if (val < ldexp(1.0, -150)) return ldexp(1.0, -151);
    if (val > MAXm1VAL) return MAXVAL;
    
  } else {
    if (val > ldexp(-1.0, -150)) return ldexp(-1.0, -151);
    if (val < -MAXm1VAL) return -MAXVAL;
  }
  
  // At this point we have AT LEAST 2 precision bits. Guaranteed 1+
  // mantissa bit.
  doubleX dx;
  dx.d = val;

  // Get exp value of val
  int exp;
  double frac = frexp(val, &exp);
  exp--;

  // with exp value, figure out how many precision/mantissa bits i get
  // to have # precision = min(26, max(2, 152 + exp)) # mantissa =
  // min(25, max(1, 151 + exp))
  long numMantissa = 151L + exp;
  if (numMantissa < 1) numMantissa = 1;
  if (numMantissa > 25) numMantissa = 25;

  unsigned long sticky = dx.x & ((1LU << (52LU - (unsigned long)numMantissa)) - 1LU);
  dx.x -= sticky;
  if (sticky != 0LU) dx.x |= 1LU << (52LU - (unsigned long)numMantissa);

  return dx.d;
}

unsigned long ValToBinary34Bit(double val) {
  if (val == 0.0) return 0.0;
    doubleX dx;
    dx.d = val;
    unsigned long result = 0UL;

    if ((dx.x & 0x8000000000000000UL) != 0UL) result = 0x200000000UL;

    unsigned long expBit = (dx.x & 0x7FF0000000000000UL) >> (52UL);
    long exp = (long)expBit - 1023L;
    exp += 127;
    expBit = (unsigned long)exp;
    if (exp < 1L) expBit = 0x0UL;
    result |= (expBit << 25UL);

    unsigned long mantissa = (dx.x & 0x000FFFFFFFFFFFFFUL) >> 27UL;
    if (exp < 1L) {
        mantissa |= 0x2000000UL;
        long shiftAmount = 1L - exp;
        mantissa >>= (unsigned long)shiftAmount;
    }
    result |= mantissa;
    return result;
}

int main(int argc, char** argv) {

  // argv[1]: filename of floating point 34 bit round to odd version

  if (argc != 2) {
    printf("Usage: %s <file of round to odd>", argv[0]);
    exit(0);
  }

  FILE* rof = fopen(argv[1], "r+");

  unsigned long onem1index;
  int onem1done = 0;

  
  double roResult;
  unsigned count = 0;
  for (unsigned long i = 0; i < 0x80000000; i++) {
    fread(&roResult, sizeof(double), 1, rof);
    
    if (roResult != roResult) continue;    
    
    unsigned long rep34bit = ValToBinary34Bit(roResult);
    if ((rep34bit & 0x1) == 0) {
      floatX fX;
      fX.x = i;
      unsigned long s = (fX.x & 0x80000000) ? 0x8000000000000000UL : 0x0UL;
      fX.x &= 0x7FFFFFFF;
      
      if (fX.x >= 0x4b000000) {
        if (fX.x >= 0x7F800000) {
	  if (roResult != roResult) {
	    printf("uh oh\n");
	  } else continue;
        }

	if (roResult != 0.0) {
	  printf("uh oh2\n");
	} else continue;
      }
      
      double xp = fX.f * 512;
      unsigned N = (unsigned)xp;
      unsigned N2 = N & 0xFF;
      unsigned I = N >> 8;
      double R;
      
      if (I & 0x1) {
        N2 = 255 - N2;
        R = (N + 1) * 0.001953125 - fX.f;
      } else R = fX.f - N * 0.001953125;
      
      if (I & 0x2) s ^= 0x8000000000000000;
      
      if (R == 0 || R == 1.953125e-03) {
	I &= 0x3;
	
	if (I == 0) {
	  if (roResult != 0.0) {
	    printf("index = %lu\n", i);
	    printf("I = %u\n", I);
	    printf("input = %.100e\n", *(float *)&i);
	    printf("res   = %.100e\n", roResult);
	    printf("\n");
            continue;
	  } else continue;
	}
	
      	if (I == 1) {
	  double tempRes = s ? -1.0 : 1.0;
	  if (roResult != tempRes) {
	    printf("index = %lu\n", i);
	    printf("s = %lx\n", s);
	    printf("I = %u\n", I);
	    printf("input = %.100e\n", *(float *)&i);
	    printf("res   = %.100e\n", roResult);
	    printf("\n");
            continue;
	  } else continue;
	}
	
      	if (I == 2) {
	  if (roResult != 0.0) {
	    printf("index = %lu\n", i);
	    printf("I = %u\n", I);
	    printf("input = %.100e\n", *(float *)&i);
	    printf("res   = %.100e\n", roResult);
	    printf("\n");
            continue;
	  } else continue;
	}
	
      	if (I == 3) {
	  double tempRes = s ? -1.0 : 1.0;
	  if (roResult != tempRes) {
	    printf("index = %lu\n", i);
	    printf("I = %u\n", I);
	    printf("input = %.100e\n", *(float *)&i);
	    printf("res   = %.100e\n", roResult);
	    printf("\n");
            continue;
	  } else continue;
	}
      }
      
      printf("Could not catch even case\n");
      printf("index = %lu\n", i);
      printf("I = %u\n", I);
      printf("R  = %.100e\n", R);
      printf("input = %.100e\n", *(float *)&i);
      printf("res   = %.100e\n", roResult);
      printf("\n");
      continue;
      
    }


    if (i <= 872158519) {
      if (i == 869073367) {
	double result = 2.9990871297513876925222575664520263671875e-07;
	continue;
      }

      floatX fX;
      fX.x = i;
      double result = (double)fX.f * PI;
      continue;
    }
    
    floatX fX;
    fX.x = i;
    
    
    if (i == 0) continue;
  }


  printf("onem1 index = %lu\n", onem1index);
  return 0;
}
