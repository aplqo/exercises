#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 5e5;

struct Line {
  unsigned long long a, b;
} l[maxn + 10];
unsigned long long sa[maxn + 10], sb[maxn + 10];

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, q;
  cin >> n;
  for (Line* i = l + 1; i < l + n + 1; ++i)
    cin >> i->a >> i->b;
  sort(l + 1, l + 1 + n, [](const Line& a, const Line& b) {
    return a.a != b.a ? a.a > b.a : a.b > b.b;
  });
  for (unsigned int i = 1; i <= n; ++i) {
    sa[i] = sa[i - 1] + l[i].a;
    sb[i] = sb[i - 1] + l[i].b;
  }
  cin >> q;
  for (unsigned int i = 0; i < q; ++i) {
    unsigned long long k;
    unsigned int m;
    cin >> k >> m;
    cout << sa[m] * k + sb[m] << "\n";
  }
  return 0;
}