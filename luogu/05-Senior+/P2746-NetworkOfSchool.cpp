#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;
constexpr int maxn = 100, maxm = maxn * maxn;

template <class T>
struct edge {
  T *from, *to;
  edge* pre = nullptr;
};
struct scc {
  edge<scc>* head = nullptr;
  unsigned int degree[2] = {};
  bool vis = false;
  static edge<scc> ed[maxm + 10], *cur;
  static inline void CreateGraph();
} sc[maxn + 10], *en = sc;
edge<scc> scc::ed[maxm + 10];
edge<scc>* scc::cur = scc::ed;
struct vertex {
  edge<vertex>* head = nullptr;
  scc* s = nullptr;
  unsigned int dfn = 0, low = 0;
  static edge<vertex> ed[maxm + 10], *cur;
  void AddEdge(const unsigned int to);
} ve[maxn + 10];
edge<vertex> vertex::ed[maxm + 10];
edge<vertex>* vertex::cur = vertex::ed;

void vertex::AddEdge(const unsigned int to)
{
  cur->from = this;
  cur->to = ve + to;
  cur->pre = head;
  head = cur;
  ++cur;
}
void tarjan(vertex* const x)
{
  static unsigned int sta = 0, top = 0;
  static vertex* stk[maxn + 10];
  stk[++top] = x;
  x->dfn = x->low = ++sta;
  for (auto* i = x->head; i; i = i->pre) {
    vertex* const to = i->to;
    if (!to->dfn) {
      tarjan(to);
      x->low = min(x->low, to->low);
    }
    else if (!to->s)
      x->low = min(x->low, to->dfn);
  }
  if (x->dfn == x->low) {
    x->s = en;
    while (stk[top] != x) {
      stk[top]->s = en;
      --top;
    }
    --top;
    ++en;
  }
}
inline void scc::CreateGraph()
{
  for (edge<vertex>* i = vertex::ed; i < vertex::cur; ++i) {
    if (i->from->s != i->to->s) {
      edge<scc>* c = cur;
      c->from = i->from->s;
      c->to = i->to->s;
      ++cur;
    }
  }
  sort(ed, cur, [](const edge<scc>& a, const edge<scc>& b) -> bool {
    return a.from != b.from ? a.from < b.from : a.to < b.to;
  });
  cur = unique(ed, cur, [](const edge<scc>& a, const edge<scc>& b) {
    return a.from != b.from ? false : a.to == b.to;
  });
  for (edge<scc>* i = ed; i < cur; ++i) {
    ++(i->to->degree[0]);
    ++(i->from->degree[1]);
    i->pre = i->from->head;
    i->from->head = i;
  }
}
unsigned int dfs(scc* const x)
{
  x->vis = true;
  if (!x->head) return 1;
  unsigned int ret = 0;
  for (auto i = x->head; i; i = i->pre)
    if (!i->to->vis) ret += dfs(i->to);
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  for (unsigned int i = 1; i <= n; ++i) {
    unsigned int t = 0;
    cin >> t;
    while (t) {
      ve[i].AddEdge(t);
      cin >> t;
    }
  }
  for (vertex* i = ve + 1; i < ve + 1 + n; ++i)
    if (!i->dfn) tarjan(i);
  scc::CreateGraph();

  unsigned int ansa =
      count_if(sc, en, [](const scc& a) { return !a.degree[0]; });
  unsigned int ansb =
      count_if(sc, en, [](const scc& a) { return !a.degree[1]; });
  ansb = max(ansb, ansa);
  if (en - sc == 1) ansb = 0;
  cout << ansa << endl << ansb << endl;
  return 0;
}
