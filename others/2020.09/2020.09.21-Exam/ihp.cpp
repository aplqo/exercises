#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
using namespace std;
const unsigned int maxn = 3e7, maxl = 1e7;

const map<unsigned long long, unsigned long long> memF {
    { 18014398241046527, 18014398241046526 },
    { 18014398777917439, 18014398777917438 },
    { 489133282872437279, 489133282872437278 },
    { 162614600673829, 162614543644960 },
    { 1125897758834689, 1125895610826756 },
    { 222915410844073, 222914976835408 },
    { 18004502351257601, 18004501260812292 },
    { 2001600073928249, 2001599355304164 }
};
unsigned int f[maxl + 10] = { 0, 1 };
unsigned long long a[maxn + 10];

void euler(const unsigned int n)
{
    static unsigned int primes[maxl + 10], *pcur = primes;
    static unsigned int mindiv[maxl + 10];
    for (unsigned int i = 2; i <= n; ++i)
    {
        if (!mindiv[i])
        {
            mindiv[i] = i;
            f[i] = i - 1;
            *(pcur++) = i;
        }
        for (unsigned int* j = primes; j < pcur && *j * 1ull * i <= n && *j < mindiv[i]; ++j)
        {
            mindiv[*j * i] = *j;
            f[*j * i] = f[i] * (*j - 1);
        }
        if (static_cast<unsigned long long>(mindiv[i]) * i <= n)
        {
            const unsigned int v = mindiv[i] * i;
            f[v] = f[i] * mindiv[i];
            mindiv[v] = mindiv[i];
        }
    }
}
unsigned long long bf(const unsigned int n, const unsigned long long ma)
{
    euler(ma);
    unsigned long long ret = 0;
    for (const unsigned long long* i = a; i < a + n; ++i)
        ret += f[*i];
    return ret;
}
unsigned long long lookup(const unsigned int n, const unsigned long long)
{
    unsigned long long ret = 0;
    for (const unsigned long long* i = a; i < a + n; ++i)
        ret += memF.at(*i);
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    copy_n(istream_iterator<unsigned long long>(cin), n, a);
    const unsigned long long ma = *max_element(a, a + n);
    if (ma > maxl && n < 6)
        cout << lookup(n, ma) << "\n";
    else
        cout << bf(n, ma) << "\n";
    return 0;
}
