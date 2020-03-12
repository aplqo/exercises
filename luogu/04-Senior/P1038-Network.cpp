#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <queue>
#include <utility>
using namespace std;
using num_t = int;
const int maxn = 100;

struct vertex;
struct edge
{
    vertex* to;
    num_t val;
    edge* pre = nullptr;
} ed[maxn * maxn + 10];
struct vertex
{
    void AddEdge(const unsigned int, num_t);

    edge* head = nullptr;
    unsigned int in = 0;
    num_t val = 0, lim;
} ve[maxn + 10];
pair<unsigned int, num_t> seq[maxn + 10], *cur = seq;

void vertex::AddEdge(const unsigned int to, num_t val)
{
    static edge* cur = ed;
    cur->to = ve + to;
    cur->val = val;
    cur->pre = head;
    ++ve[to].in;
    head = cur;
    ++cur;
}
void fun(const unsigned int n)
{
    queue<vertex*> q;
    for (vertex* i = ve + 1; i < ve + n + 1; ++i)
        if (!(i->in))
        {
            q.push(i);
            i->lim = 0;
        }
    while (!q.empty())
    {
        vertex* const cur = q.front();
        q.pop();
        cur->val -= cur->lim;
        for (edge* i = cur->head; i; i = i->pre)
        {
            vertex* const to = i->to;
            --(to->in);
            if (cur->val > 0)
                to->val += i->val * cur->val;
            if (!(to->in))
                q.push(to);
        }
    }
}
int main()
{
    ios_base::sync_with_stdio(false);
    unsigned int n, p;
    cin >> n >> p;
    for (vertex* i = ve + 1; i < ve + 1 + n; ++i)
        cin >> i->val >> i->lim;
    for (unsigned int i = 0; i < p; ++i)
    {
        unsigned int u, v;
        num_t w;
        cin >> u >> v >> w;
        ve[u].AddEdge(v, w);
    }
    fun(n);

    bool fl = false;
    for (vertex* i = ve + 1; i < ve + n + 1; ++i)
    {
        if (i->head)
            continue;
        if (i->val > 0)
            *(cur++) = make_pair(i - ve, i->val);
        if (i->val)
            fl = true;
    }
    if (!fl)
        cout << "NULL" << endl;
    else
        for (auto i = seq; i < cur; ++i)
            cout << i->first << " " << i->second << endl;
    return 0;
}