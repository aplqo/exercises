#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxn = 200, maxm = 10000;
const unsigned int maxv = maxn * 3, maxe = maxv * maxv * 2;

struct Edge {
  unsigned int from, to;
} ed[maxm + 1];
struct VirtEdge {
  unsigned int to;
  const VirtEdge* pre;
} ved[maxe + 1], *cur = ved;
const VirtEdge* head[maxv + 1];
bool col[maxm + 1], vis[maxm + 1];
unsigned int id[maxn + 1];

void addVirtEdge(const unsigned int from, const unsigned int to)
{
  cur->to = to;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
void buildGraph(const unsigned m)
{
  for (unsigned int i = 0; i < m; ++i)
    if (id[ed[i].from] > id[ed[i].to]) swap(ed[i].from, ed[i].to);
  for (unsigned int i = 0; i < m; ++i) {
    const Edge ei = ed[i];
    for (unsigned int j = 0; j < m; ++j) {
      const Edge ej = ed[j];
      if (id[ei.from] < id[ej.from] && id[ei.to] < id[ej.to] &&
          id[ej.from] < id[ei.to]) {
        addVirtEdge(i, j);
        addVirtEdge(j, i);
      }
    }
  }
}
bool dfs(const unsigned int x)
{
  vis[x] = true;
  for (const VirtEdge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (vis[to]) {
      if (col[to] == col[x]) return false;
    }
    else {
      col[to] = !col[x];
      if (!dfs(to)) return false;
    }
  }
  return true;
}
bool test(const unsigned int m)
{
  for (unsigned int i = 0; i < m; ++i)
    if (!vis[i] && !dfs(i)) return false;
  return true;
}
void solve()
{
  unsigned int n, m;
  cin >> n >> m;
  const bool skip = m > 3 * n - 6;
  for (Edge* i = ed; i < ed + m; ++i)
    cin >> i->from >> i->to;
  for (unsigned int i = 0; i < n; ++i) {
    unsigned int t;
    cin >> t;
    id[t] = i;
  }
  if (skip) {
    cout << "NO" << endl;
    return;
  }
  buildGraph(m);
  cout << (test(m) ? "YES" : "NO") << endl;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int t;
  cin >> t;
  for (unsigned int i = 0; i < t; ++i) {
    solve();
    memset(head, 0, sizeof(head));
    memset(vis, 0, sizeof(vis));
    cur = ved;
  }
  return 0;
}