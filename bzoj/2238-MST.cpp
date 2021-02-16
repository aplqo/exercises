#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cstdio>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 50000, maxm = 100000;
constexpr unsigned int inf = UINT_MAX;

class STree {
 public:
  void build(const unsigned int l, const unsigned int r, STree*(&cur))
  {
    lv = l;
    rv = r;
    mv = (l + r) >> 1;
    if (r - l == 1) {
      lc = rc = nullptr;
      val = inf;
      laz = inf;
      return;
    }
    lc = ++cur;
    lc->build(l, mv, cur);
    rc = ++cur;
    rc->build(mv, r, cur);
    this->val = min(lc->val, rc->val);
  }
  unsigned int query(const unsigned int l, const unsigned int r)
  {
    if (l == lv && r == rv) return val;
    pushdown();
    if (r <= mv)
      return lc->query(l, r);
    else if (l >= mv)
      return rc->query(l, r);
    else
      return min(lc->query(l, mv), rc->query(mv, r));
  }
  void modify(const unsigned int l, const unsigned int r,
              const unsigned int val)
  {
    if (l == lv && r == rv) {
      push(val);
      return;
    }
    pushdown();
    if (r <= mv)
      lc->modify(l, r, val);
    else if (l >= mv)
      rc->modify(l, r, val);
    else {
      lc->modify(l, mv, val);
      rc->modify(mv, r, val);
    }
    this->val = max(lc->val, rc->val);
  }

 private:
  void push(const unsigned int laz)
  {
    this->laz = this->laz ? min(this->laz, laz) : laz;
    this->val = min(this->val, laz);
  }
  void pushdown()
  {
    if (!laz) return;
    lc->push(laz);
    rc->push(laz);
    laz = 0;
  }

  unsigned int lv, mv, rv;
  STree *lc, *rc;
  unsigned int val, laz;
} SNodes[maxn * 4], *SRoot = SNodes;
struct edge {
  unsigned int from, to;
  unsigned int val;
  edge* pre;
  bool sel = false;
} ed[maxn + maxm + 1];
edge* head[maxn + 1];
unsigned int fa[maxn + 1], dep[maxn + 1], top[maxn + 1], son[maxn + 1],
    dfn[maxn + 1];

unsigned int dfsSon(const unsigned int x, const unsigned int f)
{
  unsigned int ret = 1, siz = 0;
  dep[x] = dep[f] + 1;
  fa[x] = f;
  for (edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f) continue;
    const unsigned int s = dfsSon(to, x);
    ret += s;
    if (s > siz) {
      son[x] = to;
      siz = s;
    }
  }
  return ret;
}
void dfsChain(const unsigned int x, const unsigned int tp)
{
  static unsigned int sta = 0;
  dfn[x] = ++sta;
  top[x] = tp;
  if (son[x]) dfsChain(son[x], tp);
  for (edge* i = head[x]; i; i = i->pre)
    if (!dfn[i->to]) dfsChain(i->to, i->to);
}
void modify(unsigned int x, unsigned int y, const unsigned int val)
{
  while (top[x] != top[y]) {
    if (dep[top[x]] > dep[top[y]]) {
      SRoot->modify(dfn[top[x]], dfn[x] + 1, val);
      x = fa[top[x]];
    }
    else {
      SRoot->modify(dfn[top[y]], dfn[y] + 1, val);
      y = fa[top[y]];
    }
  }
  if (dep[x] > dep[y])
    SRoot->modify(dfn[y] + 1, dfn[x] + 1, val);
  else if (dep[y] > dep[x])
    SRoot->modify(dfn[x] + 1, dfn[y] + 1, val);
}
unsigned int query(unsigned int x, unsigned int y)
{
  unsigned int ret = inf;
  while (top[x] != top[y]) {
    if (dfn[top[x]] > dfn[top[y]]) {
      ret = min(ret, SRoot->query(dfn[top[x]], dfn[x] + 1));
      x = fa[top[x]];
    }
    else {
      ret = min(ret, SRoot->query(dfn[top[y]], dfn[y] + 1));
      y = fa[top[y]];
    }
  }
  if (dep[x] > dep[y])
    ret = min(ret, SRoot->query(dfn[y] + 1, dfn[x] + 1));
  else if (dep[y] > dep[x])
    ret = min(ret, SRoot->query(dfn[x] + 1, dfn[y] + 1));
  return ret;
}
static void treeInit(const unsigned int n, const unsigned int m)
{
  dfsSon(1, 0);
  dfsChain(1, 1);
  {
    STree* c = SNodes;
    SRoot->build(1, n + 1, c);
  }
  for (edge* i = ed + 1; i < ed + m + 1; ++i)
    if (!i->sel) modify(i->from, i->to, i->val);
}

namespace dsu {
unsigned int fa[maxn + 1], siz[maxn + 1];
static void init(const unsigned int n)
{
  iota(fa, fa + 1 + n, 0);
  fill(siz, siz + 1 + n, 1);
}
unsigned int getFather(const unsigned int x)
{
  return fa[x] == x ? x : fa[x] = getFather(fa[x]);
}
bool tryMerge(const unsigned int x, const unsigned int y)
{
  unsigned int fx = getFather(x), fy = getFather(y);
  if (fx == fy) return false;
  if (siz[fy] > siz[fx]) swap(fx, fy);
  fa[fy] = fx;
  siz[fx] += siz[fy];
  return true;
}
}  // namespace dsu
namespace mst {
static unsigned int kruskal(const unsigned int n, const unsigned int m)
{
  static edge* tmp[maxm + 1];
  dsu::init(n);
  transform(ed + 1, ed + 1 + m, tmp, [](edge& i) { return &i; });
  sort(tmp, tmp + m, [](edge* l, edge* r) { return l->val < r->val; });
  unsigned int cnt = 0, ret = 0;
  for (unsigned int i = 0; i < m && cnt < n - 1; ++i)
    if (dsu::tryMerge(tmp[i]->from, tmp[i]->to)) {
      ++cnt;
      ret += tmp[i]->val;
      tmp[i]->sel = true;
    }
  if (cnt < n - 1) return inf;
  return ret;
}
static void buildGraph(const unsigned int m)
{
  static const auto add = [](edge* i) {
    i->pre = head[i->from];
    head[i->from] = i;
  };
  edge* cur = ed + m + 1;
  for (edge* i = ed + 1; i < ed + m + 1; ++i) {
    if (!i->sel) continue;
    *cur = *i;
    swap(cur->from, cur->to);
    add(cur);
    add(i);
    ++cur;
  }
}
}  // namespace mst

int main(int argc, char* argv[])
{
#ifndef APDEBUG
  ios_base::sync_with_stdio(false);
#else
  if (argc > 1) freopen(argv[1], "r", stdin);
#endif
  unsigned int n, m, q;
  cin >> n >> m;
  for (edge* i = ed + 1; i < ed + 1 + m; ++i)
    cin >> i->from >> i->to >> i->val;
  cin >> q;
  const unsigned int v = mst::kruskal(n, m);
  if (v == inf) {
    for (unsigned int i = 0; i < q; ++i)
      cout << "Not connected" << endl;
    return 0;
  }
  mst::buildGraph(m);
  treeInit(n, m);
  for (unsigned int i = 0; i < q; ++i) {
    unsigned int p;
    cin >> p;
    if (!ed[p].sel)
      cout << v << endl;
    else {
      const unsigned int tmp = query(ed[p].from, ed[p].to);
      if (tmp == inf)
        cout << "Not connected" << endl;
      else
        cout << v + tmp - ed[p].val << endl;
    }
  }
  return 0;
}
