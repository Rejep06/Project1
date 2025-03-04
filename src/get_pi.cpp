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
        Pi = Pi + (4.0_longnum / a2 - 2.0_longnum / a3 - 1.0_longnum / a4 - 1.0_longnum / a5)/a1;
        a1 = a1 * on;
    }


    LongNumber ans{(int64_t)0, precision}, p{(int64_t)1<<32, precision}, mod{(int64_t)1000000, precision}, pp{(int64_t)1, precision};

    LongNumber Pi_int{Pi};
    Pi_int.get_int();
    
    while (!Pi_int.isZero()){
        LongNumber x = Pi_int / mod;
        x.get_int();
        LongNumber y = x * mod;
        LongNumber r = Pi_int - y;
        ans = ans + r * pp;
        Pi_int = x;
        pp *= p;
    }
    Pi -= ans;
    LongNumber p2{(int64_t)1, precision};
    while (!Pi.isZero()){
        Pi.PrintLongNumber();
        Pi *= mod;
        LongNumber x_int{Pi};
        x_int.get_int();
        p2 *= p;
        ans += (x_int/p2); 
        ans.PrintLongNumber();
        Pi -= x_int;
        std::cout<<"y\n";
    }

    ans.PrintLongNumber();
}

// while (x!=0){
//     x/=2;
// }

