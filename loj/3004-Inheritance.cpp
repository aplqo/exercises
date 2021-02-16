#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 1000, maxk = 1e4, maxm = 3e5;

struct Edge {
  unsigned int* bel;
  unsigned int from, to, val;
} ed[maxm + 10];
unsigned int belong[maxm + 10];
class Disjoint {
 public:
  void init(const unsigned int n);
  bool isConnect(const unsigned int x, const unsigned int y);
  void merge(unsigned int x, unsigned int y);

 private:
  unsigned int find(const unsigned int x);
  unsigned int fa[maxn + 10], siz[maxn + 10];
} eset[maxk + 10];

void Disjoint::init(const unsigned int n)
{
  iota(fa + 1, fa + 1 + n, 1);
  fill(siz + 1, siz + 1 + n, 1);
}
bool Disjoint::isConnect(const unsigned int x, const unsigned int y)
{
  return find(x) == find(y);
}
void Disjoint::merge(unsigned int x, unsigned int y)
{
  x = find(x);
  y = find(y);
  if (siz[x] < siz[y]) swap(x, y);
  fa[y] = x;
  siz[x] += siz[y];
}
unsigned int Disjoint::find(const unsigned int x)
{
  return fa[x] == x ? x : fa[x] = find(fa[x]);
}
unsigned int insert(const unsigned int k, const unsigned int x,
                    const unsigned int y)
{
  if (eset[k].isConnect(x, y)) return 0;
  unsigned int l = 1, r = k;
  while (l <= r) {
    const unsigned int mid = (l + r) >> 1;
    if (eset[mid].isConnect(x, y))
      l = mid + 1;
    else
      r = mid - 1;
  }
  eset[l].merge(x, y);
  return l;
}

void kruskal(const unsigned int n, const unsigned int m, const unsigned int k)
{
  for (Disjoint* i = eset; i < eset + k + 1; ++i)
    i->init(n);
  for (unsigned int i = 0; i < m; ++i)
    ed[i].bel = belong + i;
  sort(ed, ed + m, [](const Edge& a, const Edge& b) { return a.val > b.val; });
  for (Edge* i = ed; i < ed + m; ++i)
    *(i->bel) = insert(k, i->from, i->to);
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m, k;
  cin >> n >> m >> k;
  for (Edge* i = ed; i < ed + m; ++i)
    cin >> i->from >> i->to >> i->val;
  kruskal(n, m, k);
  copy(belong, belong + m, ostream_iterator<unsigned int>(cout, "\n"));
  cout.put('\n');
  return 0;
}