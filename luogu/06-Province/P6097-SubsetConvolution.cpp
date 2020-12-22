#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxln = 20, maxn = 1u << maxln;
constexpr unsigned long long mod = 1e9 + 9;

unsigned int fa[maxln + 10][maxn], fb[maxln + 10][maxn], fs[maxln + 10][maxn + 10];
unsigned int pcnt[maxn + 10];

inline unsigned int fma(const unsigned long long v, const unsigned long long a, const unsigned long long b)
{
    return (v + (a * b) % mod) % mod;
}
static void initPopcnt(const unsigned int n)
{
    for (unsigned int i = 1; i < n; ++i)
    {
        const unsigned int lb = i & -i;
        pcnt[i] = pcnt[i ^ lb] + 1;
    }
}

template <unsigned int v>
void fmt(unsigned int dest[], const unsigned int lgn)
{
    const unsigned int msk = 1u << lgn;
    for (unsigned int i = 0; i < lgn; ++i)
    {
        const unsigned int b = 1u << i;
        for (unsigned int j = 0; j <= msk; ++j)
            if (b & j)
                dest[j] = fma(dest[j], dest[j ^ b], v);
    }
}
void subsetConvolution(const unsigned int lgn)
{
    const unsigned int n = 1u << lgn;
    for (unsigned int i = 0; i <= lgn; ++i)
    {
        fmt<1>(fa[i], lgn);
        fmt<1>(fb[i], lgn);
    }
    for (unsigned int i = 0; i <= lgn; ++i)
    {
        for (unsigned int j = 0; j <= i; ++j)
            for (unsigned int k = 0; k < n; ++k)
                fs[i][k] = fma(fs[i][k], fa[j][k], fb[i - j][k]);
        fmt<mod - 1>(fs[i], lgn);
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int lgn, n;
    cin >> lgn;
    n = 1u << lgn;
    initPopcnt(n);
    for (unsigned int i = 0; i < n; ++i)
        cin >> fa[pcnt[i]][i];
    for (unsigned int i = 0; i < n; ++i)
        cin >> fb[pcnt[i]][i];
    subsetConvolution(lgn);
    for (unsigned int i = 0; i < n; ++i)
        cout << fs[pcnt[i]][i] << " ";
    cout.put('\n');
    return 0;
}