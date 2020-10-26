#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
#include <set>
#include <utility>
using namespace std;
const unsigned int maxn = 10000;

set<pair<unsigned int, unsigned int>> ed;
unsigned int degree[2][maxn + 10];

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
    static void merge(unsigned int x, unsigned int y)
    {
        x = find(x);
        y = find(y);
        if (x == y)
            return;
        if (siz[x] < siz[y])
            swap(x, y);
        fa[y] = x;
        siz[x] += siz[y];
    }
}
inline void addEdge(const unsigned int from, const unsigned int to)
{
    Disjoint::merge(from, to);
    ++degree[1][from];
    ++degree[0][to];
}
unsigned int count(const unsigned int n)
{
    static unsigned int s[maxn + 10];
    unsigned int cnt = 0, sum = 0;
    for (unsigned int i = 1; i <= maxn; ++i)
    {
        const unsigned int f = Disjoint::find(i);
        if (Disjoint::siz[f] < 2)
            continue;
        if (f == i)
            ++cnt;
        s[f] += degree[1][i] > degree[0][i] ? degree[1][i] - degree[0][i] : degree[0][i] - degree[1][i];
    }
    for (unsigned int i = 1; i <= maxn; ++i)
        if (Disjoint::find(i) == i)
            sum += s[i] > 2 ? (s[i] - 2) / 2 : 0;
    return cnt - 1 + sum;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    Disjoint::init(maxn);
    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned int x, y;
        cin >> x >> y;
        const auto e = make_pair(x, y);
        if (ed.find(e) != ed.end())
            continue;
        ed.insert(e);
        addEdge(x, y);
    }
    cout << count(n) + ed.size() + 1 << "\n";
    return 0;
}