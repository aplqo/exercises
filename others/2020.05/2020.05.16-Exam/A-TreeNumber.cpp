/*Luogu team T133175: Tree numbering */
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
using namespace std;
constexpr unsigned int inf = UINT_MAX;
const int maxn = 10000, maxl = 15;

unsigned int col;
struct vertex;
struct edge
{
    vertex* to;
    edge* pre = nullptr;
} ed[maxn * 2 + 10];
struct vertex
{
    edge* head = nullptr;
    unsigned int f[maxl + 10] = { inf };
    unsigned int fst = 0, snd = 0;

    void dfs(const vertex* fa)
    {
        for (edge* i = head; i; i = i->pre)
        {
            if (i->to == fa)
                continue;
            i->to->dfs(this);
        }
        for (unsigned int i = 1; i <= col; ++i)
        {
            f[i] = i;
            for (edge* j = head; j; j = j->pre)
            {
                if (j->to == fa)
                    continue;
                const vertex& to = *(j->to);
                f[i] += (to.fst == i ? to.f[to.snd] : to.f[to.fst]);
            }
            update(i);
        }
    }
    void addEdge(const unsigned int to);

private:
    void update(const unsigned int v)
    {
        if (f[v] < f[fst])
        {
            snd = fst;
            fst = v;
        }
        else if (f[v] < f[snd])
            snd = v;
    }
} ve[maxn + 10];
void vertex::addEdge(const unsigned int to)
{
    static edge* cur = ed;
    cur->to = ve + to;
    cur->pre = head;
    head = cur;
    ++cur;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    col = log2(n) + 1;
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int u, v;
        cin >> u >> v;
        ve[u].addEdge(v);
        ve[v].addEdge(u);
    }
    ve[1].dfs(nullptr);
    cout << ve[1].f[ve[1].fst] << endl;
    return 0;
}