#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <vector>
using namespace std;
const int maxn = 100000, maxm = 400000;
using num_t = unsigned int;

struct vertex;
struct edge
{
    num_t val;
    vertex* to;
    edge* pre = nullptr;
} ed[maxm * 2 + maxn * 2 + 10];
struct vertex
{
    edge* head = nullptr;
    bool vis = false;
    num_t val = numeric_limits<num_t>::max();
    enum
    {
        Computer, // computer
        Server, // server
        Other // other
    } type
        = Other;
    unsigned int from;
} ve[maxn + 10];
unsigned int l[maxn + 10];

inline void AddEdge(const unsigned int from, const unsigned int to, const num_t val)
{
    static edge* cur = ed;
    vertex* pto = ve + from;
    cur->to = ve + to;
    cur->pre = pto->head;
    cur->val = val;
    pto->head = cur;
    ++cur;
}
inline void dijkstra(const unsigned int s)
{
    using pair_t = pair<num_t, unsigned int>;
    priority_queue<pair_t, vector<pair_t>, greater<pair_t>> q;
    ve[s].val = 0;
    ve[s].from = s;
    q.push(make_pair(0, s));
    while (!q.empty())
    {
        pair_t cur = q.top();
        q.pop();
        vertex* const ptr = ve + cur.second;
        if (ptr->vis)
            continue;
        ptr->vis = true;
        if (ptr->type == vertex::Server)
            continue;
        for (edge* i = ptr->head; i; i = i->pre)
        {
            if (ptr->val + i->val < i->to->val)
            {
                i->to->val = ptr->val + i->val;
                i->to->from = (ptr->type == vertex::Computer ? cur.second : ptr->from);
                q.push(make_pair(i->to->val, i->to - ve));
            }
        }
    }
}
inline unsigned int Read(const decltype(vertex::Computer) typ)
{
    unsigned int t;
    cin >> t;
    for (unsigned int i = 0; i < t; ++i)
    {
        cin >> l[i];
        ve[l[i]].type = typ;
    }
    sort(l, l + t);
    for (unsigned int i = 1; i < t; ++i)
    {
        AddEdge(l[i - 1], l[i], 0);
        AddEdge(l[i], l[i - 1], 0);
    }
    return t;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m, s, t;
    cin >> n >> m;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int u, v;
        num_t w;
        cin >> u >> v >> w;
        AddEdge(u, v, w);
        AddEdge(v, u, w);
    }
    Read(vertex::Computer);
    s = l[0];
    t = Read(vertex::Server);

    dijkstra(s);
    unsigned int lv = l[0];
    for (unsigned int* i = l; i < l + t; ++i)
        if (ve[lv].val > ve[*i].val || (ve[lv].val == ve[*i].val && ve[lv].from > ve[*i].from))
            lv = *i;
    cout << ve[lv].val << " " << ve[lv].from << " " << lv << endl;
    return 0;
}