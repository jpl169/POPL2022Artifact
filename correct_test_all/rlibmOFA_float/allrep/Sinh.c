#define __MPFR_ELEM__ mpfr_sinh
#define	__ELEM__ rlibm_sinh
#include "LibTestHelper.h"

int main(int argc, char** argv) {
    RunTest("SinhLogFile.txt");
    return 0;
}
