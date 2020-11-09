#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <unordered_map>
using namespace std;
const unsigned int maxn = 6, maxp = 4, maxm = 150;

long long po[maxm + 10][maxp + 10];
long long k[maxn + 10], p[maxn + 10], m;
unsigned long long ans = 0;
unordered_map<long long, unsigned int> mp;

static void initPow(const unsigned int m)
{
    for (unsigned int i = 1; i <= m; ++i)
    {
        po[i][0] = 1;
        for (unsigned int j = 1; j <= maxp; ++j)
            po[i][j] = po[i][j - 1] * i;
    }
}
void dfsPre(const unsigned int pos, const unsigned int dep, const long long acc)
{
    if (!dep)
    {
        auto it = mp.find(acc);
        if (it == mp.end())
            it = mp.insert({ acc, 0 }).first;
        ++(it->second);
        return;
    }
    for (unsigned int i = 1; i <= m; ++i)
        dfsPre(pos + 1, dep - 1, acc + k[pos] * po[i][p[pos]]);
}
void dfsSuf(const unsigned int pos, const unsigned int dep, const long long acc)
{
    if (!dep)
    {
        if (const auto it = mp.find(-acc); it != mp.end())
            ans += it->second;
        return;
    }
    for (unsigned int i = 1; i <= m; ++i)
        dfsSuf(pos - 1, dep - 1, acc + k[pos] * po[i][p[pos]]);
}
unsigned long long quickPow(unsigned long long a, unsigned long long e)
{
    unsigned long long ret = 1;
    for (; e; e >>= 1)
    {
        if (e & 0x01)
            ret *= a;
        a *= a;
    }
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n >> m;
    for (unsigned int i = 0; i < n; ++i)
        cin >> k[i] >> p[i];
    initPow(m);
    mp.reserve(quickPow(m, (n + 1) / 2));
    dfsPre(0, n / 2, 0);
    dfsSuf(n - 1, n - n / 2, 0);
    cout << ans << "\n";
    return 0;
}