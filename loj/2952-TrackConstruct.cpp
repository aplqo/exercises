#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <fstream>
#include <iostream>
#include <set>
#include <utility>
using namespace std;
const unsigned int maxn = 5e4;

struct Edge {
  unsigned int to, val;
  const Edge* pre;
} ed[maxn * 2 + 1];
const Edge* head[maxn + 1];
unsigned int mid;

void addEdge(const unsigned int from, const unsigned int to,
             const unsigned int val)
{
  static Edge* cur = ed;
  cur->to = to;
  cur->val = val;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
pair<unsigned int, unsigned int> dfs(const unsigned int x, const unsigned int f)
{
  unsigned int ret = 0, cnt = 0;
  multiset<unsigned int> chain;
  for (const Edge* i = head[x]; i; i = i->pre) {
    if (i->to == f) continue;
    const auto [chl, cn] = dfs(i->to, x);
    cnt += cn;
    if (chl + i->val >= mid)
      ++cnt;
    else
      chain.insert(chl + i->val);
  }
  auto it = chain.begin();
  while (it != chain.end()) {
    const unsigned int cv = *it;
    chain.erase(it++);
    auto t = chain.lower_bound(mid - cv);
    if (t != chain.end()) {
      if (t == it) ++it;
      chain.erase(t);
      ++cnt;
    }
    else
      ret = max(ret, cv);
  }
  return make_pair(ret, cnt);
}
unsigned int binarySearch(const unsigned int ws, const unsigned int m)
{
  int l = 0, r = ws;
  while (l <= r) {
    mid = (l + r) >> 1;
    if (dfs(1, 0).second >= m)
      l = mid + 1;
    else
      r = mid - 1;
  }
  return r;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
  ifstream fin("track.in");
  ofstream fout("track.out");
  istream& in = fin;
  ostream& out = fout;
#else
  istream& in = cin;
  ostream& out = cout;
#endif
  unsigned int n, m, ws = 0;
  in >> n >> m;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int u, v, w;
    in >> u >> v >> w;
    ws += w;
    addEdge(u, v, w);
    addEdge(v, u, w);
  }
  out << binarySearch(ws, m) << endl;
  return 0;
}