#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <stack>
using namespace std;
const int maxv = 50000, maxe = 50000;

struct edge
{
    unsigned int from, to;
    unsigned int bcc;
    edge *pre = nullptr, *rev;
} ed[maxe * 2 + 10], *cur = ed;
edge* head[maxv + 10];
unsigned int ben = 1;
bool isCut[maxv + 10];

inline void init()
{
    fill(isCut, isCut + maxv + 1, false);
    fill(head, head + maxv + 1, nullptr);
    fill(ed, ed + maxe * 2, edge());
    cur = ed;
    ben = 0;
}
inline edge* AddEdge(const unsigned int from, const unsigned int to)
{
    cur->from = from;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    return cur++;
}
namespace tarjan
{
    static unsigned int dfn[maxv + 10], low[maxv + 10], sta = 0;
    static stack<edge*> stk;
    unsigned int root;
    inline void init(const unsigned int n)
    {
        fill(dfn, dfn + n + 1, 0);
        fill(low, low + n + 1, 0);
        sta = 0;
        ben = 1;
    }
    void tarjan(const unsigned int x, edge* f)
    {
        dfn[x] = low[x] = ++sta;
        unsigned int cnt = 0;
        for (edge* i = head[x]; i; i = i->pre)
        {
            if (i == f)
                continue;
            const unsigned int to = i->to;
            if (i->bcc)
                continue;
            stk.push(i);
            if (!dfn[to])
            {
                ++cnt;
                tarjan(to, i->rev);
                low[x] = min(low[x], low[to]);
            }
            else
                low[x] = min(low[x], dfn[to]);
            if (((x != root && dfn[x] <= low[to]) || (x == root && cnt > 1)))
            {
                while (stk.top() != i)
                {
                    stk.top()->bcc = ben;
                    stk.top()->rev->bcc = ben;
                    stk.pop();
                }
                i->bcc = i->rev->bcc = ben;
                stk.pop();
                ++ben;
                isCut[x] = true;
            }
        }
        if (x == root && cnt > 1)
        {
            while (!stk.empty())
            {
                stk.top()->bcc = ben;
                stk.top()->rev->bcc = ben;
                stk.pop();
            }
            ++ben;
            isCut[x] = true;
        }
    }
}
namespace ebcc
{
    static unsigned int cut[maxv + 10], cnt[maxv + 10];
    static bool ign[maxv + 10], vis[maxv + 10];

    void Init(const unsigned int n)
    {
        fill(cut, cut + n + 1, 0);
        fill(cnt, cnt + n + 1, 0);
        fill(ign, ign + n + 1, false);
        fill(vis, vis + n + 1, false);
    }
    void add(const edge& i, const unsigned int x)
    {
        if (isCut[x])
        {
            if (!cut[i.bcc])
                cut[i.bcc] = x;
            else if (cut[i.bcc] != x)
                ign[i.bcc] = true;
        }
        else if (!vis[x])
        {
            vis[x] = true;
            ++cnt[i.bcc];
        }
    }
    inline void CreateGraph(const unsigned int n)
    {
        for (edge* i = ed; i < cur; ++i)
        {
            add(*i, i->from);
            add(*i, i->to);
        }
    }
    void Solve(const unsigned int n)
    {
        unsigned int num = 0;
        unsigned long long sol = 1;
        for (unsigned int i = 1; i < ben; ++i)
        {
            if (ign[i] || !cnt[i])
                continue;
            if (cut[i])
            {
                ++num;
                sol *= cnt[i];
            }
        }
        if (find(isCut + 1, isCut + 1 + n, true) == isCut + n + 1)
            cout << 2 << " " << (n * (n - 1) / 2) << endl;
        else
            cout << num << " " << sol << endl;
    }
} // namespace ebcc

void Solve(const unsigned int id)
{
    unsigned int n, nv = 0;
    cin >> n;
    if (!n)
        exit(0);
    init();
    for (unsigned int i = 1; i <= n; ++i)
    {
        unsigned int u, v;
        edge *t1, *t2;
        cin >> u >> v;
        t1 = AddEdge(u, v);
        t2 = AddEdge(v, u);
        t1->rev = t2;
        t2->rev = t1;
        nv = max({ nv, v, u });
    }
    tarjan::init(nv);
    ebcc::Init(nv);
    for (unsigned int i = 1; i <= nv; ++i)
        if (!tarjan::dfn[i])
        {
            tarjan::root = i;
            tarjan::tarjan(i, nullptr);
        }
    ebcc::CreateGraph(nv);
    cout << "Case " << id << ": ";
    ebcc::Solve(nv);
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    for (unsigned int i = 1;; ++i)
        Solve(i);
    return 0;
}