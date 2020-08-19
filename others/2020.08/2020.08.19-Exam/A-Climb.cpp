#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
#include <queue>
using namespace std;
const unsigned int maxn = 5e5;
constexpr int inf = INT_MAX / 2;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxn + 10];
const Edge* head[maxn + 10];
int f[maxn + 10], w[maxn + 10];
unsigned int fa[maxn + 10];
bool ban[maxn + 10];

void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur++;
}
void bfs(const unsigned int n, const int k)
{
    static int dep[maxn + 10], jmp[maxn + 10], g[maxn + 10];
    queue<unsigned int> q;
    fill(f + 2, f + n + 1, inf);
    fill(jmp + 1, jmp + 1 + n, inf);
    fill(g + 1, g + 1 + n, inf);
    q.push(1);
    while (!q.empty())
    {
        const unsigned int cur = q.front();
        q.pop();
        f[cur] = (ban[fa[cur]] ? f[fa[cur]] : g[dep[fa[cur]]]) + w[cur];
        if (!ban[cur])
            f[cur] = min(f[cur], dep[cur] * k + jmp[dep[cur]]);
        g[dep[cur]] = min(g[dep[cur]], f[cur]);
        jmp[dep[cur] + 1] = min({ jmp[dep[cur] + 1], f[cur] - k * dep[cur], jmp[dep[cur]] });
        for (const Edge* i = head[cur]; i; i = i->pre)
        {
            dep[i->to] = dep[cur] + 1;
            q.push(i->to);
        }
    }
    for (unsigned int i = 1; i <= n; ++i)
        if (!ban[i])
            f[i] = min(f[i], g[dep[i]]);
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, k;
    cin >> n >> k;
    for (unsigned int i = 2; i <= n; ++i)
    {
        unsigned int b;
        cin >> fa[i] >> w[i] >> b;
        ban[i] = b;
        addEdge(fa[i], i);
    }
    bfs(n, k);
    copy(f + 1, f + 1 + n, ostream_iterator<int>(cout, "\n"));
    return 0;
}