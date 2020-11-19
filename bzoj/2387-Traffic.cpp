#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <functional>
#include <iostream>
#include <numeric>
#include <queue>
using namespace std;
const unsigned int maxn = 300000, maxm = 900000;
constexpr unsigned int inf = UINT_MAX;

class Graph
{
public:
    struct Edge
    {
        unsigned int to;
        const Edge* pre;
    };
    const Edge* head[maxn + 10] = {};

    inline void addEdge(const unsigned int from, const unsigned int to)
    {
        new (cur) Edge { to, head[from] };
        head[from] = cur++;
    }

private:
    Edge ed[maxm * 2 + 10], *cur = ed;
} orig, rev;
class Array
{
public:
    inline void push_back(const unsigned int v) { *(ptr++) = v; }

    unsigned int val[maxn + 10], *ptr = val;
} west, east;
bool isEnd[maxn + 10], visWest[maxn + 10], visEast[maxn + 10];
unsigned int north[maxn + 10], sorth[maxn + 10], y[maxn + 10];
unsigned int cntEast[maxn + 10];

inline void addDirEdge(const unsigned int from, const unsigned int to)
{
    orig.addEdge(from, to);
    rev.addEdge(to, from);
}
static void discretize(const unsigned int n)
{
    static unsigned int tmp[maxn + 10];
    copy(y + 1, y + n + 1, tmp);
    sort(tmp, tmp + n);
    unsigned int* const cur = unique(tmp, tmp + n);
    for (unsigned int* i = y + 1; i < y + 1 + n; ++i)
        *i = lower_bound(tmp, cur, *i) + 1 - tmp;
}
void bfsConnect(const Graph& g, const Array& src, bool vis[])
{
    queue<unsigned int> q;
    for (const unsigned int* i = src.val; i < src.ptr; ++i)
    {
        vis[*i] = true;
        q.push(*i);
    }
    while (!q.empty())
    {
        const unsigned int cur = q.front();
        q.pop();
        for (auto i = g.head[cur]; i; i = i->pre)
            if (!vis[i->to])
            {
                vis[i->to] = true;
                q.push(i->to);
            }
    }
}
template <class T, unsigned int ini>
unsigned int bfsExtreme(const unsigned int s)
{
    static bool vis[maxn + 10];
    static T cmp;
    unsigned int ret = ini;
    queue<unsigned int> q;
    q.push(s);
    vis[s] = true;
    while (!q.empty())
    {
        const unsigned int cur = q.front();
        q.pop();
        if (isEnd[cur])
            ret = min(ret, y[cur], cmp);
        for (auto i = orig.head[cur]; i; i = i->pre)
            if (!vis[i->to])
            {
                vis[i->to] = true;
                q.push(i->to);
            }
    }
    return ret;
}
template <template <typename> class T, int delt, unsigned int ini>
void westExtreme(const unsigned int* beg, const unsigned int* end, unsigned int res[])
{
    T<unsigned int> cmp;
    unsigned int lst = ini;
    for (const unsigned int* i = beg; i != end; i += delt)
        if (visWest[*i])
        {
            res[*i] = bfsExtreme<T<unsigned int>, ini>(*i);
            res[*i] = min(res[*i], lst, cmp);
            lst = res[*i];
        }
}
void solve(const unsigned int n)
{
    static const auto yCmp = [](unsigned int a, unsigned int b) { return y[a] > y[b]; };
    discretize(n);
    sort(east.val, east.ptr, yCmp);
    sort(west.val, west.ptr, yCmp);
    bfsConnect(orig, west, visEast);
    bfsConnect(rev, east, visWest);
    for (const unsigned int* i = east.val; i < east.ptr; ++i)
        if (visEast[*i])
            cntEast[y[*i]] = 1;
    partial_sum(cntEast + 1, cntEast + 1 + n, cntEast + 1);
    westExtreme<less, 1, inf>(west.val, west.ptr, sorth);
    westExtreme<greater, -1, 0>(west.ptr - 1, west.val - 1, north);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m, a, b;
    cin >> n >> m >> a >> b;
    for (unsigned int i = 1; i <= n; ++i)
    {
        unsigned int x;
        cin >> x >> y[i];
        if (!x)
            west.push_back(i);
        else if (x == a)
        {
            isEnd[i] = true;
            east.push_back(i);
        }
    }
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int x, y, k;
        cin >> x >> y >> k;
        if (k == 1)
            addDirEdge(x, y);
        else
        {
            addDirEdge(x, y);
            addDirEdge(y, x);
        }
    }
    solve(n);
    for (const unsigned int* i = west.val; i < west.ptr; ++i)
        if (!visWest[*i])
            cout << "0\n";
        else
            cout << cntEast[north[*i]] - cntEast[sorth[*i] - 1] << "\n";
    return 0;
}