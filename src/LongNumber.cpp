#include <LongNumber.hpp>
#include <compare>
#include <bitset>
#include <cmath>
#include <stdexcept>
#include <limits>
#include <iostream>

const uint64_t MAXNUM = (1ull << 32);

using LongNumber = LongNumberArithmetics::LongNumber;

// Конструкторы:

LongNumber::LongNumber(int64_t x, int per)
{
    precision = per;
    precision32 = (per + 31) / 32;
    sign = 1;

    if (x < 0)
    {
        x = -x;
        sign = -1;
    }
    while (x)
    {
        nums.push_front(static_cast<uint32_t>(x % MAXNUM));
        x /= MAXNUM;
    }

    for (uint32_t i = 0; i < precision32; i++)
    {
        nums.push_back(0);
    }
}

LongNumberArithmetics::LongNumber::LongNumber(int64_t x) : LongNumber(x, 0) {}

LongNumber::LongNumber(long double x, int per)
{
    precision = per;
    precision32 = (per + 31) / 32;
    sign = 1;

    if (x < 0)
    {
        x = -x;
        sign = -1;
    }

    uint64_t y = static_cast<uint64_t>(std::trunc(x));
    x -= static_cast<long double>(y);
    if (y == 0)
        nums.push_front(0);
    while (y)
    {
        nums.push_front(static_cast<uint32_t>(y % MAXNUM));
        y /= MAXNUM;
    }

    x -= trunc(x);

    int p = precision;
    while (p > 0)
    {
        x *= 65536;
        uint32_t a = trunc(x);
        nums.push_back(a << 16);

        x -= trunc(x);
        x *= 65536;
        nums[nums.size() - 1] += trunc(x);

        x -= trunc(x);
        p -= 32;
    }

    nums[nums.size() - 1] &= static_cast<uint32_t>(-(1 << (32 - (precision % 32))));
}

// функция для суммы
std::deque<uint32_t> LongSum(std::deque<uint32_t> a, std::deque<uint32_t> b, int per1, int per2)
{
    std::deque<uint32_t> sum;
    auto i = per1, j = per2;
    uint64_t r = 0;
    while (i != 0)
    {
        if (i > j)
        {
            i--;
            sum.push_front(a[i + a.size() - per1]);
        }
        else if (i < j)
        {
            j--;
            sum.push_front(b[j + b.size() - per2]);
        }
        else
        {
            i--;
            j--;
            r += static_cast<uint64_t>(a[i + a.size() - per1]) + b[i + b.size() - per2];
            sum.push_front(static_cast<uint32_t>(r % MAXNUM));
            r /= MAXNUM;
        }
    }
    i = a.size() - per1;
    j = b.size() - per2;

    while (i != 0 || j != 0)
    {
        if (j == 0)
        {
            i--;
            r += static_cast<uint64_t>(a[i]);
            sum.push_front(r % MAXNUM);
            r /= MAXNUM;
        }
        else if (i == 0)
        {
            j--;
            r += static_cast<uint64_t>(b[j]);
            sum.push_front(r % MAXNUM);
            r /= MAXNUM;
        }
        else
        {
            i--;
            j--;
            r += static_cast<uint64_t>(a[i]) + b[j];
            sum.push_front(static_cast<uint32_t>(r % MAXNUM));
            r /= MAXNUM;
        }
    }
    if (r)
        sum.push_front(r);

    return sum;
}

