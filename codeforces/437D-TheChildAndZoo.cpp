#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
using namespace std;
const unsigned int maxn = 1e5, maxm = 1e5;

struct Edge {
  unsigned int from, to;
  unsigned int val;
} ed[maxm + 10];
unsigned int a[maxn + 10];

namespace dsu {
unsigned int fa[maxn + 10], siz[maxn + 10];
static void init(const unsigned int n)
{
  iota(fa, fa + n + 1, 0);
  fill(siz, siz + 1 + n, 1);
}
unsigned int find(const unsigned int x)
{
  return fa[x] == x ? x : fa[x] = find(fa[x]);
}
unsigned long long merge(unsigned int x, unsigned int y)
{
  x = find(x);
  y = find(y);
  if (x == y) return 0;
  if (siz[x] < siz[y]) swap(x, y);
  const unsigned long long ret =
      static_cast<unsigned long long>(siz[x]) * siz[y];
  fa[y] = x;
  siz[x] += siz[y];
  return ret;
}
}  // namespace dsu

double kruskal(const unsigned int n, const unsigned int m)
{
  dsu::init(n);
  sort(ed, ed + m, [](const Edge& a, const Edge& b) { return a.val > b.val; });
  unsigned long long ret = 0;
  const Edge* ptr = ed;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned long long v = 0;
    for (; !v; ++ptr)
      v = dsu::merge(ptr->from, ptr->to);
    ret += v * (ptr - 1)->val;
  }
  return ret / (static_cast<double>(n) * (n - 1) / 2.0);
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  copy_n(istream_iterator<unsigned int>(cin), n, a + 1);
  for (Edge* i = ed; i < ed + m; ++i) {
    cin >> i->from >> i->to;
    i->val = min(a[i->from], a[i->to]);
  }
  cout << fixed << setprecision(numeric_limits<double>::digits10)
       << kruskal(n, m) << "\n";
  return 0;
}