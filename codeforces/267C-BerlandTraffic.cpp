#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
using namespace std;
const unsigned int maxn = 100, maxm = 5000;
using Real = double;
constexpr Real eps = 1e-9;

struct Edge
{
    unsigned int from, to;
    int c;
} ed[maxm + 1];
unsigned int degree[maxn + 1];
Real equ[maxn + 1][maxn + 1], f[maxn + 1];

static void buildEquation(const unsigned int n, const unsigned int m)
{
    for (unsigned int i = 0; i < n; ++i)
        equ[i][i] = degree[i];
    for (const Edge* i = ed; i < ed + m; ++i)
    {
        if (i->from != n - 1 && i->from != 0)
            --equ[i->from][i->to];
        if (i->to != n - 1 && i->to != 0)
            --equ[i->to][i->from];
    }
    equ[0][0] = equ[n - 1][n - 1] = 1;
    equ[0][n] = 0;
    equ[n - 1][n] = 1;
}
inline bool isZero(const Real v)
{
    return fabs(v) <= eps;
}
void gaussian(const unsigned int n)
{
    for (unsigned int i = 0; i < n; ++i)
    {
        if (isZero(equ[i][i]))
        {
            for (unsigned int j = i + 1; j < n; ++j)
                if (!isZero(equ[j][i]))
                {
                    swap_ranges(equ[j] + i, equ[j] + n + 1, equ[i] + i);
                    break;
                }
        }
        if (isZero(equ[i][i]))
            continue;
        for (unsigned int j = i + 1; j < n; ++j)
        {
            if (isZero(equ[j][i]))
                continue;
            const Real rat = equ[j][i] / equ[i][i];
            for (unsigned int k = i; k <= n; ++k)
                equ[j][k] -= rat * equ[i][k];
        }
    }
    for (unsigned int j = n - 1; j > 0; --j)
    {
        if (isZero(equ[j][j]))
            continue;
        equ[j][n] /= equ[j][j];
        f[j] = equ[j][n];
        for (unsigned int i = 0; i < j; ++i)
            equ[i][n] -= equ[i][j] * equ[j][n];
    }
}
static Real buildSolution(const unsigned int n, const unsigned int m)
{
    Real miv = numeric_limits<Real>::max();
    for (const Edge* i = ed; i < ed + m; ++i)
        if (!isZero(f[i->to] - f[i->from]))
            miv = min(miv, fabs(i->c / (f[i->to] - f[i->from])));
    if (miv != numeric_limits<Real>::max())
        for (unsigned int i = 0; i < n; ++i)
            f[i] *= miv;
    Real ret = 0;
    for (const Edge* i = ed; i < ed + m; ++i)
    {
        if (i->to == n - 1 && f[i->from] < f[n - 1])
            ret += f[n - 1] - f[i->from];
        else if (i->from == n - 1 && f[i->to] < f[n - 1])
            ret += f[n - 1] - f[i->to];
    }
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    for (Edge* i = ed; i < ed + m; ++i)
    {
        cin >> i->from >> i->to >> i->c;
        --(i->from);
        --(i->to);
        ++degree[i->from];
        ++degree[i->to];
    }
    buildEquation(n, m);
    gaussian(n);
    ios_base::sync_with_stdio(true);
    printf("%.5lf\n", buildSolution(n, m));
    for (const Edge* i = ed; i < ed + m; ++i)
        printf("%.5lf\n", f[i->to] - f[i->from]);
    return 0;
}