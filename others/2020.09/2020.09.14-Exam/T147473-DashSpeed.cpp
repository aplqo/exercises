#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
#include <stack>
#include <vector>
using namespace std;
const unsigned int maxn = 70000;

unsigned int ans[maxn + 10], n;

namespace Lca {
struct Edge {
  unsigned int to;
  const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
unsigned int fa[maxn + 1], dep[maxn + 1], son[maxn + 10], top[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to);
unsigned int dfsSon(const unsigned int x, const unsigned int f);
void dfsChain(const unsigned int x, const unsigned int tp);
static void init();
unsigned int lca(unsigned int a, unsigned int b);
unsigned int distance(const unsigned int a, const unsigned int b);
}  // namespace Lca
namespace dsu {
unsigned int fa[maxn + 10], siz[maxn + 10];
struct Chain {
  unsigned int v1, v2, len;
  pair<unsigned int, unsigned int> distVertex(const unsigned int x) const;
  inline bool operator<(const Chain& r) const { return len < r.len; }
};
Chain dis[maxn + 10];
stack<unsigned int> ans;
stack<pair<unsigned int, Chain>> stk;

static void init(const unsigned int n);
unsigned int find(const unsigned int x);
inline void merge(unsigned int x, unsigned int y);
void undo(const size_t stat);
inline unsigned int maxChain();
}  // namespace dsu

namespace Lca {
inline void addEdge(const unsigned int from, const unsigned int to)
{
  static Edge* cur = ed;
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, head[to]};
  head[to] = cur++;
}
unsigned int dfsSon(const unsigned int x, const unsigned int f)
{
  fa[x] = f;
  dep[x] = dep[f] + 1;
  unsigned int ret = 1, ms = 0;
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f) continue;
    const unsigned int sz = dfsSon(to, x);
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
static void init()
{
  dfsSon(1, 0);
  dfsChain(1, 1);
}
unsigned int lca(unsigned int a, unsigned int b)
{
  while (top[a] != top[b]) {
    if (dep[top[a]] > dep[top[b]]) swap(a, b);
    b = fa[top[b]];
  }
  return dep[a] < dep[b] ? a : b;
}
unsigned int distance(const unsigned int a, const unsigned int b)
{
  return dep[a] + dep[b] - 2 * dep[lca(a, b)];
}
}  // namespace Lca
namespace dsu {
pair<unsigned int, unsigned int> Chain::distVertex(const unsigned int x) const
{
  const unsigned int d1 = Lca::distance(v1, x), d2 = Lca::distance(v2, x);
  return d1 < d2 ? make_pair(v2, d2) : make_pair(v1, d1);
}
inline Chain mergeBlock(const unsigned int x, const unsigned int rf,
                        const unsigned int rt)
{
  const auto c1 = dis[rf].distVertex(x), c2 = dis[rt].distVertex(x);
  return Chain{c1.first, c2.first, c1.second + c2.second};
}

static void init(const unsigned int n)
{
  fill(siz, siz + n + 1, 1);
  iota(fa, fa + n + 1, 0);
  for (unsigned int i = 1; i <= n; ++i)
    dis[i] = Chain{i, i, 0};
  ans.push(0);
}
unsigned int find(unsigned int x)
{
  while (fa[x] != x)
    x = fa[x];
  return x;
}
void merge(const unsigned int x, const unsigned int y)
{
  unsigned int rf = find(x), rt = find(y);
  if (rf == rt) return;
  if (siz[rf] < siz[rt]) swap(rf, rt);
  stk.emplace(rt, dis[rf]);
  fa[rt] = rf;
  siz[rf] += siz[rt];
  dis[rf] = max({dis[rf], dis[rt], mergeBlock(x, rf, rt)});
  ans.emplace(max(dis[rf].len, ans.top()));
}
inline void undo(const size_t stat)
{
  while (stk.size() > stat) {
    const unsigned int tp = stk.top().first, f = fa[tp];
    dis[f] = stk.top().second;
    siz[f] -= siz[tp];
    fa[tp] = tp;
    ans.pop();
    stk.pop();
  }
}
inline unsigned int maxChain() { return ans.top(); }
}  // namespace dsu
class STree {
 public:
  typedef pair<unsigned int, unsigned int> Edge;
  void build(const unsigned int l, const unsigned int r, STree*(&cur));
  void add(const unsigned int l, const unsigned int r, const Edge e);
  void dfs() const;

 private:
  unsigned int lv, mv, rv;
  STree *lc, *rc;
  vector<Edge> val;
} SNodes[maxn * 4 + 10], *root = SNodes;

void STree::build(const unsigned int l, const unsigned int r, STree*(&cur))
{
  lv = l;
  mv = (l + r) >> 1;
  rv = r;
  if (r - l == 1) return;
  lc = ++cur;
  lc->build(l, mv, cur);
  rc = ++cur;
  rc->build(mv, r, cur);
}
void STree::add(const unsigned int l, const unsigned int r, const Edge e)
{
  if (l == lv && r == rv) {
    val.push_back(e);
    return;
  }
  if (r <= mv)
    lc->add(l, r, e);
  else if (l >= mv)
    rc->add(l, r, e);
  else {
    lc->add(l, mv, e);
    rc->add(mv, r, e);
  }
}
void STree::dfs() const
{
  const size_t stat = dsu::stk.size();
  for (const Edge& e : val)
    dsu::merge(e.first, e.second);
  if (rv - lv > 1) {
    lc->dfs();
    rc->dfs();
  }
  else
    ans[lv] = dsu::maxChain();
  dsu::undo(stat);
}
inline void addEdge(const unsigned int from, const unsigned int to,
                    const unsigned int l, const unsigned int r)
{
  Lca::addEdge(from, to);
  root->add(l, r, STree::Edge(from, to));
}
static void init(const unsigned int n)
{
  dsu::init(n);
  STree* c = SNodes;
  root->build(1, n + 1, c);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int m;
  cin >> n >> m;
  init(n);
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int u, v, l, r;
    cin >> u >> v >> l >> r;
    addEdge(u, v, l, r + 1);
  }
  Lca::init();
  root->dfs();
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int p;
    cin >> p;
    cout << ans[p] << "\n";
  }
  return 0;
}