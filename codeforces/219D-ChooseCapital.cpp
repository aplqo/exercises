#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 2e5;

struct edge
{
    unsigned int to;
    unsigned int val;
    const edge *pre, *rev;
} ed[maxn * 2];
const edge* head[maxn + 1];
unsigned int dis[maxn + 1], rootDis[maxn + 1];

edge* addEdge(const unsigned from, const unsigned int to, const unsigned int val)
{
    static edge* cur = ed;
    cur->to = to;
    cur->val = val;
    cur->pre = head[from];
    head[from] = cur;
    return cur++;
}
void dfsPath(const unsigned int x, const unsigned int f)
{
    for (const edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == f)
            continue;
        dfsPath(to, x);
        dis[x] += dis[to] + i->val;
    }
}
inline void change(const unsigned int from, const unsigned int to, const edge* const e)
{
    dis[to] += dis[from] - dis[to] - e->val + e->rev->val;
}
void dfsRoot(const unsigned int x, const unsigned int f)
{
    rootDis[x] = dis[x];
    for (const edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == f)
            continue;
        change(x, to, i);
        dfsRoot(to, x);
    }
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int s, t;
        cin >> s >> t;
        edge *a = addEdge(s, t, 0), *b = addEdge(t, s, 1);
        a->rev = b;
        b->rev = a;
    }
    dfsPath(1, 0);
    dfsRoot(1, 0);
    const unsigned int v = *min_element(rootDis + 1, rootDis + 1 + n);
    cout << v << endl;
    for (unsigned int i = 1; i <= n; ++i)
        if (rootDis[i] == v)
            cout << i << " ";
    cout << endl;
    return 0;
}