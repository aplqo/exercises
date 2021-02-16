#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
using namespace std;
const unsigned int maxv = 1000, maxl = 20, maxt = 100;
typedef unsigned long long distance_t;
constexpr distance_t inf = numeric_limits<distance_t>::max() / maxl;

struct Edge {
  unsigned int u, v;
  unsigned int val;
} ed[maxt + 10];
unsigned int mp[maxv + 10], vcnt;
struct Matrix {
  Matrix()
  {
    for (unsigned int i = 1; i <= vcnt; ++i)
      fill(val[i], val[i] + vcnt + 1, inf);
  }
  Matrix(const unsigned int vcnt) : Matrix()
  {
    for (unsigned int i = 1; i <= vcnt; ++i)
      val[i][i] = 0;
  }
  Matrix operator*(const Matrix& r) const
  {
    Matrix ret;
    for (unsigned int i = 1; i <= vcnt; ++i)
      for (unsigned int j = 1; j <= vcnt; ++j)
        for (unsigned int k = 1; k <= vcnt; ++k)
          ret.val[i][k] = min(ret.val[i][k], val[i][j] + r.val[j][k]);
    return ret;
  }

  distance_t val[maxt * 2 + 10][maxt * 2 + 10];
};
namespace Disjoint {
unsigned int fa[maxv + 10], siz[maxv + 10];
static void init(const unsigned int n)
{
  iota(fa, fa + 1 + n, 0);
  fill(siz, siz + 1 + n, 1);
}
unsigned int find(const unsigned int x)
{
  return fa[x] == x ? x : fa[x] = find(fa[x]);
}
void merge(unsigned int x, unsigned int y)
{
  x = find(x);
  y = find(y);
  if (x == y) return;
  if (siz[x] < siz[y]) swap(x, y);
  fa[y] = x;
  siz[x] += siz[y];
}
}  // namespace Disjoint
Matrix operator^(Matrix a, unsigned int e)
{
  Matrix ret(vcnt);
  for (; e; e >>= 1) {
    if (e & 0x01) ret = ret * a;
    a = a * a;
  }
  return ret;
}
Matrix build(const unsigned int t, const unsigned int s)
{
  Disjoint::init(maxv);
  for (const Edge* i = ed; i < ed + t; ++i)
    Disjoint::merge(i->u, i->v);
  const unsigned int rs = Disjoint::find(s);
  for (unsigned int i = 1; i <= maxv; ++i)
    if (Disjoint::find(i) == rs) mp[i] = ++vcnt;
  Matrix ret;
  for (const Edge* i = ed; i < ed + t; ++i)
    if (Disjoint::find(i->u) == rs)
      ret.val[mp[i->u]][mp[i->v]] = ret.val[mp[i->v]][mp[i->u]] = i->val;
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, t, s, e;
  cin >> n >> t >> s >> e;
  for (Edge* i = ed; i < ed + t; ++i)
    cin >> i->val >> i->u >> i->v;
  cout << (build(t, s) ^ n).val[mp[s]][mp[e]] << "\n";
  return 0;
}