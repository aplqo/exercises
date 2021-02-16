#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxp = 100000, maxn = 10000;
const unsigned int maxv = maxn, maxe = maxp;

struct edge {
  unsigned int from, to;
  unsigned int val;
} ed[maxe + 1];
unsigned int c[maxn + 1];
unsigned int fa[maxv + 1], siz[maxv + 1];

static void init(const unsigned int n)
{
  fill(siz + 1, siz + 1 + n, 1);
  iota(fa, fa + 1 + n, 0);
}
unsigned int getFather(const unsigned int x)
{
  return fa[x] == x ? x : fa[x] = getFather(fa[x]);
}
inline bool tryMerge(const unsigned int x, const unsigned int y)
{
  unsigned int fx = getFather(x), fy = getFather(y);
  if (fx == fy) return false;
  if (siz[fx] < siz[fy]) swap(fx, fy);
  fa[fy] = fx;
  siz[fx] += siz[fy];
  return true;
}
unsigned int kruskal(const unsigned int n, const unsigned int p)
{
  sort(ed, ed + p, [](const edge& a, const edge& b) { return a.val < b.val; });
  const edge* ptr = ed;
  unsigned int ret = 0;
  for (unsigned int i = 1; i < n; ++i) {
    while (!tryMerge(ptr->from, ptr->to))
      ++ptr;
    ret += ptr->val;
  }
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, p;
  cin >> n >> p;
  copy_n(istream_iterator<unsigned int>(cin), n, c + 1);
  for (edge* i = ed; i < ed + p; ++i) {
    cin >> i->from >> i->to >> i->val;
    i->val = c[i->from] + c[i->to] + 2 * (i->val);
  }
  init(n);
  cout << (kruskal(n, p) + *min_element(c + 1, c + 1 + n)) << endl;
  return 0;
}
