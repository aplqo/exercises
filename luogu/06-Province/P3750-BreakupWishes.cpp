#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;
typedef unsigned long long num;
const unsigned int maxn = 100000;
constexpr num mod = 100003;

class Number
{
public:
    Number() = default;
    Number(const num v)
        : val(v % mod)
    {
    }
#define assop(x)                                                     \
    inline friend Number operator x(const Number l, const Number r); \
    inline Number& operator x##=(const Number r) { return *this = *this x r; }
    assop(+);
    assop(*);
#undef assop
    inline friend Number operator/(const Number l, const num r);
    inline friend Number operator^(Number a, unsigned int e);
    inline friend ostream& operator<<(ostream& os, const Number v);
    inline Number& operator/=(const num r) { return *this = *this / r; }

    static void initInv(const unsigned int n)
    {
        for (unsigned int i = 2; i <= n; ++i)
            inv[i] = (Number(i) ^ (mod - 2)).val;
    }

private:
    num val = 0;
    static num inv[maxn + 1];
};
num Number::inv[maxn + 1] = { 0, 1 };
#define impl(x) \
    inline Number operator x(const Number l, const Number r) { return l.val x r.val; }
impl(+);
impl(*);
#undef impl
inline Number operator/(const Number l, const num r)
{
    return l.val * Number::inv[r];
}
inline Number operator^(Number a, unsigned int e)
{
    Number ret = 1;
    for (unsigned int i = 1; e; i <<= 1)
    {
        if (e & i)
        {
            ret *= a;
            e ^= i;
        }
        a *= a;
    }
    return ret;
}
inline ostream& operator<<(ostream& os, const Number v)
{
    return os << v.val;
}

vector<unsigned int> fact[maxn + 1];
Number f[maxn + 1];
bool lamp[maxn + 1];

void initFactor(const unsigned int n)
{
    for (unsigned int i = 1; i <= n; ++i)
        for (unsigned int j = i; j <= n; j += i)
            fact[j].push_back(i);
}
unsigned int minSwitch(const unsigned int n)
{
    unsigned int ret = 0;
    for (unsigned int i = n; i > 0; --i)
    {
        if (!lamp[i])
            continue;
        ++ret;
        for (auto j : fact[i])
            lamp[j] = !lamp[j];
    }
    return ret;
}
Number factor(const unsigned int n)
{
    Number ret = 1;
    for (unsigned int i = 2; i <= n; ++i)
        ret *= i;
    return ret;
}
Number dp(const unsigned int n, const unsigned int k, const unsigned int mins)
{
    Number ret = 0;
    f[n] = 1;
    for (unsigned int i = n - 1; i > 0; --i)
        f[i] = (f[i + 1] + 1) * (n - i) / i + 1;
    ret = accumulate(f + k + 1, f + mins + 1, Number(0));
    return (ret + k) * factor(n);
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, k;
    cin >> n >> k;
    for (unsigned int i = 1; i <= n; ++i)
    {
        unsigned int t;
        cin >> t;
        lamp[i] = t;
    }
    Number::initInv(n);
    initFactor(n);
    const unsigned int ms = minSwitch(n);
    cout << (k >= ms ? factor(n) * ms : dp(n, k, ms)) << endl;
    return 0;
}