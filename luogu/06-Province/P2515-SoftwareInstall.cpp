#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
using namespace std;
const unsigned int maxn = 100, maxm = 1000, maxv = 1000;
constexpr int ninf = INT_MIN / 2;

struct Edge {
  unsigned int to;
  Edge *pre, *nxt, *rev;
} ed[maxn * 4 + 10];
Edge head[maxn * 4 + 10];
vector<unsigned int> cyc;
bool inCycle[maxn * 4 + 10];
int f[maxn * 4 + 10][maxm + 10], w[maxn * 4 + 10], v[maxn * 4 + 10], m;
unsigned int vcnt, n;

namespace Disjoint {
unsigned int fa[maxn + 10], siz[maxn + 10];
static void init(const unsigned int n)
{
  fill(siz, siz + 1 + n, 1);
  iota(fa, fa + 1 + n, 0);
}
unsigned int find(const unsigned int x)
{
  return fa[x] == x ? x : fa[x] = find(fa[x]);
}
void merge(unsigned int x, unsigned int y)
{
  x = find(x);
  y = find(y);
  if (x == y) return;
  if (siz[x] < siz[y]) swap(x, y);
  fa[y] = x;
  siz[x] += siz[y];
}
}  // namespace Disjoint
inline Edge* addDirectEdge(const unsigned int from, const unsigned int to)
{
  static Edge* cur = ed;
  cur->to = to;
  head[from].pre->nxt = cur;
  cur->pre = head[from].pre;
  cur->nxt = &head[from];
  head[from].pre = cur;
  return cur++;
}
inline void addEdge(const unsigned int from, const unsigned int to)
{
  Edge *const e1 = addDirectEdge(from, to), *const e2 = addDirectEdge(to, from);
  e1->rev = e2;
  e2->rev = e1;
  Disjoint::merge(from, to);
}
void mergeEdge(const unsigned int x, const unsigned int rt)
{
  if (head[x].pre == &head[x]) return;
  head[rt].pre->nxt = head[x].nxt;
  head[x].nxt->pre = head[rt].pre;
  head[x].pre->nxt = &head[rt];
  head[rt].pre = head[x].pre;
  head[x].pre = head[x].nxt = &head[x];
}
bool dfs(const unsigned int x, const Edge* const fe)
{
  static unsigned int stk[maxn + 10], *top = stk;
  static bool vis[maxn + 10];
  *(top++) = x;
  vis[x] = true;
  for (const Edge* i = head[x].nxt; i != &head[x]; i = i->nxt) {
    const unsigned int to = i->to;
    if (i == fe || to > n) continue;
    if (vis[to]) {
      static const auto join = [](const unsigned int x, const unsigned int rt) {
        w[rt] += w[x];
        v[rt] += v[x];
        inCycle[x] = true;
        mergeEdge(x, vcnt);
      };
      ++vcnt;
      while (*(top - 1) != to)
        join(*(--top), vcnt);
      join(to, vcnt);
      addDirectEdge(0, vcnt);
      top = stk;
      return true;
    }
    if (dfs(to, i->rev)) return true;
  }
  --top;
  return false;
}
void dpTree(const unsigned int x, const Edge* fe)
{
  for (const Edge* i = head[x].nxt; i != &head[x]; i = i->nxt) {
    const unsigned int to = i->to;
    if (inCycle[to] || i == fe) continue;
    dpTree(to, i->rev);
    for (int i = m; i >= 0; --i)
      for (int j = 1; j <= i; ++j)
        f[x][i] = max(f[x][i], f[x][i - j] + f[to][j]);
  }
  for (int i = m; i >= w[x]; --i)
    f[x][i] = f[x][i - w[x]] + v[x];
  fill(f[x], f[x] + w[x], 0);
}
static void init(const unsigned int n)
{
  Disjoint::init(n);
  vcnt = n;
  for (Edge* i = head; i < head + n * 2 + 1; ++i)
    i->pre = i->nxt = i;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  cin >> n >> m;
  copy_n(istream_iterator<unsigned int>(cin), n, w + 1);
  copy_n(istream_iterator<unsigned int>(cin), n, v + 1);
  init(n);
  for (unsigned int i = 1; i <= n; ++i) {
    unsigned int d;
    cin >> d;
    addEdge(d, i);
  }
  for (unsigned int i = 1; i <= n; ++i)
    if (Disjoint::find(i) == i) dfs(i, nullptr);
  dpTree(0, nullptr);
  cout << *max_element(f[0], f[0] + 1 + m) << "\n";
  return 0;
}