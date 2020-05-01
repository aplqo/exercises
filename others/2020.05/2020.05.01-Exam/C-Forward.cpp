/*Luogu team T132121: Push forward*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
using namespace std;
const int maxn = 100000;

struct dat
{
    unsigned int f, g, v = 0;
};
struct vertex;
struct edge
{
    vertex* to;
    unsigned int val;
    edge* pre = nullptr;
} ed[maxn * 2];
struct vertex
{
    template <class T>
    void solve(int beg, int end, dat out[]);
    void dp1(vertex* f);
    void dp2(vertex* f);
    unsigned int dist()
    {
        return max(cf.f + cf.g, cf.v);
    }
    void AddEdge(const unsigned int to, const unsigned int val);

    edge* head = nullptr;
    dat p[maxn + 10], s[maxn + 10], of, cf;
    int cnt = 0;
} ve[maxn + 10];
unsigned int ans;

void vertex::AddEdge(const unsigned int to, const unsigned int val)
{
    static edge* cur = ed;
    cur->to = ve + to;
    cur->val = val;
    cur->pre = head;
    head = cur;
    ++cur;
}

edge* chl[maxn + 10];
dat maxv(dat l, dat r)
{
    dat ret;
    ret.v = max(l.v, r.v);
    if (l.f > r.f)
    {
        ret.f = l.f;
        ret.g = max(l.g, r.f);
    }
    else if (l.f > r.g)
    {
        ret.f = r.f;
        ret.g = l.f;
    }
    else
    {
        ret.f = r.f;
        ret.g = r.g;
    }
    return ret;
}
template <class T>
void vertex::solve(int beg, int end, dat out[])
{
    T inc;
    for (int i = beg; i != end; i = inc(i, 1))
    {
        dat t = chl[i]->to->of;
        t.f += chl[i]->val;
        t.g += chl[i]->val;
        out[i] = maxv(t, out[inc(i, -1)]);
    }
}
void vertex::dp1(vertex* f)
{
    for (edge* i = head; i; i = i->pre)
    {
        if (i->to == f)
            continue;
        chl[++cnt] = i;
        i->to->dp1(this);
    }
    solve<plus<unsigned int>>(1, cnt + 1, p);
    solve<minus<unsigned int>>(cnt, 0, s);
    cf = of = p[cnt];
    cf.v = of.v = dist();
}
void vertex::dp2(vertex* f)
{
    unsigned int c = 1;
    for (edge* i = head; i; i = i->pre)
    {
        if (i->to == f)
            continue;
        cf = maxv(maxv(p[c - 1], s[c + 1]), f->cf);
        cf.v = dist();
        ans = max(ans, cf.v + i->to->of.v);
        i->to->dp2(this);
        ++c;
    }
}

int main()
{
    unsigned int n;
    cin >> n;
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int x, y, d;
        cin >> x >> y >> d;
        ve[x].AddEdge(y, d);
        ve[y].AddEdge(x, d);
    }
    ve[0].dp1(nullptr);
    ve[0].dp2(nullptr);
    cout << ans << endl;
    return 0;
}