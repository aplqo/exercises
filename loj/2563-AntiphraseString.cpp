#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <unordered_map>
const int maxln = 64;
using Map = std::unordered_map<unsigned long long, int>;

Map factors;
Map::const_iterator fEnd;
unsigned long long mod, n, k;

namespace pollardRho {
std::mt19937 rnd;

unsigned long long multiply(unsigned long long a, unsigned long long b,
                            const unsigned long long m)
{
  unsigned long long ret = 0;
  for (; b; b >>= 1) {
    if (b & 0x01) ret = (ret + a) % m;
    a = (a + a) % m;
  }
  return ret;
}
unsigned long long quickPow(unsigned long long a, unsigned long long e,
                            const unsigned long long m)
{
  unsigned long long ret = 1;
  for (; e; e >>= 1) {
    if (e & 0x01) ret = multiply(ret, a, m);
    a = multiply(a, a, m);
  }
  return ret;
}

static void init() { rnd.seed((std::random_device())()); }
inline std::pair<unsigned int, unsigned long long> lowbit(unsigned long long x)
{
  unsigned int ret = 0;
  while (!(x & 0x01)) {
    ++ret;
    x >>= 1;
  }
  return std::make_pair(ret, x);
}
bool millerRabin(const unsigned long long n)
{
  static constexpr unsigned int testA[] = {2, 3, 5, 7, 11, 13, 17, 19, 23};
  if (n < 3) return n == 2;
  const auto [r, d] = lowbit(n - 1);
  for (const auto a : testA) {
    if (a > n - 2) break;
    unsigned long long x = quickPow(a, d, n);
    if (x == 1 || x == n - 1) continue;
    for (unsigned int i = 1; i < r; ++i) {
      x = multiply(x, x, n);
      if (x == n - 1) goto End;
    }
    return false;
  End:;
  }
  return true;
}
unsigned long long pollardRho(const unsigned long long n)
{
  std::uniform_int_distribution<unsigned long long> dis(1, n - 1);
  static const auto f = [](unsigned long long x, unsigned long long c,
                           const unsigned long long n) {
    return (multiply(x, x, n) + c) % n;
  };
  const unsigned long long c = dis(rnd);
  unsigned long long x = dis(rnd), y = x;
  for (unsigned int i = 1;; i <<= 1) {
    unsigned long long acc = 1;
    for (unsigned int j = 0; j < i; ++j) {
      y = f(y, c, n);
      acc = multiply(acc, x >= y ? x - y : y - x, n);
      if ((j & 0x7fu) == 0) {
        const unsigned long long g = std::gcd(n, acc);
        if (g > 1) return g;
      }
    }
    const unsigned long long g = std::gcd(n, acc);
    if (g > 1) return g;
    x = y;
  }
  return 1;
}
void factor(const unsigned long long n)
{
  if (millerRabin(n)) {
    ++factors[n];
    return;
  }
  unsigned long long g;
  do {
    g = pollardRho(n);
  } while (g <= 1 || g == n);
  factor(g);
  factor(n / g);
}
}  // namespace pollardRho

struct Number {
  inline friend Number operator+(const Number l, const Number r)
  {
    unsigned long long ret{l.val + r.val};
    return Number{ret >= mod ? ret - mod : ret};
  }
  inline void operator+=(const Number r)
  {
    val += r.val;
    if (val >= mod) val -= mod;
  }
  inline friend Number operator*(const Number l, const Number r)
  {
    return Number{(l.val * r.val) % mod};
  }
  inline void operator*=(const Number r) { val = (val * r.val) % mod; }

  unsigned long long val;
};

Number quickPow(Number a, unsigned long long e)
{
  Number ret{1};
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= a;
    a *= a;
  }
  return ret;
}
inline Number g(const unsigned long long x)
{
  return quickPow(Number{k % mod}, (x + 1) >> 1);
}
inline unsigned long long h(const unsigned long long x)
{
  return x & 0x01 ? x : x >> 1;
}
/*!
 * @param pacc (1-p_i)
 */
Number dfs(const Map::const_iterator it, unsigned long long factor,
           const Number pacc)
{
  if (it == fEnd) return Number{h(factor) % mod} * g(factor) * pacc;
  const auto next = std::next(it);
  const Number npacc = pacc * Number{mod + 1 - it->first % mod};
  Number ret = it->first == 2 ? Number{0} : dfs(next, factor, npacc);
  for (int i = 1; i < it->second; ++i) {
    factor *= it->first;
    ret += dfs(next, factor, npacc);
  }
  factor *= it->first;
  return ret + dfs(next, factor, pacc);
}

unsigned long long solve(const unsigned long long n)
{
  if (n == 1) return 1;
  factors.clear();
  factors.reserve(maxln * 20);
  pollardRho::factor(n);
  fEnd = factors.cend();
  return dfs(factors.cbegin(), 1, Number{1}).val;
}

int main()
{
  std::ios::sync_with_stdio(false);
  unsigned int t;
  std::cin >> t;
  pollardRho::init();
  for (int i = t; i; --i) {
    std::cin >> n >> k >> mod;
    std::cout << solve(n) << "\n";
  }
  return 0;
}