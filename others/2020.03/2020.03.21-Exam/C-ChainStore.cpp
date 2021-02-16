#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iostream>
using namespace std;
const int maxn = 100000;

struct store {
  unsigned long long a, val;
} st[maxn + 10], *en = st;

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned long long n, s, ans = 0;
  cin >> n >> s;
  for (unsigned int i = 0; i < n; ++i) {
    cin >> en->a >> en->val;
    if (en->val >= en->a && en->a <= s) {
      cout << "-1" << endl;
      return 0;
    }
    if (en->a <= s) {
      en->val = en->a - en->val;
      ++en;
    }
  }
  sort(st, en, [](const store a, const store b) -> bool {
    return a.val != b.val ? a.val < b.val : a.a < b.a;
  });
  for (store* i = st; i < en && s; ++i) {
    if (i->a <= s) {
      unsigned long long t = (s - i->a) / i->val;
      ans += t;
      s -= t * i->val;
      if (s >= i->a) {
        s -= i->val;
        ++ans;
      }
    }
  }
  cout << ans << endl;
  return 0;
}