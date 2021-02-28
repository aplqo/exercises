#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <map>
#include <tuple>
const int maxn = 2000;

namespace Divide {
inline unsigned long long highMultiplyAdd(const unsigned long long a,
                                          const unsigned long long b,
                                          const unsigned long long c)
{
  return (static_cast<__uint128_t>(a) * b + c) >> 64;
}
inline unsigned int Log2(unsigned long long x)
{
  unsigned int ret = 0;
  while (x) {
    ++ret;
    x >>= 1;
  }
  return ret;
}
struct Magic {
  unsigned long long multiply, increase;
  unsigned int postShift;

  static Magic init(const unsigned long long mod)
  {
    constexpr unsigned int bits = 64;
    const unsigned int lgm = Log2(mod);
    const unsigned long long initPow2 = 1ull << (bits - 1);
    bool hasDown = false;
    unsigned int exponent, downShift;
    unsigned long long downMultiply;
    unsigned long long quotient = initPow2 / mod, reminder = initPow2 % mod;

    for (exponent = 0;; ++exponent) {
      if (reminder >= mod - reminder) {
        quotient = (quotient << 1) + 1;
        reminder = (reminder << 1) - mod;
      }
      else {
        quotient <<= 1;
        reminder <<= 1;
      }

      const unsigned long long tmp = 1ull << exponent;
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
unsigned long long divisor;
Magic mg;
static void init(const unsigned long long x)
{
  divisor = x;
  mg = Magic::init(x);
}
inline unsigned long long divide(const unsigned long long x)
{
  return highMultiplyAdd(x, mg.multiply, mg.increase) >> mg.postShift;
}
inline unsigned int mod(const unsigned long long x)
{
  return x - divide(x) * divisor;
}
}  // namespace Divide

unsigned int mod;

struct Number {
  inline friend Number operator+(const Number l, const Number r)
  {
    const unsigned int ret = l.val + r.val;
    return Number{ret >= mod ? ret - mod : ret};
  }
  inline void operator+=(const Number r)
  {
    val += r.val;
    if (val >= mod) val -= mod;
  }
  inline friend Number operator-(const Number l, const Number r)
  {
    return Number{l.val >= r.val ? l.val - r.val : l.val + mod - r.val};
  }
  inline friend Number operator*(const Number l, const Number r)
  {
    return Number{static_cast<unsigned int>(
        Divide::mod(static_cast<unsigned long long>(l.val) * r.val))};
  }
  inline void operator*=(const Number r)
  {
    val = Divide::mod(static_cast<unsigned long long>(val) * r.val);
  }

  unsigned int val;
};
Number fac[maxn + 10][maxn + 10], ifac[maxn + 10][maxn + 10], inv[maxn + 10];

Number quickPow(Number a, unsigned long long e)
{
  Number ret{1};
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= a;
    a *= a;
  }
  return ret;
}
inline Number inverse(const Number x) { return quickPow(x, mod - 2); }

static void initFactor(const unsigned int n, Number fac[], Number ifac[])
{
  fac[0].val = fac[1].val = 1;
  for (unsigned int i = 2; i <= n; ++i)
    fac[i] = fac[i - 1] * Number{i};

  ifac[n] = inverse(fac[n]);
  for (unsigned int i = n - 1; i; --i)
    ifac[i] = ifac[i + 1] * Number{i + 1};
  ifac[0].val = 1;
}
static void initPow(const int n, const int k, Number val[][maxn + 10])
{
  std::fill(val[0], val[0] + n + 1, Number{1});
  for (int i = 2; i <= n; ++i)
    val[1][i] = quickPow(val[1][i], k);
  for (int i = 2; i <= n; ++i) {
    val[i][0].val = val[i][1].val = 1;
    for (int j = 2; j <= n; ++j)
      val[i][j] = val[i - 1][j] * val[1][j];
  }
}
static void initFactorPow(const int n, const int k)
{
  initFactor(n, fac[1], ifac[1]);
  initPow(n, k, fac);
  initPow(n, k, ifac);
}
static void initModularInv(const unsigned int n)
{
  inv[0].val = inv[1].val = 1;
  for (unsigned int i = 2; i <= n; ++i)
    inv[i].val = mod - (inv[mod % i] * Number{mod / i}).val;
}
static void init(const int n, const int k)
{
  Divide::init(mod);
  initFactorPow(n, k);
  initModularInv(n);
}

void ln(const Number a[], Number dest[], const unsigned int n)
{
  dest[0].val = 0;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned long long acc = 0;
    for (unsigned int j = 1; j < i; ++j)
      acc += (Number{j} * a[i - j] * dest[j]).val;
    dest[i] = a[i] - Number{Divide::mod(acc)} * inv[i];
  }
}
void exp(const Number a[], Number dest[], const unsigned int n)
{
  dest[0].val = 1;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned long long acc = 0;
    for (unsigned int j = 1; j <= i; ++j)
      acc += (Number{j} * dest[i - j] * a[j]).val;
    dest[i] = Number{Divide::mod(acc)} * inv[i];
  }
}

Number f1Mem[2][maxn + 10][maxn + 10];
Number (*f1Cur)[maxn + 10] = f1Mem[0], (*f1Lst)[maxn + 10] = f1Mem[1];

inline Number f(const int size, const int exp) { return f1Lst[exp][size - 1]; }
inline void getWHat(const int n, const int exp, Number dest[])
{
  ln(ifac[exp], dest, n + 1);
}
void dpG(const int size, const int maxCount, const int exp, Number dest[])
{
  static Number pf[maxn + 10];
  getWHat(maxCount, exp, pf);
  for (int i = 1; i <= maxCount; ++i)
    pf[i] *= f(size, exp * i);
  pf[0].val = 0;
  ::exp(pf, dest, maxCount + 1);
  for (int i = 1; i <= maxCount; ++i)
    dest[i] *= ifac[i * exp][size];
}
inline Number f1(const int limit, const int exp, const int n, const Number g[])
{
  unsigned long long acc = 0;
  for (int i = 0; i * limit <= n; ++i)
    acc += (f1Lst[exp][n - i * limit] * g[i] * fac[exp][n] *
            ifac[exp][n - limit * i])
               .val;
  return Number{Divide::mod(acc)};
}
Number solve(const int n, const int k)
{
  init(n, k);
  for (int i = 1; i <= n; ++i)
    std::fill(f1Lst[i], f1Lst[i] + n + 1, Number{1});
  for (int size = 2; size < n; ++size) {
    const int q = n / size;
    for (int exp = 1; exp * size <= n; ++exp) {
      static Number g[maxn + 10];
      dpG(size, q / exp, exp, g);
      for (int cn = 0; cn * exp <= n; ++cn)
        f1Cur[exp][cn] = f1(size, exp, cn, g);
    }
    std::swap(f1Cur, f1Lst);
  }
  return f(n, 1) * ifac[1][n - 1];
}

int main()
{
  std::ios::sync_with_stdio(false);
  while (true) {
    int n, k;
    std::cin >> n >> k >> mod;
    if (std::cin.fail()) break;
    std::cout << solve(n, k).val << "\n";
  }
  return 0;
}