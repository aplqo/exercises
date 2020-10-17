#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 100, maxl = 60, maxs = 1 << 17;
constexpr unsigned int inf = UINT_MAX;

unsigned int factors[maxl + 10], n;
int a[maxn + 10];
unsigned int sel[maxn + 10][maxs + 10], val[maxn + 10][maxs + 10];

static void getPrime(const unsigned int n)
{
    unsigned int cnt = 0;
    for (unsigned int i = 2; i <= n; ++i)
    {
        if (factors[i])
            continue;
        factors[i] = 1 << (cnt++);
        for (unsigned int j = i; j <= n; j += i)
            factors[j] |= factors[i];
    }
}
unsigned int dp(const unsigned int pos, const unsigned int fac)
{
    static bool vis[maxn + 1][maxs + 1];
    if (pos >= n)
        return 0;
    if (vis[pos][fac])
        return val[pos][fac];
    unsigned int ret = inf;
    for (int i = 1; i <= maxl; ++i)
    {
        if ((fac & factors[i]) || abs(i - a[pos]) >= ret)
            continue;
        const unsigned int nv = abs(i - a[pos]) + dp(pos + 1, fac | factors[i]);
        if (nv < ret)
        {
            sel[pos][fac] = i;
            ret = nv;
        }
    }
    vis[pos][fac] = true;
    return val[pos][fac] = ret;
}
void printSolution(const unsigned int n)
{
    unsigned int fac = 0;
    for (unsigned int i = 0; i < n; ++i)
    {
        cout << sel[i][fac] << " ";
        fac |= factors[sel[i][fac]];
    }
    cout.put('\n');
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    cin >> n;
    copy_n(istream_iterator<int>(cin), n, a);
    getPrime(maxl);
    dp(0, 0);
    printSolution(n);
    return 0;
}