#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
template <template <typename> class T>
using Heap =
    priority_queue<unsigned int, vector<unsigned int>, T<unsigned int>>;

Heap<less> lq;
Heap<greater> gq;

inline void push(const unsigned int v, const unsigned int k)
{
  gq.push(v);
  if (gq.size() > k) {
    lq.push(gq.top());
    gq.pop();
  }
}
inline unsigned int get()
{
  const unsigned int ret = gq.top();
  gq.pop();
  if (!lq.empty()) {
    gq.push(lq.top());
    lq.pop();
  }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int k, n, p, ans = 0;
  cin >> n >> k >> p;
  for (unsigned int i = 0; i < n; ++i) {
    unsigned int op;
    cin >> op;
    if (op == 1) {
      unsigned int x;
      cin >> x;
      push(p ? x ^ ans : x, k);
    }
    else
      cout << (ans = get()) << "\n";
  }
  return 0;
}