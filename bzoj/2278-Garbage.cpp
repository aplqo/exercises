#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <vector>
using namespace std;
const unsigned int maxn = 5e5;

struct Edge
{
    unsigned int to;
    list<Edge>::iterator rev;
};
list<Edge> gra[maxn + 10];
unsigned int degree[maxn + 10];
vector<unsigned int> ans;

inline void addEdge(const unsigned int from, const unsigned int to)
{
    gra[from].push_front(Edge { to });
    gra[to].push_front(Edge { from });
    gra[from].front().rev = gra[to].begin();
    gra[to].front().rev = gra[from].begin();
    ++degree[from];
    ++degree[to];
}
namespace dsu
{
    unsigned int fa[maxn + 10], siz[maxn + 10];

    static void init(const unsigned int n)
    {
        iota(fa, fa + n + 1, 0);
        fill(siz, siz + n + 1, 1);
    }
    unsigned int find(const unsigned int x)
    {
        return fa[x] == x ? x : fa[x] = find(fa[x]);
    }
    void merge(unsigned int x, unsigned int y)
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
void dfs(const unsigned int x)
{
    while (!gra[x].empty())
    {
        const auto ed = *gra[x].begin();
        gra[ed.to].erase(ed.rev);
        gra[x].pop_front();
        dfs(ed.to);
    }
    ans.push_back(x);
}

void buildGraph(const unsigned int n, const unsigned int m)
{
    dsu::init(n);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int u, v, s, t;
        cin >> u >> v >> s >> t;
        if (s != t)
        {
            addEdge(u, v);
            dsu::merge(u, v);
        }
    }
}
bool Check(const unsigned int n)
{
    for (unsigned int i = 0; i < n; ++i)
        if (degree[i] & 0x01)
            return false;
    return true;
}
unsigned int countBlock(const unsigned int n)
{
    unsigned int ret = 0;
    for (unsigned int i = 1; i <= n; ++i)
        if (dsu::find(i) == i && dsu::siz[i] > 1)
            ++ret;
    return ret;
}
void PrintSolution(const unsigned int n)
{
    for (unsigned int i = 1; i <= n; ++i)
    {
        if (dsu::find(i) != i || dsu::siz[i] < 2)
            continue;
        dfs(i);
        cout << ans.size() - 1 << " ";
        copy(ans.rbegin(), ans.rend(), ostream_iterator<unsigned int>(cout, " "));
        cout << "\n";
        ans.clear();
    }
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    buildGraph(n, m);
    if (!Check(n))
    {
        cout << "NIE\n";
        return 0;
    }
    ans.reserve(m * 5);
    cout << countBlock(n) << "\n";
    PrintSolution(n);
    return 0;
}