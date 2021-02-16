#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
constexpr unsigned long long mod = 19491001, primitiveRoot = 7, maxk = 500000;
const unsigned long long w3 = 18827933, w[3] = {1, w3, (w3 * w3) % mod};

class Number {
 public:
#define decl(x)                                                   \
  inline friend Number operator x(const Number l, const Number r) \
  {                                                               \
    return Number{(l.val x r.val) % mod};                         \
  }                                                               \
  inline void operator x##=(const Number r) { val = (val x r.val) % mod; }
  decl(+);
  decl(*);
#undef decl
  inline friend std::ostream& operator<<(std::ostream& os, const Number v)
  {
    return os << v.val;
  }

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
inline Number inverse(const Number x) { return quickPow(x, mod - 2); }
Number fac[maxk + 10], invFac[maxk + 10];

static void initFactor(const unsigned int k)
{
  fac[0].val = 1;
  for (unsigned int i = 1; i <= k; ++i)
    fac[i] = fac[i - 1] * Number{i};
}
static void initModularInv(const unsigned int k)
{
  static Number prd[maxk + 10], iprd[maxk + 10];
  prd[0] = fac[0];
  for (unsigned int i = 1; i <= k; ++i)
    prd[i] = prd[i - 1] * fac[i];
  iprd[k] = inverse(prd[k]);
  for (unsigned int i = k; i; --i) {
    invFac[i] = iprd[i] * prd[i - 1];
    iprd[i - 1] = iprd[i] * fac[i];
  }
  invFac[0].val = 1;
}
static void init(const unsigned int k)
{
  initFactor(k);
  initModularInv(k);
}

inline Number C(const unsigned int n, const unsigned int m)
{
  return fac[n] * invFac[m] * invFac[n - m];
}
inline Number count1(const unsigned int n, const unsigned int k)
{
  return quickPow(Number{k}, n);
}
Number count2(const unsigned int n, const unsigned int k)
{
  Number ret{};
  for (unsigned int i = 0; i <= k; ++i)
    ret += quickPow(Number{(2 * i + mod - k) % mod}, n) * C(k, i);
  return ret * inverse(quickPow(Number{2}, k));
}
inline Number product(const unsigned int a, const unsigned int b,
                      const unsigned int c)
{
  return Number{a} * Number{w[0]} + Number{b} * Number{w[1]} +
         Number{c} * Number{w[2]};
}
inline Number C(const unsigned int k, const unsigned int a,
                const unsigned int b)
{
  return fac[k] * invFac[a] * invFac[b] * invFac[k - a - b];
}
Number count3(const unsigned int n, const unsigned int k)
{
  Number ret{};
  for (unsigned int i{0}; i <= k; ++i)
    for (unsigned int j{0}; j + i <= k; ++j)
      ret += quickPow(product(i, j, k - i - j), n) * C(k, i, j);
  return ret * inverse(quickPow(Number{3}, k));
}

int main()
{
  unsigned int n, k, d;
  std::cin >> n >> k >> d;
  switch (d) {
    case 1:
      std::cout << count1(n, k) << "\n";
      break;
    case 2:
      init(k);
      std::cout << count2(n, k) << "\n";
      break;
    case 3:
      init(k);
      std::cout << count3(n, k) << "\n";
      break;
  }
  return 0;
}