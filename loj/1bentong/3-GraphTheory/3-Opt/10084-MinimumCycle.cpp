#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 3000, maxm = 10000;
using real = double;
constexpr real eps = 1e-9l;

struct edge {
  unsigned int from, to;
  real val;
} ed[maxm], *cur = ed;

void addEdge(const unsigned int from, const unsigned int to, const real val)
{
  cur->from = from;
  cur->to = to;
  cur->val = val;
  ++cur;
}
bool bellmanFord(const unsigned int n, const real mid)
{
  static real dis[maxn + 1];
  fill(dis + 1, dis + 1 + n, 0);
  for (unsigned int i = 0; i < n / 4; ++i)  // wrong solution!
  {
    bool s = true;
    for (const edge* j = ed; j < cur; ++j)
      if (dis[j->to] > dis[j->from] + j->val - mid) {
        dis[j->to] = dis[j->from] + j->val - mid;
        s = false;
      }
    if (s) break;
  }
  for (const edge* j = ed; j < cur; ++j)
    if (dis[j->to] > dis[j->from] + j->val - mid) return false;
  return true;
}
real binarySearch(const unsigned int n)
{
  real l = -1e7, r = 1e7;
  while (r - l > eps) {
    const real mid = (l + r) / 2;
    if (bellmanFord(n, mid))
      l = mid;
    else
      r = mid;
  }
  return l;
}
int main()
{
  unsigned int n, m;
  scanf("%u%u", &n, &m);
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int u, v;
    real w;
    scanf("%u%u%lf", &u, &v, &w);
    addEdge(u, v, w);
  }
  printf("%.8lf\n", binarySearch(n));
  return 0;
}