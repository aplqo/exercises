#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 100000, maxm = 300000, maxl = 25;
const unsigned long long inf = ULLONG_MAX >> 1;

struct edge
{
    unsigned int from, to;
    unsigned int val;
    bool sel = false;
    edge* pre = nullptr;
} ed[maxm * 2 + 10];
edge* head[maxn + 10];

namespace lca
{
    unsigned int lg[maxn + 10];
    unsigned int fa[maxn + 10][maxl + 1], dep[maxn + 1];
    using result = pair<unsigned long long, unsigned long long>;
    result mv[maxn + 10][maxl + 1];

    void getLog(const unsigned int n)
    {
        for (unsigned int i = 2; i <= n; ++i)
            lg[i] = lg[i >> 1] + 1;
    }
    inline void update(result& p, const unsigned long long val)
    {
        if (val > p.first)
        {
            p.second = p.first;
            p.first = val;
        }
        else if (val > p.second && val != p.first)
            p.second = val;
    }
    inline void update(result& p, const result& val)
    {
        update(p, val.first);
        update(p, val.second);
    }

    void init(const unsigned int x, const unsigned int f)
    {
        fa[x][0] = f;
        dep[x] = dep[f] + 1;
        for (unsigned int i = 1; i <= lg[dep[x]]; ++i)
        {
            fa[x][i] = fa[fa[x][i - 1]][i - 1];
            mv[x][i] = mv[x][i - 1];
            update(mv[x][i], mv[fa[x][i - 1]][i - 1]);
        }
        for (edge* i = head[x]; i; i = i->pre)
            if (!dep[i->to])
            {
                mv[i->to][0] = make_pair(i->val, 0);
                init(i->to, x);
            }
    }
    result lca(unsigned int a, unsigned int b)
    {
        result ret = make_pair(0, 0);
        if (dep[a] != dep[b])
        {
            if (dep[a] > dep[b])
                swap(a, b);
            unsigned int d = dep[b] - dep[a], t = 1;
            for (unsigned int i = 0; d; ++i, t <<= 1)
                if (d & t)
                {
                    update(ret, mv[b][i]);
                    b = fa[b][i];
                    d ^= t;
                }
        }
        if (a == b)
            return ret;
        for (unsigned int i = lg[dep[a]] + 1; i;)
        {
            const unsigned int m = i - 1;
            if (fa[a][m] == fa[b][m])
                --i;
            else
            {
                update(ret, mv[a][m]);
                update(ret, mv[b][m]);
                a = fa[a][m];
                b = fa[b][m];
            }
        }
        update(ret, mv[a][0]);
        update(ret, mv[b][0]);
        return ret;
    }
}
namespace find
{
    unsigned int fa[maxn + 1], siz[maxn + 1];
    void init(const unsigned int n)
    {
        iota(fa, fa + n + 1, 0);
        fill(siz, siz + n + 1, 1);
    }
    unsigned int getFather(const unsigned int x)
    {
        return fa[x] == x ? x : fa[x] = getFather(fa[x]);
    }
    bool tryMerge(const unsigned int x, const unsigned int y)
    {
        unsigned int fx = getFather(x), fy = getFather(y);
        if (fx == fy)
            return false;
        if (siz[fx] < siz[fy])
            swap(fx, fy);
        fa[fy] = fx;
        siz[fx] += siz[fy];
        return true;
    }
}
namespace mst
{
    edge* tmp[maxm + 1];
    unsigned long long kruskal(const unsigned int n, const unsigned int m)
    {
        find::init(n);
        transform(ed, ed + m, tmp, [](edge& i) { return &i; });
        sort(tmp, tmp + m, [](edge* a, edge* b) { return a->val < b->val; });
        unsigned int pos = 0;
        unsigned long long ret = 0;
        for (unsigned int i = 1; i < n; ++i)
        {
            while (!find::tryMerge(tmp[pos]->from, tmp[pos]->to))
                ++pos;
            tmp[pos]->sel = true;
            ret += tmp[pos]->val;
            ++pos;
        }
        return ret;
    }
    void build(const unsigned int n, const unsigned int m)
    {
        const static auto add = [](edge& e) {
            e.pre = head[e.from];
            head[e.from] = &e;
        };
        edge* cur = ed + m;
        for (unsigned int i = 0; i < m; ++i)
        {
            if (!tmp[i]->sel)
                continue;
            edge& th = *tmp[i];
            add(th);

            cur->from = th.to;
            cur->to = th.from;
            cur->val = th.val;
            add(*cur);
            ++cur;
        }
    }
}

unsigned long long solve(const unsigned int n, const unsigned int m)
{
    const unsigned long long mst = mst::kruskal(n, m);
    mst::build(n, m);
    lca::init(1, 0);
    unsigned long long ret = inf;
    const auto test = [mst, &ret](const unsigned long long l, const unsigned long long v) {
        const unsigned long long t = mst - l + v;
        if (t > mst)
            ret = min(ret, t);
    };
    for (edge* i = ed; i < ed + m; ++i)
    {
        if (i->sel || i->from == i->to)
            continue;
        const auto r = lca::lca(i->from, i->to);
        test(r.first, i->val);
        test(r.second, i->val);
    }
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    lca::getLog(n + 1);
    for (edge* i = ed; i < ed + m; ++i)
        cin >> i->from >> i->to >> i->val;
    cout << solve(n, m) << endl;
    return 0;
}