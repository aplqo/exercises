#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <numeric>
#include <utility>
using namespace std;
const unsigned int maxn = 100, maxm = 1000;
constexpr unsigned long long mod = 31011;

class Number {
 public:
  Number() = default;
  Number(const unsigned long long x) : val(x % mod){};
#define decl(x)                                                    \
  friend inline Number operator x(const Number l, const Number r); \
  inline Number& operator x##=(const Number r) { return *this = *this x r; }
  decl(+);
  decl(-);
  decl(*);
#undef decl
  bool isZero() const { return !val; }
  inline Number operator-() const { return mod - 1; }
  inline friend ostream& operator<<(ostream& os, const Number v);
  unsigned long long val = 0;

 private:
};
#define impl(x)                                            \
  inline Number operator x(const Number l, const Number r) \
  {                                                        \
    return (l.val + mod) x r.val;                          \
  }
impl(+);
impl(-);
impl(*);
#undef impl
inline ostream& operator<<(ostream& os, const Number v) { return os << v.val; }

struct Edge {
  unsigned int from, to, val;
  unsigned int rf, rt;  // root[from] root[to]
  enum vtyp { NotVisted, SetRooted, Visited };
  vtyp vis = NotVisted;
} ed[maxm + 1];
Number equ[maxn + 1][maxn + 1];

namespace dsu {
unsigned int fa[maxn + 1], siz[maxn + 1];
static void init(const unsigned int n)
{
  fill(siz, siz + 1 + n, 1);
  iota(fa, fa + 1 + n, 0);
}
unsigned int find(const unsigned int x)
{
  return fa[x] == x ? x : fa[x] = find(fa[x]);
}
void merge(unsigned int fx, unsigned int fy)
{
  fx = find(fx);
  fy = find(fy);
  if (fx == fy) return;
  if (siz[fx] < siz[fy]) swap(fx, fy);
  fa[fy] = fx;
  siz[fx] += siz[fy];
}
}  // namespace dsu
int clear(const unsigned int src, const unsigned int dest, const unsigned int n)
{
  int ret = 1;
  while (!equ[dest][src].isZero()) {
    if (equ[dest][src].val < equ[src][src].val) {
      ret = -ret;
      swap_ranges(equ[dest], equ[dest] + n, equ[src]);
    }
    const unsigned long long r = equ[dest][src].val / equ[src][src].val;
    for (unsigned int i = 0; i < n; ++i)
      equ[dest][i] -= equ[src][i] * r;
  }
  return ret;
}
Number gaussian(const unsigned int n)
{
  int rat = 1;
  for (unsigned int i = 0; i < n; ++i) {
    if (equ[i][i].isZero()) {
      for (unsigned int j = i + 1; j < n; ++j)
        if (!equ[j][i].isZero()) {
          swap_ranges(equ[j] + i, equ[j] + n, equ[i] + i);
          rat -= rat;
          break;
        }
    }
    if (equ[i][i].isZero()) continue;
    for (unsigned int j = i + 1; j < n; ++j)
      rat *= clear(i, j, n);
  }
  return (rat + mod) % mod;
}
Number det(const unsigned int n)
{
  Number acc = gaussian(n - 1);
  for (unsigned int i = 0; i < n - 1; ++i)
    acc *= equ[i][i];
  return acc;
}
inline bool same(const Edge* i, const Edge* p)
{
  return dsu::find(i->from) == dsu::find(p->from) &&
         dsu::find(i->to) == dsu::find(p->to);
}

pair<Number, unsigned int> count(Edge* p, Edge* e)
{
  static unsigned int mp[maxn + 1], vis[maxn + 1], sta = 0;
  memset(equ, 0, sizeof(equ));
  unsigned int cnt = 0;
  const auto id = [&cnt](unsigned int p) {
    if (vis[p] < sta) {
      mp[p] = cnt++;
      vis[p] = sta;
    }
    return mp[p];
  };
  ++sta;
  for (Edge* i = p; i != e && i->val == p->val; ++i) {
    if (i->vis != Edge::NotVisted) continue;
    i->rf = dsu::find(i->from);
    i->rt = dsu::find(i->to);
    i->vis = i->rf != i->rt ? Edge::SetRooted : Edge::Visited;
  }

  for (Edge* i = p; i != e && i->val == p->val; ++i)
    if (i->vis == Edge::SetRooted) dsu::merge(i->rf, i->rt);

  for (Edge* i = p; i != e && i->val == p->val; ++i)
    if (i->vis == Edge::SetRooted && same(i, p)) {
      const unsigned int ff = id(i->rf), ft = id(i->rt);
      equ[ff][ff] += 1;
      equ[ft][ft] += 1;
      equ[ff][ft] -= 1;
      equ[ft][ff] -= 1;
      i->vis = Edge::Visited;
    }
  return make_pair(cnt ? det(cnt) : 1, cnt ? cnt - 1 : 0);
}
Number kruskal(const unsigned int n, const unsigned int m)
{
  sort(ed, ed + m, [](const Edge a, const Edge b) { return a.val < b.val; });
  Number ret = 1;
  unsigned int c = 1;
  for (Edge* i = ed; i < ed + m && c < n; ++i) {
    if (i->vis == Edge::Visited) continue;
    const auto [s, e] = count(i, ed + m);
    ret *= s;
    c += e;
  }
  if (c < n) return 0;
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  dsu::init(n);
  for (Edge* i = ed; i < ed + m; ++i) {
    cin >> i->from >> i->to >> i->val;
    --(i->from);
    --(i->to);
  }
  cout << kruskal(n, m) << endl;
  return 0;
}