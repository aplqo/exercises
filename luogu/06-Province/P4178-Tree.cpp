#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e5;

struct edge
{
    unsigned int to;
    unsigned long long val;
    const edge* pre;
} ed[maxn * 2 + 1];
const edge* head[maxn + 1];
class Array
{
public:
    void push_back(const unsigned int x) { *(cur++) = x; }
    void clear() { cur = val; }
    unsigned int* begin() { return val; }
    unsigned int* end() { return cur; }

private:
    unsigned int val[maxn + 1], *cur = val;
} a;
bool vis[maxn + 1];
unsigned long long dis[maxn + 1];
unsigned int cnt[maxn + 1], sub[maxn + 1];
unsigned int siz[maxn + 1];
unsigned int k;

void addEdge(const unsigned int from, const unsigned int to, const unsigned int val)
{
    static edge* cur = ed;
    cur->to = to;
    cur->val = val;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
void dfsPoint(const unsigned int x, const unsigned int f, const unsigned int n, unsigned int& pos, unsigned int& maxv)
{
    siz[x] = 1;
    unsigned int ms = 0;
    for (const edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == f || vis[to])
            continue;
        dfsPoint(to, x, n, pos, maxv);
        ms = max(ms, siz[to]);
        siz[x] += siz[to];
    }
    ms = max(ms, n - siz[x]);
    if (ms < maxv)
    {
        maxv = ms;
        pos = x;
    }
}
inline unsigned int treeCenter(const unsigned int x, const unsigned int n)
{
    unsigned int ret, m = UINT_MAX;
    dfsPoint(x, 0, n, ret, m);
    return ret;
}
void dfsSubtree(const unsigned int x, const unsigned int f, const unsigned int s)
{
    ++cnt[s];
    sub[x] = s;
    a.push_back(x);
    for (const edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == f || vis[to])
            continue;
        dis[to] = dis[x] + i->val;
        dfsSubtree(to, x, s);
    }
}
unsigned int countSubTree(const unsigned int x)
{
    unsigned int ret = 0;
    sort(a.begin(), a.end(), [](const unsigned int l, const unsigned int r) { return dis[l] < dis[r]; });
    const unsigned int* r = a.end() - 1;
    for (unsigned int* l = a.begin(); l != a.end() && l < r; ++l)
    {
        --cnt[sub[*l]];
        for (; dis[*r] + dis[*l] > k && r > l; --cnt[sub[*r]], --r)
            ;
        if (r - l > cnt[sub[*l]])
            ret += r - l - cnt[sub[*l]];
    }
    return ret;
}
unsigned int dfsTree(const unsigned int x)
{
    vis[x] = true;
    unsigned int subCnt = 0;
    dis[x] = 0;
    sub[x] = 0;
    cnt[0] = 1;
    a.clear();

    a.push_back(x);
    for (const edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (vis[to])
            continue;
        dis[to] = dis[x] + i->val;
        dfsSubtree(to, x, ++subCnt);
    }
    unsigned int ret = countSubTree(x);
    fill(cnt, cnt + subCnt + 1, 0);
    for (const edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (vis[to])
            continue;
        ret += dfsTree(treeCenter(to, siz[to]));
    }
    return ret;
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
        unsigned int u, v, w;
        cin >> u >> v >> w;
        addEdge(u, v, w);
        addEdge(v, u, w);
    }
    cin >> k;
    cout << dfsTree(treeCenter(1, n)) << endl;
    return 0;
}