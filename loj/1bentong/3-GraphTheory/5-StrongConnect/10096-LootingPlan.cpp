#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;
const int maxn = 5e5;

template <class T>
struct edge {
  T *from, *to;
  edge* pre = nullptr;
  inline bool operator==(const edge& r) const
  {
    return this->from != r.from ? false : this->to == r.to;
  }
};
struct scc {
  unsigned int val = 0, sum = 0;
  unsigned int degree = 0;
  bool isEnd = false, vis = false;
  edge<scc>* head = nullptr;

  static inline void CreateGraph();

  static edge<scc> ed[maxn + 10], *cur;
} sc[maxn + 10], *sen = sc;
edge<scc> scc::ed[maxn + 10], *scc::cur = scc::ed;
struct vertex {
  unsigned int val = 0;
  unsigned int dfn = 0, low = 0;
  bool isEnd = false;
  scc* sc = nullptr;
  edge<vertex>* head = nullptr;
  inline void AddEdge(const unsigned int to);

  static edge<vertex> ed[maxn + 10], *cur;
} ve[maxn + 10];
scc* seq[maxn + 10];
edge<vertex> vertex::ed[maxn + 10], *vertex::cur = vertex::ed;
inline void vertex::AddEdge(const unsigned int to)
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
  x->dfn = x->low = ++sta;
  stk[++top] = x;
  for (auto i = x->head; i; i = i->pre) {
    vertex* const to = i->to;
    if (!to->dfn) {
      tarjan(to);
      x->low = min(x->low, to->low);
    }
    else if (!to->sc)
      x->low = min(x->low, to->dfn);
  }
  if (x->low == x->dfn) {
    while (stk[top + 1] != x) {
      stk[top]->sc = sen;
      sen->val += stk[top]->val;
      sen->isEnd |= stk[top]->isEnd;
      --top;
    }
    ++sen;
  }
}
inline void scc::CreateGraph()
{
  for (edge<vertex>* i = vertex::ed; i < vertex::cur; ++i)
    if (i->from->sc != i->to->sc) {
      cur->from = i->from->sc;
      cur->to = i->to->sc;
      ++cur;
    }
  sort(ed, cur, [](edge<scc>& a, edge<scc> b) -> bool {
    return a.from != b.from ? a.from < b.from : a.to < b.to;
  });
  cur = unique(ed, cur);
  for (auto i = ed; i < cur; ++i) {
    i->pre = i->from->head;
    i->from->head = i;
    ++(i->to->degree);
  }
}
inline void topological()
{
  queue<scc*> q;
  for (scc* i = sc; i < sen; ++i)
    if (!i->degree) q.push(i);
  unsigned int pos = 0;
  while (!q.empty()) {
    scc* const cur = q.front();
    q.pop();
    seq[pos++] = cur;
    for (auto i = cur->head; i; i = i->pre) {
      --(i->to->degree);
      if (!i->to->degree) q.push(i->to);
    }
  }
}
inline unsigned int dp(const unsigned int s)
{
  unsigned int p = find(seq, seq + (sen - sc), ve[s].sc) - seq;
  ve[s].sc->vis = true;
  for (unsigned int i = p; i < sen - sc; ++i) {
    if (!seq[i]->vis) continue;
    for (auto j = seq[i]->head; j; j = j->pre) {
      j->to->vis = true;
      j->to->sum = max(j->to->sum, seq[i]->sum + seq[i]->val);
    }
  }
  scc* const ans =
      *max_element(seq + p, seq + (sen - sc), [](const scc* a, const scc* b) {
        return (a->isEnd && a->vis ? a->sum + a->val : 0) <
               (b->isEnd && b->vis ? b->sum + b->val : 0);
      });
  return ans->sum + ans->val;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m, s, p;
  cin >> n >> m;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int b, e;
    cin >> b >> e;
    ve[b].AddEdge(e);
  }
  for (vertex* i = ve + 1; i < ve + 1 + n; ++i)
    cin >> i->val;
  cin >> s >> p;
  for (unsigned int i = 0; i < p; ++i) {
    unsigned int t;
    cin >> t;
    ve[t].isEnd = true;
  }
  for (vertex* i = ve + 1; i < ve + n + 1; ++i)
    if (!i->dfn) tarjan(i);
  scc::CreateGraph();
  topological();
  cout << dp(s) << endl;
  return 0;
}
