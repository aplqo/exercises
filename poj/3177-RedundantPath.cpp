#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const int maxv = 5000, maxe = 10000;

struct edge {
  edge() : pre(NULL), isBridge(false) {}
  unsigned int from, to;
  bool isBridge;
  edge *pre, *rev;
} ed[maxe * 2 + 10], *cur = ed;
unsigned int degree[maxv + 10];
struct vertex {
  vertex() : head(NULL), dfn(0), low(0) {}
  unsigned int dfn, low;
  edge* head;
} ve[maxv + 10];
unsigned int fa[maxv + 10], siz[maxv + 10];

inline edge* AddEdge(const unsigned int from, const unsigned int to)
{
  cur->from = from;
  cur->to = to;
  cur->pre = ve[from].head;
  ve[from].head = cur;
  return cur++;
}

inline void Init(const unsigned int n)
{
  for (unsigned int i = 1; i <= n; ++i)
    fa[i] = i;
  fill(siz, siz + n + 1, 1);
}
unsigned int GetFather(const unsigned int x)
{
  return fa[x] == x ? x : fa[x] = GetFather(fa[x]);
}
inline void Merge(const unsigned int x, const unsigned int y)
{
  unsigned int fx = GetFather(x), fy = GetFather(y);
  if (fx == fy) return;
  if (siz[fx] < siz[fy]) swap(fx, fy);
  fa[fy] = fx;
  siz[fx] += siz[fy];
}

void tarjan(const unsigned int x, const edge* fa)
{
  static unsigned int sta = 0;
  ve[x].dfn = ve[x].low = ++sta;
  for (edge* i = ve[x].head; i; i = i->pre) {
    if (i == fa) continue;
    const unsigned int to = i->to;
    if (!ve[to].dfn) {
      tarjan(to, i->rev);
      ve[x].low = min(ve[x].low, ve[to].low);
      if (ve[x].dfn < ve[to].low) i->isBridge = true;
    }
    else
      ve[x].low = min(ve[x].low, ve[to].dfn);
  }
}
inline void CreateGraph(const unsigned int n)
{
  Init(n);
  for (edge* i = ed; i < cur; ++i)
    if (!(i->isBridge || i->rev->isBridge)) Merge(i->from, i->to);
  for (edge* i = ed; i < cur; ++i) {
    if (!(i->isBridge)) continue;
    const unsigned int ff = GetFather(i->from), ft = GetFather(i->to);
    ++degree[ff];
    ++degree[ft];
  }
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int f, r;
  cin >> f >> r;
  for (unsigned int i = 0; i < r; ++i) {
    unsigned int u, v;
    edge *t, *r;
    cin >> u >> v;
    t = AddEdge(u, v);
    r = AddEdge(v, u);
    t->rev = r;
    r->rev = t;
  }
  tarjan(1, NULL);
  CreateGraph(f);
  unsigned int ans = count(degree + 1, degree + 1 + f, 1);
  cout << (ans == 1 ? 0 : (ans + 1) / 2) << endl;
  return 0;
}