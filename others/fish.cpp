#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const int maxn = 100;

struct pool {
  unsigned long init, dif;
  mutable unsigned long val = 0;
} p[maxn + 10];
class heap {
 public:
  void make(const pool* beg, const pool* end)
  {
    transform(beg, end, dat, [](const pool& i) -> const pool* {
      i.val = i.init;
      return &i;
    });
    this->num = distance(beg, end);
    make_heap(dat, dat + num, cmp);
  }
  const pool* top() { return dat[0]; }
  bool empty() { return !num; }
  void push(const pool* i)
  {
    dat[num] = i;
    ++num;
    push_heap(dat, dat + num, cmp);
  }
  void pop()
  {
    pop_heap(dat, dat + num, cmp);
    --num;
  }

 private:
  bool (*cmp)(const pool*, const pool*) =
      [](const pool* a, const pool* b) -> bool { return a->val < b->val; };
  const pool* dat[maxn + 10];
  unsigned int num = 0;
} h;
unsigned long dis[maxn + 10], ans[maxn + 10];

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, t;
  cin >> n;
  for (pool* i = p; i < p + n; ++i)
    cin >> i->init;
  for (pool* i = p; i < p + n; ++i)
    cin >> i->dif;
  for (unsigned long* i = dis + 1; i < dis + n; ++i)
    cin >> *i;
  cin >> t;
  for (unsigned int i = 0; i < n; ++i) {
    static unsigned long d = 0;
    d += dis[i];
    h.make(p, p + i + 1);
    if (t < d) break;
    for (unsigned long ti = t - d; ti && !h.empty(); --ti) {
      const pool* cur = h.top();
      h.pop();
      ans[i] += cur->val;
      if (cur->val > cur->dif) {
        cur->val -= cur->dif;
        h.push(cur);
      }
    }
  }
  cout << *max_element(ans, ans + n) << endl;
  return 0;
}
