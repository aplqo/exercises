#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <climits>
#include <cstdio>
#include <deque>
#include <utility>
using namespace std;
using Range = pair<unsigned int, unsigned int>;
constexpr unsigned int inf = UINT_MAX / 2;
const unsigned int maxn = 5e5, maxm = 1e5, maxl = 19;
const unsigned int maxv = maxn * 4 * 2, maxe = maxm * maxl * 2 + maxn * 2;

struct edge {
  unsigned int to;
  unsigned int val;
  edge* pre;
} ed[maxe * 2];
edge* head[maxv + 1];
unsigned int vCnt;
unsigned int dis[maxv + 1];
bool vis[maxv + 1];
unsigned int no[2][maxn + 1];
unsigned int lc[maxn * 4 + 1], rc[maxn * 4 + 1];

enum { In = 1, Out = 0 };
inline unsigned int read()
{
  unsigned int ret = 0;
  char c = getchar();
  while (!isdigit(c))
    c = getchar();
  while (isdigit(c)) {
    ret = ret * 10 + c - '0';
    c = getchar();
  }
  return ret;
}
void addEdge(const unsigned int from, const unsigned int to,
             const unsigned int val)
{
  static edge* cur = ed;
  cur->to = to;
  cur->val = val;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
template <unsigned int f>
void build(const unsigned int root, const Range r)
{
  if (r.second - r.first == 1) {
    no[f][r.first] = root;
    return;
  }
  lc[root] = ++vCnt;
  rc[root] = ++vCnt;
  const unsigned int m = (r.first + r.second) >> 1;
  if (f == In) {
    addEdge(lc[root], root, 0);
    addEdge(rc[root], root, 0);
  }
  else {
    addEdge(root, lc[root], 0);
    addEdge(root, rc[root], 0);
  }
  build<f>(lc[root], Range(r.first, m));
  build<f>(rc[root], Range(m, r.second));
}
template <unsigned int f, unsigned int val>
void add(const unsigned int root, const Range tr, const unsigned int v,
         const Range ar)  // tr: node range, ar: add range
{
  if (tr == ar) {
    if (f == In)
      addEdge(root, v, val);
    else
      addEdge(v, root, val);
    return;
  }
  const unsigned int m = (tr.first + tr.second) >> 1;
  if (ar.second <= m)
    add<f, val>(lc[root], Range(tr.first, m), v, ar);
  else if (ar.first >= m)
    add<f, val>(rc[root], Range(m, tr.second), v, ar);
  else {
    add<f, val>(lc[root], Range(tr.first, m), v, Range(ar.first, m));
    add<f, val>(rc[root], Range(m, tr.second), v, Range(m, ar.second));
  }
}
static void bfs(const unsigned int s)
{
  deque<unsigned int> q;
  fill(dis, dis + vCnt + 1, inf);
  dis[no[In][s]] = dis[no[Out][s]] = 0;
  q.push_back(no[In][s]);
  while (!q.empty()) {
    const unsigned int c = q.front();
    q.pop_front();
    vis[c] = true;
    for (edge* i = head[c]; i; i = i->pre) {
      if (vis[i->to]) continue;
      dis[i->to] = min(dis[i->to], dis[c] + i->val);
      if (i->val)
        q.push_back(i->to);
      else
        q.push_front(i->to);
    }
  }
}
void addEdge(const unsigned int n, const unsigned int root[2], const Range l,
             const Range r)
{
  const unsigned int vp = ++vCnt;
  add<Out, 1>(root[Out], make_pair(1, n + 1), vp, r);
  add<In, 0>(root[In], make_pair(1, n + 1), vp, l);
}

int main()
{
  const auto n = read();
  const auto m = read();
  const auto p = read();
  unsigned int root[2];
  root[In] = ++vCnt;
  build<In>(root[In], make_pair(1u, n + 1));
  root[Out] = ++vCnt;
  build<Out>(root[Out], make_pair(1u, n + 1));
  for (unsigned int i = 1; i <= n; ++i) {
    addEdge(no[In][i], no[Out][i], 0);
    addEdge(no[Out][i], no[In][i], 0);
  }
  for (unsigned int i = 0; i < m; ++i) {
    Range l, r;
    l.first = read();
    l.second = read() + 1;
    r.first = read();
    r.second = read() + 1;
    addEdge(n, root, l, r);
    addEdge(n, root, r, l);
  }
  bfs(p);
  for (unsigned int i = 1; i <= n; ++i)
    printf("%u\n", dis[no[Out][i]]);
  return 0;
}