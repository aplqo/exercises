#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 2000, maxe = maxn * maxn;

struct Edge {
  unsigned int u, v;
  unsigned long long val;
} ed[maxe + 10];

namespace dsu {
unsigned int fa[maxn + 10], siz[maxn + 10];
static void init(const unsigned int n)
{
  fill(siz, siz + 1 + n, 1);
  iota(fa, fa + 1 + n, 0);
}
unsigned int find(const unsigned int x)
{
  return fa[x] == x ? x : fa[x] = find(fa[x]);
}
bool merge(unsigned int x, unsigned int y)
{
  x = find(x);
  y = find(y);
  if (x == y) return false;
  if (siz[x] < siz[y]) swap(x, y);
  fa[y] = x;
  siz[x] += siz[y];
  return true;
}
}  // namespace dsu
unsigned long long kruskal(const unsigned int n)
{
  dsu::init(n + 1);
  const unsigned int e = n * (n + 1) / 2;
  sort(ed, ed + e, [](const Edge& a, const Edge& b) { return a.val < b.val; });
  const Edge* ptr = ed;
  unsigned long long ret = 0;
  for (unsigned int i = 0; i < n; ++i) {
    while (ptr < ed + e && !dsu::merge(ptr->u, ptr->v))
      ++ptr;
    ret += (ptr++)->val;
  }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  {
    Edge* cur = ed;
    for (unsigned int i = 1; i <= n; ++i)
      for (unsigned int j = i; j <= n; ++j) {
        cin >> cur->val;
        cur->u = i;
        cur->v = j + 1;
        ++cur;
      }
  }
  cout << kruskal(n) << "\n";
  return 0;
}