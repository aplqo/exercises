#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdio>
#include <queue>
using namespace std;
const unsigned int maxn = 1000, maxe = 1000;
using real = double;

struct edge {
  unsigned int to;
  const edge* pre;
} ed[maxe * 2 + 1];
const edge* head[maxn + 1];
unsigned int dis[maxn + 1], nxt[maxn + 1][maxn + 1];
real f[maxn + 1][maxn + 1];

void addEdge(const unsigned int from, const unsigned int to)
{
  static edge* cur = ed;
  cur->to = to;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
void bfs(const unsigned int s)
{
  static unsigned int vis[maxn + 1], sta = 0;
  queue<unsigned int> q;
  q.push(s);
  dis[s] = 0;
  ++sta;
  vis[s] = sta;
  while (!q.empty()) {
    const unsigned int cur = q.front();
    q.pop();
    for (const edge* i = head[cur]; i; i = i->pre) {
      const unsigned int to = i->to;
      if (vis[to] == sta) continue;
      dis[to] = dis[cur] + 1;
      vis[to] = sta;
      q.push(to);
    }
  }
}
static void init(const unsigned int n)
{
  for (unsigned int i = 1; i <= n; ++i) {
    fill(nxt[i] + 1, nxt[i] + 1 + n, UINT_MAX);
    nxt[i][i] = i;
  }
  for (unsigned int i = 1; i <= n; ++i) {
    bfs(i);
    for (unsigned int j = 1; j <= n; ++j)
      for (const edge* k = head[j]; k; k = k->pre)
        if (dis[k->to] + 1 == dis[j]) nxt[j][i] = min(nxt[j][i], k->to);
  }
}
real dp(const unsigned int posc, const unsigned int posk)
{
  if (posk == posc) return 0;
  if (nxt[posc][posk] == posk) return 1;
  if (f[posc][posk] > 0) return f[posc][posk];
  const unsigned int nc = nxt[nxt[posc][posk]][posk];
  if (nc == posk) return 1;
  unsigned int cnt = 1;
  real ret = dp(nc, posk);
  for (const edge* i = head[posk]; i; i = i->pre) {
    ret += dp(nc, i->to);
    ++cnt;
  }
  return f[posc][posk] = ret / cnt + 1;
}
int main()
{
  unsigned int n, e, c, m;
  scanf("%u%u%u%u", &n, &e, &c, &m);
  for (unsigned int i = 0; i < e; ++i) {
    unsigned int s, t;
    scanf("%u%u", &s, &t);
    addEdge(s, t);
    addEdge(t, s);
  }
  init(n);
  printf("%.3lf\n", round(dp(c, m) * 1e3) / 1e3);
  return 0;
}