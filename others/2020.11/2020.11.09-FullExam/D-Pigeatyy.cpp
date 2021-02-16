#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
using namespace std;
const unsigned int maxn = 1e5;
typedef double real;
constexpr real ninf = numeric_limits<real>::lowest() / 2, eps = 1e-7;

struct Edge {
  unsigned int to;
  const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
real deltPow[maxn + 10], point[maxn + 10], power[maxn + 10], pi;
unsigned int degree[maxn + 10];
struct MaxPos {
  real val;
  unsigned int from;

  inline bool operator<(const MaxPos& r) const { return val < r.val; }
};
constexpr MaxPos nil{ninf, 0};
struct Chain {
  MaxPos fst, snd;
  inline void update(const MaxPos v);
} f[maxn + 10];

inline void Chain::update(const MaxPos v)
{
  if (fst < v) {
    snd = fst;
    fst = v;
  }
  else
    snd = max(snd, v);
}
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
inline real score(const real p1, const real p2)
{
  const real delt = p1 - p2;
  return (delt > 0 ? 1 : -1) * (sqrt(fabs(delt) + 1) - 1);
}
template <unsigned int a>
inline real nxtPoint(const real old, const unsigned int to)
{
  return old + deltPow[to] - a * score(old, point[to]);
}
template <unsigned int a>
real dfsRooted(const unsigned int x, const unsigned int fa)
{
  static constexpr Chain nch{nil, nil};
  f[x] = degree[x] <= 1 ? Chain{MaxPos{nxtPoint<a>(pi, x), x}, nil} : nch;
  for (const Edge* i = head[x]; i; i = i->pre)
    if (i->to != fa)
      f[x].update(MaxPos{nxtPoint<a>(dfsRooted<a>(i->to, x), x), i->to});
  return f[x].fst.val;
}
template <unsigned int a>
real dfsChangeRt(const unsigned int x, const unsigned int fa)
{
  real ret = degree[x] <= 1 ? f[x].fst.val : ninf;
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == fa) continue;
    if (f[x].fst.from != to)
      f[to].update(MaxPos{nxtPoint<a>(f[x].fst.val, to), x});
    else
      f[to].update(MaxPos{nxtPoint<a>(f[x].snd.val, to), x});
    ret = max(ret, dfsChangeRt<a>(to, x));
  }
  return ret;
}
void updateDelt(const unsigned int n, const real p)
{
  for (unsigned int i = 1; i <= n; ++i)
    deltPow[i] = 2 * score(p, power[i]);
}
template <unsigned int a>
real maxPoint(const real po, const unsigned int n)
{
  updateDelt(n, po);
  dfsRooted<a>(1, 0);
  return dfsChangeRt<a>(1, 0);
}
template <unsigned int a>
real binarySearch(const unsigned int n, const real pt)
{
  real l = -1e8, r = 1e8;
  while (r - l >= eps) {
    const real mid = (l + r) / 2;
    if (maxPoint<a>(mid, n) >= pt)
      r = mid;
    else
      l = mid;
  }
  return r;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int t, n, pt, a;
  cin >> t >> n >> pi >> pt >> a;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int u, v;
    cin >> u >> v;
    addEdge(u, v);
  }
  for (unsigned int i = 1; i <= n; ++i)
    cin >> power[i] >> point[i];
  cout.precision(6);
  cout << fixed
       << round((a ? binarySearch<1>(n, pt) : binarySearch<0>(n, pt)) * 1e6) /
              1e6
       << "\n";
  return 0;
}