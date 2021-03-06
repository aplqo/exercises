#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <queue>
const int maxn = 1e5, maxse = 2e5;
const int maxv = maxn * 2, maxe = (maxv + maxse) * 2;

namespace ISAP {
struct Edge {
  int from, to;
  mutable int flow;
  const Edge *pre, *rev;
} ed[maxe + 10];
const Edge *head[maxv + 10], *cur[maxv + 10];
int dep[maxv + 10];

inline void addEdge(const int from, const int to)
{
  static Edge *cur = ed;
  new (cur) Edge{from, to, 1, head[from], cur + 1};
  head[from] = cur++;
  new (cur) Edge{to, from, 0, head[to], cur - 1};
  head[to] = cur++;
}
bool bfs(const int n, const int s, const int t)
{
  static bool vis[maxv + 10];
  std::queue<int> q;
  std::memset(vis, 0, sizeof(vis[0]) * n);
  vis[t] = true;
  q.push(t);
  while (!q.empty()) {
    const int cur = q.front();
    q.pop();
    for (const Edge *i = head[cur]; i; i = i->pre)
      if (i->rev->flow && !vis[i->to]) {
        vis[i->to] = true;
        dep[i->to] = dep[cur] + 1;
        q.push(i->to);
      }
  }
  return vis[s];
}
int augment(const int x, const int cap, const int sink)
{
  if (!cap || x == sink) return cap;
  int rst = cap;
  for (const Edge *&i = cur[x]; i && rst; i = i->pre)
    if (i->flow && dep[x] == dep[i->to] + 1) {
      const int v = augment(i->to, std::min(i->flow, rst), sink);
      i->flow -= v;
      i->rev->flow += v;
      rst -= v;
    }
  return cap - rst;
}
int isap(const int n, const int source, const int sink)
{
  int ret = 0;
  while (bfs(n, source, sink)) {
    std::memcpy(cur, head, sizeof(head[0]) * n);
    ret += augment(source, INT_MAX, sink);
  }
  return ret;
}

};  // namespace ISAP
int edge[maxn + 19][2];
bool used[maxn + 10];
int n;

int maxMatch(const int n)
{
  const int source = n * 2 - 1, sink = source + 1;
  for (int i = 1; i < n; ++i)
    ISAP::addEdge(source, i);
  for (int i = 0; i < n - 1; ++i)
    ISAP::addEdge(i + n, sink);
  return ISAP::isap(sink + 1, source, sink);
}
void construct(const int x)
{
  for (auto i = ISAP::head[x]; i; i = i->pre) {
    const int to = i->to - n;
    if (0 <= to && to < n - 1 && !used[to]) {
      used[to] = true;
      edge[to][0] = 1 + x;
      for (auto j = ISAP::head[i->to]; j; j = j->pre)
        if (0 <= j->to && j->to < n && j->flow) {
          edge[to][1] = j->to + 1;
          construct(j->to);
          break;
        }
    }
  }
}

int main()
{
  std::ios::sync_with_stdio(false);
  std::cin >> n;
  for (int i = 0; i < n - 1; ++i) {
    int c;
    std::cin >> c;
    for (int j = c; j; --j) {
      int p;
      std::cin >> p;
      ISAP::addEdge(p - 1, i + n);
    }
  }
  if (maxMatch(n) == n - 1) {
    construct(0);
    for (int i = 0; i < n - 1; ++i)
      std::cout << edge[i][0] << " " << edge[i][1] << "\n";
  }
  else
    std::cout << "-1\n";
  return 0;
}