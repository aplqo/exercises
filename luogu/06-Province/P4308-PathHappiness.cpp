#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
using namespace std;
const unsigned int maxn = 100, maxm = 1000;
typedef long double real;
constexpr real ninf = numeric_limits<real>::lowest();

struct Edge
{
    unsigned int from, to;
} ed[maxm + 10];
real val[maxn + 10];
real po[maxn + 10], path[maxn + 10][maxn + 10][maxn + 10], cycle[maxn + 10];

static void initPow(const unsigned int n, const real p)
{
    po[0] = 1;
    for (unsigned int i = 1; i <= n; ++i)
        po[i] = po[i - 1] * p;
}
void dpPath(const unsigned int n, const unsigned int m)
{
    for (unsigned int i = 0; i <= n; ++i)
        for (unsigned int j = 0; j <= n; ++j)
        {
            fill(path[i][j], path[i][j] + n + 1, ninf);
            path[i][i][1] = 0;
        }
    for (const Edge* i = ed; i < ed + m; ++i)
        path[i->from][i->to][1] = max(path[i->from][i->to][1], val[i->to] * po[1]);
    for (unsigned int k = 2; k <= n; ++k)
        for (unsigned int v = 1; v <= n; ++v)
            for (const Edge* i = ed; i < ed + m; ++i)
                path[v][i->to][k] = max(path[v][i->to][k], path[v][i->from][k - 1] + po[k] * val[i->to]);
}
void dpCycle(const unsigned int n)
{
    for (unsigned int i = 1; i <= n; ++i)
        for (unsigned int k = 1; k <= n; ++k)
            cycle[i] = max(cycle[i], path[i][i][k] / (1 - po[k]));
}
real findMax(const unsigned int n, const unsigned int v0)
{
    real ret = 0;
    for (unsigned int i = 1; i <= n; ++i)
        for (unsigned int k = 1; k <= n; ++k)
            ret = max(ret, path[v0][i][k] + po[k] * cycle[i]);
    return ret + val[v0];
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m, v0;
    real p;
    cin >> n >> m;
    copy_n(istream_iterator<real>(cin), n, val + 1);
    cin >> v0 >> p;
    for (Edge* i = ed; i < ed + m; ++i)
        cin >> i->from >> i->to;
    initPow(n, p);
    dpPath(n, m);
    dpCycle(n);
    cout << fixed << setprecision(1) << findMax(n, v0) << "\n";
    return 0;
}