#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <utility>
using namespace std;
using num_t = int;
const int maxn = 1e5;

struct node {
  num_t val = 0;
  node *pre = nullptr, *nxt = nullptr;
} a[maxn + 10], *cur = a + 1;
using pair_t = pair<num_t, node*>;
class heap {
 public:
  void make(node* beg, node* en)
  {
    for (node* i = beg; i && i < en; i = i->nxt) {
      *(this->en) = make_pair(abs(i->val), i);
      ++(this->en);
    }
    make_heap(dat, this->en, cmp());
  }
  pair_t top() { return dat[0]; }
  bool empty() { return en <= dat; }
  void push(node* p)
  {
    *en = make_pair(abs(p->val), p);
    ++en;
    push_heap(dat, en, cmp());
  }
  void pop()
  {
    pop_heap(dat, en, cmp());
    --en;
  }

 private:
  using cmp = greater<pair_t>;
  pair_t dat[maxn + 10], *en = dat;
} h, gar;

inline pair_t get()
{
  while (!gar.empty() && h.top() == gar.top()) {
    h.pop();
    gar.pop();
  }
  pair_t ret = h.top();
  h.pop();
  return ret;
}
inline void remove(node* p)
{
  if ((!p) || p == a || p == cur) return;
  p->pre->nxt = p->nxt;
  p->nxt->pre = p->pre;
  if ((p->pre == a || p->nxt == cur) && p->val < 0) return;
  gar.push(p);
}

inline void read(unsigned int n)
{
  cin >> cur->val;
  bool lst = cur->val > 0;
  for (unsigned int i = 1; i < n; ++i) {
    num_t t;
    cin >> t;
    if (!t)
      continue;
    else if ((t > 0) ^ lst) {
      ++cur;
      cur->val = t;
      lst = t > 0;
    }
    else
      cur->val += t;
  }
  ++cur;
}
inline void make()
{
  a[0].nxt = a + 1;
  cur->pre = cur - 1;
  for (node* i = a + 1; i < cur; ++i) {
    i->pre = i - 1;
    i->nxt = i + 1;
  }
  if (a[1].val < 0) remove(a + 1);
  if ((cur - 1)->val < 0) remove(cur - 1);
}

int main()
{
  ios_base::sync_with_stdio(false);
  unsigned int n, m;
  cin >> n >> m;
  if (m == 0) {
    cout << "0" << endl;
    return 0;
  }

  read(n);
  make();
  unsigned int cnt =
      count_if(a + 1, cur, [](const node& i) { return i.val > 0; });
  num_t ans = accumulate(a + 1, cur, num_t(0), [](num_t v, const node& i) {
    return i.val > 0 ? v + i.val : v;
  });

  h.make(a->nxt, cur);
  for (; cnt > m; --cnt) {
    pair_t thi = get();
    node* t = thi.second;
    ans -= thi.first;
    t->val += t->pre->val + t->nxt->val;
    remove(t->pre);
    remove(t->nxt);
    if ((t->nxt == cur || t->pre == a) && t->val < 0) continue;
    h.push(t);
  }
  cout << ans << endl;
  return 0;
}