#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
using namespace std;
const unsigned int maxn = 200, maxm = 10200;
constexpr unsigned int maxv = maxn + 2, maxe = maxm * 2 + maxn * 2 * 2;
typedef int Flow;
constexpr Flow inf = numeric_limits<Flow>::max();

struct Edge {
  unsigned int from, to;
  Flow cap;
  mutable Flow flow;
  const Edge *pre, *rev;
} ed[maxe * 2 + 10];
const Edge *head[maxv + 10], *from[maxv + 10];
const Edge* ptr[maxm + 10];
Flow acc[maxv + 10], lower[maxm + 10];
unsigned int dep[maxv + 10];

inline const Edge* addEdge(const unsigned int from, const unsigned int to,
                           const Flow cap)
{
  static Edge* cur = ed;
  const Edge* const ret = cur;
  new (cur) Edge{from, to, cap, 0, head[from], cur + 1};
  head[from] = cur++;
  new (cur) Edge{to, from, 0, 0, head[to], cur - 1};
  head[to] = cur++;
  return ret;
}
inline void addRangeEdge(const unsigned int id, const unsigned int from,
                         const unsigned int to, const Flow low, const Flow up)
{
  acc[from] -= low;
  acc[to] += low;
  ptr[id] = addEdge(from, to, up - low);
  ::lower[id] = low;
}

void bfs(const unsigned int t)
{
  static bool vis[maxv + 10];
  queue<unsigned int> q;
  vis[t] = true;
  q.emplace(t);
  while (!q.empty()) {
    const unsigned int cur = q.front();
    q.pop();
    for (const Edge* i = head[cur]; i; i = i->pre) {
      const unsigned int to = i->to;
      if (!vis[to] && i->rev->cap > i->rev->flow) {
        dep[to] = dep[cur] + 1;
        vis[to] = true;
        q.emplace(to);
      }
    }
  }
}
void augment(const unsigned int s, const unsigned int t)
{
  Flow ret = inf;
  for (unsigned int x = t; x != s; x = from[x]->from)
    ret = min(ret, from[x]->cap - from[x]->flow);
  for (unsigned int x = t; x != s; x = from[x]->from) {
    from[x]->flow += ret;
    from[x]->rev->flow -= ret;
  }
}
void isap(const unsigned int n, const unsigned int s, const unsigned int t)
{
  static const Edge* cur[maxv + 10];
  static unsigned int cnt[maxv + 10];
  bfs(t);
  for (unsigned int i = 0; i < n; ++i)
    ++cnt[dep[i]];
  copy(head, head + n, cur);
  unsigned int x = s;
  while (dep[s] < n) {
    if (x == t) {
      augment(s, t);
      x = s;
    }
    bool adv = false;
    for (const Edge*& i = cur[x]; i; i = i->pre)
      if (dep[i->to] + 1 == dep[x] && i->cap > i->flow) {
        adv = true;
        from[i->to] = i;
        x = i->to;
        break;
      }
    if (!adv) {
      unsigned int mv = n;
      for (const Edge* i = head[x]; i; i = i->pre)
        if (i->cap > i->flow) mv = min(mv, dep[i->to]);
      --cnt[dep[x]];
      if (!cnt[dep[x]]) break;
      dep[x] = mv + 1;
      ++cnt[mv + 1];
      cur[x] = head[x];
      if (x != s) x = from[x]->from;
    }
  }
}

static void buildGraph(const unsigned int n)
{
  const unsigned int s = 0, t = n + 1;
  for (unsigned int i = 1; i <= n; ++i)
    if (acc[i] > 0)
      addEdge(s, i, acc[i]);
    else if (acc[i] < 0)
      addEdge(i, t, -acc[i]);
}
bool check()
{
  for (const Edge* i = head[0]; i; i = i->pre)
    if (i->cap > i->flow) return false;
  return true;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int i = m; i; --i) {
    unsigned int s, t, l, u;
    cin >> s >> t >> l >> u;
    addRangeEdge(m - i, s, t, l, u);
  }
  buildGraph(n);
  isap(n + 2, 0, n + 1);
  if (check()) {
    cout << "YES\n";
    for (unsigned int i = 0; i < m; ++i)
      cout << ptr[i]->flow + lower[i] << "\n";
  }
  else
    cout << "NO\n";
  return 0;
}