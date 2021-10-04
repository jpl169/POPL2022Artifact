#include "Log2Small_math.h"
#include "float34RO_headers/constants.h"
#include "float34RO_headers/log2Small.h"
#include "math.h"

double rlibm34_log2_small(float x) {
  floatX fix, fit, spec;
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

    // Special case when we have denormal input and exact result
    int exp;
    spec.f = frexpf(fix.f, &exp);
    if (spec.x == 0x3f000000) return (double)(exp - 1);

    fix.f *= 8.388608e+06;
    m -= 23;
  }
  
  m += fix.x >> 23;
  m -= 127;
  fix.x &= 0x007FFFFF;
  
  if (fix.x == 0) {
    return (double)m;
  }
  
  fix.x |= 0x3F800000;
  
  fit.x = fix.x & 0x007F0000;
  int FIndex = fit.x >> 16;
  fit.x |= 0x3F800000;
  
  double f = fix.f - fit.f;
  f *= __log_oneByF_small[FIndex];
  
  // Find the index of polynomial coefficients
  doubleX dX;
  dX.d = f;
  const double* coeffs = __log2CoeffsSmall[0];

  double y = coeffs[4];
  y *= f;
  y += coeffs[3];
  y *= f;
  y += coeffs[2];
  y *= f;
  y += coeffs[1];
  y *= f;
  y += coeffs[0];
  y *= f;
  
  y += __log2_lut_small[FIndex];
  y += m;
  
  return y;
}