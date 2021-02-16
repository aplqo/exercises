#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <limits>
#include <queue>
using namespace std;
const unsigned int maxn = 1000, maxs = 1000, maxt = 1000;
const unsigned int maxe = maxs * 2 + maxt * 2;
using real = double;
constexpr real inf = numeric_limits<real>::max();

struct edge {
  unsigned int to;
  real k, r;
  bool typ;  // true: 1/k false: k
  const edge* pre;

  inline real eval(const real v, const real t) const
  {
    return v * (typ ? 1.0 / (k + r * t) : (k + r * t));
  }
} ed[maxe];
const edge* head[maxn + 1];
bool fix[maxn + 1] = {true};

void addEdge(const unsigned int from, const unsigned int to, bool typ,
             const real k, const real r)
{
  static edge* cur = ed;
  cur->to = to;
  cur->k = k;
  cur->r = r;
  cur->typ = typ;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
bool spfa(const unsigned int n, const real t)
{
  static bool que[maxn + 1];
  static unsigned int cnt[maxn + 1];
  static real dis[maxn + 1];
  queue<unsigned int> q;
  fill(dis, dis + 1 + n, 0);
  fill(cnt, cnt + 1 + n, 0);
  fill(que, que + 1 + n, false);
  q.push(0);
  dis[0] = 1;
  while (!q.empty()) {
    const unsigned int cur = q.front();
    q.pop();
    que[cur] = false;
    for (const edge* i = head[cur]; i; i = i->pre) {
      const unsigned int to = i->to;
      const real nv = i->eval(dis[cur], t);
      if (dis[to] < nv) {
        dis[to] = nv;
        if (!que[to]) {
          ++cnt[to];
          if (cnt[to] > n) return false;
          que[to] = true;
          q.push(to);
        }
      }
    }
  }
  return true;
}
real binarySearch(const unsigned int n, const unsigned int mk)
{
  real l = 0, r = mk;
  while (r - l >= 1e-5) {
    const real m = (l + r) / 2;
    if (spfa(n, m))
      r = m;
    else
      l = m;
  }
  if (l >= 0 && l <= 0) return -1;
  return l;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, s, t, mk = UINT_MAX;
  cin >> n >> s >> t;
  for (unsigned int i = 0; i < s; ++i) {
    unsigned int o, a, b, k;
    cin >> o >> a >> b >> k;
    if (o == 1) mk = min(mk, k);
    addEdge(b, a, o == 2, k, o == 1 ? -1 : 1);
  }
  for (unsigned int i = 0; i < t; ++i) {
    unsigned int c, t;
    cin >> c >> t;
    addEdge(0, c, false, t, 0);
    addEdge(c, 0, true, t, 0);
    fix[c] = true;
  }
  cout << binarySearch(n, mk) << endl;
  return 0;
}