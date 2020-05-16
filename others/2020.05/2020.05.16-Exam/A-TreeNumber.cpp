/*Luogu team T133175: Tree numbering */
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const int maxn = 10000;

struct edge
{
    unsigned int to;
    edge* pre = nullptr;
} ed[maxn * 2 + 10];
edge* head[maxn + 10];
unsigned int odd, even;

void addEdge(const unsigned int from, const unsigned int to)
{
    static edge* cur = ed;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
void dfs(const unsigned int x, const unsigned int f, const unsigned int d = 0)
{
    if (d & 0x01)
        ++odd;
    else
        ++even;
    for (edge* i = head[x]; i; i = i->pre)
    {
        if (i->to == f)
            continue;
        dfs(i->to, x, d + 1);
    }
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int u, v;
        cin >> u >> v;
        addEdge(u, v);
        addEdge(v, u);
    }
    dfs(1, 0);
    cout << max(odd, even) + min(odd, even) * 2 << endl;
    return 0;
}