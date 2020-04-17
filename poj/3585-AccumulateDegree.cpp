#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const unsigned int inf = UINT_MAX;
const int maxn = 2e5;

struct edge
{
    unsigned int to;
    unsigned int val;
    edge* pre;
} ed[maxn * 2 + 10], *cur = ed;
edge* head[maxn + 10];
unsigned int f[maxn + 10], ans;

inline void AddEdge(const unsigned int from, const unsigned int to, const unsigned int val)
{
    cur->val = val;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
inline bool isOut(const unsigned int x, const unsigned int fa)
{
    return head[x]->to == fa && !head[x]->pre;
}
void dpinit(const unsigned int x, const unsigned int fa)
{
    if (isOut(x, fa))
    {
        f[x] = inf;
        return;
    }
    for (const edge* i = head[x]; i; i = i->pre)
    {
        if (i->to == fa)
            continue;
        dpinit(i->to, x);
        f[x] += min(i->val, f[i->to]);
    }
}
inline void change(const unsigned int from, const unsigned int to, const unsigned int ev)
{
    f[from] -= min(f[to], ev);
    f[to] += head[from]->pre ? min(f[from], ev) : ev;
}
void dfs(const unsigned int x, const unsigned int fa)
{
    for (const edge* i = head[x]; i; i = i->pre)
    {
        if (i->to == fa)
            continue;
        if (isOut(i->to, x))
            ans = max(ans, min(f[x] - i->val, i->val));
        else
        {
            change(x, i->to, i->val);
            ans = max(ans, f[i->to]);
            dfs(i->to, x);
            change(i->to, x, i->val);
        }
    }
}
void solve()
{
    unsigned int n;
    cin >> n;
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int x, y, z;
        cin >> x >> y >> z;
        AddEdge(x, y, z);
        AddEdge(y, x, z);
    }
    dpinit(1, 0);
    ans = f[1];
    dfs(1, 0);
    cout << ans << endl;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int t;
    cin >> t;
    for (unsigned int i = 0; i < t; ++i)
    {
        solve();
        ans = 0;
        cur = ed;
        fill(f, f + maxn + 1, 0);
        fill(head, head + maxn + 1, static_cast<edge*>(NULL));
    }
    return 0;
}