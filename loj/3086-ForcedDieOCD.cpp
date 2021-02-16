#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
const unsigned int mod = 1e9 + 7;

class Number {
 public:
  constexpr Number() = default;
  constexpr Number(const unsigned long long v) : val(v % mod){};

#define decl(x)                                                   \
  inline friend Number operator x(const Number l, const Number r) \
  {                                                               \
    return (l.val x r.val);                                       \
  }                                                               \
  inline void operator x##=(const Number r) { val = (val x r.val) % mod; }
  decl(+);
  decl(*);
#undef decl
  inline friend std::ostream& operator<<(std::ostream& os, const Number r)
  {
    return os << r.val;
  }

 private:
  unsigned long long val = 0;
};

struct Vector {
  Number val[5];
};
struct Matrix {
  void unit()
  {
    for (unsigned int i = 0; i < 5; ++i)
      val[i].val[i] = 1;
  }
  Vector val[5];
};
Vector operator*(const Matrix& m, const Vector& v)
{
  Vector ret{};
  for (unsigned int i = 0; i < 5; ++i)
    for (unsigned int j = 0; j < 5; ++j)
      ret.val[j] += m.val[i].val[j] * v.val[i];
  return ret;
}
Matrix operator*(const Matrix& l, const Matrix& r)
{
  Matrix ret{};
  for (unsigned int i = 0; i < 5; ++i)
    ret.val[i] = l * r.val[i];
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
Number solve(const unsigned int n)
{
  static constexpr Matrix mat{{{1, 1, 0, 0, 0},
                               {1, 0, 0, 0, 0},
                               {2, 0, 1, 1, 0},
                               {0, 0, 1, 0, 0},
                               {mod - 2, 0, 0, 0, 1}}};
  static constexpr Vector iv{0, 0, 1, 1, 1};
  return (quickPow(mat, n - 1) * iv).val[0];
}
int main()
{
  std::ios::sync_with_stdio(false);
  unsigned int t;
  std::cin >> t;
  for (unsigned int i = t; i; --i) {
    unsigned int n;
    std::cin >> n;
    std::cout << solve(n) << "\n";
  }
  return 0;
}