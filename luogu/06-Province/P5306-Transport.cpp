#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1e5;

class Vector {
 public:
  inline void push_back(const long long v) { *(ptr++) = v; }
  inline void clear() { ptr = val; };

  long long val[maxn + 10], *ptr = val;
};
struct Edge {
  unsigned int to;
  int dis;
  const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
unsigned int siz[maxn + 10];
unsigned long long ans;
bool vis[maxn + 10];
long long a[maxn + 10];
Vector rootTo, rootFrom, subFrom, subTo;

inline void addEdge(const unsigned int from, const unsigned int to,
                    const int dis)
{
  static Edge* cur = ed;
  new (cur) Edge{to, dis, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, dis, head[to]};
  head[to] = cur++;
}

void dfsPoint(const unsigned int x, const unsigned int f, const unsigned int n,
              unsigned int& pos, unsigned int& mxv)
{
  siz[x] = 1;
  unsigned int sz = 0;
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f || vis[to]) continue;
    dfsPoint(to, x, n, pos, mxv);
    sz = max(sz, siz[to]);
    siz[x] += siz[to];
  }
  sz = max(sz, n - siz[x]);
  if (sz < mxv) {
    pos = x;
    mxv = sz;
  }
}
inline unsigned int treeCenter(const unsigned int x, const unsigned int siz)
{
  unsigned int pos = 0, ms = UINT_MAX;
  dfsPoint(x, 0, siz, pos, ms);
  return pos;
}
void dfsSubtree(const unsigned int x, const unsigned int f, long long sum,
                const long long mx)
{
  rootTo.push_back(mx);
  subTo.push_back(mx);
  sum -= a[x];
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    const long long d = sum + i->dis;
    if (to != f && !vis[to]) dfsSubtree(to, x, d, max(mx, d));
  }
}
void dfsAddVertex(const unsigned int x, const unsigned int f,
                  const long long sum, const long long mx)
{
  if (mx >= 0) {
    rootFrom.push_back(sum);
    subFrom.push_back(sum);
  }
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    const long long d = a[to] - i->dis;
    if (to != f && !vis[to]) dfsAddVertex(to, x, sum + d, min(d, mx + d));
  }
}
unsigned long long count(Vector& from, Vector& to)
{
  sort(from.val, from.ptr);
  sort(to.val, to.ptr);
  unsigned long long ret = 0;
  const long long* itFrom = from.val;
  for (const long long* it = to.val; it != to.ptr; ++it) {
    while (itFrom != from.ptr && *itFrom < *it)
      ++itFrom;
    ret += from.ptr - itFrom;
  }
  return ret;
}
void conquer(const unsigned int x)
{
  vis[x] = true;
  rootFrom.push_back(0);
  rootTo.push_back(0);
  unsigned long long exclude = 1;
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (vis[to]) continue;

    dfsAddVertex(to, x, a[to] - i->dis, a[to] - i->dis);
    dfsSubtree(to, x, i->dis - a[x], i->dis - a[x]);
    exclude += count(subFrom, subTo);

    subFrom.clear();
    subTo.clear();
  }
  ans += count(rootFrom, rootTo) - exclude;
  rootFrom.clear();
  rootTo.clear();

  for (const Edge* i = head[x]; i; i = i->pre)
    if (!vis[i->to]) conquer(treeCenter(i->to, siz[i->to]));
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  copy_n(istream_iterator<long long>(cin), n, a + 1);
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int x, y, d;
    cin >> x >> y >> d;
    addEdge(x, y, d);
  }
  conquer(treeCenter(1, n));
  cout << ans << "\n";
  return 0;
}
