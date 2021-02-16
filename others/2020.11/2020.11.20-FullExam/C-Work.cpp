#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
const unsigned int maxn = 30000, maxm = 200000, maxp = 4;
constexpr unsigned long long inf = ULLONG_MAX / 2;

struct Edge {
  unsigned int to, val;
  const Edge* pre;
} ed[maxm * 2 + 10], *cur = ed;
const Edge* head[maxn + 10];
unsigned int key[maxn + 10];
bool vis[1 << (maxp + 1)][maxn + 10];
unsigned long long dis[1 << (maxp + 1)][maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to,
                    const unsigned int val)
{
  new (cur) Edge{to, val, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, val, head[to]};
  head[to] = cur++;
}
void dijkstra(const unsigned int n, const unsigned int con)
{
  typedef pair<unsigned long long, unsigned int> Pair;
  priority_queue<Pair, vector<Pair>, greater<Pair>> q;
  unsigned long long* const cdis = dis[con];
  bool* const cvis = vis[con];
  for (unsigned int i = 1; i <= n; ++i)
    if (cdis[i] != inf) q.emplace(cdis[i], i);
  while (!q.empty()) {
    const Pair cur = q.top();
    q.pop();
    if (cvis[cur.second]) continue;
    cvis[cur.second] = true;
    for (const Edge* i = head[cur.second]; i; i = i->pre) {
      const Pair nxt(cur.first + i->val, i->to);
      if (cdis[nxt.second] > nxt.first) {
        cdis[nxt.second] = nxt.first;
        q.push(nxt);
      }
    }
  }
}
unsigned long long minSteinerTree(const unsigned int n, const unsigned int p)
{
  memset(vis, 0, sizeof(vis));
  fill(reinterpret_cast<unsigned long long*>(dis),
       reinterpret_cast<unsigned long long*>(dis) +
           sizeof(dis) / sizeof(unsigned long long),
       inf);
  const unsigned int msk = 1 << p;
  for (unsigned int i = 1; i <= n; ++i)
    dis[key[i]][i] = dis[0][i] = 0;
  for (unsigned int i = 1; i < msk; ++i) {
    for (unsigned int j = 1; j <= n; ++j)
      for (unsigned int k = i; k; k = (k - 1) & i)
        dis[i][j] = min(dis[i][j], dis[k ^ i][j] + dis[k][j]);
    dijkstra(n, i);
  }
  unsigned long long ret = inf;
  for (unsigned int i = 1; i <= n; ++i)
    ret = min(ret, dis[msk - 1][i]);
  return ret;
}

void solve()
{
  unsigned int n, m, p;
  cin >> n >> m >> p;
  if (!n && !m && !p) exit(0);
  key[1] = 1;
  for (unsigned int i = 1; i <= p; ++i) {
    unsigned int v;
    cin >> v;
    key[v] = 1 << i;
  }
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int u, v, w;
    cin >> u >> v >> w;
    addEdge(u, v, w);
  }
  cout << minSteinerTree(n, p + 1) << "\n";
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  while (true) {
    solve();
    memset(head, 0, sizeof(head));
    memset(key, 0, sizeof(key));
    cur = ed;
  }
  return 0;
}