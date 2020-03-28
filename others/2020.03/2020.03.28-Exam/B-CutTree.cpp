#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;
using num_t = unsigned long long;
const int maxn = 1e5;
constexpr num_t mod = 1e9 + 7;

struct edge
{
    inline void set(bool x) const
    {
        this->vis = true;
        this->sel = x;
        rev->sel = x;
        rev->vis = true;
    }
    inline void clear() const
    {
        this->vis = false;
        this->sel = true;
        rev->vis = false;
        rev->sel = true;
    }
    unsigned int to;
    mutable bool sel = true, vis = false;
    edge *pre = nullptr, *rev = nullptr;
} ed[maxn * 2 + 10], *cur = ed;
edge* head[maxn + 10];
bool typ[maxn + 10], vis[maxn + 10];

inline edge* AddEdge(const unsigned int from, const unsigned int to)
{
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    return cur++;
}
bool test(const unsigned int x, unsigned int& cnt)
{
    bool ret = true;
    vis[x] = true;
    if (typ[x])
        ++cnt;
    for (edge* i = head[x]; i; i = i->pre)
    {
        if (vis[i->to])
            continue;
        if (!i->sel)
        {
            unsigned int t = 0;
            ret &= test(i->to, t);
            ret &= (t == 1);
        }
        else
            ret &= test(i->to, cnt);
    }
    return ret;
}
num_t Normal(const edge* pos)
{
    if (pos >= cur)
    {
        unsigned int tmp = 0;
        bool res;
        memset(vis, 0, sizeof(vis));
        res = test(1, tmp);
        return res && tmp == 1;
    }
    if (pos->vis)
        return Normal(pos + 1);

    pos->set(true);
    num_t ret = Normal(pos + 1);
    pos->clear();

    pos->set(false);
    ret = (ret + Normal(pos + 1)) % mod;
    pos->clear();

    return ret;
}
num_t Chain(const unsigned int n)
{
    bool* beg = find(typ + 1, typ + 1 + n, true);
    num_t ret = 1;
    while (beg < typ + n + 1)
    {
        bool* c = find(beg + 1, typ + 1 + n, true);
        if (c == typ + 1 + n)
            break;
        ret = (ret * (c - beg)) % mod;
        beg = c;
    }
    return ret;
}
num_t Flower(const unsigned int n)
{
    if (typ[1])
        return 1;
    return count(typ + 1, typ + 1 + n, true);
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    for (unsigned int i = 1; i <= n; ++i)
    {
        unsigned int t;
        cin >> t;
        typ[i] = t;
    }
    bool isFlower = true, isChain = true;
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int u, v;
        cin >> u >> v;
        isFlower &= (u == 1 && v == i);
        isChain &= (u == i && v == i + 1);
        edge *ea = AddEdge(u, v), *eb = AddEdge(v, u);
        ea->rev = eb;
        eb->rev = ea;
    }
    if (isFlower)
        cout << Flower(n) << endl;
    else if (isChain)
        cout << Chain(n) << endl;
    else
        cout << Normal(ed) << endl;
    return 0;
}