#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <queue>
using namespace std;
constexpr unsigned int primes[32] = {
    0,  2,  3,  5,  7,  11, 13, 17, 19, 23, 29,  31,  37,  41,  43,  47,
    53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127};

struct Node {
  unsigned int ex, nxt, max;
  unsigned long long val;

  inline bool operator<(const Node& r) const { return val < r.val; }
};

unsigned long long kth(const unsigned long long n, const unsigned int k)
{
  priority_queue<Node> q;
  for (unsigned int i = 1; i <= 31; ++i) {
    Node acc{0, i - 1, primes[i], 1};
    while (static_cast<double>(acc.val) * primes[i] <= n) {
      acc.val *= primes[i];
      ++acc.ex;
      q.push(acc);
    }
  }
  for (unsigned int i = k; i > 1; --i) {
    const Node cur = q.top();
    q.pop();
    if (cur.ex == 1) continue;
    for (unsigned int i = 1; i <= cur.nxt; ++i)
      q.push(Node{cur.ex - 1, i, cur.max, cur.val / cur.max * primes[i]});
  }
  return q.top().val;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned long long n;
  unsigned int k;
  cin >> n >> k;
  cout << kth(n, k) << "\n";
  return 0;
}