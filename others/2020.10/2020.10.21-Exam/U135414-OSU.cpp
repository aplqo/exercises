#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <numeric>
#include <queue>
using namespace std;
const unsigned int maxn = 2000;
typedef double real;
constexpr real eps = 1e-10, inf = numeric_limits<real>::max();

struct Distance {
  const struct Point *px, *py;
  real approx;

  inline bool operator<(const Distance& r) const { return approx < r.approx; }
};
struct Point {
  int x, y;
  int t;
} p[maxn + 10];
real approx[maxn + 1][maxn + 1];
Distance dist[maxn * maxn + 10];
unsigned int seq[maxn + 10];

inline Distance speed(const Point* a, const Point* b)
{
  return Distance{
      a, b,
      hypot(static_cast<real>(a->x - b->x), a->y - b->y) / abs(a->t - b->t)};
}
unsigned int dp(const unsigned int n, const real f)
{
  static unsigned int vis[maxn + 10], sta;
  static unsigned int dist[maxn + 10];
  unsigned int ret = 0;
  memset(dist, 0, sizeof(dist));
  ++sta;
  vis[0] = sta;
  for (unsigned int i = 0; i <= n; ++i) {
    if (vis[i] < sta) continue;
    ret = max(ret, dist[i]);
    const real* const d = approx[i];
    for (unsigned int j = i + 1; j <= n; ++j) {
      if (d[j] > f) continue;
      vis[j] = sta;
      dist[j] = max(dist[j], dist[i] + 1);
    }
  }
  return ret;
}
Distance binarySearch(const unsigned int n, const unsigned int k,
                      const unsigned int dcnt)
{
  int l = 0, r = dcnt - 1;
  while (l <= r) {
    const int mid = (l + r) / 2;
    if (dp(n, dist[mid].approx) >= k)
      r = mid - 1;
    else
      l = mid + 1;
  }
  return dist[l];
}
static unsigned int buildGraph(const unsigned int n)
{
  Distance* pdist = dist;
  p[0] = Point{0, 0, 0};
  for (unsigned int i = 1; i <= n; ++i)
    for (unsigned int j = 0; p[j].t < p[i].t; ++j) {
      *(pdist++) = speed(p + i, p + j);
      approx[j][i] = (pdist - 1)->approx;
    }
  sort(dist, pdist);
  return pdist - dist;
}
pair<unsigned long long, unsigned long long> sqrtUll(const unsigned long long a)
{
  unsigned long long acc = a;
  unsigned long long reta = 1, retb = 1;
  for (unsigned long long i = 2; i * i <= a && acc > 1; ++i) {
    if (acc % i) continue;
    bool even = false;
    while (acc % i == 0) {
      if (even) {
        reta *= i;
        retb /= i;
      }
      else
        retb *= i;
      acc /= i;
      even = !even;
    }
  }
  if (acc > 1) retb *= acc;
  return make_pair(reta, retb);
}
inline ostream& operator<<(ostream& os, const Distance& d)
{
  const auto sq = [](const long long a) { return a * a; };
  auto [a, b] = sqrtUll(sq(d.px->x - d.py->x) + sq(d.py->y - d.px->y));
  unsigned long long c = abs(d.px->t - d.py->t);
  const unsigned int g = gcd(a, c);
  return os << a / g << " " << b << " " << c / g;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, k;
  cin >> n >> k;
  for (Point* i = p + 1; i < p + 1 + n; ++i)
    cin >> i->t >> i->x >> i->y;
  const unsigned int dcnt = buildGraph(n);
  cout << binarySearch(n, k, dcnt) << "\n";
  return 0;
}