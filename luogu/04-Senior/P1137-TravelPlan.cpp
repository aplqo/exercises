#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;
const int maxn = 100000, maxm = 200000;

struct vertex;
struct edge
{
    vertex* to = nullptr;
    edge* pre = nullptr;
} ed[maxm + 10];
struct vertex
{
    void AddEdge(const unsigned int to);
    edge* head = nullptr;
    unsigned int dep = 1, in = 0;
} ve[maxn + 10];
void vertex::AddEdge(const unsigned int to)
{
    static edge* cur = ed;
    cur->to = ve + to;
    cur->pre = head;
    head = cur;
    ++ve[to].in;
    ++cur;
}
vertex* seq[maxn + 10];

inline void topological(const unsigned int n)
{
    queue<vertex*> q;
    for (vertex* i = ve + 1; i < ve + n + 1; ++i)
        if (!i->in)
            q.push(i);
    unsigned int pos = 0;
    while (!q.empty())
    {
        vertex* cur = q.front();
        q.pop();
        seq[pos++] = cur;
        for (const edge* i = cur->head; i; i = i->pre)
        {
            --(i->to->in);
            if (!i->to->in)
                q.push(i->to);
        }
    }
}
inline void solve(const unsigned int n)
{
    for (unsigned int i = 0; i < n; ++i)
    {
        for (edge* j = seq[i]->head; j; j = j->pre)
            j->to->dep = max(j->to->dep, seq[i]->dep + 1);
    }
}
int main()
{
    ios_base::sync_with_stdio(false);
    unsigned int n, m;
    cin >> n >> m;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int x, y;
        cin >> x >> y;
        ve[x].AddEdge(y);
    }
    topological(n);
    solve(n);
    for (unsigned int i = 1; i <= n; ++i)
        cout << ve[i].dep << endl;
    return 0;
}