// функция вычитание
std::deque<uint32_t> LongSub(std::deque<uint32_t> a, std::deque<uint32_t> b, int per1, int per2)
{
    std::deque<uint32_t> sub;
    int i = per1, j = per2;
    int64_t r = 0;

    while (i != 0)
    {
        if (i > j)
        {
            i--;
            sub.push_front(a[i + a.size() - per1]);
        }
        else if (i < j)
        {
            j--;
            sub.push_front(static_cast<uint32_t>(r + MAXNUM - b[j + b.size() - per2]));
            r = -1;
        }
        else
        {
            i--;
            j--;
            if (static_cast<int64_t>(a[i + a.size() - per1]) + r < static_cast<int64_t>(b[i + b.size() - per2]))
            {
                r += MAXNUM + static_cast<int64_t>(a[i + a.size() - per1]) - b[i + b.size() - per2];
                sub.push_front(static_cast<uint32_t>(r));
                r = -1;
            }
            else
            {
                r += static_cast<int64_t>(a[i + a.size() - per1]) - b[i + b.size() - per2];
                sub.push_front(static_cast<uint32_t>(r));
                r = 0;
            }
        }
    }

    i = a.size() - per1;
    j = b.size() - per2;

    while (i != 0 || j != 0)
    {
        if (i == 0)
        {
            j--;
            sub.push_front(static_cast<uint32_t>(r + MAXNUM - b[j]));
            r = -1;
        }
        else if (j == 0)
        {
            i--;
            sub.push_front(static_cast<uint32_t>(r + a[i]));
            r = 0;
        }
        else
        {
            i--;
            j--;
            if (static_cast<int64_t>(a[i]) + r < static_cast<int64_t>(b[j]))
            {
                r += MAXNUM + static_cast<int64_t>(a[i]) - b[j];
                sub.push_front(static_cast<uint32_t>(r));
                r = -1;
            }
            else
            {
                r += static_cast<int64_t>(a[i]) - b[j];
                sub.push_front(static_cast<uint32_t>(r));
                r = 0;
            }
        }
    }

    while (sub.size() - std::max(per1, per2) > 1 && sub[0] == 0)
    {
        sub.pop_front();
    }

    return sub;
}

/*
функция сравнение

-1 a > b
 0 a == b
 1 a < b
*/
int LongBig(std::deque<uint32_t> a, std::deque<uint32_t> b, int per1, int per2)
{
    while (a.size() - per1 > 1 && a[0] == 0)
    {
        a.pop_front();
    }

    while (b.size() - per2 > 1 && b[0] == 0)
    {
        b.pop_front();
    }

    if (a.size() - per1 == b.size() - per2)
    {
        std::deque<unsigned int>::size_type i = 0;
        while (i < a.size() && i < b.size())
        {
            if (a[i] > b[i])
            {
                return 1;
            }
            if (a[i] < b[i])
            {
                return -1;
            }
            i+=1;
        }

        if (i < a.size())
        {
            while (a[i] == 0 && i < a.size())
            {
                i++;
            }
            if (i < a.size())
                return 1;
        }
        else
        {
            while (b[i] == 0 && i < b.size())
            {
                i++;
            }
            if (i < b.size())
                return 1;
        }
        return 0;
    }
    else
    {
        if (a.size() - per1 > b.size() - per2)
            return 1;
        else if (a.size() - per1 < b.size() - per2)
            return -1;
        else
            return 0;
    }
}

/*
    Функция определяет вычитать или суммировать
*/
std::deque<uint32_t> LongArithmetics(std::deque<uint32_t> a, int *sgn1, std::deque<uint32_t> b, int sgn2, int per1, int per2)
{
    std::deque<uint32_t> ans;
    if (*sgn1 == sgn2)
    {
        ans = LongSum(a, b, per1, per2);
    }
    else
    {
        int q = LongBig(a, b, per1, per2);
        if (q > 0)
        {
            ans = LongSub(a, b, per1, per2);
        }
        else
        {
            ans = LongSub(b, a, per2, per1);
            *sgn1 = sgn2;
        }
        if (q == 0)
            *sgn1 = 1;
    }
    return ans;
}

// оператор +=
LongNumber &LongNumber::operator+=(const LongNumber &that)
{
    nums = LongArithmetics(nums, &sign, that.nums, that.sign, precision32, that.precision32);
    precision32 = std::max(precision32, that.precision32);
    precision = std::max(precision, that.precision);
    return *this;
}

