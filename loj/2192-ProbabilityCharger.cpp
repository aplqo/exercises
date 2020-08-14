#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 500000;
using real = double;

struct Edge
{
    unsigned int to;
    real pro;
    const Edge* pre;

} ed[maxn * 2 + 10];
const Edge* head[maxn + 1];
real notCon[maxn + 1], ans[maxn + 1];
real q[maxn + 1];

inline real conProb(const Edge* e, const real vp)
{
    return 1 - e->pro + e->pro * vp;
}
inline void addEdge(const unsigned int from, const unsigned int to, const real p)
{
    static Edge* cur = ed;
    cur->to = to;
    cur->pro = p;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
void dfsRoot(const unsigned int x, const unsigned int f)
{
    notCon[x] = 1 - q[x];
    for (const Edge* i = head[x]; i; i = i->pre)
        if (i->to != f)
        {
            dfsRoot(i->to, x);
            notCon[x] *= conProb(i, notCon[i->to]);
        }
}
void dfsChange(const unsigned int x, const unsigned int f, const real fp)
{
    notCon[x] *= fp;
    ans[x] = 1 - notCon[x];
    for (const Edge* i = head[x]; i; i = i->pre)
        if (i->to != f)
            dfsChange(i->to, x, conProb(i, notCon[x] / conProb(i, notCon[i->to])));
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
        unsigned int a, b;
        real p;
        cin >> a >> b >> p;
        p /= 100;
        addEdge(a, b, p);
        addEdge(b, a, p);
    }
    copy_n(istream_iterator<real>(cin), n, q + 1);
    transform(q + 1, q + 1 + n, q + 1, [](real p) { return p / 100; });
    dfsRoot(1, 0);
    dfsChange(1, 0, 1);
    ios_base::sync_with_stdio(true);
    printf("%.6lf\n", accumulate<real*, real>(ans + 1, ans + 1 + n, 0));
    return 0;
}