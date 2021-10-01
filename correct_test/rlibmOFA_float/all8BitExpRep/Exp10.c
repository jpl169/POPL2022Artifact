#define __MPFR_ELEM__ mpfr_exp10
#define	__ELEM__ rlibm_exp10
#include "LibTestHelper.h"

int main(int argc, char** argv) {
    
    RunTest("Exp10LogFile.txt");
    return 0;
}
