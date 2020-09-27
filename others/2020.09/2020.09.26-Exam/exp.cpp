#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iomanip>
#include <iostream>
using namespace std;
const unsigned int maxn = 200, maxb = 9, maxk = 1 << maxb, maxl = 64;
const unsigned int msk = maxk - 1;
typedef long double real;

real pro[maxn + 10][maxn + maxl + 10][maxk + 10];

#define Bit(x, i) (((x) >> (i)) & 0x01)
inline unsigned int findSame(unsigned int x)
{
    const unsigned int b = Bit(x, maxb - 1);
    unsigned int ret = 0;
    x >>= maxb;
    if (!x)
        return 0;
    while ((x & 0x01) == b)
    {
        ++ret;
        x >>= 1;
    }
    return ret;
}
static void dpProbably(const unsigned int n, const unsigned int x, const real p)
{
    pro[0][findSame(x)][x & msk] = 1;
    for (unsigned int i = 0; i < n; ++i)
        for (unsigned int j = 0; j <= n + maxl; ++j)
            for (unsigned int k = 0; k < maxk; ++k)
            {
                pro[i + 1][Bit(k, maxb - 1) == Bit(k, maxb - 2) ? j + 1 : 0][(k << 1) & msk] += p * pro[i][j][k];
                const unsigned int s = k + 1;
                if (Bit(s, maxb - 1) == Bit(k, maxb - 1))
                    pro[i + 1][j][s & msk] += (1 - p) * pro[i][j][k];
                else
                    pro[i + 1][Bit(s, maxb - 1) ? 0 : j][s & msk] += (1 - p) * pro[i][j][k];
            }
}
#undef Bit

inline unsigned int cntz(unsigned int x)
{
    unsigned int ret = 0;
    while (!(x & 0x01))
    {
        ++ret;
        x >>= 1;
    }
    return ret;
}
real expect(const unsigned int n)
{
    real ret = 0;
    for (unsigned int j = 0; j <= n + maxl; ++j)
    {
        ret += pro[n][j][0] * (maxb + j);
        for (unsigned int k = 1; k < maxk; ++k)
            ret += pro[n][j][k] * cntz(k);
    }
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int x, n, p;
    cin >> x >> n >> p;
    dpProbably(n, x, p / 100.0);
    cout << fixed << setprecision(10) << expect(n) << "\n";
    return 0;
}
