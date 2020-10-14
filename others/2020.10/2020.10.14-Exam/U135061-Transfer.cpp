#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <deque>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;
const unsigned int maxn = 1e5, maxm = 2e5, maxc = 1e6;
constexpr unsigned int inf = UINT_MAX;
template <class T>
using Map = unordered_map<unsigned int, T>;

struct VirtVertex
{
    unsigned int pos, col;
};
struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxm * 2 + 10];
Map<const Edge*> head[maxn + 10];
Map<unsigned int> val[maxn + 10];
vector<unsigned int> cols[maxn + 10];

inline void createVirt(const unsigned int pos, const unsigned int col)
{
    if (val[pos].find(col) != val[pos].end())
        return;
    val[pos][col] = inf;
    head[pos][col] = nullptr;
    cols[pos].emplace_back(col);
}
inline void addDirctEdge(const unsigned int from, const unsigned int to, const unsigned int col)
{
    static Edge* cur = ed;
    createVirt(from, col);
    const Edge*(&h) = head[from].at(col);
    new (cur) Edge { to, h };
    h = cur++;
}
template <bool dir>
void update(deque<VirtVertex>& q, const VirtVertex v, const unsigned int nv)
{
    auto it = val[v.pos].find(v.col);
    if (nv < it->second)
    {
        if (it->second == inf)
        {
            if constexpr (dir)
                q.push_front(v);
            else
                q.push_back(v);
        }
        it->second = nv;
    }
}
unsigned int bfs(const unsigned int n)
{
    deque<VirtVertex> q;
    val[1][0] = 1;
    q.emplace_back(VirtVertex { 1, 0 });
    while (!q.empty())
    {
        const auto cur = q.front();
        const unsigned int cv = val[cur.pos].at(cur.col);
        q.pop_front();
        if (!cur.col)
        {
            for (unsigned int j : cols[cur.pos])
                update<true>(q, VirtVertex { cur.pos, j }, cv);
        }
        else
        {
            for (const Edge* i = head[cur.pos].at(cur.col); i; i = i->pre)
                update<true>(q, VirtVertex { i->to, cur.col }, cv);
            update<false>(q, VirtVertex { cur.pos, 0 }, cv + 1);
        }
    }
    unsigned int ret = inf;
    for (auto v : val[n])
        ret = min(ret, v.second);
    return ret;
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
        unsigned int u, v, c;
        cin >> u >> v >> c;
        addDirctEdge(u, v, c);
        addDirctEdge(v, u, c);
    }
    for (unsigned int i = 1; i <= n; ++i)
        val[i][0] = inf;
    const unsigned int v = bfs(n);
    if (v == inf)
        cout << "-1\n";
    else
        cout << v << "\n";
    return 0;
}