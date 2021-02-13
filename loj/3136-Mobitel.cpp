#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
const unsigned int maxr = 300, maxs = 300, maxn = 1e6, maxSqrt = 5e3;
constexpr unsigned long long mod = 1e9 + 7;

unsigned int val[maxr + 10][maxs + 10];
unsigned int id[maxn + 10], rev[maxn + 10];

static unsigned int initDiv(const unsigned int n)
{
    unsigned int acc = 0, last = 0;
    for (unsigned int i = 1; i <= n; ++i)
        if (const unsigned int val = (n + i - 1) / i; val != last)
        {
            rev[acc] = val;
            id[val] = acc++;
            last = val;
        }
    return acc;
}
unsigned int dp(const unsigned int r, const unsigned int s, const unsigned int n, const unsigned int cnt)
{
    static const auto minMul = [](const unsigned int base, const unsigned int mul) {
        return id[(base + mul - 1) / mul];
    };
    static unsigned int f[2][maxs + 10][maxSqrt + 10];
    using Slice = unsigned int(*)[maxSqrt + 10];
    Slice cur = f[0], nxt = f[1];
    cur[1][minMul(n, val[1][1])] = 1;
    for (unsigned int i = 1; i <= r; ++i)
    {
        static const auto addEq = [](unsigned int& a, const unsigned int b) {
            a = (a + b) % mod;
        };
        std::memset(nxt, 0, sizeof(f[0]));
        for (unsigned int j = 1; j <= s; ++j)
            for (unsigned int k = 0; k < cnt; ++k)
            {
                if (i < r)
                    addEq(nxt[j][minMul(rev[k], val[i + 1][j])], cur[j][k]);
                if (j < s)
                    addEq(cur[j + 1][minMul(rev[k], val[i][j + 1])], cur[j][k]);
            }
        std::swap(cur, nxt);
    }
    return nxt[s][id[1]];
}

int main()
{
    std::ios::sync_with_stdio(false);
    unsigned int r, s, n;
    std::cin >> r >> s >> n;
    for (unsigned int i = 1; i <= r; ++i)
        std::copy_n(std::istream_iterator<unsigned int>(std::cin), s, val[i] + 1);
    std::cout << dp(r, s, n, initDiv(n)) << "\n";
    return 0;
}