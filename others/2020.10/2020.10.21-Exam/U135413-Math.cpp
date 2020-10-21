#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e7;

unsigned int mindiv[maxn + 1], square[maxn + 1];

static void eular(const unsigned int n)
{
    static unsigned int primes[maxn + 1], *pcur = primes;
    fill(square, square + n + 1, 1);
    for (unsigned long long i = 2; i <= n; ++i)
    {
        if (!mindiv[i])
        {
            mindiv[i] = i;
            *(pcur++) = i;
        }
        for (const unsigned int* j = primes; j < pcur && *j * i <= n && *j < mindiv[i]; ++j)
        {
            mindiv[*j * i] = *j;
            square[*j * i] = square[i];
        }
        if (mindiv[i] * i <= n)
        {
            const unsigned int v = mindiv[i] * i;
            mindiv[v] = mindiv[i];
            if ((i / square[i]) % mindiv[i])
                square[v] = square[i];
            else
                square[v] = square[i] * mindiv[i] * mindiv[i];
        }
    }
}
long long sum(const unsigned int n, const long long m)
{
    long long ret = 0;
    for (unsigned int i = 1; i <= n; ++i)
        ret += static_cast<unsigned long long>(sqrt(m / (i / square[i]))) & 0x01 ? -1 : 1;
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    unsigned long long m;
    cin >> n >> m;
    eular(n);
    cout << sum(n, m) << "\n";
    return 0;
}