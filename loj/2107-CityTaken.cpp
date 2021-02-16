#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
#include <utility>
using namespace std;
const unsigned int maxn = 300000, maxm = 300000;

class Heap {
 public:
  void push(const unsigned int x, const long long v);
  void multiply(const long long v) { ptr->pushMul(v); }
  void add(const long long delt) { ptr->pushAdd(delt); }
  pair<unsigned int, long long> top() const
  {
    return make_pair(ptr->x, ptr->val);
  }
  void pop()
  {
    ptr->pushdown();
    ptr = merge(ptr->lc, ptr->rc);
  }
  bool empty() const { return !ptr; }
  void merge(Heap& other) { ptr = merge(ptr, other.ptr); }

 private:
  struct node {
    unsigned int x;
    long long val;
    unsigned int dist = 0;
    long long laza = 0, lazm = 1;
    node *lc = nullptr, *rc = nullptr;

    void pushAdd(const long long l);
    void pushMul(const long long l);
    void pushdown();
  };
  node* ptr = nullptr;
  static node* merge(node* x, node* y);
  static node nod[maxn + 1], *cur;
};
Heap::node Heap::nod[maxn + 1], *Heap::cur = Heap::nod;
struct Edge {
  struct City* to;
  const Edge* pre;
} ed[maxn + 1];
struct City {
  long long h, v;
  unsigned int cnt = 0, dep = 0;
  bool typ;
  Heap hp;
  const Edge* head = nullptr;

  void dfs();
  void addEdge(City& to);
} c[maxm + 1];
unsigned int succ[maxn + 1], from[maxn + 1];

void Heap::push(const unsigned int x, const long long v)
{
  cur->x = x;
  cur->val = v;
  cur->dist = 1;
  ptr = merge(ptr, cur++);
}
void Heap::node::pushAdd(const long long l)
{
  val += l;
  laza += l;
}
void Heap::node::pushMul(const long long l)
{
  val *= l;
  laza *= l;
  lazm *= l;
}
void Heap::node::pushdown()
{
  if (!laza && lazm == 1) return;
  if (lc) {
    lc->pushMul(lazm);
    lc->pushAdd(laza);
  }
  if (rc) {
    rc->pushMul(lazm);
    rc->pushAdd(laza);
  }
  laza = 0;
  lazm = 1;
}
Heap::node* Heap::merge(Heap::node* x, Heap::node* y)
{
  if (!x || !y) return x ? x : y;
  x->pushdown();
  y->pushdown();
  if (x->val > y->val) swap(x, y);
  x->rc = merge(x->rc, y);
  if (!x->lc || x->lc->dist < x->rc->dist) swap(x->lc, x->rc);
  x->dist = (x->rc ? x->rc->dist : 0) + 1;
  return x;
}
void City::addEdge(City& to)
{
  static Edge* cur = ed;
  cur->to = &to;
  cur->pre = head;
  head = cur++;
}
void City::dfs()
{
  for (const Edge* i = head; i; i = i->pre) {
    City* const to = i->to;
    to->dep = dep + 1;
    to->dfs();
    hp.merge(to->hp);
  }
  while (!hp.empty() && hp.top().second < h) {
    ++cnt;
    const unsigned int k = hp.top().first;
    succ[k] = c[from[k]].dep - dep;
    hp.pop();
  }
  if (hp.empty()) return;
  if (typ)
    hp.multiply(v);
  else
    hp.add(v);
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int i = 1; i <= n; ++i)
    cin >> c[i].h;
  for (unsigned int i = 2; i <= n; ++i) {
    unsigned int f, a;
    cin >> f >> a >> c[i].v;
    c[f].addEdge(c[i]);
    c[i].typ = a == 1;
  }
  for (unsigned int i = 1; i <= m; ++i) {
    unsigned int ci;
    long long s;
    cin >> s >> ci;
    from[i] = ci;
    c[ci].hp.push(i, s);
  }
  c[0].h = LLONG_MAX;
  c[0].addEdge(c[1]);
  c[0].dfs();
  for (unsigned int i = 1; i <= n; ++i)
    cout << c[i].cnt << endl;
  copy(succ + 1, succ + 1 + m, ostream_iterator<unsigned int>(cout, "\n"));
  return 0;
}