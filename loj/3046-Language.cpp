#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
const unsigned int maxn = 1e5;
const unsigned int maxNode = maxn * 160;
constexpr unsigned int inf = UINT_MAX;

struct Edge {
  unsigned int to;
  const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
  static Edge* cur = ed;
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, head[to]};
  head[to] = cur++;
}
namespace HLD {
unsigned int dep[maxn + 10], fa[maxn + 10];
unsigned int son[maxn + 10], top[maxn + 10], dfn[maxn + 10], rev[maxn + 10];

unsigned int dfsSon(const unsigned int x, const unsigned int f)
{
  dep[x] = dep[f] + 1;
  fa[x] = f;
  unsigned int ms = 0, ret = 1;
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
  static unsigned int sta = 0;
  dfn[x] = ++sta;
  rev[dfn[x]] = x;
  top[x] = tp;
  if (son[x]) dfsChain(son[x], tp);
  for (const Edge* i = head[x]; i; i = i->pre)
    if (!dfn[i->to]) dfsChain(i->to, i->to);
}
static void init()
{
  dfsSon(1, 0);
  dfsChain(1, 1);
}
unsigned int lca(unsigned int a, unsigned int b)
{
  while (top[a] != top[b]) {
    if (dep[top[a]] > dep[top[b]]) std::swap(a, b);
    b = fa[top[b]];
  }
  return dep[a] < dep[b] ? a : b;
}
inline unsigned int distance(const unsigned int a, const unsigned int b)
{
  return dep[a] + dep[b] - 2 * dep[lca(a, b)];
}
}  // namespace HLD
class SegmentTree {
 public:
  void add(unsigned int pos, const int val);
  void merge(SegmentTree* other);

  static SegmentTree mem[maxNode + 10], *cur;

  unsigned int left, right;
  long long sum;
  int cnt;

  static SegmentTree* create(const unsigned int l, const unsigned int r);

 private:
  void pushup();

  unsigned int lv, mv, rv;
  SegmentTree *lc, *rc;
};
SegmentTree SegmentTree::mem[maxNode + 10],
    *SegmentTree::cur = SegmentTree::mem;
SegmentTree* root[maxn + 10];

void SegmentTree::add(const unsigned int pos, const int val)
{
  if (rv - lv == 1) {
    cnt += val;
    if (cnt > 0) left = right = lv;
    return;
  }
  if (pos < mv) {
    if (!lc) lc = create(lv, mv);
    lc->add(pos, val);
  }
  else {
    if (!rc) rc = create(mv, rv);
    rc->add(pos, val);
  }
  pushup();
}
void SegmentTree::merge(SegmentTree* other)
{
  if (!other) return;
  if (rv - lv == 1) {
    cnt += other->cnt;
    if (cnt > 0) left = right = lv;
    return;
  }
  if (!lc)
    lc = other->lc;
  else
    lc->merge(other->lc);
  if (!rc)
    rc = other->rc;
  else
    rc->merge(other->rc);
  pushup();
}
SegmentTree* SegmentTree::create(const unsigned int l, const unsigned int r)
{
  SegmentTree* ret = cur++;
  ret->lv = l;
  ret->rv = r;
  ret->mv = (l + r) >> 1;
  ret->cnt = ret->sum = ret->left = ret->right = 0;
  ret->lc = ret->rc = nullptr;
  return ret;
}
void SegmentTree::pushup()
{
  bool both = true;
  sum = cnt = 0;
  left = right = inf;
  if (lc && lc->cnt > 0) {
    left = lc->left;
    right = lc->right;
    sum = lc->sum;
    cnt = 1;
  }
  else
    both = false;
  if (rc && rc->cnt > 0) {
    left = std::min(left, rc->left);
    right = rc->right;
    sum += rc->sum;
    cnt = 1;
  }
  else
    both = false;
  if (both) sum += HLD::distance(HLD::rev[lc->right], HLD::rev[rc->left]);
}

static void init(const unsigned int n)
{
  HLD::init();
  for (unsigned int i = 1; i <= n; ++i)
    root[i] = SegmentTree::create(1, n + 1);
}
void pathAdd(const unsigned int x, const unsigned int y, const unsigned int lc,
             const unsigned int pos)
{
  root[x]->add(HLD::dfn[pos], 1);
  root[y]->add(HLD::dfn[pos], 1);
  if (HLD::fa[lc]) root[HLD::fa[lc]]->add(HLD::dfn[pos], -2);
}
long long dfs(const unsigned int x, const unsigned int f)
{
  long long ret = 0;
  for (const Edge* i = head[x]; i; i = i->pre)
    if (i->to != f) {
      ret += dfs(i->to, x);
      root[x]->merge(root[i->to]);
    }
  if (root[x]->cnt > 0)
    ret += (root[x]->sum +
            HLD::distance(HLD::rev[root[x]->left], HLD::rev[root[x]->right])) /
           2;
  return ret;
}

int main()
{
  std::ios::sync_with_stdio(false);
  unsigned int n, m;
  std::cin >> n >> m;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int x, y;
    std::cin >> x >> y;
    addEdge(x, y);
  }
  init(n);
  for (unsigned int i = m; i; --i) {
    unsigned int x, y;
    std::cin >> x >> y;
    const unsigned int lc = HLD::lca(x, y);
    pathAdd(x, y, lc, x);
    pathAdd(x, y, lc, y);
  }
  std::cout << dfs(1, 0) / 2 << "\n";
  return 0;
}