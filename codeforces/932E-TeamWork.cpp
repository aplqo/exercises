#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <iostream>
const unsigned int maxk = 5000;
constexpr unsigned long long mod = 1e9 + 7;

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
Number pow2[maxk + 10], pown[maxk + 10], s[maxk + 10][maxk + 10];

Number quickPow(Number a, unsigned long long e)
{
  Number ret{1};
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= a;
    a *= a;
  }
  return ret;
}
static void initStirling(const unsigned int k)
{
  s[0][0].val = s[1][1].val = 1;
  for (unsigned int i = 2; i <= k; ++i)
    for (unsigned int j = 1; j <= i; ++j)
      s[i][j] = s[i - 1][j - 1] + s[i - 1][j] * Number{j};
}
static void initPow2(const unsigned int n, const unsigned int k)
{
  const unsigned int ck = std::min(n, k);
  for (unsigned int i = ck; i <= k; ++i)
    pow2[i].val = 1;
  pow2[ck] = quickPow(Number{2}, n - ck);
  for (unsigned int i = ck; i; --i)
    pow2[i - 1] = pow2[i] * Number{2};
}
static void initPown(const unsigned int n, const unsigned int k)
{
  const unsigned int ck = std::min(n, k);
  pown[1].val = n;
  for (unsigned int i = 1; i < ck; ++i)
    pown[i + 1] = pown[i] * Number{n - i};
}
Number sum(const unsigned int k)
{
  Number ret{0};
  for (unsigned int i = 0; i <= k; ++i)
    ret += s[k][i] * pow2[i] * pown[i];
  return ret;
}
int main()
{
  std::ios::sync_with_stdio(false);
  unsigned int n, k;
  std::cin >> n >> k;
  initStirling(k);
  initPow2(n, k);
  initPown(n, k);
  std::cout << sum(k) << "\n";
  return 0;
}