#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxn = 5e4, maxm = 1e5;
const unsigned int maxv = maxn * 2, maxe = maxm * 2;
const char tab[3][2] = {
    { 'B', 'C' },
    { 'A', 'C' },
    { 'A', 'B' }
};
const unsigned int dis[3] = { 0, 1, 2 };
const unsigned int pos[3][3] = { // {A,B,C} -> {0,1}
    { -1u, 0, 1 }, { 0, -1u, 1 }, { 0, 1, -1u }
};

struct Constraint
{
    unsigned int from, hf, to, ht;
} con[maxm + 1];
struct edge
{
    struct vertex* to;
    const edge* pre;
} ed[maxe], *cur = ed;
struct scc
{
    inline void set(const bool v)
    {
        vis = true;
        val = v;
    }

    scc* rev;
    bool vis = false, val;
} sc[maxv + 1], *sen = sc;
struct vertex
{
    scc* sc = nullptr;
    unsigned int dfn = 0, low = 0;
    const edge* head = nullptr;

    void addEdge(vertex& to);
} ve[maxn + 1][2];
unsigned int typ[maxn + 1];
unsigned int sta, xpos[maxn + 1];
void vertex::addEdge(vertex& to)
{
    cur->to = &to;
    cur->pre = head;
    head = cur;
    ++cur;
}

void tarjan(vertex* const x)
{
    static unsigned int top = 0;
    static vertex* stk[maxv + 1];
    stk[++top] = x;
    x->dfn = x->low = ++sta;
    for (const edge* i = x->head; i; i = i->pre)
    {
        vertex* const to = i->to;
        if (!to->dfn)
        {
            tarjan(to);
            x->low = min(x->low, to->low);
        }
        else if (!to->sc)
            x->low = min(x->low, to->dfn);
    }
    if (x->dfn == x->low)
    {
        while (stk[top] != x)
            stk[top--]->sc = sen;
        x->sc = sen;
        --top;
        ++sen;
    }
}
inline bool buildSccGraph(const unsigned int n)
{
    for (unsigned int i = 0; i < n; ++i)
    {
        if (ve[i][0].sc == ve[i][1].sc)
            return false;
        ve[i][0].sc->rev = ve[i][1].sc;
        ve[i][1].sc->rev = ve[i][0].sc;
    }
    for (scc* i = sc; i < sen; ++i)
    {
        if (!i->vis)
        {
            i->set(true);
            i->rev->set(false);
        }
    }
    return true;
}
inline void buildGraph(const unsigned int m)
{
    for (Constraint* i = con; i < con + m; ++i)
    {
        const unsigned int from = i->from, to = i->to;
        if (i->hf == dis[typ[from]])
            continue;
        else if (i->ht == dis[typ[to]])
        {
            const unsigned int pf = pos[typ[from]][i->hf];
            ve[from][pf].addEdge(ve[from][!pf]);
            continue;
        }
        const unsigned int pf = pos[typ[from]][i->hf], pt = pos[typ[to]][i->ht];
        ve[from][pf].addEdge(ve[to][pt]);
        if (from != to)
            ve[to][!pt].addEdge(ve[from][!pf]);
    }
}
bool Twosat(const unsigned int n, const unsigned int m)
{
    memset(ve, 0, sizeof(ve));
    memset(sc, 0, sizeof(sc));
    sen = sc;
    cur = ed;
    sta = 0;
    buildGraph(m);
    for (unsigned int i = 0; i < n; ++i)
    {
        if (!ve[i][0].dfn)
            tarjan(&ve[i][0]);
        if (!ve[i][1].dfn)
            tarjan(&ve[i][1]);
    }
    return buildSccGraph(n);
}
bool solve(const unsigned int n, const unsigned int m, const unsigned int d)
{
    const unsigned int msk = (1 << d) - 1;
    for (unsigned int i = 0; i <= msk; ++i)
    {
        for (unsigned int j = 0; j < d; ++j)
            typ[xpos[j]] = static_cast<bool>(i & (1 << j));
        if (Twosat(n, m))
            return true;
    }
    return false;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    static char str[maxn + 10];
    unsigned int n, m, d;
    cin >> n >> d;
    cin >> str;
    for (unsigned int i = 0, px = 0; i < n; ++i)
        if (str[i] != 'x')
            typ[i] = str[i] - 'a';
        else
            xpos[px++] = i;
    cin >> m;
    for (Constraint* i = con; i < con + m; ++i)
    {
        char f, t;
        cin >> i->from >> f >> i->to >> t;
        --(i->from);
        --(i->to);
        i->hf = f - 'A';
        i->ht = t - 'A';
    }
    if (solve(n, m, d))
    {
        for (unsigned int i = 0; i < n; ++i)
            if (ve[i][0].sc->val)
                cout.put(tab[typ[i]][0]);
            else
                cout.put(tab[typ[i]][1]);
    }
    else
        cout << "-1" << endl;
    return 0;
}