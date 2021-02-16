#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <limits>
using namespace std;
using num_t = unsigned int;
constexpr num_t inf = numeric_limits<num_t>::max();
const int maxn = 3000, maxr = 8000;

struct vertex;
struct edge {
  vertex *from, *to;
  edge* pre = nullptr;
} ed[maxr + 10], *cur = ed;
struct scc {
  unsigned int degree = 0;
  bool vis = false;
  num_t cost = inf;
} sc[maxn + 10], *sen = sc;
struct vertex {
  edge* head = nullptr;
  scc* s = nullptr;
  bool vis = false;
  num_t cost = inf;
  unsigned int dfn = 0, low = 0;

  inline void AddEdge(const unsigned int to);
} ve[maxn + 10];
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
  for (edge* i = x->head; i; i = i->pre) {
    vertex* const to = i->to;
    if (!to->dfn) {
      tarjan(to);
      x->low = min(x->low, to->low);
    }
    else if (!to->s)
      x->low = min(x->low, to->dfn);
  }
  if (x->low == x->dfn) {
    while (stk[top + 1] != x) {
      sen->vis |= stk[top]->vis;
      stk[top]->s = sen;
      sen->cost = min(sen->cost, stk[top]->cost);
      --top;
    }
    ++sen;
  }
}
inline void solve(const unsigned int n)
{
  for (edge* i = ed; i < cur; ++i) {
    if (i->from->s != i->to->s) {
      i->to->s->vis |= i->from->s->vis;
      ++(i->to->s->degree);
    }
  }
  num_t ans = 0;
  bool fail = false;
  for (scc* i = sc; i < sen; ++i) {
    if (i->degree) continue;
    if (!i->vis) {
      fail = true;
      break;
    }
    ans += i->cost;
  }
  if (fail) {
    unsigned int t =
        find_if(ve + 1, ve + 1 + n, [](const vertex& i) { return !i.s->vis; }) -
        ve;
    cout << "NO" << endl << t << endl;
  }
  else {
    cout << "YES" << endl << ans << endl;
  }
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, p, r;
  cin >> n >> p;
  for (unsigned int i = 0; i < p; ++i) {
    unsigned int id;
    cin >> id;
    ve[id].vis = true;
    cin >> ve[id].cost;
  }
  cin >> r;
  for (unsigned int i = 0; i < r; ++i) {
    unsigned int a, b;
    cin >> a >> b;
    ve[a].AddEdge(b);
  }

  for (vertex* i = ve + 1; i < ve + 1 + n; ++i)
    if (!i->dfn) tarjan(i);

  solve(n);
  return 0;
}