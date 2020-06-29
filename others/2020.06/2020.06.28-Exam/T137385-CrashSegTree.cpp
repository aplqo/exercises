#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <utility>
using namespace std;
using Range = pair<unsigned int, unsigned int>;
const unsigned int maxn = 1e5, maxm = 1e5;

int tree[maxn + 1];
Range segs[maxn * 4 + 1], *cur = segs;
struct query
{
    unsigned int l, r;
    mutable unsigned int ans;
} qry[maxm];
const query* seq[maxm];

#define lowbit(x) ((x) & -(x))
static inline void modify(unsigned int p, const unsigned int n, const int val)
{
    for (; p <= n; p += lowbit(p))
        tree[p] += val;
}
static inline unsigned int queryPre(unsigned int p)
{
    unsigned int ret = 0;
    for (; p; p -= lowbit(p))
        ret += tree[p];
    return ret;
}
#undef lowbit

void dfsRead(const unsigned int l, const unsigned int r)
{
    if (r > l)
    {
        unsigned int m;
        cin >> m;
        dfsRead(l, m);
        dfsRead(m + 1, r);
    }
    *(cur++) = make_pair(l, r);
}
static void solve(const unsigned int n, const unsigned int m)
{
    transform(qry, qry + m, seq, [](const query& i) { return &i; });
    sort(seq, seq + m, [](const query* a, const query* b) { return a->l < b->l; });
    sort(segs, cur);
    for (const Range* i = segs; i < cur; ++i)
        modify(i->second, n, 1);
    const Range* p = segs;
    for (unsigned int i = 0; i < m; ++i)
    {
        for (const unsigned int cl = seq[i]->l; p < cur && p->first < cl; ++p)
            modify(p->second, n, -1);
        seq[i]->ans = 2 * (seq[i]->r + 1 - seq[i]->l) - queryPre(seq[i]->r);
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    dfsRead(1, n);
    for (query* i = qry; i < qry + m; ++i)
        cin >> i->l >> i->r;
    solve(n, m);
    for (const query* i = qry; i < qry + m; ++i)
        cout << i->ans << endl;
    return 0;
}