// оператор -=
LongNumber &LongNumber::operator-=(const LongNumber &that)
{
    nums = LongArithmetics(nums, &sign, that.nums, -that.sign, precision32, that.precision32);
    precision32 = std::max(precision32, that.precision32);
    precision = std::max(precision, that.precision);
    return *this;
}

// функция умножение
std::deque<uint32_t> LongMultiple(std::deque<uint32_t> a, std::deque<uint32_t> b)
{
    std::deque<uint32_t> ans;
    ans.push_front(0);
    for (int i = a.size() - 1; i >= 0; i--)
    {
        uint64_t r = 0;
        std::deque<uint32_t> c;
        for (int j = b.size() - 1; j >= 0; j--)
        {
            r += static_cast<uint64_t>(a[i]) * b[j];
            c.push_front(static_cast<uint32_t>(r % MAXNUM));
            r /= MAXNUM;
        }
        if (r > 0)
        {
            c.push_front(static_cast<uint32_t>(r));
        }
        for (int l = a.size() - 1; l > i; l--)
            c.push_back(0);
        ans = LongSum(ans, c, 0, 0);
    }
    return ans;
}

// оператор *=
LongNumber &LongNumber::operator*=(const LongNumber &that)
{
    nums = LongMultiple(nums, that.nums);
    for (uint32_t j = 0; j < std::min(precision32, that.precision32); j++)
        nums.pop_back();
    precision32 = std::max(precision32, that.precision32);
    precision = std::max(precision, that.precision);
    nums[nums.size() - 1] &= (-(1 << (32 - precision % 32)));
    return *this;
}

// Функция деления
std::deque<uint32_t> LongDiv(std::deque<uint32_t> a, std::deque<uint32_t> b, int per)
{
    std::deque<uint32_t> ans;
    std::deque<uint32_t> a1;
    for (std::deque<unsigned int>::size_type i = 0; i < b.size() - 1; i++)
    {
        a1.push_back(a[i]);
    }

    for (auto i = 0; i < per; i++)
        a.push_back(0);

    for (auto j = b.size() - 1; j < a.size(); j++)
    {
        a1.push_back(a[j]);
        std::deque<uint32_t> q;
        uint64_t r;
        if (a1.size() > b.size())
        {
            r = (static_cast<uint64_t>(a1[0]) * MAXNUM + a1[1]) / b[0];

            uint64_t t = r;
            while (t > 0)
            {
                q.push_front(t % MAXNUM);
                t /= MAXNUM;
            }
        }
        else
        {
            r = a1[0] / b[0];
            q = {(uint32_t)r};
        }

        std::deque<uint32_t> d = LongMultiple(q, b);

        while (LongBig(a1, d, 0, 0) == -1)
        {
            r--;
            q.clear();
            if (r == 0)
                q = {0};

            uint64_t t = r;
            while (t > 0)
            {
                q.push_front(t % MAXNUM);
                t /= MAXNUM;
            }
            d = LongMultiple(q, b);
        }

        while (d[0] == 0 && d.size() > 1)
            d.pop_back();

        a1 = LongSub(a1, d, 0, 0);

        for (auto w : q)
        {
            ans.push_back(w);
        }
    }

    while (ans.size() - per > 1 && ans[0] == 0)
    {
        ans.pop_front();
    }

    return ans;
}

// степень
LongNumber LongNumber::pow(uint32_t power) const
{
    LongNumber res{(int64_t)1, 0}, a = *this;
    while (power)
    {
        if (power & 1)
            res *= a;
        a *= a;
        power >>= 1;
    }
    return res;
}

// корень
LongNumber LongNumber::sqrt() const
{
    if ((*this).sign < 0)
    {
        throw std::invalid_argument("Number is negative");
    }
    LongNumber num1 = 0, num2 = 1;
    while (num1 != num2)
    {
        num1 = num2;
        num2 += *this / num2;
        num2 >>= 1;
    }
    return num2;
}

// оператор ==
bool LongNumber::operator==(const LongNumber &that)
{
    if (this == &that)
        return 1;

    int q = LongBig(nums, that.nums, precision32, that.precision32);
    return (sign == that.sign && q == 0);
}

