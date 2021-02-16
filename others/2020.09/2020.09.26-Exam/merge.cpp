#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <climits>
#include <cstring>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
using namespace std;
const unsigned int maxn = 1000, maxm = 100000;
constexpr unsigned long long inf = UINT_MAX;

struct Edge {
  unsigned int to;
  const Edge* pre;
} ed[maxm * 2 + 10];
const Edge* head[maxn + 10];
unsigned int dep[maxn + 10];
bool val[maxn + 10], vis[maxn + 10];
vector<unsigned int> blk;

inline void addEdge(const unsigned int from, const unsigned int to)
{
  static Edge* cur = ed;
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, head[to]};
  head[to] = cur++;
}
bool dfsTest(const unsigned int x)
{
  vis[x] = true;
  blk.emplace_back(x);
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (vis[to] && val[to] == val[x])
      return false;
    else if (!vis[to]) {
      val[to] = !val[x];
      if (!dfsTest(to)) return false;
    }
  }
  return true;
}
unsigned int bfs(const unsigned int x)
{
  static unsigned int vis[maxn + 10], inque[maxn + 10], sta = 0;
  ++sta;
  unsigned int ret = 0;
  queue<unsigned int> q;
  q.push(x);
  dep[x] = 0;
  vis[x] = inque[x] = sta;
  while (!q.empty()) {
    const unsigned int cur = q.front();
    q.pop();
    ret = max(ret, dep[cur]);
    for (const Edge* i = head[cur]; i; i = i->pre) {
      const unsigned int to = i->to;
      dep[to] = vis[to] < sta ? dep[cur] + 1 : min(dep[to], dep[cur] + 1);
      vis[to] = sta;
      if (inque[to] < sta) {
        inque[to] = sta;
        q.emplace(to);
      }
    }
  }
  return ret;
}
unsigned int longest()
{
  unsigned int ret = 0;
  for (unsigned int i : blk)
    ret = max(ret, bfs(i));
  blk.clear();
  return ret;
}
pair<bool, unsigned int> findMax(const unsigned int n)
{
  unsigned int ret = 0;
  for (unsigned int i = 1; i <= n; ++i) {
    if (vis[i]) continue;
    if (!dfsTest(i)) return make_pair(false, 0);
    ret += longest();
  }
  return make_pair(true, ret);
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
    addEdge(u, v);
  }
  blk.reserve(n);
  const auto [suc, va] = findMax(n);
  if (!suc)
    cout << "-1\n";
  else
    cout << va << "\n";
  return 0;
}