#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
using namespace std;
using num_t = uint_fast32_t;
constexpr num_t inf = numeric_limits<num_t>::max();
const int maxd = 1e6, maxq = 65536;

num_t primes[maxq + 10], mindiv[maxq + 10], *pc = primes;
bool notPrime[maxd + 10];

inline void euler(const num_t n)
{
    static num_t lst = 2;
    for (num_t i = lst; i <= n; ++i)
    {
        if (!mindiv[i])
        {
            mindiv[i] = i;
            *(pc++) = i;
        }
        for (num_t* j = primes; j < pc && *j * i <= maxq && *j <= mindiv[i]; ++j)
            mindiv[*j * i] = *j;
    }
    lst = n + 1;
}
inline void eratosh(const num_t l, const num_t r)
{
    for (num_t* p = primes; p < pc && (*p) * (*p) <= r; p++)
    {
        const num_t pr = *p;
        for (num_t i = (l + pr - 1) / pr * pr; i <= r; i += pr)
            if (i != *p)
                notPrime[i - l] = true;
    }
}
inline void solve(const num_t l, const num_t r)
{
    euler(ceil(sqrtl(r)) + 1);
    fill(notPrime, notPrime + r - l + 1, false);
    if (l == 1)
        notPrime[0] = true;
    eratosh(l, r);
    num_t n[3] = { 0, 0, inf }, d[3] = { 0, 0, 0 };
    num_t lst = find(notPrime, notPrime + r - l + 1, false) - notPrime;
    for (num_t i = lst + 1; i < r - l + 1; ++i)
    {
        if (notPrime[i])
            continue;
        if (i - lst > d[2])
        {
            d[0] = lst;
            d[1] = i;
            d[2] = i - lst;
        }
        if (i - lst < n[2])
        {
            n[0] = lst;
            n[1] = i;
            n[2] = i - lst;
        }
        lst = i;
    }
    if (n[2] == inf)
    {
        cout << "There are no adjacent primes." << endl;
        return;
    }
    cout << n[0] + l << "," << n[1] + l << " are closest, ";
    cout << d[0] + l << "," << d[1] + l << " are most distant." << endl;
}

int main()
{
    ios_base::sync_with_stdio(false);
    while (cin)
    {
        num_t l, r;
        cin >> l >> r;
        if (cin.fail())
            return 0;
        solve(l, r);
    }
    return 0;
}