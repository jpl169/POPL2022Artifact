#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

enum RoundMode {RNE, RNA, RNZ, RNP, RNN};

#ifndef FLOATINTUNION
#define FLOATINTUNION
typedef union {
  float f;
  unsigned x;
} floatint;

typedef union {
  double d;
  unsigned long x;
} doubleint;
#endif

float RoundDoubleToF8N(double v, int bitlength, enum RoundMode rnd) {
  unsigned numMantissa = bitlength - 9;

  doubleint temp;
  temp.d = v;
  
  if (*(unsigned long*)&v == 0x380fd5165ef3fe01) {
    printf("temp.d = %.100e\n", temp.d);
    printf("temp.x = %lx\n", temp.x);
  }
  
  // Take care of NaN and infinity
  if ((temp.x & 0x7FF0000000000000) == 0x7FF0000000000000) return v;
  
  // Take care of zero
  if ((temp.x & 0x7FFFFFFFFFFFFFFF) == 0) return v;

  unsigned sign = (temp.x >= 0x8000000000000000) ? 0x80000000 : 0x0;
  temp.x &= 0x7FFFFFFFFFFFFFFF;
  
  // Otherwise it's a number that rounds to a real value.
  long exp = (temp.x >> 52lu) & 0x7FF;
  exp -= 1023l;
  unsigned long mantissa = temp.x & 0x000FFFFFFFFFFFFF;
  
  if (*(unsigned long*)&v == 0x380fd5165ef3fe01) {
    printf("mantissa = %lx\n", mantissa);
  }
  
  unsigned vminus = 0;
  unsigned roundBit = 0;
  unsigned sticky = 0;
  
  if (exp < -150) {
    vminus = 0;
    roundBit = 0;
    sticky = 1;
  } else if (exp >= 128) {
    vminus = (1u << ((unsigned)bitlength - 1lu)) - 1u;
    vminus -= (1u << numMantissa);
    roundBit = 1;
    sticky = 1;
  } else {
    // double value is normal. Exp >= -150
    if (*(unsigned long*)&v == 0x380fd5165ef3fe01) {
      printf("mantissa = %lx\n", mantissa);
    }
    if ((mantissa & 0x000000000FFFFFFF) != 0) sticky = 1;
    if (*(unsigned long*)&v == 0x380fd5165ef3fe01) {
      printf("mantissa = %lx\n", mantissa);
    }
    mantissa &= 0xFFFFFFFFF0000000;
    if (*(unsigned long*)&v == 0x380fd5165ef3fe01) {
      printf("mantissa = %lx\n", mantissa);
    }
    mantissa <<= 3lu;
    
    if (*(unsigned long*)&v == 0x380fd5165ef3fe01) {
      printf("mantissa = %lx\n", mantissa);
      printf("exp = %ld\n", exp);
    }
    
    if (exp < -126l) {
      long offset = -126l - exp;
      mantissa |= 0x0080000000000000;
      mantissa >>= (unsigned long)offset;
      exp = 0l;
    } else {
      exp += 127l;
    }

    unsigned long infExt = ((unsigned long)exp << 55lu) | mantissa;
    if (*(unsigned long*)&v == 0x380fd5165ef3fe01) {
      printf("infExt = %lx\n", infExt);
    }
    
    // 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
    // xxxx xxxx xxxx xxxx rsss ssss ssss ssss ssss ssss ssss ssss ssss ssss ssss ssss
    if ((infExt & ((1lu << (63lu - (unsigned long)bitlength)) - 1lu)) != 0) sticky = 1;
    infExt >>= (63lu - (unsigned long)bitlength);
    roundBit = infExt & 0x1;
    vminus = (unsigned)(infExt >> 1lu);
  }
  
  if (*(unsigned long*)&v == 0x380fd5165ef3fe01) {
    printf("mantissa = %lx\n", mantissa);
    printf("vminus = %x\n", vminus);
    printf("roundbit = %d\n", roundBit);
    printf("sticky = %d\n", sticky);
  }
  
  unsigned lastBit = vminus & 0x1;
  unsigned roundDecision = 0;
  switch (rnd) {
    case RNE:
      roundDecision = (lastBit & roundBit) | (roundBit & sticky);
      break;
    case RNA:
      roundDecision = roundBit;
      break;
    case RNZ:
      roundDecision = 0;
      break;
    case RNP:
      if (!sign) roundDecision = roundBit | sticky;
      break;
    case RNN:
      if (sign) roundDecision = roundBit | sticky;
      break;
  }
  
  vminus += roundDecision;
  floatint res;
  res.x = vminus;
  if (bitlength < 32) res.x <<= 32 - bitlength;
  res.x |= sign;
  
  if (*(unsigned long*)&v == 0x380fd5165ef3fe01) {
    printf("res.x = %lx\n", res.x);
  }
  
  return res.f;
}

