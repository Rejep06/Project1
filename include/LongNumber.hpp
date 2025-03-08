#ifndef HEADER_GUARD_LONGNUMBER_HPP_INCLUDED
#define HEADER_GUARD_LONGNUMBER_HPP_INCLUDED

#include <deque>
#include <cstdint>
#include <string>

namespace LongNumberArithmetics
{
    class LongNumber
    {
    private:
        std::deque<uint32_t> nums;
        int sign, precision, precision32;

    public:
        LongNumber(int64_t x, int per);
        LongNumber(int64_t x);
        ~LongNumber() = default;

        LongNumber(long double x, int per);
        LongNumber(const LongNumber &that) = default;

        LongNumber &operator=(const LongNumber &that) = default;
        LongNumber &operator+=(const LongNumber &that);
        LongNumber &operator-=(const LongNumber &that);
        LongNumber &operator*=(const LongNumber &that);
        LongNumber &operator/=(const LongNumber &that);
        LongNumber &operator>>=(uint32_t shift);
        LongNumber &operator<<=(uint32_t shift);

        LongNumber &get_int();
        int get();
        void setPrecision(uint32_t newPrecision);
        LongNumber withPrecision(uint32_t precision) const;
        LongNumber abs() const;
        LongNumber pow(uint32_t power) const;
        LongNumber sqrt() const;

        std::string toString(uint32_t decimalPrecision) const;

        bool operator==(const LongNumber &that);
        bool isZero() const;
        bool operator!=(const LongNumber &that);
        bool operator<(const LongNumber &that);
        bool operator>(const LongNumber &that);
        bool operator<=(const LongNumber &that);
        bool operator>=(const LongNumber &that);

        friend LongNumber operator+(const LongNumber &first, const LongNumber &second);
        friend LongNumber operator-(const LongNumber &first, const LongNumber &second);
        friend LongNumber operator*(const LongNumber &first, const LongNumber &second);
        friend LongNumber operator/(const LongNumber &first, const LongNumber &second);
        friend LongNumber operator>>(const LongNumber &number, const unsigned shift);
        friend LongNumber operator<<(const LongNumber &number, const unsigned shift);

        void PrintLongNumber();
    };

    void fix_precision_literal(int per);
    LongNumber operator""_longnum(long double number);

    LongNumber operator""_longnum(const uint64_t number);

};

#endif