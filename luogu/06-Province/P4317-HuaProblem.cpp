#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
constexpr unsigned long long M = 10000007, phi = 9988440;
const unsigned int maxl = 50;

template <unsigned long long mod>
class Number
{
public:
    Number() = default;
    Number(const unsigned long long v)
        : val(v % mod) {};
#define decl(x)                               \
    inline Number operator x(const Number r); \
    inline Number& operator x##=(const Number r) { return *this = *this x r; }
    decl(+);
    decl(*);
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
impl(+);
impl(*);
#undef impl

Number<phi> c[maxl + 1][maxl + 1];

static void init(const unsigned int n)
{
    for (unsigned int i = 0; i <= n; ++i)
        c[i][0] = c[i][i] = 1;
    for (unsigned int i = 1; i <= n; ++i)
        for (unsigned int j = 1; j < i; ++j)
            c[i][j] = c[i - 1][j] + c[i - 1][j - 1];
}
template <unsigned long long mod>
Number<mod> quickPow(Number<mod> a, unsigned long long v)
{
    Number<mod> ret = 1;
    for (unsigned long long i = 1; v; i <<= 1)
    {
        if (v & i)
        {
            ret *= a;
            v ^= i;
        }
        a *= a;
    }
    return ret;
}
Number<M> calc(const unsigned int n, const unsigned int x)
{
    Number<M> ret = x ? x : 1;
    for (unsigned int i = 1; i <= n; ++i)
        ret *= quickPow<M>(i + x, c[n][i].toull());
    return ret;
}
unsigned int popcnt(unsigned long long x)
{
    unsigned int ret = 0;
    for (; x; x -= x & -x)
        ++ret;
    return ret;
}
Number<M> dp(unsigned long long x)
{
    const unsigned int cnt = popcnt(x);
    Number<M> ret = cnt;
    unsigned int acc = 0, p = 1;
    for (unsigned long long i = 1; x; i <<= 1, ++p)
        if (x & i)
        {
            ++acc;
            ret *= calc(p - 1, cnt - acc);
            x ^= i;
        }
    return ret;
}
int main()
{
    unsigned long long x;
    cin >> x;
    init(maxl);
    cout << dp(x).toull() << endl;
    return 0;
}