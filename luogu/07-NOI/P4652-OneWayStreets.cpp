#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 100000, maxm = 100000, maxp = 100000;

enum Direct { B = 0, L = 1, R = 2 } direct[maxm + 10]{};
const char ch[3] = {'B', 'L', 'R'};
struct Constraint {
  unsigned int from, to, lca;
} con[maxp + 10];
class Array {
 public:
  inline void init() { ptr = val; }
  inline void push_back(const unsigned int x) { *(ptr++) = x; }

  unsigned int val[maxn + 10], *ptr;
};
namespace OrigGraph {
struct Edge {
  unsigned int from, to;
  mutable bool isBridge;
  Direct dir, *val;
  const Edge *pre, *rev;
} ed[maxm * 2 + 10];
const Edge* head[maxn + 10];

inline void addEdge(const unsigned int id, const unsigned int from,
                    const unsigned int to)
{
  static Edge* cur = ed;
  new (cur) Edge{from, to, false, R, direct + id, head[from], cur + 1};
  head[from] = cur++;
  new (cur) Edge{to, from, false, L, direct + id, head[to], cur - 1};
  head[to] = cur++;
}
}  // namespace OrigGraph
namespace Disjoint {
unsigned int fa[maxn + 10], siz[maxn + 10];

static void init(const unsigned int n)
{
  iota(fa, fa + 1 + n, 0);
  fill(siz, siz + 1 + n, 1);
}
unsigned int find(const unsigned int x)
{
  return fa[x] == x ? x : fa[x] = find(fa[x]);
}
inline void merge(unsigned int x, unsigned int y)
{
  x = find(x);
  y = find(y);
  if (x == y) return;
  if (siz[x] < siz[y]) swap(x, y);
  fa[y] = x;
  siz[x] += siz[y];
}
}  // namespace Disjoint
namespace Tree {
struct Edge {
  unsigned int to;
  Direct *val, dir;
  const Edge *pre, *rev;
} ed[maxn * 2 + 10], *cur = ed;
const Edge *head[maxn + 10], *fed[maxn + 10];
bool vis[maxn + 10];

template <Direct d>
inline Edge* addDirEdge(const unsigned int from, const unsigned int to,
                        Direct* p)
{
  new (cur) Edge{to, p, d, head[from]};
  head[from] = cur;
  return cur++;
}
inline void addEdge(const unsigned int from, const unsigned int to, Direct* p)
{
  Edge *const e1 = addDirEdge<R>(from, to, p), *const e2 =
                                                   addDirEdge<L>(to, from, p);
  e1->rev = e2;
  e2->rev = e1;
}
namespace LCA {
unsigned int top[maxn + 10], son[maxn + 10], dep[maxn + 10], fa[maxn + 10];

unsigned int dfsSon(const unsigned int x, const Edge* fe)
{
  dep[x] = dep[fe->to] + 1;
  fed[x] = fe;
  fa[x] = fe->to;
  unsigned int ret = 1, ms = 0;
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (i == fe) continue;
    const unsigned int sz = dfsSon(to, i->rev);
    ret += sz;
    if (sz > ms) {
      ms = sz;
      son[x] = to;
    }
  }
  return ret;
}
void dfsChain(const unsigned int x, const unsigned int tp)
{
  top[x] = tp;
  if (son[x]) dfsChain(son[x], tp);
  for (const Edge* i = head[x]; i; i = i->pre)
    if (!top[i->to]) dfsChain(i->to, i->to);
}
static void init(const unsigned int root)
{
  static const Edge nil{};
  dfsSon(root, &nil);
  dfsChain(root, root);
}
unsigned int lca(unsigned int a, unsigned int b)
{
  while (top[a] != top[b]) {
    if (dep[top[a]] > dep[top[b]]) swap(a, b);
    b = fa[top[b]];
  }
  return dep[a] < dep[b] ? a : b;
}
}  // namespace LCA
void selectEdgeUp(unsigned int x, const unsigned int lc)
{
  while (x != lc && !vis[x]) {
    *fed[x]->val = fed[x]->dir;
    vis[x] = true;
    x = LCA::fa[x];
  }
}
void selectEdgeDown(unsigned int x, const unsigned int lc)
{
  while (x != lc && !vis[x]) {
    *fed[x]->rev->val = fed[x]->rev->dir;
    vis[x] = true;
    x = LCA::fa[x];
  }
}
}  // namespace Tree
namespace Tarjan {
unsigned int dfn[maxn + 10], low[maxn + 10];

void tarjan(const unsigned int x, const OrigGraph::Edge* fe)
{
  static unsigned int sta = 0;
  dfn[x] = low[x] = ++sta;
  for (auto i = OrigGraph::head[x]; i; i = i->pre) {
    if (i == fe) continue;
    const unsigned int to = i->to;
    if (!dfn[to]) {
      tarjan(to, i->rev);
      low[x] = min(low[x], low[to]);
      if (dfn[x] < low[to]) i->isBridge = i->rev->isBridge = true;
    }
    else
      low[x] = min(low[x], dfn[to]);
  }
}
static void createTree(const unsigned int n, const unsigned int m)
{
  static Array roots;
  roots.init();
  for (unsigned int i = 1; i <= n; ++i)
    if (!dfn[i]) {
      tarjan(i, nullptr);
      roots.push_back(i);
    }
  Disjoint::init(n);
  using OrigGraph::ed;
  for (auto i = ed; i < ed + m * 2; ++i)
    if (!i->isBridge) Disjoint::merge(i->from, i->to);
  for (auto i = ed; i < ed + m * 2; ++i)
    if (i->isBridge && i->dir == R)
      Tree::addEdge(Disjoint::find(i->from), Disjoint::find(i->to), i->val);
  for (const unsigned int* i = roots.val; i < roots.ptr; ++i)
    Tree::LCA::init(Disjoint::find(*i));
}
}  // namespace Tarjan
void applyConstraint(const unsigned int n, const unsigned int m,
                     const unsigned int p)
{
  Tarjan::createTree(n, m);
  for (Constraint* i = con; i < con + p; ++i) {
    i->from = Disjoint::find(i->from);
    i->to = Disjoint::find(i->to);
    i->lca = Tree::LCA::lca(i->from, i->to);
  }
  sort(con, con + p, [](const Constraint& a, const Constraint& b) {
    return Tree::LCA::dep[a.lca] < Tree::LCA::dep[b.lca];
  });
  for (const Constraint* i = con; i < con + p; ++i) {
    Tree::selectEdgeDown(i->to, i->lca);
    Tree::selectEdgeUp(i->from, i->lca);
  }
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m, p;
  cin >> n >> m;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int a, b;
    cin >> a >> b;
    OrigGraph::addEdge(i, a, b);
  }
  cin >> p;
  for (Constraint* i = con; i < con + p; ++i)
    cin >> i->from >> i->to;
  applyConstraint(n, m, p);
  transform(direct, direct + m, ostream_iterator<char>(cout),
            [](Direct i) { return ch[i]; });
  cout.put('\n');
  return 0;
}