#ifdef APDEBUG
#undef _MSC_VER
#include "debug_tools/program.h"
#endif
#include <cmath>
#include <cstdint>
#include <iostream>
#include <utility>
using namespace std;
using num_t = uint_fast64_t;
using exp_t = uint_fast64_t;
using pair_t = pair<num_t, exp_t>; // first: div second: exp
const int maxl = 45, maxq = 1e6;
const num_t mod = 9901;

num_t po[maxl + 10], ans = 1;
num_t primes[maxq + 10], mindiv[maxq + 10], *pc = primes;
num_t a;
exp_t b;
pair_t p[maxq + 10], *cur = p;

inline num_t QuickPow(exp_t e)
{
    num_t ret = 1;
    exp_t t = 1;
    for (exp_t i = 0; e; ++i, t <<= 1)
        if (e & t)
        {
            ret = (ret * po[i]) % mod;
            e ^= t;
        }
    return ret;
}
inline void getPow(num_t p, exp_t val)
{
    po[0] = p % mod;
    for (exp_t i = 1; i <= val; ++i)
        po[i] = (po[i - 1] * po[i - 1]) % mod;
}
num_t sum(num_t p, exp_t e)
{
    if (e == 0)
        return 0;
    const num_t ex = QuickPow(e >> 1), v = sum(p, e >> 1) * ((ex + 1) % mod);
    if (e & 0x01)
        return (v % mod + (ex * ex) % mod * (p % mod)) % mod;
    else
        return v % mod;
}
inline void euler(num_t en)
{
    for (num_t i = 2; i <= en; ++i)
    {
        if (!mindiv[i])
        {
            *(pc++) = i;
            mindiv[i] = i;
        }
        for (num_t* p = primes; p < pc && *p <= mindiv[i] && (*p) * i <= en; ++p)
            mindiv[*p * i] = *p;
    }
}
void getDiv(num_t a)
{
    for (num_t* i = primes; a > 1 && i < pc; ++i)
        if (!(a % *i))
        {
            *cur = make_pair(*i, exp_t(0));
            while (!(a % *i))
            {
                a /= *i;
                ++(cur->second);
            }
            ++cur;
        }
    if (a > 1)
        *(cur++) = make_pair(a, exp_t(1));
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> a >> b;
    if (a == 0 || b == 0)
    {
        cout << "1" << endl;
        return 0;
    }
    euler(ceil(sqrtl(a)) + 1);
    getDiv(a);
    for (pair_t* i = p; i < cur; ++i)
        i->second *= b;
    for (pair_t* i = p; i < cur; ++i)
    {
        getPow(i->first, ceil(log2l(i->second)) + 2);
        ans = (ans * (sum(i->first, i->second) + 1)) % mod;
    }
    cout << ans << endl;
    return 0;
}
