#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
using namespace std;
using res_t = unsigned int;
constexpr int maxn = 1e4, maxm = 5e4, maxl = 16;

struct edge {
  unsigned int vx, vy;
  bool sel = false;
  res_t val;
  edge* pre = nullptr;
} ed[(maxm + 10) * 2 + maxn], *cur = ed;
edge* head[maxn + 10];

namespace find {
unsigned int fa[maxn + 10], siz[maxn + 10];

inline void init(const unsigned int n)
{
  iota(fa, fa + n + 1, 0);
  fill(siz, siz + n + 1, 1);
}
unsigned int GetFather(const unsigned int x)
{
  return fa[x] == x ? x : fa[x] = GetFather(fa[x]);
}
inline bool TryMerge(const unsigned int x, const unsigned int y)
{
  unsigned int fx = GetFather(x), fy = GetFather(y);
  if (fx == fy) return false;
  if (siz[fx] < siz[fy]) swap(fx, fy);
  fa[fy] = fx;
  siz[fx] += siz[fy];
  return true;
}
}  // namespace find
namespace lca {
unsigned int fa[maxn + 10][maxl + 10], dep[maxn + 10], lg[maxn + 10];
res_t lim[maxn + 10][maxl + 10];

inline void GetLog(const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i)
    lg[i] = lg[i >> 1] + 1;
}
void init(const unsigned int x, const unsigned int f)
{
  dep[x] = dep[f] + 1;
  fa[x][0] = f;
  for (unsigned int i = 1; i <= lg[dep[x]]; ++i) {
    fa[x][i] = fa[fa[x][i - 1]][i - 1];
    lim[x][i] = min(lim[fa[x][i - 1]][i - 1], lim[x][i - 1]);
  }
  for (edge* i = head[x]; i; i = i->pre)
    if (!dep[i->vy]) {
      lim[i->vy][0] = i->val;
      init(i->vy, x);
    }
}
res_t lca(unsigned int a, unsigned int b)
{
  res_t ret = numeric_limits<res_t>::max();
  if (dep[a] != dep[b]) {
    if (dep[a] > dep[b]) swap(a, b);
    unsigned int d = dep[b] - dep[a], t = 1;
    for (unsigned int i = 0; d; ++i, t <<= 1)
      if (d & t) {
        ret = min(ret, lim[b][i]);
        b = fa[b][i];
        d ^= t;
      }
  }
  if (a == b) return ret;
  for (unsigned int k = lg[dep[a]] + 1; k;) {
    unsigned int m = k - 1;
    if (fa[a][m] == fa[b][m])
      --k;
    else {
      ret = min({ret, lim[a][m], lim[b][m]});
      a = fa[a][m];
      b = fa[b][m];
    }
  }
  ret = min({ret, lim[a][0], lim[b][0]});
  return ret;
}
}  // namespace lca
inline void build()
{
  static auto connect = [](edge* i) -> void {
    i->pre = head[i->vx];
    head[i->vx] = i;
  };
  copy(ed, cur, cur);
  const unsigned int off = cur - ed;
  edge* ncur = cur + off;
  for (edge* i = ed; i < cur; ++i) {
    if (!i->sel) continue;
    if (find::GetFather(i->vx) == i->vx) {
      ncur->vx = 0;
      ncur->vy = i->vx;
      connect(ncur);
      ++ncur;
    }
    connect(i);
    swap((i + off)->vx, (i + off)->vy);
    connect(i + off);
  }
}
void kruskal(const unsigned int n)
{
  static edge* e = ed;
  find::init(n);
  sort(ed, cur,
       [](const edge& a, const edge& b) -> bool { return a.val > b.val; });

  for (unsigned int i = 0; i < n - 1 && e < cur; ++i) {
    while (!find::TryMerge(e->vx, e->vy) && e < cur)
      ++e;
    if (e < cur) e->sel = true;
    ++e;
  }
}

int main()
{
  ios_base::sync_with_stdio(false);
  unsigned int n, m, q;
  cin >> n >> m;
  for (unsigned int i = 0; i < m; ++i, ++cur)
    cin >> cur->vx >> cur->vy >> cur->val;
  kruskal(n);
  build();
  lca::GetLog(n + 1);
  lca::init(0, 0);

  cin >> q;
  for (unsigned int i = 0; i < q; ++i) {
    unsigned int x, y;
    cin >> x >> y;
    if (find::GetFather(x) == find::GetFather(y))
      cout << lca::lca(x, y) << endl;
    else
      cout << "-1" << endl;
  }
  return 0;
}