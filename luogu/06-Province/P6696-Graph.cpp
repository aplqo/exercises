#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <vector>
using namespace std;
const unsigned int maxn = 100000, maxm = 200000;
typedef double real;
constexpr real eps = 1e-7;

struct Edge
{
    unsigned int to, val;
    const Edge* pre;
} ed[maxm * 2 + 10];
const Edge* head[maxn + 10];
struct Ploy
{
    real k, b;

    inline real eval(const real v) const { return v * k + b; }
} p[maxn + 10];
bool vis[maxn + 10];
vector<unsigned int> vert;
real ans[maxn + 10];

inline Ploy operator-(const int l, const Ploy r)
{
    return Ploy { -r.k, l - r.b };
}
inline void addEdge(const unsigned int from, const unsigned int to, const unsigned int val)
{
    static Edge* cur = ed;
    new (cur) Edge { to, val, head[from] };
    head[from] = cur++;
    new (cur) Edge { from, val, head[to] };
    head[to] = cur++;
}
void dfsEquation(const unsigned int x, const unsigned int root)
{
    static const auto equal = [](real a, real b) { return fabs(a - b) <= eps; };
    vis[x] = true;
    vert.push_back(x);
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        const Ploy d = i->val - p[x];
        if (vis[to])
        {
            const bool ek = equal(d.k, p[to].k), eb = equal(d.b, p[to].b);
            if (ek && !eb || equal(p[to].k, 0) && !equal(p[to].b + p[x].eval(p[to].b), i->val))
            {
                cout << "NO\n";
                exit(0);
            }
            else if (ek && eb)
                continue;
            p[root] = Ploy { 0, static_cast<real>(d.b - p[to].b) / (p[to].k - d.k) };
        }
        else
        {
            p[to] = d;
            dfsEquation(to, root);
        }
    }
}
inline real minAbsolute()
{
    static real tmp[maxn + 10];
    real* cur = tmp;
    for (unsigned int i : vert)
        *(cur++) = -p[i].b * p[i].k;
    const unsigned int len = cur - tmp;
    if (len & 0x01)
    {
        nth_element(tmp, tmp + len / 2, cur);
        return tmp[len / 2];
    }
    else
    {
        const unsigned int half = len >> 1;
        nth_element(tmp, tmp + half, cur);
        real ret = tmp[half];
        nth_element(tmp, tmp + half - 1, cur);
        return (ret + tmp[half - 1]) / 2;
    }
}
void solveBlock(const unsigned int root)
{
    p[root] = Ploy { 1, 0 };
    dfsEquation(root, root);
    for_each(vert.cbegin(), vert.cend(),
        [x = p[root].k ? minAbsolute() : p[root].b](unsigned int i) { ans[i] = p[i].eval(x); });
    vert.clear();
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int a, b, c;
        cin >> a >> b >> c;
        addEdge(a, b, c);
    }
    for (unsigned int i = 1; i <= n; ++i)
        if (!vis[i])
            solveBlock(i);
    cout << "YES\n";
    copy(ans + 1, ans + 1 + n, ostream_iterator<real>(cout, " "));
    cout.put('\n');
    return 0;
}