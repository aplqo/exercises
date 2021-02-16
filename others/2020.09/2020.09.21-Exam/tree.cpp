#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 100000;
constexpr unsigned long long mod = 1e9 + 7;

class Number {
 public:
  Number() = default;
  Number(const unsigned long long v) : val(v % mod){};
#define decl(x)                                                   \
  friend inline Number operator x(const Number l, const Number r) \
  {                                                               \
    return (l.val + mod) x r.val;                                 \
  }                                                               \
  inline Number operator x##=(const Number r) { return *this = *this x r; }
  decl(+);
  decl(-);
  decl(*);
#undef decl
  friend inline ostream& operator<<(ostream& os, const Number v)
  {
    return os << v.val;
  }

 private:
  unsigned long long val = 0;
};
struct Edge {
  unsigned int to;
  const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
unsigned int degree[maxn + 10];
Number toFather[maxn + 10], fromFather[maxn + 10], sumFrom[maxn + 10],
    sumTo[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
  static Edge* cur = ed;
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, head[to]};
  head[to] = cur++;
  ++degree[from];
  ++degree[to];
}
namespace lca {
unsigned int fa[maxn + 10], top[maxn + 10], son[maxn + 10], dep[maxn + 10];

unsigned int dfsSon(const unsigned int x, const unsigned int f)
{
  dep[x] = dep[f] + 1;
  fa[x] = f;
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
unsigned int lca(unsigned int a, unsigned int b)
{
  while (top[a] != top[b]) {
    if (dep[top[a]] > dep[top[b]]) swap(a, b);
    b = fa[top[b]];
  }
  return dep[a] < dep[b] ? a : b;
}
}  // namespace lca
void dfsToExp(const unsigned int x, const unsigned int f)
{
  for (const Edge* i = head[x]; i; i = i->pre)
    if (i->to != f) {
      dfsToExp(i->to, x);
      toFather[x] += toFather[i->to];
    }
  toFather[x] += degree[x];
}
void dfsFromExp(const unsigned int x, const unsigned int f)
{
  Number sum = degree[x] + fromFather[x];
  for (const Edge* i = head[x]; i; i = i->pre)
    if (i->to != f) sum += toFather[i->to];
  for (const Edge* i = head[x]; i; i = i->pre)
    if (i->to != f) {
      fromFather[i->to] = sum - toFather[i->to];
      dfsFromExp(i->to, x);
    }
}
void dfsSum(const unsigned int x, const unsigned int f)
{
  sumFrom[x] += fromFather[x];
  sumTo[x] += toFather[x];
  for (const Edge* i = head[x]; i; i = i->pre)
    if (i->to != f) {
      sumFrom[i->to] = sumFrom[x];
      sumTo[i->to] = sumTo[x];
      dfsSum(i->to, x);
    }
}
Number query(const unsigned int from, const unsigned int to)
{
  const unsigned int l = lca::lca(from, to);
  return (sumTo[from] - sumTo[l]) + (sumFrom[to] - sumFrom[l]);
}
static void init()
{
  lca::dfsSon(1, 0);
  lca::dfsChain(1, 1);
  dfsToExp(1, 0);
  dfsFromExp(1, 0);
  dfsSum(1, 0);
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, q;
  cin >> n >> q;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int x, y;
    cin >> x >> y;
    addEdge(x, y);
  }
  init();
  for (unsigned int i = 0; i < q; ++i) {
    unsigned int u, v;
    cin >> u >> v;
    cout << query(u, v) << "\n";
  }
  return 0;
}