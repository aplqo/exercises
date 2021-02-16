#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxl = 64;

unsigned long long mod;
struct memory {
  memory(const unsigned long long c = 0, const unsigned long long s = 0)
      : cnt(c), sum(s)
  {
  }
  memory operator+(const memory r) const
  {
    return memory((cnt + r.cnt) % mod, (sum + r.sum) % mod);
  }
  unsigned long long cnt = 0, sum = 0;
} mem[maxl + 2][2][2][2];
bool vis[maxl + 2][2][2][2];
unsigned long long n, m, k;

#define b(x, i) (((x) & (1ull << ((i)-1))) ? 1u : 0u)
memory f(const unsigned int l, const bool ln, const bool lm, const bool lk)
{
  if (!l) return memory(1, 0);
  if (vis[l][ln][lm][lk]) return mem[l][ln][lm][lk];
  memory ret;
  const unsigned int mi = ln ? b(n, l) : 1;
  const unsigned int mj = lm ? b(m, l) : 1;
  for (unsigned int i = 0; i <= mi; ++i)
    for (unsigned int j = 0; j <= mj; ++j) {
      if (lk && b(k, l) > (i ^ j)) continue;
      auto t = f(l - 1, ln && b(n, l) == i, lm && b(m, l) == j,
                 lk && b(k, l) == (i ^ j));
      t.sum = (t.sum +
               (t.cnt *
                ((static_cast<unsigned long long>(i ^ j) << (l - 1)) % mod)) %
                   mod) %
              mod;
      ret = ret + t;
    }
  vis[l][ln][lm][lk] = true;
  return mem[l][ln][lm][lk] = ret;
}

int main()
{
  unsigned int t;
  cin >> t;
  static const auto d = [](const unsigned long long v) {
    return v ? log2(v) + 1 : 1;
  };
  for (unsigned int i = 0; i < t; ++i) {
    cin >> n >> m >> k >> mod;
    const auto t = f(max({d(--n), d(--m), d(k)}), true, true, true);
    k %= mod;
    cout << (t.sum + mod - (t.cnt * k) % mod) % mod << endl;
    memset(vis, 0, sizeof(vis));
  }
  return 0;
}