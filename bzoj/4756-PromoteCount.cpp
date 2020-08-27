#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1e5;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxn + 1];
const Edge* head[maxn + 1];
unsigned int mp, n;
unsigned int p[maxn + 1], ans[maxn + 1];

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
}

namespace FTree
{
#define lowbit(x) ((x) & -(x))
    unsigned int val[maxn + 1];
    void increase(unsigned int p)
    {
        for (; p <= mp; p += lowbit(p))
            ++val[p];
    }
    unsigned int query(unsigned int x)
    {
        unsigned int ret = 0;
        for (; x; x -= lowbit(x))
            ret += val[x];
        return ret;
    }
#undef lowbit
}
static void discretize(const unsigned int n)
{
    static unsigned int tmp[maxn + 1];
    copy(p + 1, p + 1 + n, tmp + 1);
    sort(tmp + 1, tmp + 1 + n);
    unsigned int* e = unique(tmp + 1, tmp + 1 + n);
    mp = e - tmp;
    for (unsigned int* i = p + 1; i < p + 1 + n; ++i)
        *i = lower_bound(tmp + 1, e, *i) - tmp;
}
unsigned int dfs(const unsigned int x)
{
    const unsigned int oldv = FTree::query(p[x]);
    unsigned int siz = 1;
    for (const Edge* i = head[x]; i; i = i->pre)
        siz += dfs(i->to);
    FTree::increase(p[x]);
    ans[x] = siz - (FTree::query(p[x]) - oldv);
    return siz;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    cin >> n;
    copy_n(istream_iterator<unsigned int>(cin), n, p + 1);
    for (unsigned int i = 2; i <= n; ++i)
    {
        unsigned int f;
        cin >> f;
        addEdge(f, i);
    }
    discretize(n);
    dfs(1);
    copy(ans + 1, ans + 1 + n, ostream_iterator<unsigned int>(cout, "\n"));
    return 0;
}