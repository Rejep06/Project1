#ifndef HEADER_GUARD_LONGNUMBER_HPP_INCLUDED
#define HEADER_GUARD_LONGNUMBER_HPP_INCLUDED

#include <deque>
#include <cstdint>

namespace LongNumberArithmetics
{
    class LongNumber
    {
    private:
        std::deque<uint32_t> nums;
        int sign, percision, percision32;

    public:
        LongNumber(int64_t x, int per);
        ~LongNumber() = default;

        LongNumber(long double x, int per);
        LongNumber(const LongNumber &that) = default;

        LongNumber &operator=(const LongNumber &that) = default;
        LongNumber &operator+=(const LongNumber &that);
        LongNumber &operator-=(const LongNumber &that);
        LongNumber &operator*=(const LongNumber &that);
        LongNumber &operator/=(const LongNumber &that);
        
        LongNumber &get_int();
        // LongNumber &to_int();

        bool operator==(const LongNumber &that);
        bool isZero() const;
        bool operator!=(const LongNumber &that);
        bool operator<(const LongNumber &that);
        bool operator>(const LongNumber &that);

        void PrintLongNumber();
    };

    void fix_precision_literal(int per);
    LongNumber operator""_longnum(long double number);
    LongNumber operator+(const LongNumber &first, const LongNumber &second);
    LongNumber operator-(const LongNumber &first, const LongNumber &second);
    LongNumber operator*(const LongNumber &first, const LongNumber &second);
    LongNumber operator/(const LongNumber &first, const LongNumber &second);

}

#endif