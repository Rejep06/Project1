#include "LongNumber.hpp"
#include <iostream>

using namespace std;
using namespace LongNumberArithmetics;

int main()
{
    LongNumber x{(long double)22, 200};
    LongNumber y{(long double)22, 100};
    try{
        x = x / y;
        x.PrintLongNumber();
    }
    catch(std::runtime_error& exc){
        printf("[ERROR!] Runtime error: %s\n", exc.what());
    }
    
}