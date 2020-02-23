#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cstdio>
#include <iostream>
#include <utility>
using namespace std;
using real_t = double;
using equ_t = pair<int, int>;
const int maxn = 1000, maxm = 100000;

struct vertex;
struct edge
{
    vertex* to;
    int val;
    edge* pre;
} ed[maxm * 2 + 10];
struct vertex
{
    equ_t equ;
    bool vis = false;
    edge* head = nullptr;
} ve[maxn + 10];
real_t x;
int mat[maxn + 10][maxn + 10];

inline void AddEdge(const unsigned int from, const unsigned int to, const int val)
{
    static edge* cur = ed;
    cur->to = ve + to;
    cur->pre = ve[from].head;
    cur->val = val;
    ve[from].head = cur;
    ++cur;
    mat[from][to] = val;
}
bool dfs(const vertex* p)
{
    bool fl = false;
    for (edge* i = p->head; i; i = i->pre)
    {
        equ_t cur = make_pair(-p->equ.first, i->val - p->equ.second);
        if (i->to->vis && i->to->equ.first != cur.first)
        {
            fl = true;
            x = (i->to->equ.second - cur.second) / static_cast<real_t>(cur.first - i->to->equ.first);
        }
        else if (!i->to->vis)
        {
            i->to->vis = true;
            i->to->equ = cur;
            fl |= dfs(i->to);
        }
    }
    return fl;
}
int main()
{
    int n, m;
    cin >> n >> m;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int a, b;
        int v;
        cin >> a >> b >> v;
        if (mat[a][b] && mat[a][b] != v)
        {
            printf("IMPOSSIBLE\n");
            return 0;
        }
        AddEdge(a, b, v);
        AddEdge(b, a, v);
    }
    ve[1].equ = make_pair(1, 0);
    if (dfs(ve + 1))
    {
        for (vertex* i = ve + 1; i < ve + n + 1; ++i)
            printf("%.2lf\n", i->equ.first * x + i->equ.second);
    }
    else
        printf("IMPOSSIBLE\n");
    return 0;
}
