#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <queue>
using namespace std;
constexpr unsigned int maxn = 500, maxm = 500;
constexpr unsigned int maxv = maxn + maxm + 2, maxe = (maxn + maxm + maxm * maxn) * 2;
constexpr int inf = INT_MAX;

struct Edge
{
    unsigned int from, to;
    int cap;
    mutable int flw;
    const Edge *pre, *rev;
} ed[maxe];
const Edge *head[maxv + 10], *par[maxv + 10];
unsigned int dep[maxv + 10];

inline Edge* addDirEdge(const unsigned int from, const unsigned int to, const int cap)
{
    static Edge* cur = ed;
    new (cur) Edge { from, to, cap, 0, head[from] };
    head[from] = cur;
    return cur++;
}
inline void addEdge(const unsigned int from, const unsigned int to, const unsigned int cap)
{
    Edge *a = addDirEdge(from, to, cap), *b = addDirEdge(to, from, 0);
    a->rev = b;
    b->rev = a;
}

void bfs(const unsigned int t)
{
    static unsigned int vis[maxv + 10], sta;
    queue<unsigned int> q;
    ++sta;
    vis[t] = sta;
    dep[t] = 0;
    q.push(t);
    while (!q.empty())
    {
        const unsigned int cur = q.front();
        q.pop();
        for (const Edge* i = head[cur]; i; i = i->pre)
            if (vis[i->to] != sta && i->rev->cap > i->rev->flw)
            {
                dep[i->to] = dep[cur] + 1;
                vis[i->to] = sta;
                q.push(i->to);
            }
    }
}
int augment(const unsigned int s, const unsigned int t)
{
    int ret = inf;
    {
        unsigned int x = t;
        while (x != s)
        {
            ret = min(ret, par[x]->cap - par[x]->flw);
            x = par[x]->from;
        }
    }
    {
        unsigned int x = t;
        while (x != s)
        {
            par[x]->flw += ret;
            par[x]->rev->flw -= ret;
            x = par[x]->from;
        }
    }
    return ret;
}
unsigned int isap(const unsigned int n, const unsigned int s, const unsigned int t)
{
    static unsigned int cnt[maxv + 10];
    static const Edge* cur[maxv + 10];
    bfs(t);
    memset(cnt, 0, sizeof(cnt));
    for (unsigned int i = 0; i <= n; ++i)
        ++cnt[dep[i]];
    copy(head, head + 1 + n, cur);
    unsigned int ret = 0, x = s;
    while (dep[s] < n)
    {
        if (x == t)
        {
            ret += augment(s, t);
            x = s;
        }
        bool adv = false;
        for (const Edge*& i = cur[x]; i; i = i->pre)
            if (dep[x] == dep[i->to] + 1 && i->cap > i->flw)
            {
                adv = true;
                par[i->to] = i;
                x = i->to;
                break;
            }
        if (!adv)
        {
            unsigned int miv = n;
            for (const Edge* i = head[x]; i; i = i->pre)
                if (i->cap > i->flw)
                    miv = min(miv, dep[i->to]);
            --cnt[dep[x]];
            if (!cnt[dep[x]])
                break;
            dep[x] = miv + 1;
            ++cnt[dep[x]];
            cur[x] = head[x];
            if (x != s)
                x = par[x]->from;
        }
    }
    return ret;
}
void increaseCap(const unsigned int s)
{
    for (const Edge* i = head[s]; i; i = i->pre)
        ++(const_cast<Edge*>(i)->cap);
}
static void buildGraph(const unsigned int n, const unsigned int m)
{
    const unsigned int s = n + m, t = n + m + 1;
    for (unsigned int i = 0; i < n; ++i)
        addEdge(s, i, 1);
    for (unsigned int i = n; i < s; ++i)
        addEdge(i, t, 1);
}
pair<unsigned int, unsigned long long> solve(const unsigned int n, const unsigned int m, const unsigned int r, const unsigned int tim)
{
    const unsigned int s = n + m, t = s + 1;
    unsigned int ret = 0;
    unsigned long long punish = 0, acc = r;
    for (unsigned int i = tim / r; i; --i, acc += r)
    {
        const unsigned int delt = isap(n + m + 2, s, t);
        if (!delt)
            break;
        ret += delt;
        punish += acc * delt;
        increaseCap(s);
    }
    return make_pair(ret, punish);
}
void printSolution(const unsigned int n, const unsigned int r)
{
    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned int acc = 0;
        for (const Edge* j = head[i]; j; j = j->pre)
            if (j->to >= n && j->cap == j->flw && j->cap > 0)
            {
                cout << i + 1 << " " << j->to - n + 1 << " " << acc << "\n";
                acc += r;
            }
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m, r, t, k;
    cin >> n >> m >> r >> t >> k;
    for (unsigned int i = 0; i < k; ++i)
    {
        unsigned int a, b;
        cin >> a >> b;
        addEdge(a - 1, b - 1 + n, 1);
    }
    buildGraph(n, m);
    const auto [cn, pu] = solve(n, m, r, t);
    cout << cn << " " << pu << "\n";
    printSolution(n, r);
    return 0;
}