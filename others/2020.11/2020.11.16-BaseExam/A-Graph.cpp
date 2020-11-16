#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
using namespace std;
const unsigned int maxn = 2000, maxm = 10;
const unsigned int maxv = maxn + maxm * 2, maxe = maxn * maxn + maxn * maxm;

struct Point
{
    int x, y;
} p[maxn + 10];
struct Poly
{
    unsigned int c, k;
    unsigned int* vertices;

    static unsigned int buf[maxn * maxm + 10];
} pol[maxm + 10];
unsigned int Poly::buf[maxn * maxm + 10];
struct Edge
{
    unsigned int from, to;
    unsigned long long dist;
} ed[maxe + 10];

namespace Disjoint
{
    unsigned int fa[maxn + 10], siz[maxn + 10];

    static void init(const unsigned int n)
    {
        fill(siz, siz + 1 + n, 1);
        iota(fa, fa + 1 + n, 0);
    }
    unsigned int find(const unsigned int x)
    {
        return fa[x] == x ? x : fa[x] = find(fa[x]);
    }
    bool merge(unsigned int x, unsigned int y)
    {
        x = find(x);
        y = find(y);
        if (x == y)
            return false;
        if (siz[x] < siz[y])
            swap(x, y);
        fa[x] = y;
        siz[x] += siz[y];
        return true;
    }
}
inline unsigned int distance(const Point& l, const Point& r)
{
    static const auto sq = [](int i) -> unsigned int { return i * i; };
    return sq(l.x - r.x) + sq(l.y - r.y);
}
static Edge* buildGraph(const unsigned int n)
{
    Edge* cur = ed;
    for (unsigned int i = 1; i <= n; ++i)
        for (unsigned int j = i + 1; j <= n; ++j)
            new (cur++) Edge { i, j, distance(p[i], p[j]) };
    return cur;
}
vector<Edge> kruskal(const unsigned int n, Edge* const ecur)
{
    sort(ed, ecur, [](const Edge& a, const Edge& b) { return a.dist < b.dist; });
    Disjoint::init(n);
    vector<Edge> ret;
    ret.reserve(ecur - ed);
    for (const Edge* i = ed; i < ecur; ++i)
        if (Disjoint::merge(i->from, i->to))
            ret.push_back(*i);
    return ret;
}
unsigned long long changeCost(const unsigned int msk, const unsigned int n, const unsigned int m, const vector<Edge>& vec)
{
    Disjoint::init(n);
    unsigned long long ret = 0;
    for (unsigned int i = 0; i < m; ++i)
        if (msk & (1u << i))
        {
            const Poly p = pol[i];
            ret += p.c;
            const unsigned int t = p.vertices[0];
            for_each(p.vertices + 1, p.vertices + p.k, [t](unsigned int i) { Disjoint::merge(i, t); });
        }
    for (auto i = vec.cbegin(); i != vec.cend(); ++i)
        if (Disjoint::merge(i->from, i->to))
            ret += i->dist;
    return ret;
}
unsigned long long enumerate(const unsigned int n, const unsigned int m)
{
    const vector<Edge> mst = kruskal(n, buildGraph(n));
    const unsigned int msk = 1 << m;
    unsigned long long ret = ULLONG_MAX;
    for (unsigned int i = 0; i < msk; ++i)
        ret = min(ret, changeCost(i, n, m, mst));
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    for (Point* i = p + 1; i < p + 1 + n; ++i)
        cin >> i->x >> i->y;
    {
        unsigned int* cur = Poly::buf;
        for (Poly* i = pol; i < pol + m; ++i)
        {
            cin >> i->k >> i->c;
            i->vertices = cur;
            copy_n(istream_iterator<unsigned int>(cin), i->k, i->vertices);
            cur += i->k;
        }
    }
    cout << enumerate(n, m) << "\n";
    return 0;
}