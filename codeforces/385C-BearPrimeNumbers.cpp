#ifdef APTESt
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 1e6, maxx = 1e7;

unsigned int mindiv[maxx + 10];
unsigned int f[maxx + 10];
unsigned int x[maxn + 10];

static void eular(const unsigned int mx)
{
    static unsigned int primes[maxx + 10], *pcur = primes;
    for (unsigned int i = 2; i <= mx; ++i)
    {
        if (!mindiv[i])
        {
            *(pcur++) = i;
            mindiv[i] = i;
        }
        for (const unsigned int* j = primes; j < pcur && *j <= mindiv[i] && 1ull * (*j) * i <= mx; ++j)
            mindiv[*j * i] = *j;
    }
}
void addNumber(unsigned int x)
{
    while (x > 1)
    {
        ++f[mindiv[x]];
        const unsigned int t = mindiv[x];
        while (mindiv[x] == t)
            x /= mindiv[x];
    }
}
static void getF(const unsigned int n, const unsigned int mx)
{
    for_each(x + 1, x + 1 + n, addNumber);
    partial_sum(f, f + 1 + mx, f);
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n;
    copy_n(istream_iterator<unsigned int>(cin), n, x + 1);
    const unsigned int mx = *max_element(x + 1, x + 1 + n);
    eular(mx);
    getF(n, mx);
    cin >> m;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int l, r;
        cin >> l >> r;
        if (l > mx)
            cout << "0\n";
        else
            cout << f[min(r, mx)] - f[min(l - 1, mx)] << "\n";
    }
    return 0;
}