// метод проверка на ноль
bool LongNumber::isZero() const
{
    for (auto num : nums)
    {
        if (num != 0)
        {
            return false;
        }
    }
    return true;
}

// оператор /=
LongNumber &LongNumber::operator/=(const LongNumber &that)
{
    *this = *this / that;
    return *this;
}

// оператор >>=
LongNumber &LongNumber::operator>>=(uint32_t shift)
{
    *this = *this >> shift;
    return *this;
}

// оператор <<=
LongNumber &LongNumber::operator<<=(uint32_t shift)
{
    *this = *this << shift;
    return *this;
}

// проседура изменение точности
void LongNumber::setPrecision(uint32_t newPrecision)
{
    uint32_t newPrecision32 = newPrecision / 32 + (newPrecision % 32 != 0);
    precision = newPrecision;
    if (newPrecision32 == precision32)
    {
        return;
    }
    int diff = newPrecision32 - precision32;
    precision32 = newPrecision32;

    if (diff > 0)
    {
        nums.insert(nums.end(), diff, 0);
    }
    else
    {
        nums.erase(nums.end() + diff, nums.end());
    }
}

// функция изменения точности
LongNumber LongNumber::withPrecision(uint32_t precision) const
{
    LongNumber result(*this);
    result.setPrecision(precision);
    return result;
}

// модул
LongNumber LongNumber::abs(void) const
{
    LongNumber result = *this;
    result.sign = 1;
    return result;
}

namespace LongNumberArithmetics
{
    // оператор >>
    LongNumber operator>>(const LongNumber &number, unsigned shift)
    {
        LongNumber result{number};
        uint32_t numDigits = shift / 32;
        if (numDigits >= number.nums.size())
        {
            return (0.0_longnum).withPrecision(number.precision);
        }
        result.nums.erase(result.nums.end() - numDigits, result.nums.end());
        shift = shift % 32;
        if (shift)
        {
            uint32_t carry = 0;
            for (std::deque<unsigned int>::size_type i = 0; i < result.nums.size(); i++)
            {
                uint32_t newCarry = result.nums[i] << (32 - shift);
                result.nums[i] >>= shift;
                result.nums[i] |= carry;
                carry = newCarry;
            }
        }
        while (result.nums.size() - result.precision > 0 && result.nums[0] == 0)
        {
            result.nums.pop_front();
        }
        if (result.nums.size() - result.precision32 < 1)
            result.nums.push_front(0);
        return result;
    }

    // оператор +
    LongNumber operator+(const LongNumber &first, const LongNumber &second)
    {
        LongNumber copy{first};

        copy += second;

        return copy;
    }

    // оператор *
    LongNumber operator*(const LongNumber &first, const LongNumber &second)
    {
        LongNumber copy{first};

        copy *= second;

        return copy;
    }

    // оператор -
    LongNumber operator-(const LongNumber &first, const LongNumber &second)
    {
        LongNumber copy{first};

        copy -= second;

        return copy;
    }

