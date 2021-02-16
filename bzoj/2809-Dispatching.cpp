#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
const unsigned int maxn = 100000;

struct Edge {
  unsigned int to;
  const Edge* pre;
} ed[maxn + 1];
const Edge* head[maxn + 1];
class Heap {
 public:
  void init(const unsigned long long x);
  void merge(Heap& other);
  void pop();

  unsigned long long sum = 0;
  unsigned int cnt = 0;

 private:
  struct node {
    void pushup()
    {
      if (!lc || lc->dist < rc->dist) swap(lc, rc);
      dist = (rc ? rc->dist : 0) + 1;
    }

    unsigned long long val;
    unsigned int dist = 0;
    node *lc = nullptr, *rc = nullptr;
  };
  static node* merge(node* x, node* y);

  node* top = nullptr;
  static node nod[maxn + 1], *cur;
} h[maxn + 1];
Heap::node Heap::nod[maxn + 1], *Heap::cur = Heap::nod;
unsigned long long ans = 0, m;
unsigned long long c[maxn + 1], l[maxn + 1];

void Heap::init(const unsigned long long x)
{
  cur->val = x;
  cur->dist = 1;
  top = cur;
  cnt = 1;
  sum = x;
  ++cur;
}
void Heap::merge(Heap& other)
{
  sum += other.sum;
  cnt += other.cnt;
  top = merge(top, other.top);
  other.top = nullptr;
}
void Heap::pop()
{
  --cnt;
  sum -= top->val;
  top = merge(top->lc, top->rc);
}
Heap::node* Heap::merge(Heap::node* x, Heap::node* y)
{
  if (!x || !y) return x ? x : y;
  if (x->val < y->val) swap(x, y);
  x->rc = merge(x->rc, y);
  x->pushup();
  return x;
}

void addEdge(const unsigned int from, const unsigned int to)
{
  static Edge* cur = ed;
  cur->to = to;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
void dfs(const unsigned int x)
{
  h[x].init(c[x]);
  for (const Edge* i = head[x]; i; i = i->pre) {
    dfs(i->to);
    h[x].merge(h[i->to]);
  }
  while (h[x].sum > m)
    h[x].pop();
  ans = max(ans, h[x].cnt * l[x]);
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n >> m;
  for (unsigned int i = 1; i <= n; ++i) {
    unsigned int b;
    cin >> b >> c[i] >> l[i];
    addEdge(b, i);
  }
  dfs(0);
  cout << ans << endl;
  return 0;
}