#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cstring>
#include <functional>
#include <iostream>
#include <iterator>
#include <queue>
#include <tuple>
#include <type_traits>
using namespace std;
const unsigned int maxn = 500;
const unsigned int maxv = maxn * 2 + 2, maxe = maxn * maxn + maxn * 2 + maxv * 2 * 2;
constexpr int inf = INT_MAX / 2;

struct Edge
{
    unsigned int from, to;
    int cap;
    mutable int flw = 0;
    const Edge *pre, *rev;
} ed[maxe + 1], *ecur = ed;
const Edge *head[maxv + 1], *par[maxv + 1];
unsigned int dep[maxv + 1], f[maxn + 1], a[maxn + 1];
bool sel[maxn + 1];

void addDirEdge(const unsigned int from, const unsigned int to, const int cap)
{
    ecur->from = from;
    ecur->to = to;
    ecur->cap = cap;
    ecur->pre = head[from];
    head[from] = ecur;
    ecur->rev = &ed[(ecur - ed) ^ 1];
    ++ecur;
}
inline void addEdge(const unsigned int from, const unsigned int to, const int cap)
{
    addDirEdge(from, to, cap);
    addDirEdge(to, from, 0);
}
void bfs(const unsigned int t)
{
    static bool vis[maxv + 1];
    queue<unsigned int> q;
    memset(vis, 0, sizeof(vis));
    q.push(t);
    vis[t] = true;
    dep[t] = 0;
    while (!q.empty())
    {
        const unsigned int cur = q.front();
        q.pop();
        for (const Edge* i = head[cur]; i; i = i->pre)
            if (!vis[i->to] && i->rev->cap > i->rev->flw)
            {
                vis[i->to] = true;
                dep[i->to] = dep[cur] + 1;
                q.push(i->to);
            }
    }
}
int augment(const unsigned int s, const unsigned int t)
{
    int ret = inf;
    unsigned int x = t;
    while (x != s)
    {
        ret = min(ret, par[x]->cap - par[x]->flw);
        x = par[x]->from;
    }
    x = t;
    while (x != s)
    {
        par[x]->flw += ret;
        par[x]->rev->flw -= ret;
        x = par[x]->from;
    }
    return ret;
}
int isap(const unsigned int s, const unsigned int t, const unsigned int n)
{
    static const Edge* cur[maxv + 1];
    static unsigned int cnt[maxv + 1];
    bfs(t);
    memset(cnt, 0, sizeof(cnt));
    for (unsigned int i = 0; i < n; ++i)
        ++cnt[dep[i]];
    copy(head, head + n, cur);
    int ret = 0;
    unsigned int x = s;
    while (dep[s] < n)
    {
        if (x == t)
        {
            ret += augment(s, t);
            x = s;
        }
        bool adv = false;
        for (const Edge* i = cur[x]; i; i = i->pre)
            if (dep[i->to] + 1 == dep[x] && i->cap > i->flw)
            {
                adv = true;
                par[i->to] = i;
                cur[x] = i;
                x = i->to;
                break;
            }
        if (!adv)
        {
            unsigned int mv = n;
            for (const Edge* i = head[x]; i; i = i->pre)
                if (i->cap > i->flw)
                    mv = min(mv, dep[i->to] + 1);
            --cnt[dep[x]];
            if (!cnt[dep[x]])
                break;
            cur[x] = head[x];
            dep[x] = mv;
            ++cnt[dep[x]];
            if (x != s)
                x = par[x]->from;
        }
    }
    return ret;
}
template <bool rev>
void dp(const int b, const int n)
{
    conditional_t<rev, minus<int>, plus<int>> nxt;
    fill(f, f + n, 1);
    for (int i = b; i < n && i >= 0; i = nxt(i, 1))
        for (int j = b; j != i; j = nxt(j, 1))
            if (((a[j] < a[i]) ^ rev) || a[j] == a[i])
                f[i] = max(f[i], f[j] + 1);
}
tuple<unsigned int, unsigned int, unsigned int> buildGraph(const unsigned int n)
{
    const unsigned int s = n * 2 + 1, t = n * 2;
    for (unsigned int i = 0; i < n; ++i)
        addEdge(i * 2, i * 2 + 1, 1);
    dp<false>(0, n);
    for (unsigned int i = 0; i < n; ++i)
        for (unsigned int j = 0; j < i; ++j)
            if (a[j] <= a[i] && f[j] + 1 == f[i])
                addEdge(j * 2 + 1, i * 2, inf);
    const unsigned int mv = *max_element(f, f + n);
    for (unsigned int i = 1; i < n; ++i)
        if (mv == f[i])
            addEdge(i * 2 + 1, t, inf);
    if (mv == f[0])
        addEdge(0 * 2 + 1, t, 1);
    dp<true>(n - 1, n);
    for (unsigned int i = 0; i < n - 1; ++i)
        if (f[i] == mv)
            addEdge(s, i * 2, inf);
    if (f[n - 1] == mv)
        addEdge(s, (n - 1) * 2, 1);
    return make_tuple(s, t, mv);
}
void rebuild(const unsigned int n)
{
    const static auto find = [](const unsigned int f, const unsigned int t) {
        for (const Edge* i = head[f]; i; i = i->pre)
            if (i->to == t)
                return const_cast<Edge*>(i);
    };
    find(0 * 2, 0 * 2 + 1)->cap = inf;
    if (f[0] == f[n - 1] + 1 && a[0] <= a[n - 1])
        find(0 * 2 + 1, (n - 1) * 2)->cap = 1;
    find((n - 1) * 2, (n - 1) * 2 + 1)->cap = inf;
    for (const Edge* i = ed; i < ecur; ++i)
        i->flw = 0;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    copy_n(istream_iterator<unsigned int>(cin), n, a);
    const auto [s, t, v] = buildGraph(n);
    cout << v << endl;
    cout << isap(s, t, n * 2 + 2) << endl;
    rebuild(n);
    cout << isap(s, t, n * 2 + 2) << endl;
    return 0;
}