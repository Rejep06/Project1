#include "LongNumber.hpp"
#include "TestSystem.hpp"
#include <iostream>

using namespace LongNumberArithmetics;
using namespace TestSystem;

LongNumber longexp1{(long double)1e-14, 320}, longexp2{(long double)-1e-14, 320};

// TESTS:

bool test_constructor()
{
    int64_t a = 2341479;
    long double x = 2213.211;

    try
    {
        LongNumber long1{a, 200};
        LongNumber long2{x, 100};
    }
    catch (const std::exception &exc)
    {
        return FAIL;
    }

    return OK;
}

bool test_constructor_copy()
{
    int64_t a = 21311213421;

    LongNumber long1{a, 20};

    LongNumber long2{long1};

    return (long1 == long2) ? OK : FAIL;
}

bool test_copy_assginment()
{
    int64_t a = 29837491236;
    LongNumber long1{a, 20};

    LongNumber long2 = long1;

    return (long1 == long2) ? OK : FAIL;
}

bool test_operator_sum()
{
    long double a = 12.12, b = 214.42;

    LongNumber long1{a, 320};
    LongNumber long2{b, 320};

    LongNumber longsum1{a + b, 320};
    LongNumber longsum2 = long1 + long2;

    LongNumber longsub = longsum1 - longsum2;

    return (longsub < longexp1 && longsub > longexp2) ? OK : FAIL;
}

bool test_operator_sub()
{
    int64_t a = 1245168, b = 12840954596;

    LongNumber long1{a, 20};
    LongNumber long2{b, 20};

    LongNumber longsum1{a - b, 20};
    LongNumber longsum2 = long1 - long2;

    return (longsum1 == longsum2) ? OK : FAIL;
}

bool test_operator_multipe()
{
    int64_t a = 1245168, b = 1284596;

    LongNumber long1{a, 20};
    LongNumber long2{b, 20};

    LongNumber longsum1{a * b, 20};
    LongNumber longsum2 = long1 * long2;

    return (longsum1 == longsum2) ? OK : FAIL;
}

bool test_operator_div()
{
    long double a = 124089168, b = 1256;

    LongNumber long1{a, 320};
    LongNumber long2{b, 320};

    LongNumber longsum1{a / b, 320};
    LongNumber longsum2 = long1 / long2;

    LongNumber longsub = longsum1 - longsum2;

    return ((longsub < longexp1) && (longexp2 < longsub)) ? OK : FAIL;
}

bool test_operator_notequal()
{
    int64_t a = 1245168, b = 12840954596;

    LongNumber long1{a, 20};
    LongNumber long2{b, 20};

    return (long1 != long2) ? OK : FAIL;
}

bool test_operator_geater()
{
    int64_t a = 1245168, b = 12840954596;

    LongNumber long1{a, 20};
    LongNumber long2{b, 20};

    return (long1 < long2) ? OK : FAIL;
}

bool test_operator_less()
{
    int64_t a = 1245168, b = -1231243;

    LongNumber long1{a, 20};
    LongNumber long2{b, 20};

    return (long1 > long2) ? OK : FAIL;
}

bool test_literal()
{
    return (3.1_longnum + 2.2_longnum == 5.3_longnum) ? OK : FAIL;
}

int main()
{
    printf("Start testing...\n");
    run_test("Test Constructor", test_constructor);
    run_test("Test Constructor Copy", test_constructor_copy);
    run_test("Test Copy Assignment", test_copy_assginment);
    run_test("Test Operator Sum", test_operator_sum);
    run_test("Test Operator Sub", test_operator_sub);
    run_test("Test Operator Multiple", test_operator_multipe);
    run_test("Test Operator Div", test_operator_div);
    run_test("Test Operator !=", test_operator_notequal);
    run_test("Test Operator>", test_operator_geater);
    run_test("Test Operator<", test_operator_less);
    run_test("Test Literal", test_literal);

    LongNumber Pi{(int64_t)0, 320}, a1{(int64_t)1, 320}, on{(int64_t)16, 320};
    for (int64_t k = 0; k < 10; k++)
    {
        LongNumber a2{8 * k + 1, 320}, a3{8 * k + 4, 320}, a4{8 * k + 5, 320}, a5{8 * k + 6, 320};
        Pi = Pi + (4.0_longnum / a2 - 2.0_longnum / a3 - 1.0_longnum / a4 - 1.0_longnum / a5) / a1;
        a1 = a1 * on;
    }
    Pi.PrintLongNumber();

    return EXIT_SUCCESS;
}