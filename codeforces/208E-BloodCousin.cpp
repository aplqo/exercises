#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
using namespace std;
const unsigned int maxn = 1e5, maxm = 1e5, maxl = 17;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxn + 10];
const Edge* head[maxn + 1];
struct Query
{
    unsigned int x, ans = 0;
} qry[maxm + 1];
vector<Query*> q[maxn + 10];
unsigned int fa[maxn + 10][maxl + 1], dep[maxn + 10], lg[maxn + 10];
unsigned int cnt[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
}
void getLog(const unsigned int n)
{
    for (unsigned int i = 2; i <= n; ++i)
        lg[i] = lg[i >> 1] + 1;
}
static void init(const unsigned int x, const unsigned int f)
{
    dep[x] = dep[f] + 1;
    fa[x][0] = f;
    for (unsigned int i = 1; i <= lg[dep[x]]; ++i)
        fa[x][i] = fa[fa[x][i - 1]][i - 1];
    for (const Edge* i = head[x]; i; i = i->pre)
        init(i->to, x);
}
unsigned int kthFa(unsigned int x, unsigned int k)
{
    for (unsigned int i = 0; k; ++i, k >>= 1)
        if (k & 0x01)
            x = fa[x][i];
    return x;
}
void dfs(const unsigned int x)
{
    for (Query* i : q[x])
        i->ans = cnt[dep[i->x]];
    for (const Edge* i = head[x]; i; i = i->pre)
        dfs(i->to);
    ++cnt[dep[x]];
    for (Query* i : q[x])
        i->ans = cnt[dep[i->x]] - i->ans - 1;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n;
    for (unsigned int i = 1; i <= n; ++i)
    {
        unsigned int f;
        cin >> f;
        addEdge(f, i);
    }
    cin >> m;
    getLog(n);
    init(0, 0);
    for (Query* i = qry; i < qry + m; ++i)
    {
        unsigned int p;
        cin >> i->x >> p;
        if (p > dep[i->x])
            i->ans = 0;
        else
            q[kthFa(i->x, p)].push_back(i);
    }
    for (const Edge* i = head[0]; i; i = i->pre)
        dfs(i->to);
    for (Query* i = qry; i < qry + m; ++i)
        cout << i->ans << " ";
    cout.put('\n');
    return 0;
}