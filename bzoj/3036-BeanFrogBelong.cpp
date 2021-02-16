#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cmath>
#include <cstdio>
#include <iostream>
#include <queue>
using namespace std;
const unsigned int maxn = 1e5, maxm = maxn * 2;
using real = double;

struct graph {
  struct edge {
    unsigned int to;
    unsigned long long val;
    edge* pre;
  } ed[maxm + 1], *cur = ed;
  edge* head[maxn + 1] = {};
  unsigned degree[maxn + 1] = {};

  void addEdge(const unsigned int from, const unsigned int to,
               const unsigned long long val)
  {
    cur->to = to;
    cur->val = val;
    cur->pre = head[from];
    head[from] = cur;
    ++degree[from];
    ++cur;
  }
  edge* getHead(const unsigned int x) { return head[x]; }
} orig, rev;
real f[maxn + 1];
unsigned int cnt[maxn + 1];

real dp(const unsigned int n)
{
  queue<unsigned int> q;
  q.push(n);
  while (!q.empty()) {
    const unsigned int cur = q.front();
    q.pop();
    for (auto i = orig.getHead(cur); i; i = i->pre)
      f[cur] += (i->val + f[i->to]);
    if (orig.getHead(cur)) f[cur] /= orig.degree[cur];
    for (auto i = rev.getHead(cur); i; i = i->pre) {
      const unsigned int to = i->to;
      ++cnt[to];
      if (cnt[to] == orig.degree[to]) q.push(to);
    }
  }
  return f[1];
}

int main()
{
  unsigned int n, m;
  scanf("%u%u", &n, &m);
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int u, v;
    unsigned long long w;
    scanf("%u%u%llu", &u, &v, &w);
    orig.addEdge(u, v, w);
    rev.addEdge(v, u, w);
  }
  printf("%.2lf\n", (round(dp(n) * 100) / 100));
  return 0;
}