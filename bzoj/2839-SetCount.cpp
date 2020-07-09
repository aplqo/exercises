#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <fstream>
#include <iostream>
using namespace std;
const unsigned int maxn = 1000000;
constexpr unsigned long long M = 1000000007;

template <unsigned long long mod>
class Number
{
public:
    Number() = default;
    Number(const unsigned long long v)
        : val(v % mod)
    {
    }
#define decl(x)                               \
    inline Number operator x(const Number r); \
    inline Number& operator x##=(const Number r) { return *this = *this x r; }
    decl(+);
    decl(*);
    decl(-);
#undef decl
    unsigned long long toull() const
    {
        return val;
    }

private:
    unsigned long long val = 0;
};
#define impl(x)                       \
    template <unsigned long long mod> \
    inline Number<mod> Number<mod>::operator x(const Number<mod> r) { return this->val x r.val; }
impl(*);
impl(+);
#undef impl
template <unsigned long long mod>
inline Number<mod> Number<mod>::operator-(const Number<mod> r)
{
    return this->val + mod - r.val;
}

Number<M> fac[maxn + 1] = { 1, 1 }, ifac[maxn + 1] = { 1, 1 };

template <unsigned long long mod>
Number<mod> quickPow(Number<mod> a, unsigned long long e)
{
    Number<mod> ret = 1;
    for (unsigned long long i = 1; e; i <<= 1)
    {
        if (i & e)
        {
            ret *= a;
            e ^= i;
        }
        a *= a;
    }
    return ret;
}
static void init(const unsigned int n)
{
    for (unsigned long long i = 2; i <= n; ++i)
    {
        fac[i] = fac[i - 1] * i;
        ifac[i] = quickPow(fac[i], M - 2);
    }
}
inline static Number<M> c(const unsigned int n, const unsigned int m)
{
    return fac[n] * ifac[n - m] * ifac[m];
}
Number<M> calc(const unsigned long long n, const unsigned long long k)
{
    Number<M> acc = 0;
    for (unsigned long long i = k; i <= n; ++i)
    {
        const Number<M> t = c(i, k) * c(n, i) * quickPow<M>(2, quickPow<M - 1>(2, n - i).toull());
        if ((i - k) & 0x01)
            acc -= t;
        else
            acc += t;
    }
    return acc;
}

int main()
{
    unsigned int n, k;
    cin >> n >> k;
    init(n);
    cout << calc(n, k).toull() << endl;
    return 0;
}
