/*
* Luogu team T129178: Strange stack
*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
using num_t = long long;
const unsigned int maxn = 1000;
constexpr unsigned long long mod = 1000000007;

class num
{
public:
    num() = default;
    num(num_t x)
        : dat(x % mod)
    {
    }
#define aop(x) \
    num operator x(const num r) const { return num((dat x r.dat) + mod); }
#define assop(x) \
    num& operator x##=(const num r) { return *this = *this x r; }
#define op(x) aop(x) assop(x)
    op(+);
    op(-);
    op(*);
#undef op
#undef aop
#undef assop
    friend ostream& operator<<(ostream& os, const num dat);

private:
    num_t dat = 0;
};
ostream& operator<<(ostream& os, const num dat)
{
    os << dat.dat;
    return os;
}

num rev;
num fact[maxn + 10] = { 1, 1 }, rfact[maxn + 10] = { 1, 1 };
num QuickPow(num x, num_t e)
{
    num ret = 1;
    for (num_t i = 1; e; i <<= 1)
    {
        if (e & i)
        {
            ret *= x;
            e ^= i;
        }
        x *= x;
    }
    return ret;
}
void GetFact(const unsigned int n)
{
    for (unsigned int i = 2; i <= n; ++i)
    {
        fact[i] = fact[i - 1] * i;
        rfact[i] = QuickPow(fact[i], mod - 2);
    }
}
num c(const unsigned int n, const unsigned int m)
{
    return fact[n] * rfact[n - m] * rfact[m];
}

int main()
{
    unsigned int n, k;
    cin >> n;
    GetFact(n);
    k = n / 3;
    num ans = c(n, k);
    for (unsigned int i = 0; i <= k - 1; ++i)
        ans -= c(n, i);
    cout << ans << endl;
    return 0;
}