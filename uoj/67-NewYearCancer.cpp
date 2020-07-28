#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
using namespace std;
const unsigned int maxn = 1e5, maxm = 1e5;

struct vertex;
struct edge
{
    vertex* to;
    const edge* pre;
} ed[maxm * 2 + 1];
struct vertex
{
    const edge* head = nullptr;
    unsigned int dfn = 0, low = 0, degree = 0;
    bool cut = false;

    void addEdge(const unsigned int to);
} ve[maxn + 1];
void vertex::addEdge(const unsigned int to)
{
    static edge* cur = ed;
    cur->to = ve + to;
    cur->pre = head;
    head = cur;
    ++cur;
    ++degree;
}

void tarjan(vertex* const x, const vertex* const root)
{
    static unsigned int sta = 0;
    x->dfn = x->low = ++sta;
    unsigned int cnt = 0;
    for (const edge* i = x->head; i; i = i->pre)
    {
        vertex* const to = i->to;
        if (!to->dfn)
        {
            tarjan(to, root);
            x->low = min(x->low, to->low);
            ++cnt;
            if (((x == root && cnt > 1) || x != root) && (x->dfn <= to->low))
                x->cut = true;
        }
        else
            x->low = min(x->low, to->dfn);
    }
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
        unsigned int u, v;
        cin >> u >> v;
        ve[u].addEdge(v);
        ve[v].addEdge(u);
    }
    tarjan(ve + 1, ve + 1);
    vector<unsigned int> ans;
    ans.reserve(n);
    for (unsigned int i = 1; i <= n; ++i)
        if (!ve[i].cut && m - ve[i].degree == n - 2)
            ans.push_back(i);
    cout << ans.size() << endl;
    copy(ans.begin(), ans.end(), ostream_iterator<unsigned int>(cout, " "));
    cout << endl;
    return 0;
}