#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
const int maxN = 50;
constexpr unsigned int mod = 998244353;

struct Number {
  inline void operator+=(const Number r)
  {
    val += r.val;
    if (val >= mod) val -= mod;
  }
  inline friend Number operator*(const Number l, const Number r)
  {
    return Number{static_cast<unsigned int>(
        (static_cast<unsigned long long>(l.val) * r.val) % mod)};
  }
  inline void operator*=(const Number r)
  {
    val = (static_cast<unsigned long long>(val) * r.val) % mod;
  }

  unsigned int val;
};

Number quickPow(Number a, unsigned int e)
{
  Number ret{1};
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= a;
    a *= a;
  }
  return ret;
}
inline Number inverse(const Number a) { return quickPow(a, mod - 2); }

struct Range {
  int l, r;

  inline unsigned int length() const;
  inline bool contains(const int l, const int r) const
  {
    return this->l <= l && r <= this->r;
  }
} rng[maxN + 10];
int orig[maxN * 2 + 10];
Number inv[maxN + 10];

inline unsigned int Range::length() const { return orig[r] - orig[l]; }
static void initModularInv(const unsigned int n)
{
  inv[0].val = inv[1].val = 1;
  for (unsigned int i = 2; i <= n; ++i)
    inv[i].val = mod - (inv[mod % i] * Number{mod / i}).val;
}
static int discretize(const int n)
{
  int *ptr = orig;
  for (const Range *i = rng + 1; i < rng + n + 1; ++i) {
    *(ptr++) = i->l;
    *(ptr++) = i->r;
  }
  std::sort(orig, ptr);
  ptr = std::unique(orig, ptr);
  for (Range *i = rng + 1; i < rng + n + 1; ++i) {
    i->l = std::lower_bound(orig, ptr, i->l) - orig;
    i->r = std::lower_bound(orig, ptr, i->r) - orig;
  }
  return ptr - orig;
}
Number dp(const int n, const int m)
{
  static Number f[maxN + 10];
  f[0].val = 1;
  for (int j = m - 2; j >= 0; --j) {
    const unsigned int length = orig[j + 1] - orig[j];
    for (int i = n; i; --i)
      if (rng[i].contains(j, j + 1)) {
        Number binom{1};
        for (int k = 1; k <= i && rng[i + 1 - k].contains(j, j + 1); ++k) {
          binom *= Number{length + k - 1} * inv[k];
          f[i] += binom * f[i - k];
        }
      }
  }
  return f[n];
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n;
  std::cin >> n;
  for (Range *i = rng + 1; i < rng + n + 1; ++i) {
    std::cin >> i->l >> i->r;
    ++(i->r);
  }
  initModularInv(n * 2);
  const int m = discretize(n);
  const Number total = std::accumulate(
      rng + 1, rng + n + 1, Number{1},
      [](Number v, const Range &r) { return v * Number{r.length()}; });
  std::cout << (dp(n, m) * inverse(total)).val << "\n";
  return 0;
}
