#include <LongNumber.hpp>
#include <cmath>
#include <cstdint>
#include <deque>
#include <iostream>

const uint64_t MAXNUM = (1ull << 32);

using LongNumber = LongNumberArithmetics::LongNumber;

LongNumber::LongNumber(int64_t x, int per)
{
    percision = per;
    percision32 = (per + 31) / 32;
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

    for (int i = 0; i < percision32; i++)
    {
        nums.push_back(0);
    }
}



LongNumber::LongNumber(long double x, int per)
{
    percision = per;
    percision32 = (per + 31) / 32;
    sign = 1;

    if (x < 0)
    {
        x = -x;
        sign = -1;
    }

    uint64_t y = static_cast<uint64_t>(std::trunc(x));
    x -= static_cast<long double>(y);

    while (y)
    {
        nums.push_front(static_cast<uint32_t>(y % MAXNUM));
        y /= MAXNUM;
    }

    uint64_t r = 1;
    while (x - trunc(x) > 0 && x * 100.0 < 4294967296.0)
    {
        x *= 10.0;
        r *= 10;
    }

    uint64_t a = static_cast<uint64_t>(trunc(x));

    int p = percision;
    while (p > 0)
    {
        a *= (uint64_t)4294967296;
        y = a / r;
        nums.push_back(y);
        a %= r;
        p -= 32;
    }

    nums[nums.size() - 1] &= static_cast<uint32_t>(-(1 << (32 - (percision % 32))));
}

LongNumber::~LongNumber()
{
    percision32 = 0;
    percision = 0;
    nums.clear();
}

LongNumber::LongNumber(const LongNumber &that)
{
    percision32 = that.percision32;
    percision = that.percision;
    nums = that.nums;
}

LongNumber &LongNumber::operator=(const LongNumber &that)
{
    percision32 = that.percision32;
    percision = that.percision;
    nums = that.nums;
    return *this;
}

std::deque<uint32_t> LongSum(std::deque<uint32_t> a, std::deque<uint32_t> b, int per1, int per2)
{
    std::deque<uint32_t> sum;
    int i = per1, j = per2;
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
        int i = 0;
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
            i++;
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

LongNumber &LongNumber::operator+(const LongNumber &that)
{
    nums = LongArithmetics(nums, &sign, that.nums, that.sign, percision32, that.percision32);
    percision32 = std::max(percision32, that.percision32);
    percision = std::max(percision, that.percision);
    return *this;
}

LongNumber &LongNumber::operator-(const LongNumber &that)
{
    nums = LongArithmetics(nums, &sign, that.nums, -that.sign, percision32, that.percision32);
    percision32 = std::max(percision32, that.percision32);
    percision = std::max(percision, that.percision);
    return *this;
}

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

LongNumber &LongNumber::operator*(const LongNumber &that)
{
    nums = LongMultiple(nums, that.nums);
    for (int j = 0; j < std::min(percision32, that.percision32); j++)
        nums.pop_back();
    percision32 = std::max(percision32, that.percision32);
    percision = std::max(percision, that.percision);
    nums[nums.size() - 1] &= (-(1 << (32 - percision % 32)));
    return *this;
}

std::deque<uint32_t> LongDiv(std::deque<uint32_t> a, std::deque<uint32_t> b, int per)
{
    std::deque<uint32_t> ans;
    std::deque<uint32_t> a1;
    for (int i = 0; i < b.size() - 1; i++)
    {
        a1.push_back(a[i]);
    }

    for (int i = 0; i < per; i++)
        a.push_back(0);

    for (int j = b.size() - 1; j < a.size(); j++)
    {
        a1.push_back(a[j]);
        std::deque<uint32_t> q;
        if (a1.size() > b.size())
        {
            uint32_t r = static_cast<uint32_t>((static_cast<uint64_t>(a1[0]) * MAXNUM + a1[1]) / b[0]);
            q = {r};
        }
        else
        {
            q = {a1[0] / b[0]};
        }

        std::deque<uint32_t> d = LongMultiple(q, b);

        if (LongBig(a1, d, 0, 0) == -1)
        {
            uint32_t r = q[0] - 1;
            q = {r};
            d = LongMultiple(q, b);
        }

        while (d[0] == 0 && d.size() > 1)
            d.pop_back();

        a1 = LongSub(a1, d, 0, 0);

        ans.push_back(q[0]);
    }

    return ans;
}

bool LongNumber::operator==(const LongNumber &that)
{
    if (this == &that)
        return 1;

    int q = LongBig(nums, that.nums, percision32, that.percision32);
    return (sign == that.sign && q == 0);
}

bool LongNumber::isZero() const {
    for (uint32_t num : nums) {
        if (num != 0) {
            return false;
        }
    }
    return true;
}


LongNumber &LongNumber::operator/(const LongNumber &that)
{
    if (that.isZero())
    {
        throw std::runtime_error("Diving to zero!");
    }
    std::deque<uint32_t> nums2 = that.nums;
    for (int i = 0; i < abs(percision32 - that.percision32); i++)
    {
        if (percision32 < that.percision)
            nums.push_back(0);
        else
            nums2.push_back(0);
    }
    percision32 = std::max(percision32, that.percision32);
    // надо проверить деление на ноль!
    nums = LongDiv(nums, nums2, percision32);
    sign = sign * that.sign;
    return *this;
}

bool LongNumber::operator!=(const LongNumber &that)
{
    return !(*this == that);
}

bool LongNumber::operator<(const LongNumber &that)
{
    int q = LongBig(nums, that.nums, percision32, that.percision32);
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

bool LongNumber::operator>(const LongNumber &that)
{
    int q = LongBig(nums, that.nums, percision32, that.percision32);
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

void LongNumber::PrintLongNumber()
{
    if (sign == -1)
        std::cout << '-';
    for (int i = 0; i < nums.size(); i++)
    {
        if (i == nums.size() - percision32)
            std::cout << '.';
        std::cout << nums[i] << ' ';
    }
    std::cout << '\n'
              << percision << '\n';
}

LongNumber LongNumberArithmetics::operator""_longnum(long double number)
{
    return LongNumber(number, 0);
}
