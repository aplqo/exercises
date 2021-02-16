#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
const unsigned int maxn = 100, maxd = 1000;

class Number {
 public:
  Number() = default;
  Number(const long long i) : val(i % mod) {}
#define decl(x)                                                \
  inline friend Number operator x(const Number, const Number); \
  inline Number& operator x##=(const Number r) { return *this = *this x r; }
  decl(+);
  decl(*);
  decl(-);
  decl(/);
#undef decl
#define oper(x) \
  inline bool operator x(const Number r) { return val x r.val; }
  oper(==);
  oper(!=);
#undef oper
  inline friend istream& operator>>(istream&, Number&);
  inline friend ostream& operator<<(ostream&, const Number);
  static unsigned int long long mod;

 private:
  unsigned long long val = 0;
};
unsigned long long Number::mod;
#define impl(x)                                            \
  inline Number operator x(const Number l, const Number r) \
  {                                                        \
    return l.val x r.val + Number::mod;                    \
  }
impl(+);
impl(*);
impl(-);
#undef impl
inline Number operator^(Number a, unsigned int long long e)
{
  Number ret = 1;
  for (unsigned long long i = 1; e; i <<= 1) {
    if (e & i) {
      ret *= a;
      e ^= i;
    }
    a *= a;
  }
  return ret;
}
inline Number operator/(const Number a, const Number b)
{
  return a.val * (b ^ (Number::mod - 2)).val;
}
inline istream& operator>>(istream& is, Number& v) { return is >> v.val; }
inline ostream& operator<<(ostream& os, const Number v) { return os << v.val; }

Number c[maxd + 10][maxn + 10], w[maxn + 10][maxn * 2 + 10];
Number ans[maxd + 10][maxn + 10];

void multiply(const unsigned int n, const unsigned int d)  // w^-1 * c
{
  for (unsigned int i = 0; i < d; ++i)
    for (unsigned int j = 0; j < n; ++j)
      for (unsigned int k = 0; k < n; ++k)
        ans[i][k] += c[i][j] * w[k][j + n];
}
void invInit(const unsigned int n)
{
  for (unsigned int i = 0; i < n; ++i)
    w[i][i + n] = 1;
}
void inverse(const unsigned int n)
{
  invInit(n);
  for (unsigned int i = 0; i < n; ++i) {
    if (w[i][i] == 0) {
      for (unsigned int j = i + 1; j < n; ++j)
        if (w[j][i] != 0) {
          swap_ranges(w[j], w[j] + n * 2, w[i]);
          break;
        }
    }
    for (unsigned int j = i + 1; j < n; ++j) {
      if (w[j][i] == 0) continue;
      const Number rat = w[j][i] / w[i][i];
      for (unsigned int k = i; k < n * 2; ++k)
        w[j][k] -= rat * w[i][k];
    }
  }
  for (unsigned int i = n - 1; i > 0; --i) {
    const Number inv = 1 / w[i][i];
    for (unsigned int j = n; j < n * 2; ++j)
      w[i][j] *= inv;
    for (unsigned int j = 0; j < i; ++j) {
      for (unsigned int k = n; k < n * 2; ++k)
        w[j][k] -= w[j][i] * w[i][k];
      w[j][i] = 0;
    }
  }
  for (unsigned int i = n; i < n * 2; ++i)
    w[0][i] /= w[0][0];
  w[0][0] = 1;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, d;
  cin >> n >> d >> Number::mod;
  for (unsigned int i = 0; i < n; ++i)
    for (unsigned int j = 0; j < n; ++j)
      cin >> w[i][j];
  for (unsigned int i = 0; i < d; ++i)
    for (unsigned int j = 0; j < n; ++j)
      cin >> c[i][j];
  inverse(n);
  multiply(n, d);
  for (unsigned int i = 0; i < d; ++i) {
    for (unsigned int j = 0; j < n; ++j)
      cout << ans[i][j] << " ";
    cout << endl;
  }
  return 0;
}