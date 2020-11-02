#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 1e5, maxq = 1e5;

struct Query
{
    enum
    {
        Mark,
        Find
    } op;
    unsigned int x, ans;
} qry[maxq + 10];
unsigned int cnt[maxn + 10] = { 0, UINT_MAX }, fa[maxn + 10] = { 0, 1 };

namespace Disjoint
{
    unsigned int fa[maxn + 10], siz[maxn + 10], root[maxn + 10];

    static void init(const unsigned int n)
    {
        iota(fa, fa + 1 + n, 0);
        fill(siz, siz + 1 + n, 1);
        copy(fa, fa + 1 + n, root);
    }
    unsigned int getFather(const unsigned int x)
    {
        return fa[x] == x ? x : fa[x] = getFather(fa[x]);
    }
    inline unsigned int getRoot(const unsigned int x)
    {
        return root[getFather(x)];
    }
    void merge(const unsigned int rt, const unsigned int x)
    {
        unsigned int fr = getFather(rt), fx = getFather(x);
        if (fr == fx)
            return;
        const unsigned int Root = root[fr];
        if (siz[fr] < siz[fx])
            swap(fr, fx);
        fa[fx] = fr;
        siz[fr] += siz[fx];
        root[fr] = Root;
    }
}

void initFinal(const unsigned int n, const unsigned int q)
{
    Disjoint::init(n);
    for (const Query* i = qry; i < qry + q; ++i)
        if (i->op == Query::Mark)
            ++cnt[i->x];
    for (unsigned int i = 1; i <= n; ++i)
        if (!cnt[i])
            Disjoint::merge(fa[i], i);
}
void answerQuery(const unsigned int n, const unsigned int q)
{
    initFinal(n, q);
    for (Query* i = qry + q - 1; i >= qry; --i)
    {
        if (i->op == Query::Mark)
        {
            --cnt[i->x];
            if (!cnt[i->x])
                Disjoint::merge(fa[i->x], i->x);
        }
        else
            i->ans = Disjoint::getRoot(i->x);
    }
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, q;
    cin >> n >> q;
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int u, v;
        cin >> u >> v;
        fa[v] = u;
    }
    for (Query* i = qry; i < qry + q; ++i)
    {
        char op;
        cin >> op >> i->x;
        i->op = op == 'C' ? Query::Mark : Query::Find;
    }
    answerQuery(n, q);
    for (const Query* i = qry; i < qry + q; ++i)
        if (i->op == Query::Find)
            cout << i->ans << "\n";
    return 0;
}