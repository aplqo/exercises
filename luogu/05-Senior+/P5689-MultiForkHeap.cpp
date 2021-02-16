#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <numeric>
using namespace std;
using num_t = long long;
constexpr num_t mod = 1e9 + 7;
const int maxn = 3e5;

unsigned int fa[maxn + 10], siz[maxn + 10];
num_t f[maxn + 10], fact[maxn + 10], inv[maxn + 10];

void exgcd(const num_t a, const num_t b, num_t& x, num_t& y)
{
  if (b == 0) {
    x = 1;
    y = 0;
    return;
  }
  exgcd(b, a % b, x, y);
  num_t t = x;
  x = y;
  y = t - (a / b) * y;
}
inline void init(const unsigned int n)
{
  // init find-union set
  iota(fa, fa + n + 1, 0);
  fill(siz, siz + n + 1, 1);
  // init dp
  fill(f, f + n + 1, 1);

  // init inverse
  fact[1] = 1;
  inv[1] = 1;
  for (unsigned int i = 2; i <= n; ++i) {
    num_t b;
    fact[i] = (fact[i - 1] * i) % mod;
    exgcd(mod, fact[i], b, inv[i]);
    inv[i] = (inv[i] % mod + mod) % mod;
  }
}
inline num_t C(const unsigned int n, const unsigned int m)
{
  if (n == m) return 1;
  num_t ret = fact[n];
  ret = (ret * inv[m]) % mod;
  ret = (ret * inv[n - m]) % mod;
  return ret;
}
unsigned int GetFather(const unsigned int x)
{
  return fa[x] == x ? x : fa[x] = GetFather(fa[x]);
}
inline void Merge(const unsigned int x, const unsigned int y)
{
  const unsigned int fx = GetFather(x), fy = GetFather(y);
  fa[fx] = fy;
  siz[fy] += siz[fx];

  f[fy] = (f[fy] * f[fx]) % mod;
  f[fy] = (f[fy] * C(siz[fy] - 1, siz[fx])) % mod;
}

int main()
{
  ios_base::sync_with_stdio(false);
  unsigned int n, q;
  cin >> n >> q;
  num_t ans = 0;
  init(n);

  for (unsigned int i = 0; i < q; ++i) {
    unsigned int op, x;
    cin >> op >> x;
    x = (x + ans) % n;
    switch (op) {
      case 1:
        unsigned int y;
        cin >> y;
        y = (y + ans) % n;
        Merge(x, y);
        break;
      case 2:
        x = GetFather(x);
        cout << f[x] << endl;
        ans = f[x];
        break;
    }
  }
  return 0;
}