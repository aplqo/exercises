#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxn = 5000;

struct Edge {
  unsigned int to;
  const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
unsigned int cnt[maxn + 10], dep[maxn * 2 + 1];
unsigned long long ans = 0;

inline void addEdge(const unsigned int from, const unsigned int to)
{
  static Edge* cur = ed;
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, head[to]};
  head[to] = cur++;
}
void dfs(const unsigned int x, const unsigned int f)
{
  unsigned int cn = 0;
  for (const Edge* i = head[x]; i; i = i->pre)
    if (i->to != f) {
      dep[i->to] = dep[x] + 1;
      const unsigned int old = cnt[dep[x] * 2];
      dfs(i->to, x);
      const unsigned int nv = cnt[dep[x] * 2] - old;
      ans += nv * cn;
      cn += nv;
    }
  ++cnt[dep[x]];
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int a, b;
    cin >> a >> b;
    addEdge(a, b);
  }
  for (unsigned int i = 1; i <= n; ++i) {
    dep[i] = 0;
    dfs(i, 0);
  }
  cout << ans / 3 << "\n";
  return 0;
}