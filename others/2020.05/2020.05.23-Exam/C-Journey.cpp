/*Luogu team T134381: Journey*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 1e4, maxm = 1e5, maxl = 15;

struct edge {
  unsigned int from, to, val;
  bool sel = false;
  edge* pre = nullptr;
} ed[maxm * 2];
edge *head[maxn + 1], *tmp[maxm + 1];

namespace lca {
unsigned int dep[maxn + 1], lg[maxn + 1];
unsigned int fa[maxn + 1][maxl + 1], mv[maxn + 1][maxl + 1];
void getLog(const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i)
    lg[i] = lg[i >> 1] + 1;
}
void init(const unsigned int x, const unsigned int f)
{
  dep[x] = dep[f] + 1;
  fa[x][0] = f;
  for (unsigned int i = 1; i <= lg[dep[x]]; ++i) {
    fa[x][i] = fa[fa[x][i - 1]][i - 1];
    mv[x][i] = max(mv[x][i - 1], mv[fa[x][i - 1]][i - 1]);
  }
  for (edge* i = head[x]; i; i = i->pre)
    if (i->to != f) {
      mv[i->to][0] = i->val;
      init(i->to, x);
    }
}
unsigned int lca(unsigned int a, unsigned int b)
{
  unsigned int ret = 0;
  if (dep[a] != dep[b]) {
    if (dep[a] > dep[b]) swap(a, b);
    unsigned int d = dep[b] - dep[a], t = 1;
    for (unsigned int i = 0; d; ++i, t <<= 1)
      if (d & t) {
        ret = max(ret, mv[b][i]);
        b = fa[b][i];
        d ^= t;
      }
  }
  if (a == b) return ret;
  for (unsigned int k = lg[dep[a]] + 1; k;) {
    const unsigned int m = k - 1;
    if (fa[a][m] == fa[b][m])
      --k;
    else {
      ret = max({ret, mv[a][m], mv[b][m]});
      a = fa[a][m];
      b = fa[b][m];
    }
  }
  return max({ret, mv[a][0], mv[b][0]});
}
}  // namespace lca
namespace find {
unsigned int fa[maxn + 1], siz[maxn + 1];
void init(const unsigned int n)
{
  iota(fa, fa + n + 1, 0);
  fill(siz + 1, siz + n + 1, 1);
}
unsigned int getFather(const unsigned int x)
{
  return fa[x] == x ? x : fa[x] = getFather(fa[x]);
}
bool tryMerge(const unsigned int x, const unsigned int y)
{
  unsigned int fx = getFather(x), fy = getFather(y);
  if (fx == fy) return false;
  if (siz[fx] < siz[fy]) swap(fx, fy);
  fa[fy] = fx;
  siz[fx] += siz[fy];
  return true;
}
}  // namespace find

void kruskal(const unsigned int n, const unsigned int m)
{
  transform(ed, ed + m, tmp, [](edge& i) { return &i; });
  sort(tmp, tmp + m,
       [](const edge* a, const edge* b) { return a->val < b->val; });
  unsigned int sel = 0;
  for (unsigned int i = 0; i < m && sel < n - 1; ++i) {
    edge* const cur = tmp[i];
    if (find::tryMerge(cur->from, cur->to)) {
      cur->sel = true;
      ++sel;
    }
  }
}
void build(const unsigned int n, const unsigned int m)
{
  const auto add = [](edge* i, const unsigned int f) {
    i->pre = head[f];
    head[f] = i;
  };
  edge* cur = ed + m;
  for (edge* i = ed; i < ed + m; ++i)
    if (i->sel) {
      add(i, i->from);
      cur->val = i->val;
      cur->to = i->from;
      add(cur, i->to);
      ++cur;
    }
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m, q;
  cin >> n >> m;
  for (edge* i = ed; i < ed + m; ++i)
    cin >> i->from >> i->to >> i->val;
  find::init(n);
  kruskal(n, m);
  build(n, m);
  lca::getLog(n);
  lca::init(1, 0);
  cin >> q;
  for (unsigned int i = 0; i < q; ++i) {
    unsigned int s, t;
    cin >> s >> t;
    if (find::getFather(s) != find::getFather(t))
      cout << "0" << endl;
    else
      cout << lca::lca(s, t) << endl;
  }
  return 0;
}