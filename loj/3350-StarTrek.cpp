#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <type_traits>
using namespace std;
const unsigned int maxn = 1e5;
constexpr unsigned long long mod = 1e9 + 7;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
bool loseRt[maxn + 10];
unsigned int cRt[maxn + 10];
bool lose[maxn + 10];
unsigned int c[maxn + 10], cnt[maxn + 10], sum[maxn + 10][2];

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
    new (cur) Edge { from, head[to] };
    head[to] = cur++;
}
inline void critical(const unsigned int x)
{
    lose[x] = !cnt[x];
    if (lose[x])
        c[x] = sum[x][false] + 1;
    else
        c[x] = cnt[x] == 1 ? sum[x][true] : 0;
}
template <bool add>
inline void updateCnt(const unsigned int rt, const unsigned int x)
{
    static const conditional_t<add, plus<unsigned int>, minus<unsigned int>> op;
    cnt[rt] = op(cnt[rt], lose[x]);
    sum[rt][lose[x]] = op(sum[rt][lose[x]], c[x]);
}
inline void changeRt(const unsigned int newRt, const unsigned int x)
{
    updateCnt<false>(x, newRt);
    critical(x);
    updateCnt<true>(newRt, x);
    critical(newRt);
}

void dfsRooted(const unsigned int x, const unsigned int f)
{
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == f)
            continue;
        dfsRooted(to, x);
        updateCnt<true>(x, to);
    }
    critical(x);
}
void dfsChangeRt(const unsigned int x, const unsigned int f)
{
    loseRt[x] = lose[x];
    cRt[x] = c[x];
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == f)
            continue;
        changeRt(to, x);
        dfsChangeRt(to, x);
        changeRt(x, to);
    }
}
unsigned long long quickPow(unsigned long long a, unsigned long long e)
{
    unsigned long long ret = 1;
    for (; e; e >>= 1)
    {
        if (e & 0x01)
            ret = (ret * a) % mod;
        a = (a * a) % mod;
    }
    return ret;
}
unsigned long long Ld_1v(const unsigned long long n, const unsigned long long d)
{
    static const auto modInv = [](unsigned long long x) { return quickPow(x, mod - 2); };
    static const auto minus = [](unsigned long long a, unsigned long long b) { return (a + mod - b) % mod; };
    static const auto multiply = [](unsigned long long a, unsigned long long b) { return (a * b) % mod; };

    unsigned long long s[2] {};
    for (unsigned int i = 1; i <= n; ++i)
        s[loseRt[i]] = (s[loseRt[i]] + cRt[i]) % mod;
    const unsigned long long e = minus(s[false], s[true]);
    const unsigned int cl = count(loseRt + 1, loseRt + 1 + n, true);
    return multiply(
        cRt[1],
        multiply(
            cl,
            multiply(
                minus(quickPow(n, 2 * d), quickPow(e, d)),
                modInv(minus((n * n) % mod, e)))));
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    unsigned long long d;
    cin >> n >> d;
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int u, v;
        cin >> u >> v;
        addEdge(u, v);
    }
    dfsRooted(1, 0);
    dfsChangeRt(1, 0);
    cout << (loseRt[1] ? Ld_1v(n, d) : (quickPow(n, 2 * d) + mod - Ld_1v(n, d)) % mod) << "\n";
    return 0;
}