#include "LongNumber.hpp"
#include <iostream>
#include <string>
#include <time.h>

using namespace LongNumberArithmetics;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Using: --precision <precision value>");
        return EXIT_FAILURE;
    }

    int precision10 = std::stoi(argv[1]);

    int precision = std::min(precision10 * 3 + 30, 400);

    fix_precision_literal(precision);

    LongNumber Pi{(int64_t)0, precision}, a1{(int64_t)1, precision};
    for (int64_t k = 0; k < 80; k++)
    {
        LongNumber a2{8 * k + 1, precision}, a3{8 * k + 4, precision}, a4{8 * k + 5, precision}, a5{8 * k + 6, precision};
        Pi += ((4 / a2) - (2 / a3) - (1 / a4) - (1 / a5)) / a1;
        a1 *= 16;
    }

    // Pi.PrintLongNumber();
    precision = precision10 / 6 * 32 + precision10 % 6;
    LongNumber p{(int64_t)1 << 32, precision}, mod{(int64_t)1000000, precision};
    int x;
    std::cout << "3.";
    Pi -= 3;
    LongNumber p2{(int64_t)1, precision};
    for (int i = 0; i < precision / 32; i++)
    {
        Pi *= mod;
        LongNumber x_int{Pi};
        x_int.get_int();
        p2 *= p;
        x = x_int.get();
        if (100000 > x)
            std::cout << 0;
        std::cout << x;
        Pi -= x_int;
    }
    x = ((Pi * (10.0_longnum).pow(precision10 % 6))).get_int().get();
    if (1000000 > x)
        std::cout << 0;
    std::cout << x;

    std::cout << "\n3,1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679\n"
              << clock() / 1000.0;
}
