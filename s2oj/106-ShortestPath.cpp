#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <utility>
using namespace std;
const unsigned int maxn = 2500, maxm = 6200;
const unsigned long long inf = ULLONG_MAX / 2;

struct Edge {
  unsigned int to, val;
  const Edge* pre;
} ed[maxm * 2 + 10];
const Edge* head[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to,
                    const unsigned int val)
{
  static Edge* cur = ed;
  new (cur) Edge{to, val, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, val, head[to]};
  head[to] = cur++;
}
unsigned long long dijkstra(const unsigned int n, const unsigned int s,
                            const unsigned int t)
{
  static bool vis[maxn + 10];
  static unsigned long long dis[maxn + 10];
  typedef pair<unsigned long long, unsigned int> Pair;
  priority_queue<Pair, vector<Pair>, greater<Pair>> q;
  fill(dis, dis + 1 + n, inf);
  dis[s] = 0;
  q.emplace(0, s);
  while (!q.empty()) {
    const Pair cur = q.top();
    q.pop();
    if (vis[cur.second]) continue;
    vis[cur.second] = true;
    for (const Edge* i = head[cur.second]; i; i = i->pre) {
      const unsigned int to = i->to;
      if (dis[to] > cur.first + i->val) {
        dis[to] = cur.first + i->val;
        q.emplace(dis[to], to);
      }
    }
  }
  return dis[t];
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m, s, t;
  cin >> n >> m >> s >> t;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int u, v, w;
    cin >> u >> v >> w;
    addEdge(u, v, w);
  }
  cout << dijkstra(n, s, t) << "\n";
  return 0;
}