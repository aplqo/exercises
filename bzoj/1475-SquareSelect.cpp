#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
#include <numeric>
#include <queue>
using namespace std;
const unsigned int maxn = 30;
const unsigned int maxv = (maxn + 1) * (maxn + 1) * 2,
                   maxe = maxv * 2 + maxn * maxn * 2 + maxn * maxn * 4 * 2;
constexpr int inf = INT_MAX / 2;

struct Edge {
  unsigned int from, to, id;
  int cap;
  mutable int flw = 0;
  const Edge* pre;
} ed[maxe + 10];
const Edge *head[maxv + 10], *par[maxv + 10];
int num[maxn + 2][maxn + 2];
unsigned int dep[maxv + 10];

void addDirEdge(const unsigned int from, const unsigned int to, const int cap)
{
  static Edge* cur = ed;
  cur->id = cur - ed;
  cur->from = from;
  cur->to = to;
  cur->cap = cap;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
inline void addEdge(const unsigned int from, const unsigned int to,
                    const int cap)
{
  addDirEdge(from, to, cap);
  addDirEdge(to, from, 0);
}
void bfs(const unsigned int t)
{
  static bool vis[maxv + 1];
  queue<unsigned int> q;
  q.push(t);
  vis[t] = true;
  while (!q.empty()) {
    const unsigned int cur = q.front();
    q.pop();
    for (const Edge* i = head[cur]; i; i = i->pre)
      if (!vis[i->to] && ed[i->id ^ 1].cap > ed[i->id ^ 1].flw) {
        vis[i->to] = true;
        dep[i->to] = dep[cur] + 1;
        q.push(i->to);
      }
  }
}
int agument(const unsigned int s, const unsigned int t)
{
  int ret = INT_MAX;
  unsigned int c = t;
  while (c != s) {
    ret = min(ret, par[c]->cap - par[c]->flw);
    c = par[c]->from;
  }
  c = t;
  while (c != s) {
    par[c]->flw += ret;
    ed[par[c]->id ^ 1].flw -= ret;
    c = par[c]->from;
  }
  return ret;
}
long long isap(const unsigned int s, const unsigned int t,
               const unsigned int cv)
{
  static unsigned int cnt[maxv + 1];
  static const Edge* cur[maxv + 1];
  bfs(t);
  copy(head, head + cv, cur);
  cur[s] = head[s];
  cur[t] = head[t];
  for (unsigned int i = 0; i < cv; ++i)
    ++cnt[dep[i]];
  ++cnt[dep[s]];
  ++cnt[dep[t]];
  long long ret = 0;
  unsigned int x = s;
  while (dep[s] < cv + 2) {
    if (x == t) {
      ret += agument(s, t);
      x = s;
    }
    bool adv = false;
    for (const Edge* i = cur[x]; i; i = i->pre)
      if (dep[i->to] + 1 == dep[x] && i->cap > i->flw) {
        adv = true;
        par[i->to] = i;
        cur[x] = i;
        x = i->to;
        break;
      }
    if (!adv) {
      unsigned int mv = cv + 2;
      for (const Edge* i = head[x]; i; i = i->pre)
        if (i->cap > i->flw) mv = min(mv, dep[i->to] + 1);
      --cnt[dep[x]];
      if (!cnt[dep[x]]) break;
      dep[x] = mv;
      cur[x] = head[x];
      ++cnt[dep[x]];
      if (x != s) x = par[x]->from;
    }
  }
  return ret;
}
pair<unsigned int, unsigned int> buildGraph(const unsigned int n,
                                            const unsigned int m)
{
  enum { In = 0, Out = 1 };
  const auto id = [n](const unsigned int x, const unsigned int y,
                      const unsigned int t) { return 2 * (x * n + y) + t; };
  for (unsigned int i = 0; i < m; ++i) {
    for (unsigned int j = 1; j < n; ++j) {
      addEdge(id(i, j - 1, Out), id(i, j, In), inf);
      addEdge(id(i, j, Out), id(i, j - 1, In), inf);
    }
    if (i == m - 1) continue;
    addEdge(id(i, 0, Out), id(i + 1, 0, In), inf);
    addEdge(id(i + 1, 0, Out), id(i, 0, In), inf);
    for (unsigned int j = 1; j < n; ++j) {
      addEdge(id(i + 1, j, Out), id(i, j, In), inf);
      addEdge(id(i, j, Out), id(i + 1, j, In), inf);
    }
  }
  const unsigned int s = id(m, n, Out), t = id(m, n, In);
  for (unsigned int i = 0; i < m; ++i)
    for (unsigned int j = 0; j < n; ++j) {
      addEdge(id(i, j, In), id(i, j, Out), num[i][j]);
      if ((i & 0x01) ^ (j & 0x01))
        addEdge(id(i, j, Out), t, inf);
      else
        addEdge(s, id(i, j, In), inf);
    }
  return make_pair(s, t);
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> m >> n;
  long long sum = 0;
  for (unsigned int i = 0; i < m; ++i) {
    copy_n(istream_iterator<int>(cin), n, num[i]);
    sum = accumulate(num[i], num[i] + n, sum);
  }
  const auto st = buildGraph(n, m);
  const unsigned int ml = max(n, m);
  cout << sum - isap(st.first, st.second, ml * ml * 2 + 2) << endl;
  return 0;
}