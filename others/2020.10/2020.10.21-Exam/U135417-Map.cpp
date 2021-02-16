#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 2e5, maxm = 4e5, maxl = 22;
const unsigned int maxv = maxn, maxe = maxm;

struct Edge {
  unsigned int from, to;
  bool isCut;
  Edge *pre, *rev;
} ed[maxm * 2 + 10], *cur = ed;
Edge* head[maxn + 10];
struct VirtEdge {
  unsigned int to;
  const VirtEdge* pre;
} ved[maxe * 2 + 10];
const VirtEdge* vhead[maxv + 10];
class Disjoint {
 public:
  void init(const unsigned int n);
  unsigned int find(const unsigned int x);
  void merge(unsigned int x, unsigned int y);
  unsigned int size(const unsigned int x);

 private:
  unsigned int fa[maxv + 10], siz[maxv + 10];
} bcc;
unsigned int siz[maxv + 1];

inline void addEdge(const unsigned int from, const unsigned int to)
{
  static Edge* rcur = ed + maxm;
  new (cur) Edge{from, to, false, head[from], rcur};
  head[from] = cur;
  new (rcur) Edge{to, from, false, head[to], cur};
  head[to] = rcur;
  ++cur;
  ++rcur;
}
inline void addVirtEdge(const unsigned int from, const unsigned int to)
{
  static VirtEdge* vcur = ved;
  new (vcur) VirtEdge{to, vhead[from]};
  vhead[from] = vcur++;
  new (vcur) VirtEdge{from, vhead[to]};
  vhead[to] = vcur++;
}
void Disjoint::init(const unsigned int n)
{
  fill(siz, siz + 1 + n, 1);
  iota(fa, fa + 1 + n, 0);
}
unsigned int Disjoint::find(const unsigned int x)
{
  return fa[x] == x ? x : fa[x] = find(fa[x]);
}
void Disjoint::merge(unsigned int x, unsigned int y)
{
  x = find(x);
  y = find(y);
  if (x == y) return;
  if (siz[x] < siz[y]) swap(x, y);
  fa[y] = x;
  siz[x] += siz[y];
}
unsigned int Disjoint::size(const unsigned int x) { return siz[find(x)]; }

namespace Tarjan {
unsigned int dfn[maxn + 10], low[maxn + 10];

void tarjan(const unsigned int x, const Edge* const fe)
{
  static unsigned int sta = 0;
  dfn[x] = low[x] = ++sta;
  for (Edge* i = head[x]; i; i = i->pre) {
    if (i == fe) continue;
    const unsigned int to = i->to;
    if (!dfn[to]) {
      tarjan(to, i->rev);
      low[x] = min(low[x], low[to]);
      if (dfn[x] < low[to]) i->isCut = i->rev->isCut = true;
    }
    else
      low[x] = min(low[x], dfn[to]);
  }
}
}  // namespace Tarjan
namespace Lca {
unsigned int fa[maxv + 10][maxl + 2], dep[maxv + 10], lg[maxv + 10];
unsigned long long sum[maxv + 10], size[maxv + 10];

static void getLog(const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i)
    lg[i] = lg[i >> 1] + 1;
}
static void dfs(const unsigned int x, const unsigned int f)
{
  fa[x][0] = f;
  dep[x] = dep[f] + 1;
  sum[x] = sum[f] + siz[x] * siz[x];
  size[x] = size[f] + siz[x];
  for (unsigned int i = 1; i <= lg[dep[x]]; ++i)
    fa[x][i] = fa[fa[x][i - 1]][i - 1];
  for (const VirtEdge* i = vhead[x]; i; i = i->pre)
    if (i->to != f) dfs(i->to, x);
}
unsigned int lca(unsigned int a, unsigned int b)
{
  if (dep[a] != dep[b]) {
    if (dep[a] > dep[b]) swap(a, b);
    for (unsigned int d = dep[b] - dep[a], msk = 1; d; msk <<= 1)
      if (d & msk) {
        b = fa[b][lg[msk]];
        d ^= msk;
      }
  }
  if (a == b) return a;
  for (unsigned int i = lg[dep[a]] + 1; i;) {
    const unsigned int m = i - 1;
    if (fa[a][m] == fa[b][m])
      i = m;
    else {
      a = fa[a][m];
      b = fa[b][m];
    }
  }
  return fa[a][0];
}
unsigned long long count(const unsigned int a, const unsigned int b)
{
  const unsigned int l = lca(a, b);
  const unsigned long long s = size[a] + size[b] - size[l] - size[fa[l][0]];
  return s * s - (sum[a] + sum[b] - sum[l] - sum[fa[l][0]]);
}
}  // namespace Lca
void createTree(const unsigned int n)
{
  for (const Edge* i = ed; i < cur; ++i)
    if (!(i->isCut)) bcc.merge(i->from, i->to);
  for (const Edge* i = ed; i < cur; ++i)
    if (i->isCut) addVirtEdge(bcc.find(i->from), bcc.find(i->to));
  for (unsigned int i = 1; i <= n; ++i)
    if (bcc.find(i) == i) siz[i] = bcc.size(i);
}
static void init(const unsigned int n)
{
  bcc.init(n);
  Lca::getLog(maxv);
  Tarjan::tarjan(1, nullptr);
  createTree(n);
  Lca::dfs(bcc.find(1), 0);
}
inline unsigned long long count(const unsigned int a, const unsigned int b)
{
  const unsigned int c1 = bcc.find(a), c2 = bcc.find(b);
  return c1 == c2 ? 0 : Lca::count(c1, c2);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m, q;
  cin >> n >> m >> q;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int x, y;
    cin >> x >> y;
    addEdge(x, y);
  }
  init(n);
  unsigned long long acc = 0;
  for (unsigned int i = 0; i < q; ++i) {
    unsigned int a, b;
    cin >> a >> b;
    acc += count(a, b);
  }
  cout << acc << "\n";
  return 0;
}