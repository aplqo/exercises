#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <functional>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 3e4;

struct edge {
  unsigned int to;
  edge* pre = nullptr;
} ed[maxn * 2 + 10];
edge* head[maxn + 1];
void addEdge(const unsigned int from, const unsigned int to)
{
  static edge* cur = ed;
  cur->to = to;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}

enum op { Max = 0, Sum = 1 };
const function<int(int, int)> fold[] = {
    [](const int l, const int r) -> int { return max(l, r); },
    [](const int l, const int r) -> int { return l + r; }};
const int initVal[] = {INT_MIN, 0};

class STree {
 public:
  void build(const unsigned int l, const unsigned int r, STree*(&cur),
             int dat[])
  {
    lv = l;
    rv = r;
    mv = (l + r) >> 1;
    if (r - l == 1) {
      val[0] = val[1] = dat[l];
      return;
    }
    lc = ++cur;
    lc->build(l, mv, cur, dat);
    rc = ++cur;
    rc->build(mv, r, cur, dat);
    pushup();
  }
  void modify(const unsigned int p, const int val)
  {
    if (rv - lv == 1 && p == lv) {
      this->val[0] = this->val[1] = val;
      return;
    }
    if (p < mv)
      lc->modify(p, val);
    else if (p >= mv)
      rc->modify(p, val);
    pushup();
  }
  template <op o>
  int query(const unsigned int l, const unsigned int r)
  {
    if (l == lv && r == rv) return val[o];
    if (r <= mv)
      return lc->query<o>(l, r);
    else if (l >= mv)
      return rc->query<o>(l, r);
    else
      return fold[o](lc->query<o>(l, mv), rc->query<o>(mv, r));
  }

 private:
  void pushup()
  {
    val[Max] = max(lc->val[Max], rc->val[Max]);
    val[Sum] = lc->val[Sum] + rc->val[Sum];
  }

  unsigned int lv, mv, rv;
  int val[2];
  STree *lc = nullptr, *rc = nullptr;
} SNode[maxn * 4 + 10], *SRoot = SNode;
int val[maxn + 1], orig[maxn + 1];

unsigned int fa[maxn + 1], son[maxn + 1];
unsigned int dfn[maxn + 1], top[maxn + 1], dep[maxn + 1];

unsigned int dfsSon(const unsigned int x, const unsigned int f)
{
  fa[x] = f;
  dep[x] = dep[f] + 1;
  unsigned int s = 0, ret = 1;
  for (edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f) continue;
    const unsigned int sz = dfsSon(to, x);
    ret += sz;
    if (sz > s) {
      son[x] = to;
      s = sz;
    }
  }
  return ret;
}
void dfsChain(const unsigned int x, const unsigned int tp)
{
  static unsigned int sta = 0;
  top[x] = tp;
  dfn[x] = ++sta;
  if (son[x]) dfsChain(son[x], tp);
  for (edge* i = head[x]; i; i = i->pre)
    if (!dfn[i->to]) dfsChain(i->to, i->to);
}
void init(const unsigned int n)
{
  dfsSon(1, 0);
  dfsChain(1, 1);
  for (unsigned int i = 1; i <= n; ++i)
    val[dfn[i]] = orig[i];
  STree* c = SNode;
  SRoot->build(1, n + 1, c, val);
}

template <op o>
int query(unsigned int x, unsigned int y)
{
  int ret = initVal[o];
  while (top[x] != top[y]) {
    if (dep[top[x]] < dep[top[y]]) {
      ret = fold[o](ret, SRoot->query<o>(dfn[top[y]], dfn[y] + 1));
      y = fa[top[y]];
    }
    else {
      ret = fold[o](ret, SRoot->query<o>(dfn[top[x]], dfn[x] + 1));
      x = fa[top[x]];
    }
  }
  if (dep[x] > dep[y]) swap(x, y);
  ret = fold[o](ret, SRoot->query<o>(dfn[x], dfn[y] + 1));
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, q;
  cin >> n;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int a, b;
    cin >> a >> b;
    addEdge(a, b);
    addEdge(b, a);
  }
  copy_n(istream_iterator<int>(cin), n, orig + 1);
  init(n);
  cin >> q;
  for (unsigned int i = 0; i < q; ++i) {
    char op[10];
    unsigned int u, v;
    int t;
    cin >> op >> u;
    if (op[0] == 'C')  // Change
    {
      cin >> t;
      SRoot->modify(dfn[u], t);
    }
    else if (op[1] == 'S')  // QSUM
    {
      cin >> v;
      cout << query<Sum>(u, v) << endl;
    }
    else  // QMAX
    {
      cin >> v;
      cout << query<Max>(u, v) << endl;
    }
  }
  return 0;
}