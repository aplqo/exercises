#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1e5;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxn * 2 + 10], *cur = ed;
const Edge* head[maxn + 10];
struct ValFrom
{
    long long val;
    unsigned int from;
    inline bool operator<(const ValFrom& r) const { return val < r.val; }
};
constexpr ValFrom vfNinf { LLONG_MIN / 4, 0 };
struct Max2Val
{
    ValFrom fst, snd;
    inline void update(const ValFrom v)
    {
        if (fst.val < v.val)
        {
            snd = fst;
            fst = v;
        }
        else
            snd = max(snd, v);
    };
};
constexpr Max2Val m2Ninf { vfNinf, vfNinf };
Max2Val f[maxn + 10];
bool vis[maxn + 10];
long long ans[maxn + 10], val[maxn + 10], lim;

inline void addEdge(const unsigned int from, const unsigned int to)
{
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
    new (cur) Edge { from, head[to] };
    head[to] = cur++;
}
void dfsRooted(const unsigned int x, const unsigned int fa)
{
    vis[x] = true;
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == fa || val[to] > lim)
            continue;
        dfsRooted(to, x);
        f[x].update(ValFrom { max(f[to].fst.val, 0ll) + val[to], to });
    }
}
void dfsChangeRt(const unsigned int x, const unsigned int fa)
{
    ans[x] = max({ f[x].fst.val + val[x], f[x].snd.val + f[x].fst.val + val[x], val[x], 0ll });
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == fa || val[to] > lim)
            continue;
        if (f[x].fst.from != to)
            f[to].update(ValFrom { max(0ll, f[x].fst.val) + val[x], x });
        else
            f[to].update(ValFrom { max(0ll, f[x].snd.val) + val[x], x });
        dfsChangeRt(to, x);
    }
}
void solve(const unsigned int n, const long long lim)
{
    fill(vis, vis + n + 1, false);
    fill(ans, ans + n + 1, 0);
    fill(f + 1, f + 1 + n, m2Ninf);
    for (unsigned int i = 1; i <= n; ++i)
        if (!vis[i] && val[i] <= lim)
        {
            dfsRooted(i, 0);
            dfsChangeRt(i, 0);
        }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, q;
    cin >> n >> q;
    copy_n(istream_iterator<long long>(cin), n, val + 1);
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int a, b;
        cin >> a >> b;
        addEdge(a, b);
    }
    for (unsigned int i = 0; i < q; ++i)
    {
        cin >> lim;
        solve(n, lim);
        copy(ans + 1, ans + n, ostream_iterator<long long>(cout, " "));
        cout << ans[n];
        cout.put('\n');
    }
    return 0;
}