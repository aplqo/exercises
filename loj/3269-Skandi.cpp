#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;
const unsigned int maxn = 500, maxm = 500;
constexpr unsigned int maxv = maxn * maxm * 2, maxe = maxn * maxm * 2;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxe * 2];
const Edge* head[maxv + 10];
bool vis[maxv + 10], mrk[maxv + 10]; // mark for soltuion output
unsigned int mat[maxv + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
    new (cur) Edge { from, head[to] };
    head[to] = cur++;
}
bool dfs(const unsigned int x)
{
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (vis[to])
            continue;
        vis[to] = true;
        if (!mat[to] || dfs(mat[to] - 1))
        {
            mat[to] = x + 1;
            return true;
        }
    }
    return false;
}
void dfsMark(const unsigned int x)
{
    mrk[x] = true;
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (vis[to])
            continue;
        vis[to] = true;
        mrk[to] = true;
        dfsMark(mat[to] - 1);
    }
}
vector<unsigned int> hungry(const unsigned int v)
{
    for (unsigned int i = 0; i < v; ++i)
    {
        dfs(i);
        memset(vis, 0, sizeof(vis));
    }
    for (unsigned int i = v; i < v * 2; ++i)
        if (mat[i])
            vis[mat[i] - 1] = true;
    for (unsigned int i = 0; i < v; ++i)
        if (!vis[i])
            dfsMark(i);
    vector<unsigned int> ret;
    ret.reserve(v * 2);
    for (unsigned int i = 0; i < v; ++i)
        if (!mrk[i])
            ret.push_back(i);
    for (unsigned int i = v; i < v * 2; ++i)
        if (mrk[i])
            ret.push_back(i);
    return ret;
}
static void buildGraph(const unsigned int n, const unsigned int m)
{
    static char str[maxm + 10];
    static unsigned int up[maxm + 10];
    const auto id = [](const unsigned int x, const unsigned int y, const unsigned int m) { return x * m + y; };
    const unsigned int off = n * m;
    for (unsigned int i = 0; i < n; ++i)
    {
        cin >> str;
        unsigned int lst = 0;
        for (unsigned int j = 0; j < m; ++j)
            if (str[j] == '1')
            {
                lst = j;
                up[j] = i;
            }
            else
                addEdge(id(i, lst, m) + off, id(up[j], j, m));
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
    const unsigned int siz = n * m;
    const vector<unsigned int>& v = hungry(siz);
    cout << v.size() << "\n";
    for (unsigned int x : v)
    {
        const unsigned int v = x >= siz ? x - siz : x;
        cout << v / m + 1 << " " << v % m + 1 << " " << (x >= siz ? "DESNO" : "DOLJE") << "\n";
    }
    return 0;
}