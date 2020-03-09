#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cstdio>
#include <iostream>
#include <map>
#include <utility>
using namespace std;
using num_t = long long;
using pair_t = pair<num_t, num_t>; // first: k,second: p
const int maxn = 6, maxp = 4, maxm = 150;

num_t po[maxm + 10][maxp + 10];
pair_t f[maxn + 10];
map<num_t, unsigned int> half;
unsigned int ans = 0;

inline void GetPow(const num_t m)
{
    for (num_t i = 1; i <= m; ++i)
    {
        po[i][0] = 1;
        for (unsigned int j = 1; j <= maxp; ++j)
            po[i][j] = po[i][j - 1] * i;
    }
}
inline num_t eval(const unsigned int pos, const num_t x)
{
    return f[pos].first * po[x][f[pos].second];
}
void dfs1(const unsigned int len, const num_t m, const num_t val)
{
    static unsigned int p = 0;
    if (!len)
    {
        auto it = half.find(val);
        if (it == half.end())
            half[val] = 1;
        else
            ++(it->second);
        return;
    }
    const unsigned int c = p;
    for (num_t i = 1; i <= m; ++i)
    {
        ++p;
        dfs1(len - 1, m, val + eval(c, i));
        --p;
    }
}
void dfs2(const unsigned int pos, const unsigned int end, const num_t m, const num_t val)
{
    if (pos == end)
    {
        auto it = half.find(-val);
        if (it != half.end())
            ans += it->second;
        return;
    }
    for (num_t i = 1; i <= m; ++i)
        dfs2(pos + 1, end, m, val + eval(pos, i));
}
int main()
{
    int n;
    num_t m;
    scanf("%d%lld", &n, &m);
    for (pair_t* i = f; i < f + n; ++i)
        scanf("%lld%lld", &(i->first), &(i->second));
    GetPow(m);
    dfs1(n / 2, m, 0);
    dfs2(n / 2, n, m, 0);
    printf("%u", ans);
    return 0;
}
