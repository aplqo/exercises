#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;
using num_t = unsigned int;
const int maxn = 1e5, maxm = 1e6;

template <class T>
struct edge
{
    T *from, *to;
    edge* pre = nullptr;
    bool operator==(const edge& b)
    {
        return from == b.from ? to == b.to : false;
    }
};
struct scc
{
    unsigned int siz = 0, degree = 0;
    unsigned int leng = 0;
    num_t cnt = 0;
    edge<scc>* head = nullptr;
} s[maxn + 10], *cnt = s;
edge<scc> ne[maxm + 10];
struct vertex
{
    edge<vertex>* head = nullptr;
    scc* s = nullptr;
    unsigned int dfn = 0, low = 0;
} ve[maxn + 10];
edge<vertex> orig[maxm + 10];
num_t mod;

inline void AddEdge(const unsigned int from, const unsigned int to)
{
    static edge<vertex>* cur = orig;
    cur->from = ve + from;
    cur->to = ve + to;
    cur->pre = ve[from].head;
    ve[from].head = cur;
    ++cur;
}
void tarjan(vertex* const x)
{
    static unsigned int sta = 0, top = 0;
    static vertex* stk[maxn + 10];
    stk[++top] = x;
    x->dfn = x->low = sta++;
    for (const edge<vertex>* i = x->head; i; i = i->pre)
    {
        vertex* to = i->to;
        if (!to->dfn)
        {
            tarjan(to);
            x->low = min(x->low, to->low);
        }
        else if (!to->s)
            x->low = min(x->low, to->dfn);
    }
    if (x->dfn == x->low)
    {
        x->s = cnt;
        while (stk[top] != x)
        {
            stk[top]->s = cnt;
            ++(cnt->siz);
            --top;
        }
        --top;
        ++(cnt->siz);
        ++cnt;
    }
}
inline void CreateGraph(const unsigned int m)
{
    edge<scc>* cur = ne;
    for (edge<vertex>* i = orig; i < orig + m; ++i)
    {
        if (i->from->s != i->to->s)
        {
            cur->from = i->to->s;
            cur->to = i->from->s;
            ++cur;
        }
    }
    sort(ne, cur, [](const edge<scc>& a, const edge<scc>& b) -> bool {
        return a.from != b.from ? a.from < b.from : a.to < b.to;
    });
    cur = unique(ne, cur);
    for (auto i = ne; i < cur; ++i)
    {
        ++(i->to->degree);
        i->pre = i->from->head;
        i->from->head = i;
    }
}
inline void topological()
{
    queue<scc*> q;
    for (scc* i = s; i < cnt; ++i)
        if (!i->degree)
        {
            i->cnt = 1;
            q.push(i);
        }
    while (!q.empty())
    {
        scc* cur = q.front();
        q.pop();
        for (auto i = cur->head; i; i = i->pre)
        {
            scc* const to = i->to;
            --(to->degree);
            if (to->leng < cur->leng + cur->siz)
            {
                to->leng = cur->leng + cur->siz;
                to->cnt = cur->cnt;
            }
            else if (to->leng == cur->leng + cur->siz)
                to->cnt = (to->cnt + cur->cnt) % mod;
            if (!to->degree)
                q.push(to);
        }
    }
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m >> mod;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int a, b;
        cin >> a >> b;
        AddEdge(a, b);
    }
    for (vertex* i = ve + 1; i < ve + 1 + n; ++i)
        if (!i->dfn)
            tarjan(i);
    CreateGraph(m);
    topological();

    unsigned int ans = 0;
    num_t sol = 0;
    for (scc* i = s; i < cnt; ++i)
    {
        if (i->leng + i->siz > ans)
        {
            ans = i->leng + i->siz;
            sol = i->cnt;
        }
        else if (i->leng + i->siz == ans)
            sol = (sol + i->cnt) % mod;
    }
    cout << ans << endl
         << sol << endl;
    return 0;
}
