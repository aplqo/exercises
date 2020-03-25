#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;
const int maxn = 1000;

template <class T>
struct edge
{
    T *from, *to;
    edge* pre = nullptr;
};
unsigned int scc[maxn + 10], *sen = scc;
struct vertex
{
    edge<vertex>* head = nullptr;
    unsigned int* s = nullptr;
    unsigned int dfn = 0, low = 0;

    inline void AddEdge(const unsigned int to);

    static edge<vertex> ed[maxn * maxn + 10], *cur;
} ve[maxn + 10];
edge<vertex> vertex::ed[maxn * maxn + 10], *vertex::cur = vertex::ed;
inline void vertex::AddEdge(const unsigned int to)
{
    cur->from = this;
    cur->to = ve + to;
    cur->pre = head;
    head = cur;
    ++cur;
}

void tarjan(vertex* const x)
{
    static unsigned int sta = 0, top = 0;
    static vertex* stk[maxn + 10];
    x->dfn = x->low = ++sta;
    stk[++top] = x;
    for (auto i = x->head; i; i = i->pre)
    {
        vertex* const to = i->to;
        if (!to->dfn)
        {
            tarjan(to);
            x->low = min(x->low, to->low);
        }
        else if (!to->s)
            x->low = min(x->low, to->dfn);
    }
    if (x->low == x->dfn)
    {
        while (stk[top] != x)
        {
            stk[top]->s = sen;
            --top;
        }
        x->s = sen;
        --top;
        ++sen;
    }
}
inline unsigned int Solve(const unsigned int m)
{
    unsigned int ret = 0;
    for (edge<vertex>* i = vertex::ed; i < vertex::cur; ++i)
        if (i->from->s != i->to->s)
            ++*(i->to->s);
    for (unsigned int* i = scc; i < sen; ++i)
        if (!(*i))
            ++ret;
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m = 0;
    cin >> n;
    for (unsigned int i = 0; i < n; ++i)
    {
        for (unsigned int j = 0; j < n; ++j)
        {
            unsigned int t;
            cin >> t;
            if (t)
            {
                ve[i].AddEdge(j);
                ++m;
            }
        }
    }
    for (vertex* i = ve; i < ve + n; ++i)
        if (!i->dfn)
            tarjan(i);
    cout << Solve(m) << endl;
    return 0;
}