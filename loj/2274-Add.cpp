#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;
const unsigned int maxn = 2e5, maxA = 1e8, maxa = 100;

struct rng {
  unsigned int l, r;
} r[maxn];
unsigned int A[maxn], delt[maxn];
unsigned int n, m, k, a;

bool test(const unsigned int val)
{
  priority_queue<unsigned int> q;
  rng* p = r;
  unsigned int acc = 0, cnt = k;
  fill(delt, delt + n, 0);
  for (unsigned int i = 0; i < n; ++i) {
    for (; p->l == i && p < r + m; ++p)
      q.push(p->r);
    while (A[i] + acc < val && cnt && !q.empty() && q.top() >= i) {
      const unsigned int r = q.top();
      q.pop();
      delt[r] += a;
      acc += a;
      --cnt;
    }
    if (A[i] + acc < val) return false;
    acc -= delt[i];
  }
  return true;
}
unsigned int solve()
{
  sort(r, r + m,
       [](rng a, rng b) { return a.l != b.l ? a.l < b.l : a.r < b.r; });
  unsigned int l = *min_element(A, A + n);
  unsigned int r = k * a + l;
  while (l <= r) {
    const unsigned int m = (l + r) >> 1;
    if (test(m))
      l = m + 1;
    else
      r = m - 1;
  }
  return r;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int t;
  cin >> t;
  for (unsigned int i = 0; i < t; ++i) {
    cin >> n >> m >> k >> a;
    for (unsigned int* i = A; i < A + n; ++i)
      cin >> *i;
    for (rng* i = r; i < r + m; ++i) {
      cin >> i->l >> i->r;
      --(i->l);
      --(i->r);
    }
    cout << solve() << endl;
  }
  return 0;
}