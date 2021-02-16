#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <numeric>
#include <utility>
using namespace std;
const unsigned int maxn = 500, maxm = 5e3;

struct edge {
  unsigned int from, to, val;
  const edge *pre, *rev;
  mutable unsigned int enable = 0;
} ed[maxm * 2 + 1];
const edge* head[maxn + 1];
const edge* seq[maxm * 2 + 1];
unsigned int s, t, sta;

edge* addEdge(const unsigned int from, const unsigned int to,
              const unsigned int val)
{
  static edge* cur = ed;
  cur->to = to;
  cur->from = from;
  cur->val = val;
  cur->pre = head[from];
  head[from] = cur;
  return cur++;
}
namespace dsu {
unsigned int fa[maxn + 1], siz[maxn + 1];

inline void init(const unsigned int n)
{
  fill(siz, siz + 1 + n, 1);
  iota(fa, fa + 1 + n, 0);
}
unsigned int getFather(const unsigned int x)
{
  return fa[x] == x ? x : fa[x] = getFather(fa[x]);
}
bool tryMerge(unsigned int x, unsigned int y)
{
  x = getFather(x);
  y = getFather(y);
  if (x == y) return false;
  if (siz[x] < siz[y]) swap(x, y);
  fa[x] = y;
  siz[x] += siz[y];
  return true;
}
}  // namespace dsu
pair<bool, unsigned int> findMin(const unsigned int x, const unsigned int f)
{
  if (x == t) return make_pair(true, UINT_MAX);
  for (const edge* i = head[x]; i; i = i->pre) {
    if (i->to == f || i->enable < sta) continue;
    const auto [tp, mv] = findMin(i->to, x);
    if (tp) return make_pair(true, min(mv, i->val));
  }
  return make_pair(false, UINT_MAX);
}
bool kruskal(const unsigned int n, const unsigned int m, const unsigned int val)
{
  dsu::init(n);
  ++sta;
  for (unsigned int i = 0; i < 2 * m && dsu::getFather(s) != dsu::getFather(t);
       ++i) {
    const edge* const e = seq[i];
    if (!(e->val > val || !dsu::tryMerge(e->from, e->to)))
      e->enable = e->rev->enable = sta;
  }
  return dsu::getFather(s) == dsu::getFather(t);
}
inline unsigned int rat(const unsigned int n, const unsigned int m,
                        const unsigned int mv)
{
  if (!kruskal(n, m, mv)) return UINT_MAX;
  const auto [suc, val] = findMin(s, 0);
  return suc ? val : UINT_MAX;
}
static void init(const unsigned int m)
{
  transform(ed, ed + m * 2, seq, [](const edge& a) { return &a; });
  sort(seq, seq + m * 2,
       [](const edge* a, const edge* b) { return a->val > b->val; });
}
pair<unsigned int, unsigned int> solve(const unsigned int n,
                                       const unsigned int m)
{
  pair<unsigned int, unsigned int> ret(UINT_MAX, 1);
  for (unsigned int i = 0; i < 2 * m; ++i) {
    if (i && seq[i]->val == seq[i - 1]->val) continue;
    const unsigned int r = rat(n, m, seq[i]->val);
    if (r == UINT_MAX) continue;
    if (1.0 * ret.first / ret.second > 1.0 * seq[i]->val / r)
      ret = make_pair(seq[i]->val, r);
  }
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int x, y, v;
    cin >> x >> y >> v;
    edge *a = addEdge(x, y, v), *b = addEdge(y, x, v);
    a->rev = b;
    b->rev = a;
  }
  cin >> s >> t;
  init(m);
  auto [d, f] = solve(n, m);
  if (d == UINT_MAX)
    cout << "IMPOSSIBLE" << endl;
  else {
    const unsigned int g = gcd(d, f);
    d /= g;
    f /= g;
    if (f > 1)
      cout << d << "/" << f << endl;
    else
      cout << d << endl;
  }
  return 0;
}