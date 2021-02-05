#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
const unsigned int k = 4, maxn = 2e5;
constexpr unsigned long long mod = 1000000007;
constexpr unsigned long long inv2 = 500000004, inv3 = 333333336, inv6 = 166666668;

class Number
{
public:
#define decl(x)                                                                                                  \
    inline friend Number operator x(const Number l, const Number r) { return Number { (l.val x r.val) % mod }; } \
    inline void operator x##=(const Number r) { val = (val x r.val) % mod; }
    decl(+);
    decl(*);
#undef decl
    inline friend Number operator-(const Number l, const Number r)
    {
        return Number { l.val >= r.val ? l.val - r.val : l.val + mod - r.val };
    }
    inline void operator-=(const Number r) { val = val >= r.val ? val - r.val : val + mod - r.val; }
    inline Number operator-() const { return Number { val ? mod - val : 0 }; }
    inline friend std::ostream& operator<<(std::ostream& os, const Number v)
    {
        return os << v.val;
    }

    unsigned long long val;
};
class Polynomial
{
public:
    inline void operator+=(const Polynomial& r)
    {
        for (unsigned int i = 0; i <= k; ++i)
            val[i] += r.val[i];
    }
    inline Polynomial operator*(const Number v) const
    {
        Polynomial ret;
        for (unsigned int i = 0; i <= k; ++i)
            ret.val[i] = val[i] * v;
        return ret;
    }
    Number eval(const Number x) const
    {
        Number ret = val[k];
        for (unsigned int i = k; i; --i)
            ret = ret * x + val[i - 1];
        return ret;
    }
    inline Polynomial operator-() const
    {
        Polynomial ret;
        for (unsigned int i = 0; i <= k; ++i)
            ret.val[i] = -val[i];
        return ret;
    }
    Polynomial shiftLeft(const unsigned int n) const;

    Number val[k + 1];
};

namespace FTree
{
#define lowbit(x) ((x) & -(x))
    Polynomial val[maxn + 10];

    void prefixAdd(unsigned int pos, const unsigned int n, const Polynomial& d)
    {
        for (; pos <= n; pos += lowbit(pos))
            val[pos] += d;
    }
    Polynomial prefixSum(unsigned int pos)
    {
        Polynomial ret {};
        for (; pos; pos -= lowbit(pos))
            ret += val[pos];
        return ret;
    }

#undef lowbit
}
const unsigned int c[k + 1][k + 1] {
    { 1 },
    { 1, 1 },
    { 1, 2, 1 },
    { 1, 3, 3, 1 },
    { 1, 4, 6, 4, 1 }
};
Polynomial shift[maxn + 10] { { 0, inv3, inv2, inv6 } };

Polynomial Polynomial::shiftLeft(const unsigned int n) const
{
    Polynomial ret {};
    for (unsigned int i = 0; i <= k; ++i)
    {
        Number acc { 1 };
        for (unsigned int j = i; j <= k; ++j)
        {
            ret.val[i] += Number { c[j][i] } * val[j] * acc;
            acc *= Number { mod - n };
        }
    }
    return ret;
}
static void initShift(const unsigned int n)
{
    for (unsigned int i = 1; i <= n; ++i)
        shift[i] = shift[0].shiftLeft(i);
}
void add(const unsigned int l, const unsigned int r, const unsigned int n, const Number delt)
{
    static const auto add = [](const unsigned int l, const unsigned int r, const unsigned int n, const Number delt) {
        static const auto preifxAdd = [](const unsigned int pos, const unsigned int n, const Number delt) {
            FTree::prefixAdd(pos, n, shift[pos - 1] * delt);
        };
        preifxAdd(l, n, delt);
        preifxAdd(r + 1, n, -delt);
    };
    add(1, 1, n, Number { r + 1 - l } * delt);
    add(l + 1, r + 1, n, -delt);
    add(n + 2 - r, n + 2 - l, n, -delt);
}
Number query(const unsigned int l, const unsigned int r)
{
    return FTree::prefixSum(r).eval(Number { r }) - FTree::prefixSum(l - 1).eval(Number { l - 1 });
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    unsigned int n, m;
    std::cin >> n >> m;
    initShift(n);
    for (unsigned int i = 1; i <= n; ++i)
    {
        unsigned int a;
        std::cin >> a;
        add(i, i, n, Number { a });
    }
    for (unsigned int i = m; i; --i)
    {
        unsigned int op, l, r;
        std::cin >> op >> l >> r;
        if (op == 1)
        {
            unsigned int d;
            if (l > r)
                std::swap(l, r);
            std::cin >> d;
            add(l, r, n, Number { d });
        }
        else
            std::cout << query(l, r) << "\n";
    }
    return 0;
}