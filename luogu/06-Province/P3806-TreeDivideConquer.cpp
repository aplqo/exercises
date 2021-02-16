#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <set>
using namespace std;
const unsigned int maxn = 1e4, maxm = 100;

struct edge {
  unsigned int to;
  unsigned int val;
  const edge* pre;
} ed[maxn * 2 + 1];
const edge* head[maxn + 1];
struct Array {
 public:
  void push_back(const unsigned int x) { *(cur++) = x; }
  unsigned int* begin() { return val; }
  unsigned int* end() { return cur; }
  void clear() { cur = val; }

 private:
  unsigned int val[maxn + 1], *cur = val;
} a;
bool vis[maxn + 1];
unsigned int siz[maxn + 1];
unsigned int dis[maxn + 1];
struct Query {
  unsigned int k;
  bool ans = false;
} q[maxm + 1];
unsigned int m;
set<unsigned int> s;

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
void dfsPoint(const unsigned int x, const unsigned int f, const unsigned int n,
              unsigned int& pos, unsigned int& mval)
{
  siz[x] = 1;
  unsigned int ms = 0;
  for (const edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f || vis[to]) continue;
    dfsPoint(to, x, n, pos, mval);
    ms = max(ms, siz[to]);
    siz[x] += siz[to];
  }
  ms = max(ms, n - siz[x]);
  if (ms < mval) {
    pos = x;
    mval = ms;
  }
}
inline unsigned int treeCenter(const unsigned int x, const unsigned int siz)
{
  unsigned int ret, mv = UINT_MAX;
  dfsPoint(x, 0, siz, ret, mv);
  return ret;
}
void dfsSubtree(const unsigned int x, const unsigned int f)
{
  a.push_back(x);
  for (const edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f || vis[to]) continue;
    dis[to] = dis[x] + i->val;
    dfsSubtree(to, x);
  }
}
void countSubtree()
{
  for (unsigned int i : a)
    for (Query* j = q; j < q + m; ++j) {
      if (j->ans || dis[i] > j->k) continue;
      j->ans = s.find(j->k - dis[i]) != s.end();
    }
  for (unsigned int i : a)
    if (s.find(dis[i]) == s.end()) s.insert(dis[i]);
}
void dfsTree(const unsigned int x)
{
  vis[x] = true;
  dis[x] = 0;
  s.insert(0);

  for (const edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (vis[to]) continue;
    a.clear();
    dis[to] = dis[x] + i->val;
    dfsSubtree(to, x);
    countSubtree();
  }
  s.clear();
  a.clear();

  for (const edge* i = head[x]; i; i = i->pre)
    if (!vis[i->to]) dfsTree(treeCenter(i->to, siz[i->to]));
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n >> m;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int u, v, w;
    cin >> u >> v >> w;
    addEdge(u, v, w);
    addEdge(v, u, w);
  }
  for (Query* i = q; i < q + m; ++i)
    cin >> i->k;
  dfsTree(treeCenter(1, n));
  for (const Query* i = q; i < q + m; ++i)
    cout << (i->ans ? "AYE" : "NAY") << endl;
  return 0;
}