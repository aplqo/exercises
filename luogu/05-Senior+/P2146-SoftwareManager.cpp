#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e5;

struct edge {
  unsigned int to;
  edge* pre = nullptr;
} ed[maxn + 10];
edge* head[maxn + 1];
unsigned int fa[maxn + 1], top[maxn + 1], siz[maxn + 1];
unsigned int son[maxn + 1], dfn[maxn + 1];

void addEdge(const unsigned int from, const unsigned int to)
{
  static edge* cur = ed;
  cur->to = to;
  cur->pre = head[from];
  head[from] = cur;
  fa[to] = from;
  ++cur;
}

class STree {
 public:
  void build(const unsigned int l, const unsigned int r, STree*(&cur))
  {
    lv = l;
    rv = r;
    mv = (l + r) >> 1;
    if (r - l == 1) return;
    lc = ++cur;
    lc->build(l, mv, cur);
    rc = ++cur;
    rc->build(mv, r, cur);
  }
  void modify(const unsigned int l, const unsigned int r, const bool typ)
  {
    if (rv - lv == 1) {
      val = typ;
      return;
    }
    if (l == lv && r == rv) {
      push(typ);
      return;
    }
    pushdown();
    if (r <= mv)
      lc->modify(l, r, typ);
    else if (l >= mv)
      rc->modify(l, r, typ);
    else {
      lc->modify(l, mv, typ);
      rc->modify(mv, r, typ);
    }
    val = lc->val + rc->val;
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
      return lc->query(l, mv) + rc->query(mv, r);
  }

 private:
  void push(const bool c)
  {
    laz = c;
    dirt = true;
    val = c ? rv - lv : 0;
  }
  void pushdown()
  {
    if (!dirt) return;
    lc->push(laz);
    rc->push(laz);
    dirt = false;
  }

  unsigned int lv, mv, rv;
  STree *lc = nullptr, *rc = nullptr;
  unsigned int val = 0;
  bool laz, dirt = false;
} SNode[maxn * 4 + 10], *SRoot = SNode;

unsigned int dfsSon(const unsigned int x)
{
  const unsigned int f = fa[x];
  unsigned int s = 0;
  for (edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f) continue;
    const unsigned int sz = dfsSon(to);
    siz[x] += sz;
    if (sz > s) {
      s = sz;
      son[x] = to;
    }
  }
  return ++siz[x];
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
void init(const unsigned int n)
{
  dfsSon(0);
  dfsChain(0, 0);
  STree* c = SNode;
  SRoot->build(1, n + 1, c);
}

void install(unsigned int x)
{
  bool fl = true;
  while (fl) {
    SRoot->modify(dfn[top[x]], dfn[x] + 1, true);
    if (!x) fl = false;
    x = fa[top[x]];
  }
}
void uninstall(const unsigned int x)
{
  SRoot->modify(dfn[x], dfn[x] + siz[x], false);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, q;
  cin >> n;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int t;
    cin >> t;
    addEdge(t, i);
  }
  init(n);
  cin >> q;
  unsigned int cur = 0;
  for (unsigned int i = 0; i < q; ++i) {
    char op[10];
    unsigned int x;
    cin >> op >> x;
    if (op[0] == 'i')
      install(x);
    else
      uninstall(x);
    const unsigned int nv = SRoot->query(dfn[0], dfn[0] + siz[0]);
    cout << (nv > cur ? nv - cur : cur - nv) << endl;
    cur = nv;
  }
  return 0;
}