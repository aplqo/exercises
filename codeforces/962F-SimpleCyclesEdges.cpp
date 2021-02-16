#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 100000, maxm = 100000;

struct Edge {
  unsigned int id, to;
  const Edge* pre;
} ed[maxm * 2 + 10];
const Edge* head[maxn + 10];
bool isInCycle[maxm + 10];
unsigned int dep[maxn + 10];
int cov[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to,
                    const unsigned int id)
{
  static Edge* cur = ed;
  new (cur) Edge{id, to, head[from]};
  head[from] = cur++;
  new (cur) Edge{id, from, head[to]};
  head[to] = cur++;
}
void dfsCover(const unsigned int x, const unsigned int f)
{
  dep[x] = dep[f] + 1;
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f) continue;
    if (!dep[to]) {
      dfsCover(to, x);
      cov[x] += cov[to];
    }
    else if (dep[to] < dep[x]) {
      ++cov[x];
      --cov[to];
    }
  }
}
void dfsEdge(const unsigned int x, const unsigned int f)
{
  static unsigned int high[maxn + 10];
  static int cnt[maxn + 10];
  if (cov[x] == 1) high[x] = high[f] + 1;
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f)
      continue;
    else if (dep[to] < dep[x] && high[x] >= dep[x] - dep[to]) {
      isInCycle[i->id] = true;
      ++cnt[x];
      --cnt[to];
    }
    else if (dep[to] == dep[x] + 1) {
      dfsEdge(to, x);
      isInCycle[i->id] = cnt[to] > 0;
      cnt[x] += cnt[to];
    }
  }
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int u, v;
    cin >> u >> v;
    addEdge(u, v, i);
  }
  for (unsigned int i = 1; i <= n; ++i)
    if (!dep[i]) {
      dfsCover(i, 0);
      dfsEdge(i, 0);
    }
  cout << count(isInCycle, isInCycle + m, true) << "\n";
  for (unsigned int i = 0; i < m; ++i)
    if (isInCycle[i]) cout << i + 1 << " ";
  return 0;
}