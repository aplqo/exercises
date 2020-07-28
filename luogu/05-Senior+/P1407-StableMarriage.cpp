#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <string>
using namespace std;
const unsigned int maxn = 4000, maxm = 20000;
const unsigned int maxv = maxn + maxm, maxe = maxn + maxm;

struct vertex;
struct edge
{
    vertex* to;
    const edge* pre;
} ed[maxe + 1];
const edge* head[maxv + 1];
struct vertex
{
    unsigned int dfn = 0, low = 0;
    unsigned int sc = 0;
    const edge* head;

    void addEdge(const unsigned int to);
} ve[maxv + 1];
unsigned int sen, mv;
void vertex::addEdge(const unsigned int to)
{
    static edge* cur = ed;
    cur->to = ve + to;
    cur->pre = head;
    head = cur;
    ++cur;
}
pair<unsigned int, unsigned int> mar[maxn + 1];

void tarjan(vertex* const x)
{
    static unsigned int sta = 0, top = 0;
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
        ++sen;
        while (stk[top] != x)
        {
            stk[top]->sc = sen;
            --top;
        }
        x->sc = sen;
        --top;
    }
}
inline unsigned int id(const string& s)
{
    static map<size_t, unsigned int> mp;
    const size_t h = hash<string>()(s);
    auto it = mp.find(h);
    return it != mp.end() ? it->second : mp[h] = mv++;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n;
    for (unsigned int i = 0; i < n; ++i)
    {
        string s1, s2;
        cin >> s1 >> s2;
        const unsigned int id1 = id(s1), id2 = id(s2);
        ve[id1].addEdge(id2);
        mar[i] = make_pair(id1, id2);
    }
    cin >> m;
    for (unsigned int i = 0; i < m; ++i)
    {
        string s1, s2;
        cin >> s1 >> s2;
        const unsigned int id1 = id(s1), id2 = id(s2);
        ve[id2].addEdge(id1);
    }
    for (vertex* i = ve; i < ve + mv; ++i)
        if (!i->dfn)
            tarjan(i);
    for (const pair<unsigned int, unsigned int>* i = mar; i < mar + n; ++i)
        cout << (ve[i->first].sc == ve[i->second].sc ? "Unsafe" : "Safe") << endl;
    return 0;
}