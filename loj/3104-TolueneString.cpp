#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
const unsigned int maxn = 1e5, maxc = 26;
constexpr unsigned long long mod = 1e9 + 7;

class Number {
 public:
  Number() = default;
  Number(const unsigned long long v) : val(v % mod){};
#define decl(x)                                                   \
  inline friend Number operator x(const Number l, const Number r) \
  {                                                               \
    return (l.val + mod) x r.val;                                 \
  }                                                               \
  inline void operator x##=(const Number r)                       \
  {                                                               \
    val = ((val + mod) x r.val) % mod;                            \
  }
  decl(+);
  decl(*);
#undef decl
  inline friend std::ostream& operator<<(std::ostream& os, const Number v)
  {
    return os << v.val;
  }

 private:
  unsigned long long val = 0;
};

struct Matrix {
  void unit()
  {
    for (unsigned int i = 0; i < maxc; ++i)
      val[i][i] = 1;
  }
  Number val[maxc][maxc];
};
Matrix operator*(const Matrix& l, const Matrix& r)
{
  Matrix ret{};
  for (unsigned int i = 0; i < maxc; ++i)
    for (unsigned int k = 0; k < maxc; ++k)
      for (unsigned int j = 0; j < maxc; ++j)
        ret.val[i][j] += l.val[k][j] * r.val[i][k];
  return ret;
}
Matrix quickPow(Matrix a, unsigned long long e)
{
  Matrix ret{};
  ret.unit();
  for (; e; e >>= 1) {
    if (e & 0x01) ret = ret * a;
    a = a * a;
  }
  return ret;
}
Matrix build(const char str[])
{
  const size_t len = strlen(str);
  Matrix ret;
  for (unsigned int i = 0; i < maxc; ++i)
    for (unsigned int j = 0; j < maxc; ++j)
      ret.val[i][j] = 1;
  for (const char* i = str + 1; i < str + len; ++i) {
    const unsigned int lst = *(i - 1) - 'a', cur = *i - 'a';
    ret.val[lst][cur] = 0;
  }
  return ret;
}
Number solve(const char str[], const unsigned long long s2l)
{
  const Matrix&& p = quickPow(build(str), s2l - 1);
  Number ret;
  for (unsigned int i = 0; i < maxc; ++i)
    for (unsigned int j = 0; j < maxc; ++j)
      ret += p.val[i][j];
  return ret;
}

int main()
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  unsigned long long n;
  static char buf[maxn + 10];
  std::cin >> n >> buf;
  std::cout << solve(buf, n) << "\n";
  return 0;
}