    // оператор /
    LongNumber operator/(const LongNumber &lnum, const LongNumber &rnum)
    {
        if (rnum.isZero())
        {
            throw std::invalid_argument("Division by zero");
        }
        if (lnum.isZero())
        {
            return (0.0_longnum).withPrecision(std::max(lnum.precision, rnum.precision));
        }
        uint32_t maxPrecision = std::max(lnum.precision, rnum.precision);
        uint32_t normPrecision = std::max(maxPrecision, 96U);

        LongNumber l_copy = lnum.abs().withPrecision(normPrecision);

        LongNumber r_copy = rnum.abs().withPrecision(normPrecision);

        l_copy <<= l_copy.precision32 * 32;

        LongNumber result(0.0L, normPrecision);
        result.sign = lnum.sign * rnum.sign;
        LongNumber rem(0.0L, normPrecision);

        for (int i = static_cast<int>(l_copy.nums.size()) * 32 - 1; i >= 0; --i)
        {
            rem <<= 1;
            uint32_t digitIndex = i / 32;
            uint32_t bitIndex = i % 32;
            bool bit = (l_copy.nums[l_copy.nums.size() - 1 - digitIndex] >> bitIndex) & 1;
            rem.nums[rem.nums.size() - 1] |= (bit ? 1 : 0);
            if (rem >= r_copy)
            {
                rem -= r_copy;
                if (digitIndex >= result.nums.size())
                {
                    result.nums.resize(digitIndex + 1, 0);
                }
                result.nums[result.nums.size() - 1 - digitIndex] |= (1U << bitIndex);
            }
        }

        result.setPrecision(maxPrecision);
        while (result.nums.size() - result.precision32 > 0 && result.nums[0] == 0)
        {
            result.nums.pop_front();
        }
        if (result.nums.size() - result.precision32 < 1)
            result.nums.push_front(0);
        return result;
    }

    // оператор <<
    LongNumber operator<<(const LongNumber &number, unsigned shift)
    {
        LongNumber result(number);
        uint32_t newDigits = shift / 32;
        if (newDigits)
        {
            result.nums.insert(result.nums.end(), newDigits, 0);
        }
        shift = shift % 32;
        if (shift)
        {
            uint32_t carry = 0;
            for (int i = static_cast<int>(result.nums.size()) - newDigits - 1; i >= 0; i--)
            {
                uint32_t newCarry = result.nums[i] >> (32 - shift);
                result.nums[i] <<= shift;
                result.nums[i] |= carry;
                carry = newCarry;
            }
            if (carry)
            {
                result.nums.push_front(carry);
            }
        }
        return result;
    }
}

// целая часть
LongNumber &LongNumberArithmetics::LongNumber::get_int()
{
    for (auto i = nums.size() - precision32; i < nums.size(); i++)
    {
        nums[i] = 0;
    }
    return *this;
}

// старший цифр
int LongNumberArithmetics::LongNumber::get()
{
    return nums[0];
}

// оператор !=
bool LongNumber::operator!=(const LongNumber &that)
{
    return !(*this == that);
}

// оператор <
bool LongNumber::operator<(const LongNumber &that)
{
    int q = LongBig(nums, that.nums, precision32, that.precision32);
    if (sign == that.sign)
    {
        if (sign > 0)
        {
            return (q == -1);
        }
        else
        {
            return (q == 1);
        }
    }
    else
    {
        return (sign < that.sign);
    }
}

// оператор <=
bool LongNumber::operator<=(const LongNumber &that)
{
    return (*this < that || *this == that);
}

// оператор >
bool LongNumber::operator>(const LongNumber &that)
{
    int q = LongBig(nums, that.nums, precision32, that.precision32);
    if (sign == that.sign)
    {
        if (sign > 0)
        {
            return (q == 1);
        }
        else
        {
            return (q == -1);
        }
    }
    else
    {
        return (sign > that.sign);
    }
}

// оператор >=
bool LongNumber::operator>=(const LongNumber &that)
{
    return (*this > that || *this == that);
}

// Тестовая печать на экран
void LongNumber::PrintLongNumber()
{
    if (sign == -1)
        std::cout << '-';
    for (std::deque<unsigned int>::size_type i = 0; i < nums.size(); i++)
    {
        if (i == nums.size() - precision32)
            std::cout << '.';
        std::cout << nums[i]; //<< ' ';
    }
    std::cout << '\n'
              << precision << '\n';
}

int literal_precision = 32;

// фиксация точности для литерала
void LongNumberArithmetics::fix_precision_literal(int per)
{
    literal_precision = per;
}

// Literal
LongNumber LongNumberArithmetics::operator""_longnum(long double number)
{
    return LongNumber(number, literal_precision);
}

// LongNumber LongNumberArithmetics::operator""_longnum(const uint64_t number)
// {
//     return LongNumber{(int64_t)number, 0};
// }