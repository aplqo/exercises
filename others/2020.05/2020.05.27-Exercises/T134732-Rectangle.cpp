#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;
const int maxn = 100000;
using num = unsigned long long;

struct rect {
  unsigned int a, b, h;
} reca[maxn], recb[maxn];
class heap {
 public:
  void push(const unsigned int v) { val.push(v); }
  void pop()
  {
    val.pop();
    clean();
  }
  unsigned int top() const { return val.top(); }
  void remove(const unsigned int v)
  {
    del.push(v);
    clean();
  }

 private:
  inline void clean()
  {
    while (!val.empty() && !del.empty() && val.top() == del.top()) {
      val.pop();
      del.pop();
    }
  }

  priority_queue<unsigned int> val, del;
};
unsigned int dis[maxn * 2];

unsigned int discretize(const unsigned int n)
{
  sort(dis, dis + n * 2);
  unsigned int* e = unique(dis, dis + n * 2);
  for (rect* i = reca; i < reca + n; ++i) {
    i->a = lower_bound(dis, e, i->a) - dis;
    i->b = lower_bound(dis, e, i->b) - dis;
  }
  return e - dis;
}
num solve(const unsigned int n, const unsigned int mv)
{
  heap h;
  h.push(0);
  num ret = 0;
  rect *pa = reca, *pb = recb;
  for (unsigned int i = 0; i < mv - 1; ++i) {
    for (; pa->a <= i && pa < reca + n; ++pa)
      h.push(pa->h);
    for (; pb->b <= i && pb < recb + n; ++pb)
      h.remove(pb->h);
    ret += num(dis[i + 1] - dis[i]) * h.top();
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
  for (unsigned int i = 0; i < n; ++i) {
    cin >> reca[i].a >> reca[i].b >> reca[i].h;
    dis[2 * i] = reca[i].a;
    dis[2 * i + 1] = reca[i].b;
  }
  const unsigned int mv = discretize(n);
  copy(reca, reca + n, recb);
  sort(reca, reca + n, [](const rect& a, const rect& b) { return a.a < b.a; });
  sort(recb, recb + n, [](const rect& a, const rect& b) { return a.b < b.b; });
  cout << solve(n, mv) << endl;
  return 0;
}