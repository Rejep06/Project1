#include "LongNumber.hpp"
#include <iostream>
#include <string>

using namespace LongNumberArithmetics;

int main(int argc, char* argv[]){
    if (argc != 2){
        printf("Using: --precision <precision value>");
        return EXIT_FAILURE;
    }

    int precision10 = std::stoi(argv[1]);
    
    int precision = precision10 * 4;

    fix_precision_literal(precision);

    LongNumber Pi{(int64_t)0, precision}, a1{(int64_t)1, precision}, on{(int64_t)16, precision};
    for (int64_t k = 0; k < 10; k++)
    {
        LongNumber a2{8 * k + 1, precision}, a3{8 * k + 4, precision}, a4{8 * k + 5, precision}, a5{8 * k + 6, precision};
        Pi = Pi + (4.0_longnum / a2 - 2.0_longnum / a3 - 1.0_longnum / a4 - 1.0_longnum / a5) / a1;
        a1 = a1 * on;
    }

    Pi.PrintLongNumber();
}

