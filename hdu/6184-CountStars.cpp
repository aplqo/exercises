#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdlib>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e5, maxm = 2e5;

struct edge
{
    unsigned int from, to;
    const edge* pre;
} ed[maxm + 1], *cur = ed;
const edge* head[maxn + 1];
unsigned int degree[maxn + 1], vis[maxn + 1], cnt[maxm + 1];

void addEdge(const unsigned int from, const unsigned int to)
{
    cur->from = from;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
    ++degree[to];
}
void build(const unsigned int m)
{
    const static auto lessThan = [](const unsigned int l, const unsigned int r) {
        return degree[l] != degree[r] ? degree[l] < degree[r] : l < r;
    };
    for (edge* i = ed; i < ed + m; ++i)
    {
        if (!lessThan(i->from, i->to))
            swap(i->from, i->to);
        i->pre = head[i->from];
        head[i->from] = i;
    }
}
unsigned long long count(const unsigned int n, const unsigned int m)
{
    static unsigned int id[maxn + 1];
    unsigned long long ret = 0;
    for (unsigned int i = 1; i <= n; ++i)
    {
        for (const edge* e = head[i]; e; e = e->pre)
        {
            vis[e->to] = i;
            id[e->to] = e - ed;
        }
        for (const edge* e = head[i]; e; e = e->pre)
            for (const edge* v = head[e->to]; v; v = v->pre)
                if (vis[v->to] == i)
                {
                    ++cnt[v - ed];
                    ++cnt[e - ed];
                    ++cnt[id[v->to]];
                }
    }
    for (unsigned int i = 0; i < m; ++i)
        ret += cnt[i] * (cnt[i] - 1) / 2;
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    while (cin)
    {
        unsigned int n, m;
        cin >> n >> m;
        if (cin.fail())
            exit(0);
        for (edge* i = ed; i < ed + m; ++i)
        {
            cin >> i->from >> i->to;
            ++degree[i->from];
            ++degree[i->to];
        }
        build(m);
        cout << count(n, m) << endl;

        fill(vis, vis + n + 1, 0);
        fill(cnt, cnt + 1 + m, 0);
        fill(degree, degree + 1 + n, 0);
        fill(head, head + 1 + n, nullptr);
    }
    return 0;
}