#include "TestSystem.hpp"
#include <iostream>

// Colors

#define BRED    "\033[1;31m"
#define BGREEN  "\033[1;32m"
#define BYELLOW "\033[1;33m"
#define BPURPLE "\033[1;35m"
#define BCYAN   "\033[1;36m"
#define RESET   "\033[0m"

using namespace TestSystem;

TestResult TestSystem::run_test(const char *name, TestScenario test, size_t timeout_ms, bool inspect)
{
    printf("Running test %20s: ", name);
    fflush(stdout);
    try {
        bool result = test();
        if (result)
        {
            printf(BGREEN "[OK]\n" RESET);
        }
        else
        {
            printf(BRED "[FAIL]\n" RESET);
        }
    }
    catch (const std::exception& exc)
    {
        printf(BPURPLE "[EXC] (exception: %s)\n" RESET, exc.what());
        exit(EXCEPTION);
    }

    return OK;
}
