#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
#include <set>
#include <utility>
using namespace std;
const unsigned int maxn = 1e5, maxm = 1e5;
const unsigned int maxv = maxn * 2, maxe = maxn + maxm;
constexpr unsigned int inf = UINT_MAX;

template <unsigned int V, unsigned int E>
struct Graph {
  struct Edge {
    unsigned int to;
    const Edge* pre;
  } ed[E + 10], *cur = ed;
  const Edge* head[V + 1] = {};

  inline void addEdge(const unsigned int from, const unsigned int to)
  {
    new (cur) Edge{to, head[from]};
    head[from] = cur++;
    new (cur) Edge{from, head[to]};
    head[to] = cur++;
  }
};
Graph<maxn, maxm * 2> orig;
Graph<maxv, maxe * 2> tree;
class STree {
 public:
  void build(const unsigned int l, const unsigned int r, STree*(&cur),
             const unsigned int v[]);
  void modify(const unsigned int p, const unsigned int val);
  unsigned int query(const unsigned int l, const unsigned int r) const;

 private:
  unsigned int lv, mv, rv;
  STree *lc, *rc;
  unsigned int val;
} SNodes[maxv * 4 + 10], *root = SNodes;
unsigned int cost[maxv + 1], vCnt, n;

void STree::build(const unsigned int l, const unsigned int r, STree*(&cur),
                  const unsigned int v[])
{
  lv = l;
  rv = r;
  mv = (l + r) >> 1;
  if (r - l == 1) {
    val = v[l];
    return;
  }
  lc = ++cur;
  rc = ++cur;
  lc->build(l, mv, cur, v);
  rc->build(mv, r, cur, v);
  val = min(lc->val, rc->val);
}
void STree::modify(const unsigned int p, const unsigned int val)
{
  if (rv - lv == 1) {
    this->val = val;
    return;
  }
  (p < mv ? lc : rc)->modify(p, val);
  this->val = min(lc->val, rc->val);
}
unsigned int STree::query(const unsigned int l, const unsigned int r) const
{
  if (l == lv && r == rv) return val;
  if (r <= mv)
    return lc->query(l, r);
  else if (l >= mv)
    return rc->query(l, r);
  else
    return min(lc->query(l, mv), rc->query(mv, r));
}
namespace tarjan {
unsigned int dfn[maxn + 1], low[maxn + 1];

void tarjan(const unsigned int x, const unsigned int f)
{
  static unsigned int stk[maxn + 1], *top = stk;
  static unsigned int sta = 0;
  dfn[x] = low[x] = ++sta;
  *(top++) = x;
  for (auto i = orig.head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f) continue;
    if (!dfn[to]) {
      tarjan(to, x);
      low[x] = min(low[x], low[to]);
      if (dfn[x] <= low[to]) {
        const unsigned int sp = ++vCnt;
        while (*(top - 1) != to) {
          tree.addEdge(sp, *(top - 1));
          --top;
        }
        tree.addEdge(sp, to);
        --top;
        tree.addEdge(sp, x);
      }
    }
    else
      low[x] = min(low[x], dfn[to]);
  }
}
}  // namespace tarjan
multiset<unsigned int> vals[maxv + 1];
unsigned int addChild(const unsigned int x, const unsigned int f)
{
  if (x <= n) return cost[x];
  for (auto i = tree.head[x]; i; i = i->pre)
    if (i->to != f) vals[x].insert(cost[i->to]);
  return *vals[x].begin();
}
namespace Chain {
unsigned int fa[maxv + 1], top[maxv + 1], son[maxv + 1], dep[maxv + 1],
    dfn[maxv + 1];
unsigned int dval[maxv + 1];

unsigned int dfsSon(const unsigned int x, const unsigned int f)
{
  dep[x] = dep[f] + 1;
  fa[x] = f;
  unsigned int ret = 1, ms = 0;
  for (auto i = tree.head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f) continue;
    const unsigned int sz = dfsSon(to, x);
    ret += sz;
    if (ms < sz) {
      son[x] = to;
      ms = sz;
    }
  }
  return ret;
}
void dfsChain(const unsigned int x, const unsigned int tp, const unsigned int f)
{
  static unsigned int sta = 0;
  dfn[x] = ++sta;
  top[x] = tp;
  if (son[x]) dfsChain(son[x], tp, x);
  for (auto i = tree.head[x]; i; i = i->pre)
    if (!top[i->to]) dfsChain(i->to, i->to, x);
  dval[dfn[x]] = addChild(x, f);
}
static void init(const unsigned int n)
{
  dfsSon(1, 0);
  dfsChain(1, 1, 0);
  {
    STree* c = SNodes;
    root->build(1, n + 1, c, dval);
  }
}
pair<unsigned int, unsigned int> query(unsigned int a, unsigned int b)
{
  unsigned int ret = inf;
  while (top[a] != top[b]) {
    if (dep[top[a]] > dep[top[b]]) swap(a, b);
    ret = min(ret, root->query(dfn[top[b]], dfn[b] + 1));
    b = fa[top[b]];
  }
  if (dfn[a] > dfn[b]) swap(a, b);
  return make_pair(a, min(ret, root->query(dfn[a], dfn[b] + 1)));
}
inline void modify(const unsigned int x, const unsigned int val)
{
  dval[dfn[x]] = val;
  root->modify(dfn[x], val);
}
}  // namespace Chain
inline void modify(const unsigned int x, const unsigned int val)
{
  if (Chain::fa[x] > n) {
    const unsigned int f = Chain::fa[x];
    vals[f].erase(vals[f].find(Chain::dval[Chain::dfn[x]]));
    vals[f].insert(val);
    Chain::modify(f, *vals[f].begin());
  }
  Chain::modify(x, val);
}
inline unsigned int query(const unsigned int a, const unsigned int b)
{
  using Chain::dfn;
  using Chain::fa;
  const auto [lca, v] = Chain::query(a, b);
  if (lca > n && fa[lca])
    return min(v, Chain::dval[dfn[fa[lca]]]);
  else
    return v;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int m, q;
  cin >> n >> m >> q;
  copy_n(istream_iterator<unsigned int>(cin), n, cost + 1);
  vCnt = n;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int a, b;
    cin >> a >> b;
    orig.addEdge(a, b);
  }
  tarjan::tarjan(1, 0);
  Chain::init(vCnt);
  for (unsigned int i = 0; i < q; ++i) {
    char op;
    unsigned int a, arg2;
    cin >> op >> a >> arg2;
    if (op == 'C')
      modify(a, arg2);
    else
      cout << query(a, arg2) << "\n";
  }
  return 0;
}