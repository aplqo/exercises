#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
const unsigned int maxn = 50000, maxk = 150;
constexpr unsigned int mod = 10007;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxn * 2 + 19];
const Edge* head[maxn + 10];
unsigned int fac[maxn + 10], s[maxk + 10][maxk + 10], ans[maxn + 10];
unsigned int sum[maxn + 10][maxk + 10], k;

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
    new (cur) Edge { from, head[to] };
    head[to] = cur++;
}
static void initFactor(const unsigned int n)
{
    fac[0] = 1;
    for (unsigned int i = 1; i <= n; ++i)
        fac[i] = (fac[i - 1] * i) % mod;
}
static void initStirling(const unsigned int k)
{
    s[0][0] = s[1][1] = 1;
    for (unsigned int i = 2; i <= k; ++i)
        for (unsigned int j = 1; j <= i; ++j)
            s[i][j] = (s[i - 1][j - 1] + (s[i - 1][j] * j) % mod) % mod;
}
static void init(const unsigned int k)
{
    initFactor(k);
    initStirling(k);
}

void dfsRooted(const unsigned int x, const unsigned int fa)
{
    unsigned int* const csum = sum[x];
    csum[0] = 1;
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == fa)
            continue;
        dfsRooted(to, x);
        csum[0] = (csum[0] + sum[to][0]) % mod;
        for (unsigned int i = 1; i <= k; ++i)
            csum[i] = (csum[i] + sum[to][i] + sum[to][i - 1]) % mod;
    }
}
void dfsChangeRoot(const unsigned int x, const unsigned int fa)
{
    static unsigned int tmp[maxn + 10][maxk + 10];
    const unsigned int* const csum = sum[x];
    for (unsigned int i = 0; i <= k; ++i)
        ans[x] = (ans[x] + static_cast<unsigned long long>(csum[i]) * fac[i] * s[k][i]) % mod;
    unsigned int* const ctmp = tmp[x];
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == fa)
            continue;
        unsigned int* const cto = sum[to];
        ctmp[0] = (csum[0] + mod - cto[0]) % mod;
        for (unsigned int j = 1; j <= k; ++j)
            ctmp[j] = (csum[j] + 2 * mod - cto[j] - cto[j - 1]) % mod;

        cto[0] = csum[0];
        for (unsigned int j = 1; j <= k; ++j)
            cto[j] = (cto[j] + ctmp[j] + ctmp[j - 1]) % mod;

        dfsChangeRoot(to, x);
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    unsigned int n;
    std::cin >> n >> k;
    init(k);
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int a, b;
        std::cin >> a >> b;
        addEdge(a, b);
    }
    dfsRooted(1, 0);
    dfsChangeRoot(1, 0);
    std::copy(ans + 1, ans + 1 + n, std::ostream_iterator<unsigned int>(std::cout, "\n"));
    return 0;
}