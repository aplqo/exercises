#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
#include <unordered_map>
using namespace std;
const unsigned int maxn = 1e5, maxq = 1e5;
constexpr unsigned int inf = UINT_MAX / 2;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
struct Variation
{
    int ans;
    unsigned int unpair;
} var[maxq + 10];
typedef unordered_map<Variation*, unsigned int> LeafList;
unsigned int degree[maxn + 10], cnt[maxn + 10];
int dep[maxn + 10], evenDep[maxn + 10];
LeafList leaves[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
    new (cur) Edge { from, head[to] };
    head[to] = cur++;
    ++degree[from];
    ++degree[to];
}
unsigned int dfsOrig(const unsigned int x, const unsigned int f)
{
    if (degree[x] == 1)
    {
        cnt[x] = 1;
        return 0;
    }
    unsigned int ret = 0;
    for (const Edge* i = head[x]; i; i = i->pre)
        if (i->to != f)
        {
            ret += dfsOrig(i->to, x);
            cnt[x] += cnt[i->to];
        }
    return ret + !(cnt[x] & 0x01);
}
inline int deltCost(const unsigned int x, const unsigned int anc)
{
    return (dep[x] - dep[anc]) - 2 * (evenDep[x] - evenDep[anc]);
}
void merge(LeafList& x, const unsigned int root)
{
    LeafList& lrt = leaves[root];
    if (x.size() > lrt.size())
        lrt.swap(x);
    for (const auto [pv, from] : x)
    {
        if (auto it = lrt.find(pv); it != lrt.end())
        {
            pv->ans += deltCost(from, root) + deltCost(it->second, root);
            pv->unpair -= 2;
            lrt.erase(it);
        }
        else
            lrt.insert({ pv, from });
    }
}
void dfsVariation(const unsigned int x, const unsigned int f)
{
    evenDep[x] = evenDep[f] + !(cnt[x] & 0x01);
    dep[x] = dep[f] + 1;
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == f)
            continue;
        dfsVariation(i->to, x);
        merge(leaves[to], x);
    }
}
inline unsigned int findNotLeaf(const unsigned int n)
{
    for (unsigned int i = 1; i <= n; ++i)
        if (degree[i] > 1)
            return i;
}
static void solveQuery(const unsigned int n, const unsigned int q)
{
    const unsigned int root = findNotLeaf(n);
    const unsigned int base = n + dfsOrig(root, 0) - 2;
    dfsVariation(root, 0);
    for (Variation* i = var; i < var + q; ++i)
    {
        i->ans += base;
        i->unpair += cnt[root] & 0x01;
    }
    if (cnt[root] & 0x01)
    {
        for (const auto [pv, from] : leaves[root])
        {
            pv->unpair -= 2;
            pv->ans += deltCost(from, 0);
        }
    }
}
void addLeaves(unsigned int buf[], const unsigned int d, Variation* const pv)
{
    static unsigned int cnt[maxn + 10];
    static bool vis[maxn + 10];
    pv->ans = d;
    pv->unpair = 0;
    for (const unsigned int* i = buf; i < buf + d; ++i)
    {
        if (degree[*i] == 1 && !vis[*i])
        {
            vis[*i] = true;
            continue;
        }
        ++cnt[*i];
    }
    for (const unsigned int* i = buf; i < buf + d; ++i)
    {
        if (cnt[*i] & 0x01)
        {
            leaves[*i][pv] = *i;
            ++(pv->unpair);
        }
        cnt[*i] = 0;
        vis[*i] = false;
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, q;
    cin >> n >> q;
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int x, y;
        cin >> x >> y;
        addEdge(x, y);
    }
    for (Variation* i = var; i < var + q; ++i)
    {
        static unsigned int buf[maxn + 10];
        unsigned int d;
        cin >> d;
        copy_n(istream_iterator<unsigned int>(cin), d, buf);
        addLeaves(buf, d, i);
    }
    solveQuery(n, q);
    for (const Variation* i = var; i < var + q; ++i)
        if (i->unpair)
            cout << "-1\n";
        else
            cout << i->ans << "\n";
    return 0;
}