// explength = 8
// We assume the double value v is a normal value in double.
float RoundDoubleToFEN(double v, int explength, int bitlength, enum RoundMode rnd, int sticky) {
  unsigned numMantissa = bitlength - (8 + 1);
  
  doubleint temp;
  temp.d = v;
  // Take care of NaN and infinity
  if ((temp.x & 0x7FF0000000000000) == 0x7FF0000000000000) return v;
  
  // Take care of zero
  if ((temp.x & 0x7FFFFFFFFFFFFFFF) == 0) return v;
  
  unsigned sign = (temp.x >= 0x8000000000000000) ? 0x80000000 : 0x0;
  temp.x &= 0x7FFFFFFFFFFFFFFF;
  
  // Otherwise it's a number that rounds to a real value.
  long exp = (temp.x >> 52lu) & 0x7FF;
  exp -= 1023l;
  unsigned long mantissa = temp.x & 0x000FFFFFFFFFFFFF;
  
  unsigned vminus = 0;
  unsigned roundBit = 0;
  
  int bias = (1 << (8 - 1)) - 1;
  
  
  if (exp < -bias - (int)numMantissa) {
    vminus = 0;
    roundBit = 0;
    sticky |= 1;
  } else if (exp >= 1 + bias) {
    vminus = (1u << ((unsigned)bitlength - 1lu)) - 1u;
    vminus -= (1u << numMantissa);
    roundBit = 1;
    sticky |= 1;
  } else {
    // double value is normal in double. Exp >= -bias - numMantissa
    if ((mantissa & 0x000000000FFFFFFF) != 0) sticky |= 1;
    mantissa &= 0xFFFFFFFFF0000000;
    mantissa <<= 3lu;
    
    if (exp < 1 - bias) {
      long offset = (1 - bias) - exp;
      mantissa |= 0x0080000000000000;
      mantissa >>= (unsigned long)offset;
      exp = 0l;
    } else {
      exp += bias;
    }
    
    unsigned long infExt = ((unsigned long)exp << (64 - 1 - explength)) | mantissa;
    
    // 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
    // xxxx xxxx xxxx xxxx rsss ssss ssss ssss ssss ssss ssss ssss ssss ssss ssss ssss
    if ((infExt & ((1lu << (63lu - (unsigned long)bitlength)) - 1lu)) != 0) sticky |= 1;
    infExt >>= (63lu - (unsigned long)bitlength);
    roundBit = infExt & 0x1;
    vminus = (unsigned)(infExt >> 1lu);
  }
  
  unsigned lastBit = vminus & 0x1;
  unsigned roundDecision = 0;
  switch (rnd) {
    case RNE:
      roundDecision = (lastBit & roundBit) | (roundBit & sticky);
      break;
    case RNA:
      roundDecision = roundBit;
      break;
    case RNZ:
      roundDecision = 0;
      break;
    case RNP:
      if (!sign) roundDecision = roundBit | sticky;
      break;
    case RNN:
      if (sign) roundDecision = roundBit | sticky;
      break;
  }
  
  vminus += roundDecision;
  floatint res;
  res.x = vminus;
  if (bitlength < 32) res.x <<= 32 - bitlength;
  res.x |= sign;
  return res.f;
}
