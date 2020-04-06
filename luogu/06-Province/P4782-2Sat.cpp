#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <stack>
using namespace std;
const int maxn = 1e6, maxm = 1e6;
constexpr int maxv = maxn * 2, maxe = maxm * 2;

struct edge
{
    union {
        struct scc* ps;
        struct vertex* pv;
    } from, to;
    edge* pre = nullptr;

    inline bool operator==(const edge& r) const
    {
        return this->from.ps == r.from.ps && this->to.ps == r.to.ps;
    }
    inline bool operator<(const edge& r) const
    {
        return this->from.ps != r.from.ps ? this->from.ps < r.from.ps : this->to.ps < r.to.ps;
    }
} ed[maxe + 10];
struct scc
{
    struct tdat
    {
        edge* head = nullptr;
        bool val = false, vis = false;
    };
    scc* rev;

    void Set(const bool v);
    inline tdat* operator->();

    static tdat* dat;
    static void CreateGraph(const unsigned int n);
    static edge* cur;
} sc[maxv + 10], *sen = sc;
edge* scc::cur = ed;
scc::tdat* scc::dat;
void scc::Set(const bool v)
{
    scc& t = *this;
    t->val = v;
    t->vis = true;
}
inline scc::tdat* scc::operator->()
{
    return dat + (this - sc);
}
struct vertex
{
    struct tdat
    {
        edge* head = nullptr;
        unsigned int dfn = 0, low = 0;
    };
    scc* sc = nullptr;

    inline void AddEdge(vertex& r);
    inline tdat* operator->();

    static edge* cur;
    static tdat* dat;
} ve[maxv + 10];
vertex::tdat* vertex::dat;
edge* vertex::cur = ed;
vertex** stk;
inline void vertex::AddEdge(vertex& r)
{
    vertex& thi = *this;
    cur->from.pv = this;
    cur->to.pv = &r;
    cur->pre = thi->head;
    thi->head = cur;
    ++cur;
}
inline vertex::tdat* vertex::operator->()
{
    return dat + (this - ve);
}

void tarjan(vertex& x)
{
    static unsigned int sta = 0, top = 0;
    x->dfn = x->low = ++sta;
    stk[++top] = &x;
    for (auto i = x->head; i; i = i->pre)
    {
        vertex& to = *(i->to.pv);
        if (!to->dfn)
        {
            tarjan(to);
            x->low = min(x->low, to->low);
        }
        else if (!to.sc)
            x->low = min(x->low, to->dfn);
    }
    if (x->dfn == x->low)
    {
        while (stk[top + 1] != &x)
        {
            stk[top]->sc = sen;
            --top;
        }
        ++sen;
    }
}
void scc::CreateGraph(const unsigned int n)
{
    for (unsigned int i = 1; i <= n; ++i)
    {
        ve[i].sc->rev = ve[i + n].sc;
        ve[i + n].sc->rev = ve[i].sc;
    }
    delete[] vertex::dat;
    delete[] stk;
    scc::dat = new scc::tdat[sen - sc];
    for (auto i = ed; i < vertex::cur; ++i)
        if (i->from.pv->sc != i->to.pv->sc)
        {
            edge tmp = *i;
            cur->from.ps = tmp.to.pv->sc;
            cur->to.ps = tmp.from.pv->sc;
            ++cur;
        }
    sort(ed, cur);
    cur = unique(ed, cur);
    for (auto i = ed; i < cur; ++i)
    {
        i->pre = (i->from.ps)->operator->()->head;
        (i->from.ps)->operator->()->head = i;
    }
}
void Topological()
{
    for (scc* i = sc; i < sen; ++i)
    {
        scc& cur = *i;
        if (!cur->vis)
        {
            i->Set(true);
            i->rev->Set(false);
        }
        if (!cur->val)
        {
            for (auto j = cur->head; j; j = j->pre)
                j->to.ps->Set(false);
        }
    }
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    vertex::dat = new vertex::tdat[n * 2 + 1];
    stk = new vertex*[n * 2 + 1];
    for (unsigned int t = 0; t < m; ++t)
    {
        unsigned int i, a, j, b;
        cin >> i >> a >> j >> b;
        ve[i + (!a) * n].AddEdge(ve[j + b * n]);
        ve[j + (!b) * n].AddEdge(ve[i + a * n]);
    }
    for (unsigned int i = 1; i <= n * 2; ++i)
        if (!ve[i]->dfn)
            tarjan(ve[i]);
    for (unsigned int i = 1; i <= n; ++i)
        if (ve[i].sc == ve[i + n].sc)
        {
            cout << "IMPOSSIBLE" << endl;
            return 0;
        }
    scc::CreateGraph(n);
    Topological();
    cout << "POSSIBLE" << endl;
    for (unsigned int i = 1; i <= n; ++i)
        cout << (*ve[i + n].sc)->val << " ";
    return 0;
}