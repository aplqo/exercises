#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <climits>
#include <iostream>
#include <vector>
const int maxn = 233333, maxt = 233;

namespace Divide {
typedef unsigned long long Integer;

inline constexpr unsigned long long highMultiplyAdd(const unsigned long long a,
                                                    const unsigned long long b,
                                                    const unsigned long long c)
{
  return (__uint128_t(a) * b + c) >> 64;
}

constexpr unsigned int Log2(Integer x)
{
  unsigned int ret = 0;
  while (x) {
    x >>= 1;
    ++ret;
  }
  return ret;
}
struct Magic {
  Integer multiply, increase;
  unsigned int postShift;

  static constexpr Magic init(const Integer mod)
  {
    constexpr unsigned int bits = 64;

    const unsigned int lgm = Log2(mod);
    const Integer initPow2 = Integer(1) << (bits - 1);
    bool hasDown = false;
    unsigned int exponent;
    Integer downMultiply, downShift;
    Integer quotient = initPow2 / mod, reminder = initPow2 % mod;

    for (exponent = 0;; ++exponent) {
      if (reminder >= mod - reminder) {
        quotient = (quotient << 1) + 1;
        reminder = (reminder << 1) - mod;
      }
      else {
        quotient <<= 1;
        reminder <<= 1;
      }

      const Integer tmp = Integer(1) << (exponent);
      if (exponent >= lgm || (mod - reminder) <= tmp) break;
      if (!hasDown && reminder <= tmp) {
        hasDown = true;
        downMultiply = quotient;
        downShift = exponent;
      }
    }

    if (exponent < lgm)
      return Magic{
          .multiply = quotient + 1, .increase = 0, .postShift = exponent};
    else
      return Magic{.multiply = downMultiply,
                   .increase = downMultiply,
                   .postShift = downShift};
  }
};
inline constexpr Integer divide(const Integer val, const Magic p)
{
  return highMultiplyAdd(val, p.multiply, p.increase) >> p.postShift;
}
inline constexpr Integer mod(const Integer val, const Integer m, const Magic p)
{
  const Integer q = divide(val, p);
  return val - q * m;
}
}  // namespace Divide

unsigned long long mod;
Divide::Magic mg;
struct Number {
#define decl(x)                                                   \
  inline friend Number operator x(const Number l, const Number r) \
  {                                                               \
    return Number{Divide::mod(l.val x r.val, mod, mg)};           \
  }                                                               \
  inline void operator x##=(const Number r)                       \
  {                                                               \
    val = Divide::mod(val x r.val, mod, mg);                      \
  }
  decl(+);
  decl(*);
#undef decl
  inline friend std::ostream& operator<<(std::ostream& os, const Number v)
  {
    return os << v.val;
  }

  unsigned long long val;
};

Number g[maxn + 10], inv[maxn + 10];
std::vector<int> factor[maxn + 10];

static void initFactor(const int n)
{
  for (int i = 1; i <= n; ++i)
    for (int j = i + i; j <= n; j += i)
      factor[j].push_back(i);
}
static void initInv(const int n)
{
  inv[0].val = inv[1].val = 1;
  for (int i = 2; i <= n; ++i)
    inv[i].val = mod - (inv[mod % i] * Number{mod / i}).val;
}
static void init(const int n)
{
  initFactor(n);
  initInv(n);
}
void dp(const int n)
{
  static Number h[maxn + 10], xf[maxn + 10];
  g[0].val = g[1].val = xf[1].val = h[1].val = 1;
  for (int i = 2; i <= n; ++i) {
    Number acc{};
    for (int j = 0; j < i; ++j)
      acc.val += (g[j] * h[i - j]).val;
    for (int j : factor[i])
      acc.val += xf[j].val;
    acc.val = Divide::mod(acc.val, mod, mg);

    acc *= inv[i];
    g[i] = acc * Number{2};
    xf[i] = acc * Number{i};

    h[i] = xf[i];
    for (int j : factor[i])
      h[i].val += xf[j].val;
    h[i].val = Divide::mod(h[i].val, mod, mg);
  }
}

int main()
{
  std::ios::sync_with_stdio(false);
  static int query[maxt + 10];
  int t, mxt = 0;
  std::cin >> t >> mod;
  mg = Divide::Magic::init(mod);
  for (int* i = query; i < query + t; ++i) {
    std::cin >> *i;
    mxt = std::max(mxt, *i);
  }
  init(mxt);
  dp(mxt);
  for (const int* i = query; i < query + t; ++i)
    std::cout << g[*i] << "\n";
  return 0;
}