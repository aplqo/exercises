#ifdef APDEBUG
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdint>
#include <iostream>
using namespace std;
typedef unsigned long long hash_t;
const hash_t base = 131;
const int maxl = 200, maxn = 30000;

hash_t hs[maxn + 10][maxl + 10], po[maxl + 10];
hash_t v[maxn + 10];
unsigned int l, s, n;
unsigned int ans;

void getPow(unsigned int v)
{
  po[0] = 1;
  for (int i = 1; i <= v; ++i)
    po[i] = po[i - 1] * base;
}
void solve(unsigned int pos)
{
  transform(hs, hs + n, v, [&pos](hash_t(&orig)[maxl + 10]) -> hash_t {
    return orig[l] + (orig[pos - 1] - orig[pos]) * po[l - pos];
  });
  sort(v, v + n);
  unsigned int lst = 0;
  hash_t lh = v[0];
  for (int i = 1; i <= n; ++i)
    if (v[i] != lh) {
      ans += (i - lst) * (i - lst - 1) / 2;
      lst = i;
      lh = v[i];
    }
}

int main()
{
  ios_base::sync_with_stdio(false);
  cin >> n >> l >> s;
  getPow(l);
  for (unsigned int i = 0; i < n; ++i) {
    for (unsigned int j = 1; j <= l; ++j) {
      char c;
      cin >> c;
      hs[i][j] = hs[i][j - 1] * base + c - '0';
    }
  }
  for (unsigned int i = 1; i <= l; ++i)
    solve(i);
  cout << ans << endl;
  return 0;
}