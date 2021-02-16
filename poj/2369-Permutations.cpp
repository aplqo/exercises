#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const int maxn = 1000;
typedef long long ans_t;

unsigned int to[maxn + 10], ring[maxn + 10];
unsigned int len[maxn + 10];

ans_t gcd(ans_t a, ans_t b)
{
  if (a < b) swap(a, b);
  while (b) {
    const ans_t t = a;
    a = b;
    b = t % b;
  }
  return a;
}
ans_t lcm(const ans_t a, const ans_t b) { return a * b / gcd(a, b); }
void find(unsigned int x, const unsigned int rn)
{
  while (!ring[x]) {
    ring[x] = rn;
    ++len[rn];
    x = to[x];
  }
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, cr = 0;
  ans_t ans = 1;
  cin >> n;
  for (unsigned int* i = to + 1; i < to + n + 1; ++i)
    cin >> *i;
  for (unsigned int i = 1; i <= n; ++i)
    if (!ring[i]) find(i, ++cr);
  for (unsigned int i = 1; i <= cr; ++i)
    ans = lcm(ans, len[i]);
  cout << ans << endl;
  return 0;
}