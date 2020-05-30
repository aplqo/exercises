#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e5, maxm = 3e5;

struct vertex;
struct edge
{
    vertex *from, *to;
    edge* pre = nullptr;
} ed[maxm];
unsigned int degree[maxn + 5], ben = 1;
struct vertex
{
    edge* head = nullptr;
    unsigned int dfn = 0, low = 0;
    unsigned int bcc = 0;

    void addEdge(const unsigned int to);
} ve[maxn + 1];
void vertex::addEdge(const unsigned int to)
{
    static edge* cur = ed;
    cur->from = this;
    cur->to = ve + to;
    cur->pre = head;
    head = cur;
    ++cur;
}

void tarjan(vertex* const x)
{
    static unsigned int sta = 0, top = 0;
    static vertex* stk[maxn + 1];
    x->dfn = x->low = ++sta;
    stk[++top] = x;
    for (edge* i = x->head; i; i = i->pre)
    {
        vertex* const to = i->to;
        if (!to->dfn)
        {
            tarjan(to);
            x->low = min(x->low, to->low);
        }
        else if (!to->bcc)
            x->low = min(x->low, to->dfn);
    }
    if (x->low == x->dfn)
    {
        while (stk[top + 1] != x)
        {
            stk[top]->bcc = ben;
            --top;
        }
        ++ben;
    }
}
pair<unsigned int, unsigned int> solve(const unsigned int n, const unsigned int m)
{
    static struct bedge
    {
        unsigned int from, to;
    } beg[maxm], *cur = beg;
    for (edge* i = ed; i < ed + m; ++i)
        if (i->from->bcc != i->to->bcc)
        {
            cur->from = i->from->bcc;
            cur->to = i->to->bcc;
            ++cur;
        }
    sort(beg, cur, [](const bedge a, const bedge b) { return a.from != b.from ? a.from < b.from : a.to < b.to; });
    const auto e = unique(beg, cur, [](const bedge a, const bedge b) { return a.from == b.from && a.to == b.to; });
    for (bedge* i = beg; i < e; ++i)
        ++degree[i->to];
    unsigned int minv = n - (ben - 1), maxv = n;
    for (unsigned int i = 1; i < ben; ++i)
    {
        if (!degree[i])
            --maxv;
    }
    return make_pair(minv, maxv);
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int x, y;
        cin >> x >> y;
        ve[x - 1].addEdge(y - 1);
    }
    for (unsigned int i = 0; i < n; ++i)
        if (!ve[i].dfn)
            tarjan(ve + i);
    const auto [minv, maxv] = solve(n, m);
    for (unsigned int i = minv; i <= maxv; ++i)
        cout << i << endl;
    return 0;
}
