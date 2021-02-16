#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <queue>
using namespace std;
const unsigned int maxn = 300000, maxm = 300000;

struct Graph {
  struct Edge {
    unsigned int to;
    const Edge* pre;
  } ed[maxm + 1], *cur = ed;
  const Edge* head[maxn + 1] = {};
  unsigned int degree[maxn + 1] = {};

  inline void addEdge(const unsigned int from, const unsigned int to);
} orig, rev;
unsigned int f[maxn + 1];
bool ign[maxn + 1];

inline void Graph::addEdge(const unsigned int from, const unsigned int to)
{
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
  ++degree[to];
}
void check(Graph& g, const unsigned int x, const unsigned int another,
           const unsigned int val)
{
  for (auto i = g.head[another]; i; i = i->pre)
    if (g.degree[i->to] == 1) {
      ign[x] = true;
      return;
    }
  f[x] += val;
}
void topological(Graph& g, const unsigned int n)
{
  queue<unsigned int> q;
  unsigned int cnt = 0;
  for (unsigned int i = 1; i <= n; ++i)
    if (!g.degree[i]) {
      q.push(i);
      ++cnt;
    }
  while (!q.empty()) {
    const unsigned int cur = q.front();
    if (q.size() == 1)
      f[cur] += n - cnt;
    else if (q.size() == 2)
      check(g, cur, q.back(), n - cnt);
    q.pop();
    for (auto j = g.head[cur]; j; j = j->pre) {
      --g.degree[j->to];
      if (!g.degree[j->to]) {
        ++cnt;
        q.push(j->to);
      }
    }
  }
}
unsigned int count(const unsigned int n)
{
  unsigned int ret = 0;
  for (unsigned int i = 1; i <= n; ++i)
    if (!ign[i] && f[i] >= n - 2) ++ret;
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int x, y;
    cin >> x >> y;
    orig.addEdge(x, y);
    rev.addEdge(y, x);
  }
  topological(orig, n);
  topological(rev, n);
  cout << count(n) << "\n";
  return 0;
}