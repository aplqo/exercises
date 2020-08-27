#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <utility>
using namespace std;
const unsigned int maxn = 2e5;

struct Edge
{
    unsigned int to;
    unsigned long long val;
    const Edge* pre;
} ed[maxn + 1];
const Edge* head[maxn + 1];
unsigned long long dist[maxn + 1], t;
int cnt[maxn + 1];

inline void addEdge(const unsigned int from, const unsigned int to, const unsigned long long val)
{
    static Edge* cur = ed;
    new (cur) Edge { to, val, head[from] };
    head[from] = cur++;
}
void dfsDist(const unsigned int x)
{
    using pair_t = pair<unsigned long long, unsigned int>;
    static pair_t stk[maxn + 1], *top = stk;
    *(top++) = pair_t(dist[x], x);
    ++cnt[x];
    if (dist[x] > t)
    {
        pair_t* p = lower_bound(stk, top, pair_t(dist[x] - t, 0));
        if (p->first + t >= dist[x])
            --p;
        --cnt[p->second];
    }
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        dist[i->to] = dist[x] + i->val;
        dfsDist(i->to);
        cnt[x] += cnt[i->to];
    }
    --top;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n >> t;
    for (unsigned int i = 2; i <= n; ++i)
    {
        unsigned int f;
        unsigned long long w;
        cin >> f >> w;
        addEdge(f, i, w);
    }
    dfsDist(1);
    copy(cnt + 1, cnt + 1 + n, ostream_iterator<int>(cout, "\n"));
    return 0;
}