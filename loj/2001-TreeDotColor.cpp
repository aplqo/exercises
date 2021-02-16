#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <type_traits>
const unsigned int maxn = 1e5, maxm = 1e5;

struct Edge {
  unsigned int to;
  const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
struct Color {
  unsigned int top, bottom, color;
};
class SegmentTree {
 public:
  void build(const unsigned int l, const unsigned int r, SegmentTree*(&cur));
  unsigned int max(const unsigned int l, const unsigned int r);
  template <class T>
  void modify(const unsigned int l, const unsigned int r, const T& v);
  template <class T>
  T get(const unsigned int pos);

 private:
  template <class T>
  inline void push(const T& v);
  template <class T>
  inline void pushdown();

  unsigned int lv, mv, rv;
  SegmentTree *lc, *rc;
  Color color;
  bool assigned;
  int val, laz;
} snodes[maxn * 4 + 10], *root = snodes;
unsigned int bottom[maxn + maxm + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
  static Edge* cur = ed;
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, head[to]};
  head[to] = cur++;
}

namespace HLD {
unsigned int dep[maxn + 10], fa[maxn + 10], size[maxn + 10];
unsigned int top[maxn + 10], son[maxn + 10], dfn[maxn + 10], rev[maxn + 10];

void dfsSon(const unsigned int x, const unsigned int f)
{
  fa[x] = f;
  dep[x] = dep[f] + 1;
  size[x] = 1;
  unsigned int ms = 0;
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f) continue;
    dfsSon(to, x);
    size[x] += size[to];
    if (size[to] > ms) {
      ms = size[to];
      son[x] = to;
    }
  }
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
static void init(const unsigned int n)
{
  dfsSon(1, 0);
  dfsChain(1, 1);
  {
    SegmentTree* c = snodes;
    root->build(1, n + 1, c);
  }
  for (unsigned int i = 1; i <= n; ++i)
    bottom[i] = i;
}

unsigned int lca(unsigned int a, unsigned int b)
{
  while (top[a] != top[b]) {
    if (dep[top[a]] > dep[top[b]]) std::swap(a, b);
    b = fa[top[b]];
  }
  return dep[a] < dep[b] ? a : b;
}
unsigned int findChild(unsigned int x, unsigned int root)
{
  unsigned int lstX, lstRt;
  while (top[x] != top[root])
    if (dep[top[x]] > dep[top[root]]) {
      lstX = top[x];
      x = fa[lstX];
    }
    else {
      lstRt = top[root];
      root = fa[lstRt];
    }
  return x == root ? lstRt : son[x];
}
inline void subtreeAdd(const unsigned int x, const int v)
{
  root->modify<int>(dfn[x], dfn[x] + size[x], v);
}
inline unsigned int subtreeMax(const unsigned int x)
{
  return root->max(dfn[x], dfn[x] + size[x]);
}
void setPath(unsigned int x, const unsigned int color)
{
  unsigned int from = 0, last = x;
  bottom[color] = x;
  while (x) {
    const Color ch = root->get<Color>(dfn[x]);
    const int val = 1 - root->get<int>(dfn[x]);
    subtreeAdd(ch.top, val);
    if (from) subtreeAdd(from, -val);
    if (x != bottom[ch.color])
      if (unsigned int chl = findChild(x, bottom[ch.color]); chl != from)
        subtreeAdd(chl, 1);
    if (x != ch.bottom)
      root->modify(dfn[x] + 1, dfn[ch.bottom] + 1,
                   Color{rev[dfn[x] + 1], ch.bottom, ch.color});
    root->modify<Color>(dfn[ch.top], dfn[x] + 1, Color{top[x], last, color});
    from = ch.top;
    x = fa[ch.top];
    if (ch.top == top[ch.top]) last = x;
  }
}
inline unsigned int countPath(const unsigned int x, const unsigned int y)
{
  return root->get<int>(dfn[x]) + 1 + root->get<int>(dfn[y]) -
         2 * root->get<int>(dfn[lca(x, y)]);
}
}  // namespace HLD
void SegmentTree::build(const unsigned int l, const unsigned int r,
                        SegmentTree*(&cur))
{
  lv = l;
  mv = (l + r) >> 1;
  rv = r;
  assigned = false;
  laz = 0;
  if (r - l == 1) {
    using namespace HLD;
    color = Color{rev[l], rev[l], rev[l]};
    val = dep[rev[l]];
    return;
  }
  lc = ++cur;
  rc = ++cur;
  lc->build(l, mv, cur);
  rc->build(mv, r, cur);
  this->val = std::max(lc->val, rc->val);
}
unsigned int SegmentTree::max(const unsigned int l, const unsigned int r)
{
  if (l == lv && r == rv) return val;
  pushdown<int>();
  if (r <= mv)
    return lc->max(l, r);
  else if (l >= mv)
    return rc->max(l, r);
  else
    return std::max(lc->max(l, mv), rc->max(mv, r));
}
template <class T>
void SegmentTree::modify(const unsigned int l, const unsigned int r, const T& v)
{
  if (l == lv && r == rv) {
    push(v);
    return;
  }
  pushdown<T>();
  if (r <= mv)
    lc->modify(l, r, v);
  else if (l >= mv)
    rc->modify(l, r, v);
  else {
    lc->modify(l, mv, v);
    rc->modify(mv, r, v);
  }
  if constexpr (std::is_same_v<T, int>) this->val = std::max(lc->val, rc->val);
}
template <class T>
T SegmentTree::get(const unsigned int pos)
{
  if constexpr (std::is_same_v<T, Color>) {
    if (assigned || rv - lv == 1) return color;
  }
  else {
    if (rv - lv == 1) {
      if constexpr (std::is_same_v<T, Color>)
        return color;
      else
        return val;
    }
  }
  pushdown<T>();
  return (pos < mv ? lc : rc)->get<T>(pos);
}
template <class T>
inline void SegmentTree::push(const T& v)
{
  color = v;
  assigned = true;
}
template <>
inline void SegmentTree::push<int>(const int& v)
{
  val += v;
  laz += v;
}
template <class T>
inline void SegmentTree::pushdown()
{
  if constexpr (std::is_same_v<T, Color>) {
    if (assigned) {
      lc->push(color);
      rc->push(color);
      assigned = false;
    }
  }
  else {
    if (laz) {
      lc->push(laz);
      rc->push(laz);
      laz = 0;
    }
  }
}

using HLD::countPath;
using HLD::setPath;
using HLD::subtreeMax;

int main()
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  unsigned int n, m;
  std::cin >> n >> m;
  for (unsigned int i = n - 1; i; --i) {
    unsigned int a, b;
    std::cin >> a >> b;
    addEdge(a, b);
  }
  HLD::init(n);
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int op, x, y;
    std::cin >> op >> x;
    switch (op) {
      case 1:
        setPath(x, n + i + 1);
        break;
      case 2:
        std::cin >> y;
        std::cout << countPath(x, y) << "\n";
        break;
      case 3:
        std::cout << subtreeMax(x) << "\n";
        break;
    }
  }
  return 0;
}