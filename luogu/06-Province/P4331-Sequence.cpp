#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <iterator>
#include <numeric>
#include <utility>
using namespace std;
const unsigned int maxn = 1000000;

struct node {
  int val = INT_MIN;
  unsigned int dist = 0;
  node *lc = nullptr, *rc = nullptr;
} nod[maxn + 1], *cur = nod;
inline const node* maybe(node* x)
{
  static const node nil;
  return x ? x : &nil;
}
class Heap {
 public:
  node* pop()
  {
    node* t = top;
    top = merge(top->lc, top->rc);
    --siz;
    t->lc = t->rc = nullptr;
    t->dist = 1;
    return t;
  }
  void push(node* x)
  {
    ++siz;
    top = merge(top, x);
  }
  void merge(Heap& other)
  {
    top = merge(top, other.top);
    siz += other.siz;
    other.top = nullptr;
    other.siz = 0;
  }

  node* top = nullptr;
  unsigned int siz = 0;

 private:
  static node* merge(node* x, node* y)
  {
    if (!x || !y) return x ? x : y;
    if (x->val < y->val) swap(x, y);
    x->rc = merge(x->rc, y);
    if (maybe(x->lc)->dist < x->rc->dist) swap(x->lc, x->rc);
    x->dist = maybe(x->rc)->dist + 1;
    return x;
  }
};
class Seq {
 public:
  void init(const int x)
  {
    cur->val = x;
    cur->dist = 1;
    hp.push(cur++);
    cnt = 1;
    top = make_pair(x, x);
  }
  void merge(Seq& other)
  {
    hp.merge(other.hp);
    cnt += other.cnt;
    fix();
  }
  pair<int, int> top;

 private:
  Heap hp;
  unsigned int cnt = 0;
  void update()
  {
    const int tp = hp.top->val;
    if (cnt & 0x01)
      top = make_pair(tp, tp);
    else
      top = make_pair(sec(), tp);
  }
  void fix()
  {
    const unsigned int ex = (cnt + 1) / 2 + (cnt & 0x01 ? 0 : 1);
    while (hp.siz > ex)
      hp.pop();
    update();
  }
  int sec()
  {
    node* t = hp.pop();
    int ret = hp.top->val;
    hp.push(t);
    return ret;
  }
} h[maxn + 10];
unsigned int fa[maxn + 10];
int a[maxn + 10], b[maxn + 10];

unsigned int find(const unsigned int x)
{
  return fa[x] == x ? x : fa[x] = find(fa[x]);
}
void merge(const unsigned int x, const unsigned int y)
{
  h[x].merge(h[y]);
  fa[y] = x;
}

void solve(const unsigned int n)
{
  static unsigned int stk[maxn + 10], *top = stk;
  *(top++) = 1;
  for (unsigned int i = 2; i <= n; ++i) {
    *(top++) = i;
    while (top - stk > 1 &&
           h[*(top - 1)].top.second < h[*(top - 2)].top.first) {
      --top;
      merge(*(top - 1), *top);
    }
    if (top - stk > 1) {
      Seq &c = h[*(top - 1)], &l = h[*(top - 2)];
      c.top.first = max(c.top.first, l.top.first);
    }
  }
}
long long getAns(const unsigned int n)
{
  long long ret = 0;
  for (unsigned int i = 1; i <= n; ++i) {
    const int t = h[find(i)].top.first;
    ret += abs(t - a[i]);
    b[i] = t + i;
  }
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
    cin >> a[i];
    a[i] -= i;
    h[i].init(a[i]);
  }
  iota(fa, fa + 1 + n, 0);
  solve(n);
  cout << getAns(n) << endl;
  copy(b + 1, b + 1 + n, ostream_iterator<int>(cout, " "));
  cout << endl;
  return 0;
}