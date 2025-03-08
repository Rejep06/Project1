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
    for (int64_t k = 0; k < 79; k++)
    {
        LongNumber a2{8 * k + 1, precision}, a3{8 * k + 4, precision}, a4{8 * k + 5, precision}, a5{8 * k + 6, precision};
        Pi += ((4 / a2) - (2 / a3) - (1 / a4) - (1 / a5)) / a1;
        a1 *= 16;
    }

    std::cout<<clock();
    LongNumber ans{(int64_t)0, precision}, p{(int64_t)1 << 32, precision}, mod{(int64_t)1000000, precision}, pp{(int64_t)1, precision};

    // Pi.PrintLongNumber();
    precision = precision10 / 6 * 32 + precision10 % 6;
    LongNumber Pi_int{Pi};
    Pi_int.get_int();

    while (!Pi_int.isZero())
    {
        LongNumber x = Pi_int / mod;
        x.get_int();
        LongNumber y = x * mod;
        LongNumber r = Pi_int - y;
        ans += r * pp;
        Pi_int = x;
        pp *= p;
    }
    Pi -= ans;
    LongNumber p2{(int64_t)1, precision};
    while (!Pi.isZero())
    {
        Pi *= mod;
        LongNumber x_int{Pi};
        x_int.get_int();
        p2 *= p;
        ans += (x_int / p2);
        Pi -= x_int;
    }

    ans.PrintLongNumber();
    std::cout<<"3,141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067\n" << clock();
}

// 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803

// while (x!=0){
//     x/=2;
// }
