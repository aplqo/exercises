#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <numeric>
#include <queue>
#include <utility>
#include <vector>
using namespace std;
const unsigned int maxn = 1e5, maxm = 5e5;
const unsigned int maxv = 1 << 22, maxe = 2 * (maxn * 21) + maxm;
using dist_t = unsigned long long;
using pair_t = pair<dist_t, unsigned int>;
constexpr unsigned long long inf = numeric_limits<dist_t>::max() / 2;

struct edge {
  unsigned int to;
  dist_t val;
  const edge* pre;
} ed[maxe + 1];
const edge* head[maxv + 1];

void addEdge(const unsigned int from, const unsigned int to, const dist_t val)
{
  static edge* cur = ed;
  cur->to = to;
  cur->val = val;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
void build(const unsigned int lv, const dist_t c)
{
  const unsigned int n = 1 << lv;
  for (unsigned int i = 0; i <= n; ++i)
    for (unsigned int j = 0; j <= lv; ++j)
      addEdge(i, i ^ (1 << j), (1 << j) * c);
}
dist_t dijkstra(const unsigned int n, const unsigned int a,
                const unsigned int b)
{
  static bool vis[maxv + 1];
  static dist_t dis[maxv + 1];
  priority_queue<pair_t, vector<pair_t>, greater<pair_t>> q;
  fill(dis, dis + 1 + n, inf);
  dis[a] = 0;
  q.emplace(0, a);
  while (!q.empty()) {
    const pair_t cur = q.top();
    q.pop();
    if (vis[cur.second]) continue;
    vis[cur.second] = true;
    for (const edge* i = head[cur.second]; i; i = i->pre)
      if (dis[i->to] > cur.first + i->val) {
        const unsigned int to = i->to;
        dis[to] = cur.first + i->val;
        q.emplace(dis[to], to);
      }
  }
  return dis[b];
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m, a, b;
  dist_t c;
  cin >> n >> m >> c;
  const unsigned int lv = ceil(log2(n));
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int f, t, v;
    cin >> f >> t >> v;
    addEdge(f, t, v);
  }
  build(lv, c);
  cin >> a >> b;
  cout << dijkstra(1 << lv, a, b) << endl;
  return 0;
}