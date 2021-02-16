#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
#include <utility>
using namespace std;
const unsigned int maxv = 50000, maxe = 100000;

struct edge {
  unsigned int from, to;
  int val, acc = 0;
  bool typ;
} ed[maxe + 1];
const edge* seq[maxe + 1];
namespace dsu {
unsigned int fa[maxv + 1], siz[maxv + 1];

static void init(const unsigned int n)
{
  fill(siz, siz + n, 1);
  iota(fa, fa + n, 0);
}
unsigned int getFather(const unsigned int x)
{
  return fa[x] == x ? x : fa[x] = getFather(fa[x]);
}
bool tryMerge(unsigned int x, unsigned int y)
{
  x = getFather(x);
  y = getFather(y);
  if (x == y) return false;
  if (siz[x] < siz[y]) swap(x, y);
  fa[y] = x;
  siz[x] += siz[y];
  return true;
}
}  // namespace dsu
pair<int, int> kruskal(const unsigned int n, const unsigned int m,
                       const int cst)
{
  dsu::init(n);
  for (edge* i = ed; i < ed + m; ++i)
    if (i->typ) {
      i->val = i->val - i->acc + cst;
      i->acc = cst;
    }
  sort(seq, seq + m, [](const edge* a, const edge* b) {
    return a->val != b->val ? a->val < b->val : !(a->typ) < !(b->typ);
  });
  int ret = 0, cnt = 0;
  unsigned int p = 0;
  for (unsigned int i = 1; i < n; ++i) {
    while (p < m && !dsu::tryMerge(seq[p]->from, seq[p]->to))
      ++p;
    ret += seq[p]->val;
    cnt += seq[p]->typ;
    ++p;
  }
  return make_pair(ret, cnt);
}
int binarySearch(const unsigned int n, const unsigned int m, const int need)
{
  int l = -100, r = 100;
  while (l <= r) {
    const int mid = (l + r) / 2;
    const auto v = kruskal(n, m, mid);
    if (v.second == need)
      return v.first - v.second * mid;
    else if (v.second > need)
      l = mid + 1;
    else
      r = mid - 1;
  }
  return kruskal(n, m, r).first - need * r;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m, need;
  cin >> n >> m >> need;
  for (edge* i = ed; i < ed + m; ++i) {
    unsigned int t;
    cin >> i->from >> i->to >> i->val >> t;
    i->typ = !t;
  }
  transform(ed, ed + m, seq, [](const edge& i) { return &i; });
  cout << binarySearch(n, m, need) << endl;
  return 0;
}