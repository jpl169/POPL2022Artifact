#define __MPFR_ELEM__ mpfr_cos
#define	__ELEM__ rlibm34_cospi
#include "LibTestHelperPi.h"

int IsSpecialCase(float x, double* specCaseRes) {
  if (x < 0) x = -x;
  int sticky = 0;
  double xp = (double)x * 2.0;
  int N = (int)xp;
  double R = x - N * 0.5;
  
  double reduced = fmod(x, 2.0);
  
  if (R == 0) {
    if (reduced == 0.0) {
      *specCaseRes = 1.0;
      return 1;
    }
    else if (reduced == 0.5) {
      *specCaseRes = 0.0;
      return 1;
    }
    else if (reduced == 1.0) {
      *specCaseRes = -1.0;
      return 1;
    }
    else if (reduced == 1.5) {
      *specCaseRes = 0.0;
      return 1;
    }
  }
  
  return 0;
}

int main(int argc, char** argv) {

    RunTest("CoshLogFile.txt");
    return 0;
}
