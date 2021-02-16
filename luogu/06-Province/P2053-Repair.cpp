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
#include <utility>
#include <vector>
using namespace std;
const unsigned int maxn = 60, maxm = 9;
const unsigned int maxv = maxn * maxm + maxn + 2,
                   maxe = (maxn * (maxn * maxm) + maxv) * 2;
constexpr int inf = INT_MAX;

struct Edge {
  unsigned int from, to;
  long long cost;
  mutable int cap;
  const Edge *pre, *rev;
} ed[maxe + 10];
const Edge *head[maxv + 10], *par[maxv + 10];
long long dis[maxv + 10], h[maxv + 10], tim[maxn + 1][maxm + 1];

void addDirEdge(const unsigned int from, const unsigned int to, const int cap,
                const long long cost)
{
  static Edge* cur = ed;
  cur->from = from;
  cur->to = to;
  cur->cap = cap;
  cur->cost = cost;
  cur->rev = &ed[(cur - ed) ^ 1];
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
inline void addEdge(const unsigned int from, const unsigned int to,
                    const long long cost)
{
  addDirEdge(from, to, 1, cost);
  addDirEdge(to, from, 0, -cost);
}
bool dijkstra(const unsigned int s, const unsigned int t, const unsigned int n)
{
  using pair_t = pair<long long, unsigned int>;
  static bool vis[maxv + 1];
  priority_queue<pair_t, vector<pair_t>, greater<pair_t>> q;
  memset(vis, 0, sizeof(vis));
  fill(dis, dis + n + 1, LLONG_MAX / 2);
  dis[s] = 0;
  q.emplace(0, s);
  while (!q.empty()) {
    const pair_t cur = q.top();
    q.pop();
    if (vis[cur.second]) continue;
    vis[cur.second] = true;
    for (const Edge* i = head[cur.second]; i; i = i->pre) {
      const long long w = h[i->from] + i->cost - h[i->to];
      if (!vis[i->to] && i->cap && dis[i->to] > cur.first + w) {
        par[i->to] = i;
        dis[i->to] = cur.first + w;
        q.emplace(dis[i->to], i->to);
      }
    }
  }
  return vis[t];
}
int dfs(const unsigned int source, const unsigned int sink)
{
  int ret = inf;
  unsigned int x = sink;
  while (x != source) {
    ret = min(ret, par[x]->cap);
    x = par[x]->from;
  }
  x = sink;
  while (x != source) {
    par[x]->cap -= ret;
    par[x]->rev->cap += ret;
    x = par[x]->from;
  }
  return ret;
}
long long flow(const unsigned int s, const unsigned int t, const unsigned int n)
{
  long long mnc = 0;
  while (dijkstra(s, t, n)) {
    const int v = dfs(s, t);
    for (unsigned int i = 0; i < n; ++i)
      if (dis[i] != inf) h[i] += dis[i];
    mnc += v * h[t];
  }
  return mnc;
}
pair<unsigned int, unsigned int> buildGraph(const unsigned int n,
                                            const unsigned int m)
{
  const auto workId = [m](const unsigned int w, const unsigned int c) {
    return c * m + w;
  };
  const auto carId = [m, n](const unsigned int c) { return n * m + c; };
  const unsigned int s = carId(n - 1) + 1, t = s + 1;
  for (unsigned int c = 0; c < n; ++c) {
    const unsigned int cid = carId(c);
    addEdge(s, cid, 0);
    for (unsigned int i = 0; i < n; ++i)
      for (unsigned int j = 0; j < m; ++j)
        addEdge(cid, workId(j, i), (i + 1) * tim[c][j]);
  }
  for (unsigned int i = 0; i < n; ++i)
    for (unsigned int j = 0; j < m; ++j)
      addEdge(workId(j, i), t, 0);
  return make_pair(s, t);
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> m >> n;
  for (unsigned int i = 0; i < n; ++i)
    copy_n(istream_iterator<unsigned int>(cin), m, tim[i]);
  const auto [s, t] = buildGraph(n, m);
  ios_base::sync_with_stdio(true);
  printf("%.2lf\n", 1.0 * flow(s, t, t + 1) / n);
  return 0;
}