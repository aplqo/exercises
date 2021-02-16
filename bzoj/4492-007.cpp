#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;
const unsigned int maxn = 200000, maxm = 600000;

struct Edge {
  unsigned int to;
  const Edge* pre;
} ed[maxm * 2 + 10];
const Edge* head[maxn + 10];
unsigned int dis1[maxn + 10], dis2[maxn + 10];
unsigned int vis[maxn + 10], sta = 0;

inline void addEdge(const unsigned int from, const unsigned int to)
{
  static Edge* cur = ed;
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, head[to]};
  head[to] = cur++;
}
void bfs(const unsigned int s, unsigned int dis[])
{
  queue<unsigned int> q;
  ++sta;
  q.push(s);
  vis[s] = sta;
  while (!q.empty()) {
    const unsigned int cur = q.front();
    q.pop();
    for (const Edge* i = head[cur]; i; i = i->pre) {
      const unsigned int to = i->to;
      if (vis[to] != sta) {
        vis[to] = sta;
        dis[to] = dis[cur] + 1;
        q.push(to);
      }
    }
  }
}
unsigned int dfs(const unsigned int x, const unsigned int dep)
{
  unsigned int ret = dep;
  vis[x] = sta;
  for (const Edge* i = head[x]; i; i = i->pre)
    if (vis[i->to] != sta && dis1[i->to] < dis1[x] && dis2[i->to] < dis2[x])
      ret = max(ret, dfs(i->to, dep + 1));
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  unsigned int s, d, a, b;  // position of 007, Dr.Null
  cin >> n >> m;
  cin >> s >> d >> a >> b;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int u, v;
    cin >> u >> v;
    addEdge(u, v);
  }
  bfs(a, dis1);
  bfs(b, dis2);
  if (dis1[d] < dis1[s] || dis2[d] < dis2[s]) {
    cout << "-1\n";
    return 0;
  }
  const unsigned int w1 = dis1[d] - dis1[s], w2 = dis2[d] - dis2[s];
  if (w1 != w2)
    cout << min(w1, w2) << "\n";
  else {
    ++sta;
    const unsigned int c = dfs(s, 0);
    ++sta;
    const unsigned int dv = dfs(d, 0);
    if (c + w1 >= dv)  // c + (disd - diss) >= d => disd - d >= disa - c
      cout << w1 << "\n";
    else if (w1)
      cout << w1 - 1 << "\n";
    else
      cout << "-1\n";
  }
  return 0;
}