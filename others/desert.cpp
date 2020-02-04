#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <iostream>
using namespace std;
using res_t = unsigned long long;
const int maxn = 3e5, maxm = 5e5, maxl = 25;
const res_t mod = 998244353;

struct edge
{
    unsigned int to;
    bool vis = false;
    edge *pre = nullptr, *rev;
} ed[maxm * 2 + 10];
edge* head[maxn + 10];
unsigned int fl[maxn + 10];
unsigned int n, m, ring;
res_t ans = 1, po[maxl + 10];

template <class T>
inline void read(T& dat)
{
    dat = 0;
    char c = getchar();
    while (!isdigit(c))
        c = getchar();
    while (isdigit(c))
    {
        dat = dat * 10 + c - '0';
        c = getchar();
    }
}
inline edge* AddEdge(unsigned int from, unsigned int to)
{
    static edge* cur = ed;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    return cur++;
}
inline void init(res_t a, unsigned int v)
{
    po[0] = a % mod;
    for (unsigned int i = 1; i <= v; ++i)
        po[i] = (po[i - 1] * po[i - 1]) % mod;
}
inline res_t QuickPow(unsigned int ex)
{
    res_t ret = 1;
    unsigned int t = 1;
    for (unsigned int i = 0; ex; ++i, t <<= 1)
        if (ex & t)
        {
            ret = (ret * po[i]) % mod;
            ex ^= t;
        }
    return ret;
}
void dfs(unsigned int x, unsigned int t)
{
    fl[x] = t;
    for (edge* i = head[x]; i; i = i->pre)
    {
        if (i->vis)
            continue;
        i->vis = true;
        i->rev->vis = true;
        if (!fl[i->to])
            dfs(i->to, t + 1);
        else
        {
            unsigned int l = (fl[x] > fl[i->to] ? fl[x] - fl[i->to] : fl[i->to] - fl[x]) + 1;
            ans = (ans * (QuickPow(l) - 1)) % mod;
            ring += l;
        }
    }
}

int main()
{
#ifdef APDEBUG
    char f[30];
    cin >> f;
    freopen(f, "r", stdin);
#endif
    read(n);
    read(m);
    for (int i = 0; i < m; ++i)
    {
        unsigned int x, y;
        read(x);
        read(y);
        edge *ex = AddEdge(x, y), *ey = AddEdge(y, x);
        ex->rev = ey;
        ey->rev = ex;
    }
    init(2, ceill(log2l(m)) + 1);
    while (true)
    {
        unsigned int i = find(fl + 1, fl + 1 + n, 0) - fl;
        if (i == n + 1)
            break;
        dfs(i, 1);
    }
    ans = (ans * QuickPow(m - ring)) % mod;
    cout << ans << endl;
    return 0;
}