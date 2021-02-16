#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <climits>
#include <cstring>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <vector>
using namespace std;
const unsigned int maxn = 200, maxm = 2e4;
const unsigned int maxv = maxm * 2 + 2, maxe = (maxn + maxm) * 2;
using len_t = int;
constexpr len_t cinf = numeric_limits<len_t>::max();
constexpr int finf = INT_MAX;

struct Edge {
  unsigned int from, to;
  mutable int cap;
  len_t cost;
  const Edge *pre, *rev;
} ed[maxe + 1];
const Edge *head[maxv + 1], *par[maxv + 1];
len_t dis[maxv + 1], h[maxv + 1];

void addDirEdge(const unsigned int from, const unsigned int to, const int cap,
                const len_t cst)
{
  static Edge* cur = ed;
  cur->from = from;
  cur->to = to;
  cur->cap = cap;
  cur->cost = cst;
  cur->rev = &ed[(cur - ed) ^ 1];
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
inline void addEdge(const unsigned int from, const unsigned int to,
                    const int cap, const len_t cst)
{
  addDirEdge(from, to, cap, cst);
  addDirEdge(to, from, 0, -cst);
}
bool dijkstra(const unsigned int s, const unsigned int t, const unsigned int n)
{
  static bool vis[maxv + 1];
  using pair_t = pair<len_t, unsigned int>;
  priority_queue<pair_t, vector<pair_t>, greater<pair_t>> q;
  memset(vis, 0, sizeof(vis));
  fill(dis, dis + n, cinf);
  q.emplace(0, s);
  dis[s] = 0;
  while (!q.empty()) {
    const pair_t cur = q.top();
    q.pop();
    if (vis[cur.second]) continue;
    vis[cur.second] = true;
    for (const Edge* i = head[cur.second]; i; i = i->pre) {
      const len_t w = h[i->from] + i->cost - h[i->to];
      if (!vis[i->to] && dis[i->to] > cur.first + w && i->cap) {
        dis[i->to] = cur.first + w;
        q.emplace(dis[i->to], i->to);
      }
    }
  }
  return vis[t];
}
int dfs(const unsigned int x, const int cap, const unsigned int sink)
{
  static bool vis[maxv + 1];
  if (!cap || x == sink) return cap;
  vis[x] = true;
  int rst = cap;
  for (const Edge* i = head[x]; i && rst; i = i->pre) {
    const len_t w = h[i->from] + i->cost - h[i->to];
    if (!vis[i->to] && dis[i->to] == dis[x] + w) {
      const int t = dfs(i->to, min(rst, i->cap), sink);
      i->cap -= t;
      i->rev->cap += t;
      rst -= t;
    }
  }
  vis[x] = false;
  return cap - rst;
}
pair<int, len_t> flow(const unsigned int s, const unsigned int t,
                      const unsigned int n)
{
  len_t mnc = 0;
  int mxf = 0;
  while (dijkstra(s, t, n)) {
    const int tv = dfs(s, finf, t);
    for (unsigned int i = 0; i < n; ++i)
      if (dis[i] != cinf) h[i] += dis[i];
    mxf += tv;
    mnc += tv * h[t];
  }
  return make_pair(mxf, mnc);
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int a, b;
    len_t c;
    cin >> a >> b >> c;
    --a;
    --b;
    addEdge(a * 2 + 1, b * 2, 1, c);
  }
  for (unsigned int i = 1; i < n - 1; ++i)
    addEdge(i * 2, i * 2 + 1, 1, 0);
  addEdge(0 * 2, 0 * 2 + 1, finf, 0);
  addEdge((n - 1) * 2, (n - 1) * 2 + 1, finf, 0);
  const auto ans = flow(0 * 2, (n - 1) * 2 + 1, n * 2);
  cout << ans.first << " " << ans.second << endl;
  return 0;
}