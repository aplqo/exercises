#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <queue>
#include <random>
using namespace std;
using num_t = long long;
const int maxn = 1e5, maxm = 1e5;

struct edge
{
    unsigned int to;
    num_t val;
    edge* pre = nullptr;
} ed[maxm + maxn * 3 + 2];
edge* head[maxn + 10];
num_t ans[maxn + 10];
bool vis[maxn + 10];
unsigned int tmp[maxn + 10];

inline void AddEdge(const unsigned int from, const unsigned to, const num_t val)
{
    static edge* cur = ed;
    cur->to = to;
    cur->val = val;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
inline void init(queue<unsigned int>& q, const unsigned int n)
{
    mt19937_64 rnd;
    rnd.seed(random_device()());
    iota(tmp + 1, tmp + n + 1, 1);
    shuffle(tmp + 1, tmp + n + 1, rnd);
    for (unsigned int* i = tmp + 1; i < tmp + n + 1; ++i)
    {
        q.push(*i);
        vis[*i] = true;
    }
}
inline void spfa(const unsigned int b, const unsigned int n)
{
    queue<unsigned int> q;
    q.push(b);
    vis[b] = true;
    init(q, n);
    while (!q.empty())
    {
        const unsigned int cur = q.front();
        q.pop();
        vis[cur] = false;
        for (edge* i = head[cur]; i; i = i->pre)
        {
            if (ans[i->to] < ans[cur] + i->val)
            {
                ans[i->to] = ans[cur] + i->val;
                if (!vis[i->to])
                {
                    q.push(i->to);
                    vis[i->to] = true;
                }
            }
        }
    }
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
        unsigned int a, b;
        num_t c;
        cin >> a >> b >> c;
        AddEdge(a - 1, b, c);
    }
    for (unsigned int i = 1; i <= n; ++i)
    {
        AddEdge(i - 1, i, 0);
        AddEdge(i, i - 1, -1);
    }
    spfa(0, n);
    cout << ans[n] << endl;
    return 0;
}
