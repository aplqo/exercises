#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxn = 200;

struct edge {
  unsigned int to;
  edge* pre = nullptr;
} ed[maxn * maxn * 2 + 10], *cur = ed;
edge* head[maxn * 2 + 1];
unsigned int mat[maxn * 2 + 1];
bool vis[maxn * 2 + 1];

void addEdge(const unsigned int from, const unsigned int to)
{
  cur->to = to;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}

bool dfs(const unsigned int x)
{
  for (edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (vis[to]) continue;
    vis[to] = true;
    if (!mat[to] || dfs(mat[to])) {
      mat[to] = x;
      return true;
    }
  }
  return false;
}
unsigned int Hungrian(const unsigned int n)
{
  unsigned int ret = 0;
  for (unsigned int i = 1; i <= n; ++i) {
    fill(vis, vis + n * 2 + 1, false);
    if (dfs(i)) ++ret;
  }
  return ret;
}

void solve()
{
  unsigned int n;
  cin >> n;
  for (unsigned int i = 1; i <= n; ++i)
    for (unsigned int j = 1; j <= n; ++j) {
      unsigned int t;
      cin >> t;
      if (t) {
        addEdge(i, j + n);
        addEdge(j + n, i);
      }
    };
  cout << (Hungrian(n) == n ? "Yes" : "No") << endl;
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
    memset(mat, 0, sizeof(mat));
    cur = ed;
  }
  return 0;
}