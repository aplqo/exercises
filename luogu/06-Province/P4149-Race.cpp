#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <map>
using namespace std;
const unsigned int maxn = 2e5;

struct edge {
  unsigned int to;
  unsigned int val;
  const edge* pre;
} ed[maxn * 2];
const edge* head[maxn + 1];
bool vis[maxn + 1];
unsigned int siz[maxn + 1];
unsigned int ans = UINT_MAX, k;
map<unsigned int, unsigned int> pat;
unsigned int dis[maxn + 1], dep[maxn + 1];

void adEdge(const unsigned int from, const unsigned int to,
            const unsigned int val)
{
  static edge* cur = ed;
  cur->to = to;
  cur->val = val;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
void dfsPoint(const unsigned int x, const unsigned int f, const unsigned int n,
              unsigned int& pos, unsigned int& maxv)
{
  unsigned int ms = 0;
  siz[x] = 1;
  for (const edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f || vis[to]) continue;
    dfsPoint(to, x, n, pos, maxv);
    ms = max(ms, siz[to]);
    siz[x] += siz[to];
  }
  ms = max(ms, n - siz[x]);
  if (ms < maxv) {
    pos = x;
    maxv = ms;
  }
}
unsigned int treeCenter(const unsigned int n, const unsigned int x)
{
  unsigned int pos, mv = UINT_MAX;
  dfsPoint(x, 0, n, pos, mv);
  return pos;
}
void dfsSubtree(const unsigned int x, const unsigned int f)
{
  if (dep[x] > ans || dis[x] > k) return;
  if (auto it = pat.find(k - dis[x]); it != pat.end())
    ans = min(ans, dep[x] + it->second);
  for (const edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f || vis[to]) continue;
    dep[to] = dep[x] + 1;
    dis[to] = dis[x] + i->val;
    dfsSubtree(to, x);
  }
}
void dfsAddVertex(const unsigned int x, const unsigned int f)
{
  if (dep[x] > ans || dis[x] > k) return;
  {
    auto it = pat.find(dis[x]);
    if (it == pat.end())
      pat[dis[x]] = dep[x];
    else
      it->second = min(it->second, dep[x]);
  }
  for (const edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f || vis[to]) continue;
    dfsAddVertex(to, x);
  }
}
void divide(const unsigned int x)
{
  vis[x] = true;
  pat[0] = 0;
  for (const edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (vis[to]) continue;
    dep[to] = 1;
    dis[to] = i->val;
    dfsSubtree(to, x);
    dfsAddVertex(to, x);
  }
  pat.clear();

  for (const edge* i = head[x]; i; i = i->pre)
    if (!vis[i->to]) divide(treeCenter(siz[i->to], i->to));
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n >> k;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int u, v, w;
    cin >> u >> v >> w;
    adEdge(u, v, w);
    adEdge(v, u, w);
  }
  divide(treeCenter(n, 0));
  if (ans == UINT_MAX)
    cout << "-1" << endl;
  else
    cout << ans << endl;
  return 0;
}