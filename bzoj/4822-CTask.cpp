#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 100000, maxm = 100000;

struct point
{
    int x, y;
    long long val;
} poi[maxn + 1];
struct Square
{
    int x[2], y[2];
    long long ans;
} qry[maxm + 1];
struct Query
{
    Query() = default;
    Query(Square* q, int x, bool t)
        : q(q)
        , x(x)
        , w(t ? 1 : -1)
    {
    }
    Square* q;
    int x;
    long long w;
} q[maxm * 2 + 1];
Query* seq[maxm * 2 + 1];

static void discreatize(const unsigned int n, const unsigned int m)
{
    static int tmp[maxm * 2 + maxn + 1];
    int* p = tmp;
    for (point* i = poi; i < poi + n; ++i)
        *(p++) = i->y;
    for (Square* i = qry; i < qry + m; ++i)
    {
        *(p++) = i->y[0];
        *(p++) = i->y[1];
    }

    sort(tmp, p);
    p = unique(tmp, p);

    const auto val = [&p](int v) -> int {
        return lower_bound(tmp, p, v) + 1 - tmp;
    };
    for (point* i = poi; i < poi + n; ++i)
        i->y = val(i->y);
    for (Square* i = qry; i < qry + m; ++i)
    {
        i->y[0] = val(i->y[0]);
        i->y[1] = val(i->y[1]);
    }
}
namespace ftree
{
#define lowbit(x) ((x) & -(x))
    long long dat[maxn + maxm * 2 + 1];
    void modify(unsigned int p, const unsigned int n, const long long val)
    {
        for (; p <= n; p += lowbit(p))
            dat[p] += val;
    }
    long long query(unsigned int p)
    {
        long long ret = 0;
        for (; p; p -= lowbit(p))
            ret += dat[p];
        return ret;
    }
    inline long long sum(const unsigned int l, const unsigned int r)
    {
        return query(r) - query(l - 1);
    }
#undef lowbit
}
static Query* addSquare(const unsigned int m)
{
    Query* c = q;
    for (Square* i = qry; i < qry + m; ++i)
    {
        new (c++) Query(i, i->x[0] - 1, false);
        new (c++) Query(i, i->x[1], true);
    }
    return c;
}
static void init(const unsigned int n, const unsigned int m)
{
    discreatize(n, m);
    sort(poi, poi + n, [](const point& a, const point& b) { return a.x < b.x; });
    addSquare(m);
    transform(q, addSquare(m), seq, [](Query& i) { return &i; });
    sort(seq, seq + m * 2, [](Query* a, Query* b) { return a->x < b->x; });
}
static void solve(const unsigned int n, const unsigned int m)
{
    point* p = poi;
    for (unsigned int i = 0; i < m * 2; ++i)
    {
        Query* const qr = seq[i];
        for (const int cx = qr->x; p->x <= cx && p < poi + n; ++p)
            ftree::modify(p->y, n + 2 * m, p->val);
        qr->q->ans += ftree::sum(qr->q->y[0], qr->q->y[1]) * qr->w;
    }
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    for (point* i = poi; i < poi + n; ++i)
        cin >> i->x >> i->y >> i->val;
    for (Square* i = qry; i < qry + m; ++i)
        cin >> i->x[0] >> i->y[0] >> i->x[1] >> i->y[1];
    init(n, m);
    solve(n, m);
    for (const Square* i = qry; i < qry + m; ++i)
        cout << i->ans << endl;
    return 0;
}