#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <stack>
using namespace std;
const int maxn = 2000, maxm = maxn * (maxn - 1);

struct scc;
struct vertex;
struct edge
{
    union {
        vertex* pv;
        scc* ps;
    } from, to;
    edge* pre = nullptr;

    inline bool operator<(const edge& r) const
    {
        return this->from.ps != r.from.ps ? this->from.ps < r.from.ps : this->to.ps < r.to.ps;
    }
    inline bool operator==(const edge& r) const
    {
        return this->from.ps == r.from.ps && this->to.ps == r.to.ps;
    }
} ed[maxm + 10];
struct scc
{
    edge* head = nullptr;
    unsigned int con = 0;
    unsigned int cnt = 0, vis = 0;

    static void CreateGraph();

    static edge* cur;
} sc[maxn + 10], *sen = sc;
edge* scc::cur = ed;
struct vertex
{
    edge* head = nullptr;
    scc* sc = nullptr;
    unsigned int dfn = 0, low = 0;

    inline void AddEdge(const unsigned int to);

    static edge* cur;
} ve[maxn + 10];
edge* vertex::cur = ed;
inline void vertex::AddEdge(const unsigned int to)
{
    cur->from.pv = this;
    cur->to.pv = ve + to;
    cur->pre = head;
    head = cur;
    ++cur;
}

void tarjan(vertex* const x)
{
    static unsigned int sta = 0;
    static stack<vertex*> stk;
    x->dfn = x->low = ++sta;
    stk.push(x);
    for (auto i = x->head; i; i = i->pre)
    {
        vertex* const to = i->to.pv;
        if (!to->dfn)
        {
            tarjan(to);
            x->low = min(x->low, to->low);
        }
        else if (!to->sc)
            x->low = min(x->low, to->dfn);
    }
    if (x->low == x->dfn)
    {
        while (stk.top() != x)
        {
            stk.top()->sc = sen;
            ++(sen->cnt);
            stk.pop();
        }
        x->sc = sen;
        ++(sen->cnt);
        stk.pop();
        ++sen;
    }
}
void scc::CreateGraph()
{
    for (auto i = ed; i < vertex::cur; ++i)
        if (i->from.pv->sc != i->to.pv->sc)
        {
            cur->from.ps = i->from.pv->sc;
            cur->to.ps = i->to.pv->sc;
            ++cur;
        }
    sort(ed, cur);
    cur = unique(ed, cur);
    for (auto i = ed; i < cur; ++i)
    {
        i->pre = i->from.ps->head;
        i->from.ps->head = i;
    }
}
unsigned int dfs(scc* const x, const unsigned int sta)
{
    unsigned int ret = x->cnt;
    x->vis = sta;
    for (auto i = x->head; i; i = i->pre)
        if (i->to.ps->vis != sta)
            ret += dfs(i->to.ps, sta);
    return ret;
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
        cin.ignore(2, '\n');
        for (unsigned int j = 1; j <= n; ++j)
            if (cin.get() == '1')
                ve[i].AddEdge(j);
    }
    for (vertex* i = ve + 1; i < ve + n + 1; ++i)
        if (!i->dfn)
            tarjan(i);
    scc::CreateGraph();
    for (unsigned int i = 0; i < sen - sc; ++i)
        sc[i].con = dfs(sc + i, i + 1);
    unsigned int ans = 0;
    for (unsigned int i = 1; i <= n; ++i)
        ans += ve[i].sc->con;
    cout << ans << endl;
    return 0;
}
