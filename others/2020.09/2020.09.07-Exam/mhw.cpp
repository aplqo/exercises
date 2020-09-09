#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <functional>
#include <iostream>
using namespace std;
const unsigned int maxn = 100000;
const unsigned int maxq = maxn * 2, maxp = maxq * 2 + maxn;

enum
{
    with = 0,
    without = 1
};
struct Point
{
    long long x, y;
} p[2][maxn + 1];
struct Query
{
    long long x, y1, y2;
    unsigned int* result;
    bool left;
} qry[2][maxq + 1];

class FTree
{
public:
    void increase(unsigned int p, const unsigned int n);
    void clear() { memset(dat, 0, sizeof(dat)); }
    unsigned int query(const unsigned int l, const unsigned int r) const { return sum(r) - sum(l - 1); }

private:
    unsigned int sum(unsigned int r) const;

    unsigned int dat[maxp + 10] = {};
};
unsigned int cover[2][maxn + 10];

#define lowbit(x) ((x) & -(x))
void FTree::increase(unsigned int p, const unsigned int n)
{
    for (; p <= n; p += lowbit(p))
        ++dat[p];
}
unsigned int FTree::sum(unsigned int r) const
{
    unsigned int ret = 0;
    for (; r; r -= lowbit(r))
        ret += dat[r];
    return ret;
}
#undef lowbit

template <unsigned int typ>
static unsigned int InitQuery(const unsigned int n, const int d)
{
    Query* cur = qry[!typ];
    const Point* pt = p[typ];
    for (unsigned int i = 0; i < n; ++i)
    {
        const long long ly = pt[i].y - d, hy = pt[i].y + d;
        new (cur++) Query { pt[i].x - d - 1, ly, hy, &cover[typ][i], true };
        new (cur++) Query { pt[i].x + d, ly, hy, &cover[typ][i], false };
    }
    return cur - qry[!typ];
}
template <unsigned int typ>
static unsigned int discretize(const unsigned int n, const unsigned int cq)
{
    static long long tmp[maxp + 10];
    Query* const qr = qry[typ];
    Point* const pt = p[typ];
    long long* cur = tmp;
    for (Point* i = pt; i < pt + n; ++i)
        *(cur++) = i->y;
    for (Query* i = qr; i < qr + cq; ++i)
        *(cur++) = i->left ? i->y1 : i->y2;
    sort(tmp, cur);
    cur = unique(tmp, cur);
    for (Point* i = pt; i < pt + n; ++i)
        i->y = lower_bound(tmp, cur, i->y) - tmp + 1;
    for (Query* i = qr; i < qr + cq; ++i)
    {
        i->y1 = lower_bound(tmp, cur, i->y1) - tmp + 1;
        i->y2 = lower_bound(tmp, cur, i->y2) - tmp + 1;
    }
    return cur - tmp;
}
template <unsigned int typ>
void QueryCover(const unsigned int n, const unsigned int cq, const unsigned int cy)
{
    static FTree tr;
    Point* const apt = p[typ];
    Query* const aqr = qry[typ];
    tr.clear();
    sort(aqr, aqr + cq, [](const Query& a, const Query& b) { return a.x < b.x; });
    sort(apt, apt + n, [](const Point& a, const Point& b) { return a.x < b.x; });
    const Point* p = apt;
    for (const Query* i = aqr; i < aqr + cq; ++i)
    {
        for (const int nx = i->x; p < apt + n && p->x <= nx; ++p)
            tr.increase(p->y, cy);
        if (i->left)
            *(i->result) = tr.query(i->y1, i->y2);
        else
            *(i->result) = tr.query(i->y1, i->y2) - *(i->result);
    }
}
static void GetCover(const unsigned int n, const unsigned int m, const unsigned int d)
{
    const unsigned int cqw = InitQuery<without>(m, d), cwo = InitQuery<with>(n, d);
    const unsigned int cyw = discretize<with>(n, cqw), cyo = discretize<without>(m, cwo);
    QueryCover<with>(n, cqw, cyw);
    QueryCover<without>(m, cwo, cyo);
}

template <unsigned int typ>
unsigned long long CSum(const unsigned int n)
{
    const unsigned int* cov = cover[typ];
    unsigned long long ret = 0;
    for (unsigned int i = 0; i < n; ++i)
        ret += 1ull * (cov[i] - 1) * cov[i] / 2;
    return ret;
}
template <unsigned int typ>
pair<unsigned long long, unsigned long long> count(const unsigned int c1, const unsigned int c2)
{
    unsigned int* cov = cover[typ];
    const unsigned long long cs = CSum<!typ>(c2);
    unsigned long long mx = 0, mn = 0;
    sort(cov, cov + c1);
    for (unsigned int i = 0; i < c1; ++i)
    {
        mx += 1ull * cov[i] * i;
        mn += 1ull * cov[i] * (c1 - 1 - i);
    }
    return make_pair(mn - cs, mx - cs);
}
template <unsigned int typ>
void ReadPoint(const unsigned int n)
{
    for (Point* i = p[typ]; i < p[typ] + n; ++i)
    {
        long long x, y;
        cin >> x >> y;
        i->x = x - y;
        i->y = x + y;
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m, d;
    cin >> n >> m >> d;
    ReadPoint<with>(n);
    ReadPoint<without>(m);
    GetCover(n, m, d);
    const auto v1 = count<with>(n, m), v2 = count<without>(m, n);
    cout << v1.first + v2.first << " " << v1.second + v2.second << "\n";
    return 0;
}