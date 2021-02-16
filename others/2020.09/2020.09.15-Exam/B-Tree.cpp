#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxn = 100000;

struct Edge {
  unsigned int to;
  const Edge* pre;
} ed[maxn + 10], *cur = ed;
const Edge* head[maxn + 10];
unsigned int f[maxn + 10][2];

inline void addEdge(const unsigned int from, const unsigned int to)
{
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
}
void dfs(const unsigned int x)
{
  for (const Edge* i = head[x]; i; i = i->pre) {
    dfs(i->to);
    f[x][0] += f[i->to][1];
  }
  for (const Edge* i = head[x]; i; i = i->pre)
    f[x][1] = max(f[x][1], f[x][0] - f[i->to][1] + f[i->to][0] + 2);
}
void solve()
{
  unsigned int n, k;
  cin >> n >> k;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int v;
    cin >> v;
    addEdge(v - 1, i);
  }
  dfs(0);
  const unsigned int v = max(f[0][0], f[0][1]);
  cout << (v >= k ? (k + 1) >> 1 : v / 2 + (k - v)) << "\n";
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
    memset(f, 0, sizeof(f));
    cur = ed;
  }
  return 0;
}