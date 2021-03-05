#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
const int maxn = 2e5, maxl = 18;
const int maxNode = maxn * maxl * 4 + maxn * 4;

class SegmentTree {
 public:
  void build(const int l, const int r, const int val[]);
  void increase(const SegmentTree *old, const int l, const int r);
  long long sum(const int l, const int r, const int accTag = 0) const;

  static SegmentTree nodes[maxNode], *cur;

 private:
  int lv, mv, rv;
  const SegmentTree *lc, *rc;
  long long val, tag, sumVal;
};
SegmentTree SegmentTree::nodes[maxNode], *SegmentTree::cur = SegmentTree::nodes;

struct Edge {
  int to, val;
  const Edge *pre;
} ed[maxn * 2 + 10];
const Edge *head[maxn + 10];
long long distToRoot[maxn + 10];
int p[maxn + 10];

inline void addEdge(const int from, const int to, const int val)
{
  static Edge *cur = ed;
  new (cur) Edge{to, val, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, val, head[to]};
  head[to] = cur++;
}
void SegmentTree::build(const int l, const int r, const int val[])
{
  lv = l;
  mv = (l + r) >> 1;
  rv = r;
  tag = sumVal = 0;
  if (r - l == 1) {
    this->val = val[l];
    return;
  }
  lc = cur;
  (cur++)->build(l, mv, val);
  rc = cur;
  (cur++)->build(mv, r, val);
  this->val = lc->val + rc->val;
}
void SegmentTree::increase(const SegmentTree *old, const int l, const int r)
{
  *this = *old;
  if (l == lv && r == rv) {
    ++tag;
    sumVal += val;
    return;
  }
  if (l < mv) {
    lc = cur;
    (cur++)->increase(old->lc, l, std::min(mv, r));
  }
  if (r > mv) {
    rc = cur;
    (cur++)->increase(old->rc, std::max(l, mv), r);
  }
  sumVal = lc->sumVal + rc->sumVal + tag * val;
}
long long SegmentTree::sum(const int l, const int r, const int accTag) const
{
  if (l == lv && r == rv) return sumVal + val * accTag;
  if (r <= mv)
    return lc->sum(l, r, accTag + tag);
  else if (l >= mv)
    return rc->sum(l, r, accTag + tag);
  else
    return lc->sum(l, mv, accTag + tag) + rc->sum(mv, r, accTag + tag);
}

namespace HLD {
int depth[maxn + 10], fa[maxn + 10], dfn[maxn + 10], fromEdge[maxn + 10];
int top[maxn + 10], son[maxn + 10];

int dfsSon(const int x, const int f, const int edge)
{
  fa[x] = f;
  distToRoot[x] = distToRoot[f] + edge;
  depth[x] = depth[f] + 1;
  fromEdge[x] = edge;
  int ret = 1, ms = 0;
  for (const Edge *i = head[x]; i; i = i->pre) {
    const int to = i->to;
    if (to == f) continue;
    const int sz = dfsSon(to, x, i->val);
    ret += sz;
    if (sz > ms) {
      ms = sz;
      son[x] = to;
    }
  }
  return ret;
}
void dfsChain(const int x, const int tp)
{
  static int sta = 0;
  dfn[x] = ++sta;
  top[x] = tp;
  if (son[x]) dfsChain(son[x], tp);
  for (const Edge *i = head[x]; i; i = i->pre)
    if (!dfn[i->to]) dfsChain(i->to, i->to);
}
const SegmentTree *init(const int n)
{
  dfsSon(1, 0, 0);
  dfsChain(1, 1);
  {
    static int tmp[maxn + 10];
    for (int i = 1; i <= n; ++i)
      tmp[dfn[i]] = fromEdge[i];
    SegmentTree *p = SegmentTree::cur++;
    p->build(1, n + 1, tmp);
    return p;
  }
}
int lca(int x, int y)
{
  while (top[x] != top[y]) {
    if (depth[top[x]] > depth[top[y]]) std::swap(x, y);
    y = fa[top[y]];
  }
  return depth[x] < depth[y] ? x : y;
}
long long pathSum(const SegmentTree *dat, int k)
{
  long long ret = 0;
  while (k) {
    ret += dat->sum(dfn[top[k]], dfn[k] + 1);
    k = fa[top[k]];
  }
  return ret;
}
const SegmentTree *pathAdd(const SegmentTree *old, int x)
{
  while (x) {
    SegmentTree *p = SegmentTree::cur++;
    p->increase(old, dfn[top[x]], dfn[x] + 1);
    old = p;
    x = fa[top[x]];
  }
  return old;
}
}  // namespace HLD

const SegmentTree *root[maxn + 10];
long long sumDist[maxn + 10];

void dfsSum(const int x, const int f)
{
  root[x] = HLD::pathAdd(root[f], p[x]);
  sumDist[x] = sumDist[f] + distToRoot[p[x]];
  for (const Edge *i = head[x]; i; i = i->pre)
    if (i->to != f) dfsSum(i->to, x);
}
static void init(const int n)
{
  root[0] = HLD::init(n);
  dfsSum(1, 0);
}
template <class Pred>
auto pathSum(const int u, const int v, const int lc, Pred p) -> decltype(auto)
{
  return p(u) + p(v) - p(lc) - p(HLD::fa[lc]);
}
long long query(const int u, const int v, const int k)
{
  const int lc = HLD::lca(u, v);
  return pathSum(u, v, lc, [](int x) { return sumDist[x]; }) +
         pathSum(u, v, lc, [](int x) { return HLD::depth[x]; }) *
             distToRoot[k] -
         pathSum(u, v, lc, [k](int x) { return HLD::pathSum(root[x], k); }) * 2;
}

int main()
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int type, n, q;
  std::cin >> type >> n >> q;
  for (int i = 1; i < n; ++i) {
    int u, v, c;
    std::cin >> u >> v >> c;
    addEdge(u, v, c);
  }
  std::copy_n(std::istream_iterator<int>(std::cin), n, p + 1);
  init(n);
  long long last = 0;
  const auto decode = [type, &last](const long long x) -> int {
    return x ^ (type * last);
  };
  for (int i = q; i; --i) {
    long long u, v, k;
    std::cin >> u >> v >> k;
    std::cout << (last = query(decode(u), decode(v), decode(k))) << "\n";
  }
  return 0;
}