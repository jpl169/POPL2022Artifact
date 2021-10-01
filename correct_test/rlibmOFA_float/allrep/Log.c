#define __MPFR_ELEM__ mpfr_log
#define	__ELEM__ rlibm_log
#include "LibTestHelper.h"

int main(int argc, char** argv) {

    RunTest("LogLogFile.txt");
    return 0;
}
