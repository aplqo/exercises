#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <utility>
using namespace std;
const int maxn = 8000, maxm = 200000;
constexpr int maxv = maxn * 2, maxe = (maxv * 2 + maxm);

template <class T>
struct edge {
  inline bool operator==(const edge& r) const
  {
    return this->from == r.from && this->to == r.to;
  }
  inline bool operator<(const edge& r) const
  {
    return this->from != r.from ? this->from < r.from : this->to < r.to;
  }
  T *from, *to;
  edge* pre = nullptr;
};
struct scc {
  inline void Set(const bool val)
  {
    this->val = val;
    this->vis = true;
  }

  edge<scc>* head = nullptr;
  unsigned int degree = 0;
  bool val = false, vis = false;
  scc* rev = nullptr;

  static inline void CreateGraph(const unsigned int n);
  static edge<scc> ed[maxe + 10], *cur;
} sc[maxv + 10], *sen = sc;
edge<scc> scc::ed[maxe + 10], *scc::cur = scc::ed;
struct vertex {
  inline void AddEdge(vertex& to)
  {
    cur->from = this;
    cur->to = &to;
    cur->pre = head;
    head = cur;
    ++cur;
  }

  edge<vertex>* head = nullptr;
  scc* sc = nullptr;
  unsigned int dfn = 0, low = 0;

  static edge<vertex> ed[maxe + 10], *cur;
} ve[maxv + 10][2];
edge<vertex> vertex::ed[maxe + 10], *vertex::cur = vertex::ed;

#ifndef NoRecurse
#define Function_init(x) static stack<vertex*>* stk = new stack<vertex*>;
#define Function_begin() auto i = x->head;
#define Function_call(x) tarjan(x)
#define Function_end()
#define Function_deinit()
#else
struct call {
  call() = default;
  call(vertex* const x) : x(x), i(x->head) {}
  vertex* x;
  edge<vertex>* i;
  bool pc = false;
};
#define Function_init(x)                    \
  stack<vertex*>* stk = new stack<vertex*>; \
  stack<call>* s = new stack<call>;         \
  s->push(x);
#define Function_begin()       \
  while (!s->empty()) {        \
    call& cur = s->top();      \
    edge<vertex>*(&i) = cur.i; \
    vertex* x = cur.x;         \
    if (cur.pc) goto ret;
#define Function_end() \
  s->pop();            \
  en:;                 \
  }
#define Function_call(x) \
  cur.pc = true;         \
  s->push(x);            \
  goto en;               \
  ret:;
#define Function_deinit() \
  delete s;               \
  delete stk;
#endif
void tarjan(vertex* const x)
{
  static unsigned int sta = 0;
  Function_init(x);
  Function_begin()
  {
    x->dfn = x->low = ++sta;
    stk->push(x);
    for (; i; i = i->pre) {
      if (!i->to->dfn) {
        Function_call(i->to);
        x->low = min(x->low, i->to->low);
      }
      else if (!i->to->sc)
        x->low = min(x->low, i->to->dfn);
    }
    if (x->low == x->dfn) {
      while (stk->top() != x) {
        stk->top()->sc = sen;
        stk->pop();
      }
      x->sc = sen;
      stk->pop();
      ++sen;
    }
  }
  Function_end();
  Function_deinit();
}

inline void scc::CreateGraph(const unsigned int n)
{
  for (auto i = vertex::ed; i < vertex::cur; ++i)
    if (i->from->sc != i->to->sc) {
      cur->from = i->to->sc;
      cur->to = i->from->sc;
      ++cur;
    }
  sort(ed, cur);
  cur = unique(ed, cur);
  for (auto i = ed; i < cur; ++i) {
    ++(i->to->degree);
    i->pre = i->from->head;
    i->from->head = i;
  }
  for (unsigned int i = 1; i <= n * 2; ++i) {
    ve[i][0].sc->rev = ve[i][1].sc;
    ve[i][1].sc->rev = ve[i][0].sc;
  }
}
inline void topological()
{
  for (scc* i = sc; i < sen; ++i) {
    if (!i->vis) {
      i->Set(true);
      i->rev->Set(false);
    }
    if (!i->val) {
      for (auto j = i->head; j; j = j->pre)
        j->to->Set(false);
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
    unsigned int a, b;
    cin >> a >> b;
    ve[a][1].AddEdge(ve[b][0]);
    ve[b][1].AddEdge(ve[a][0]);
  }
  for (unsigned int i = 1; i <= n; ++i) {
    ve[i * 2 - 1][1].AddEdge(ve[i * 2][0]);
    ve[i * 2][1].AddEdge(ve[i * 2 - 1][0]);
    ve[i * 2 - 1][0].AddEdge(ve[i * 2][1]);
    ve[i * 2][0].AddEdge(ve[i * 2 - 1][1]);
  }
  for (unsigned int i = 1; i <= n * 2; ++i) {
    if (!ve[i][0].dfn) tarjan(&ve[i][0]);
    if (!ve[i][1].dfn) tarjan(&ve[i][1]);
  }
  if (any_of(ve + 1, ve + 1 + n * 2,
             [](const vertex(&i)[2]) { return i[0].sc == i[1].sc; })) {
    cout << "NIE" << endl;
    return 0;
  }
  scc::CreateGraph(n);
  topological();
  for (unsigned int i = 1; i <= n * 2; ++i)
    if (ve[i][1].sc->val) cout << i << endl;
  return 0;
}