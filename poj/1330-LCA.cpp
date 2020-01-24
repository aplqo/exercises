#ifdef APDEBUG
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdlib>
#include <iostream>
using namespace std;
const int maxn = 10010, maxl = 20;

struct edge
{
    edge()
    {
        pre = NULL;
    }
    int to;
    edge* pre;
} ed[maxn];
edge *head[maxn], *cur = ed;
int dep[maxn], fa[maxn][maxl];
bool sel[maxn];
int lg[maxn];
int n, root;

inline void getLog(int x)
{
    for (int i = 2; i <= x; ++i)
        lg[i] = lg[i >> 1] + 1;
}
inline void AddEdge(int from, int to)
{
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
    sel[to] = true;
}
void init(int x, int f)
{
    dep[x] = dep[f] + 1;
    fa[x][0] = f;
    for (int i = 1; i <= lg[dep[x]]; ++i)
        fa[x][i] = fa[fa[x][i - 1]][i - 1];
    for (edge* i = head[x]; i; i = i->pre)
        init(i->to, x);
}
inline int lca(int x, int y)
{
    if (dep[x] != dep[y])
    {
        if (dep[x] > dep[y])
            swap(x, y);
        int d = dep[y] - dep[x], t = 1;
        for (int i = 0; d; ++i, t <<= 1)
            if (d & t)
            {
                y = fa[y][i];
                d ^= t;
            }
    }
    if (x == y)
        return x;
    for (int i = lg[dep[x]] + 1; i >= 0;)
    {
        int m = i - 1;
        if (fa[x][m] == fa[y][m])
            --i;
        else
        {
            x = fa[x][m];
            y = fa[y][m];
        }
    }
    return fa[x][0];
}
inline void solve()
{
    cin >> n;
    for (int i = 0; i < n - 1; ++i)
    {
        int a, b;
        cin >> a >> b;
        AddEdge(a, b);
    }
    root = find(sel + 1, sel + n + 1, false) - sel;
    init(root, 0);
    int a, b;
    cin >> a >> b;
    cout << lca(a, b) << endl;
}

int main()
{
    ios_base::sync_with_stdio(false);
    getLog(maxn - 1);
    int t;
    cin >> t;
    for (int i = 0; i < t; ++i)
    {
        solve();
        fill(sel, sel + n + 1, false);
        fill(head, head + n + 1, (edge*)NULL);
        cur = ed;
    }
    return 0;
}