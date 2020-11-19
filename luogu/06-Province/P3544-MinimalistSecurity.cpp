#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 500000, maxm = 3000000;

struct Line
{
    long long k, b;

    inline void operator+=(const Line& r) { *this = *this + r; }
    inline Line operator+(const Line& r) const { return Line { k + r.k, b + r.b }; }
    inline Line operator-(const long long r) const { return Line { k, b - r }; }
    inline Line operator+(const long long r) const { return Line { k, b + r }; }
    inline Line operator-() const { return Line { -k, -b }; }
    inline long long operator()(const long long i) const { return k * i + b; }
    inline bool operator!=(const Line& r) const { return k != r.k || b != r.b; }
    inline long long solve(const Line& r) const;
    inline pair<long long, long long> equalRange(const long long l, const long long r) const;
};
struct Edge
{
    unsigned int to;
    long long val;
    const Edge* pre;
} ed[maxm * 2 + 10];
const Edge* head[maxn + 10];
long long p[maxn + 10];
Line equ[maxn + 10], sum[maxn + 10] = {};
long long mn[maxn + 10], mx[maxn + 10];
bool vis[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to, const long long dis)
{
    static Edge* cur = ed;
    new (cur) Edge { to, dis, head[from] };
    head[from] = cur++;
    new (cur) Edge { from, dis, head[to] };
    head[to] = cur++;
}
[[noreturn]] inline void fail()
{
    cout << "NIE\n";
    exit(0);
}
inline long long Line::solve(const Line& r) const
{
    if (k == r.k && b != r.b)
        fail();
    const long long cv = (b - r.b) / (r.k - k);
    if (cv < 0 || (*this)(cv) != r(cv))
        fail();
    return cv;
}
inline pair<long long, long long> Line::equalRange(const long long l, const long long r) const
{
    const auto sol = [this](const long long p) { return static_cast<double>(p - b) / k; };
    double v1 = sol(l), v2 = sol(r);
    if (v1 > v2)
        swap(v1, v2);
    v1 = ceil(v1);
    v2 = floor(v2);
    if (v1 > v2)
        fail();
    return pair<long long, long long>(v1, v2);
}
bool dfsTrySolve(const unsigned int x, const unsigned int fa, const unsigned int root)
{
    vis[x] = true;
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        const Line eto = -equ[x] + i->val;
        if (vis[to])
        {
            if (to != fa && eto != equ[to])
            {
                equ[root] = Line { 0, eto.solve(equ[to]) };
                return true;
            }
            continue;
        }
        equ[to] = eto;
        if (dfsTrySolve(to, x, root))
            return true;
    }
    return false;
}
void dfsSolve(const unsigned int x, const unsigned int fa, const unsigned int root)
{
    static bool dvis[maxn + 10];
    dvis[x] = vis[x] = true;
    sum[root] += equ[x];
    if (equ[x].k != 0)
    {
        const auto [mnv, mxv] = equ[x].equalRange(0, p[x]);
        mn[root] = max(mn[root], mnv);
        mx[root] = min(mx[root], mxv);
    }
    else if (equ[x].b < 0 || equ[x].b > p[x])
        fail();
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == fa || dvis[to])
        {
            const Line t = equ[x] + equ[to];
            if (t.k || t.b != i->val)
                fail();
        }
        else
        {
            equ[to] = -equ[x] + i->val;
            dfsSolve(to, x, root);
        }
    }
}
pair<long long, long long> solve(const unsigned int n)
{
    long long mnv = 0, mxv = 0;
    copy(p, p + 1 + n, mx);
    for (unsigned int i = 1; i <= n; ++i)
        if (!vis[i])
        {
            equ[i] = Line { 1, 0 };
            dfsTrySolve(i, 0, i);
            dfsSolve(i, 0, i);
            if (mn[i] > mx[i])
                fail();
            const long long v1 = sum[i](mn[i]), v2 = sum[i](mx[i]);
            mnv += min(v1, v2);
            mxv += max(v1, v2);
        }
    return make_pair(mnv, mxv);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    copy_n(istream_iterator<int>(cin), n, p + 1);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int u, v;
        long long w;
        cin >> u >> v >> w;
        if ((u != v && p[u] + p[v] < w) || (u == v && p[u] < w))
            fail();
        w = u != v ? p[u] + p[v] - w : p[u] - w;
        addEdge(u, v, w);
    }
    const auto res = solve(n);
    cout << res.first << " " << res.second << "\n";
    return 0;
}