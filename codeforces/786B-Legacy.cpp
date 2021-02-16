#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <functional>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
using namespace std;
const unsigned int maxn = 1e5, maxq = 1e5, maxl = 17;
const unsigned int maxv = maxq * maxl * 2 + maxn * 4 * 2,
                   maxe = maxq * maxl * 2 + maxq;

enum { In = 0, Out = 1 };
struct edge {
  unsigned int to;
  unsigned long long val;
  const edge* pre;
} ed[maxe + 1];
const edge* head[maxv + 1];
unsigned long long dis[maxv + 1];
unsigned int lc[maxv + 1], rc[maxv + 1], root[2];
unsigned int pos[2][maxn + 1];
unsigned int vCnt, n;

void addEdge(const unsigned int from, const unsigned int to,
             const unsigned long long val = 0)
{
  static edge* cur = ed;
  cur->to = to;
  cur->val = val;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
using Range = pair<unsigned int, unsigned int>;
template <unsigned int t>
void build(const unsigned int rt, const Range r)
{
  if (r.second - r.first == 1) {
    pos[t][r.first] = rt;
    return;
  }
  const unsigned int m = (r.first + r.second) >> 1;
  lc[rt] = ++vCnt;
  rc[rt] = ++vCnt;
  if constexpr (t == In) {
    addEdge(lc[rt], rt);
    addEdge(rc[rt], rt);
  }
  else {
    addEdge(rt, lc[rt]);
    addEdge(rt, rc[rt]);
  }
  build<t>(lc[rt], Range(r.first, m));
  build<t>(rc[rt], Range(m, r.second));
}
template <unsigned int t>
void add(const unsigned int rt, const Range cr, const Range qr,
         const unsigned int vp, const unsigned long long w)
{
  if (cr == qr) {
    if constexpr (t == In)
      addEdge(rt, vp, w);
    else
      addEdge(vp, rt, w);
    return;
  }
  const unsigned int m = (cr.first + cr.second) >> 1;
  if (qr.second <= m)
    add<t>(lc[rt], Range(cr.first, m), qr, vp, w);
  else if (qr.first >= m)
    add<t>(rc[rt], Range(m, cr.second), qr, vp, w);
  else {
    add<t>(lc[rt], Range(cr.first, m), Range(qr.first, m), vp, w);
    add<t>(rc[rt], Range(m, cr.second), Range(m, qr.second), vp, w);
  }
}
void addEdge(const Range from, const unsigned int to,
             const unsigned long long w)
{
  const unsigned int p = ++vCnt;
  add<In>(root[In], Range(1, n + 1), from, p, w);
  addEdge(p, pos[Out][to]);
}
void addEdge(const unsigned int from, const Range to,
             const unsigned long long w)
{
  const unsigned int p = ++vCnt;
  add<Out>(root[Out], Range(1, n + 1), to, p, w);
  addEdge(pos[In][from], p);
}
static void init(const unsigned int n)
{
  root[In] = ++vCnt;
  root[Out] = ++vCnt;
  build<In>(root[In], Range(1, n + 1));
  build<Out>(root[Out], Range(1, n + 1));
  for (unsigned int i = 1; i <= n; ++i) {
    addEdge(pos[In][i], pos[Out][i]);
    addEdge(pos[Out][i], pos[In][i]);
  }
}
void dijkstra(const unsigned int s)
{
  static bool vis[maxv + 1];
  using pair_t = pair<unsigned long long, unsigned int>;
  priority_queue<pair_t, vector<pair_t>, greater<pair_t>> q;
  fill(dis, dis + vCnt + 1, ULLONG_MAX);
  q.emplace(0, s);
  dis[s] = 0;
  while (!q.empty()) {
    const auto cur = q.top();
    q.pop();
    if (vis[cur.second]) continue;
    vis[cur.second] = true;
    for (const edge* i = head[cur.second]; i; i = i->pre)
      if (dis[i->to] > cur.first + i->val) {
        dis[i->to] = cur.first + i->val;
        q.emplace(dis[i->to], i->to);
      }
  }
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int q, s;
  cin >> n >> q >> s;
  init(n);
  for (unsigned int i = 0; i < q; ++i) {
    unsigned int t, v, u, l, r, w;
    cin >> t >> v;
    switch (t) {
      case 1:
        cin >> u >> w;
        addEdge(pos[In][v], pos[Out][u], w);
        break;
      case 2:
        cin >> l >> r >> w;
        addEdge(v, Range(l, r + 1), w);
        break;
      case 3:
        cin >> l >> r >> w;
        addEdge(Range(l, r + 1), v, w);
        break;
    }
  }
  dijkstra(pos[In][s]);
  for (unsigned int i = 1; i <= n; ++i)
    if (dis[pos[Out][i]] != ULLONG_MAX)
      cout << dis[pos[Out][i]] << " ";
    else
      cout << "-1 ";
  cout << endl;
  return 0;
}