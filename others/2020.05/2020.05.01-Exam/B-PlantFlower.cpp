/* Luogu team T132120: Plant flower */
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <functional>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
using num_t = long long;
const int maxn = 200000;

struct position {
  void remove()
  {
    pre->nxt = nxt;
    nxt->pre = pre;
  }
  position *pre = nullptr, *nxt = nullptr;
  unsigned int sta = 0;
  num_t val;
} p[maxn + 10];
class heap {
 public:
  void init(position* l, position* r)
  {
    for (position* i = l; i < r; ++i)
      dat.push(make_pair(i, 0));
  }
  position* top()
  {
    clean();
    return dat.top().first;
  }
  void push(position* i)
  {
    clean();
    dat.push(make_pair(i, i->sta));
  }
  void pop()
  {
    clean();
    dat.pop();
    clean();
  }
  void remove(position* i)
  {
    clean();
    del.push(make_pair(i, i->sta));
    clean();
  }

 private:
  void clean()
  {
    while (!dat.empty() && !del.empty() && dat.top() == del.top()) {
      dat.pop();
      del.pop();
    }
  }
  using datType = pair<position*, unsigned int>;
  struct cmp {
    bool operator()(datType l, datType r)
    {
      return l.first->val != r.first->val ? l.first->val < r.first->val
                                          : l.first < r.first;
    }
  };
  using pq = priority_queue<datType, vector<datType>, cmp>;
  pq dat, del;
} h;

void init(const unsigned int n)
{
  for (position* i = p; i < p + n; ++i) {
    i->pre = i - 1;
    i->nxt = i + 1;
  }
  p[0].pre = p + n - 1;
  p[n - 1].nxt = p;
  h.init(p, p + n);
}
num_t solve(const unsigned int m)
{
  num_t ret = 0;
  for (unsigned int i = 0; i < m; ++i) {
    position* const t = h.top();
    h.pop();
    ret += t->val;
    t->val = t->pre->val + t->nxt->val - t->val;
    ++(t->sta);
    h.push(t);
    h.remove(t->pre);
    h.remove(t->nxt);
    t->nxt->remove();
    t->pre->remove();
  }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int i = 0; i < n; ++i)
    cin >> p[i].val;
  if (m * 2 > n) {
    cout << "Error!" << endl;
    return 0;
  }
  init(n);
  cout << solve(m) << endl;
  return 0;
}