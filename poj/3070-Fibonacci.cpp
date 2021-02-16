#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <vector>
using namespace std;
typedef unsigned int num;
const num mod = 1e4;

class Number {
 public:
  Number() : val(0) {}
  Number(num i) : val(i % mod) {}
#define decl(x)                                                    \
  inline friend Number operator x(const Number l, const Number r); \
  inline Number operator x##=(const Number r) { return *this = *this x r; }
  decl(*);
  decl(+);
#undef decl
  inline friend ostream& operator<<(ostream& os, const Number r);

 private:
  num val;
};
#define impl(x)                                            \
  inline Number operator x(const Number l, const Number r) \
  {                                                        \
    return l.val x r.val;                                  \
  }
impl(*);
impl(+);
#undef impl
inline ostream& operator<<(ostream& os, const Number r) { return os << r.val; }
struct vec {
  vec() {}
  vec(unsigned int x, unsigned int y)
  {
    val[0] = x;
    val[1] = y;
  }
  inline Number& operator[](const unsigned int i) { return val[i]; }
  inline Number operator[](const unsigned int i) const { return val[i]; }
  Number val[2];
};
struct matrix {
  matrix()
  {
    val[0] = {1, 0};
    val[1] = {0, 1};
  }
  vec operator*(const vec r) const
  {
    vec ret(0, 0);
    for (unsigned int i = 0; i < 2; ++i)
      for (unsigned int j = 0; j < 2; ++j)
        ret[j] += r[i] * val[i][j];
    return ret;
  }
  matrix operator*(const matrix r) const
  {
    matrix ret;
    ret.val[0] = {0, 0};
    ret.val[1] = {0, 0};
    for (unsigned int i = 0; i < 2; ++i)
      ret.val[i] = r * val[i];
    return ret;
  }
  matrix operator^(unsigned int e) const
  {
    matrix ret, v = *this;
    for (unsigned int i = 1; e; i <<= 1) {
      if (e & i) {
        ret = ret * v;
        e ^= i;
      }
      v = v * v;
    }
    return ret;
  }

  vec val[2];
};

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  matrix m;
  m.val[0] = {0, 1};
  m.val[1] = {1, 1};
  while (true) {
    int n;
    cin >> n;
    if (n == -1) break;
    if (!n)
      cout << "0" << endl;
    else
      cout << ((m ^ (n - 1)) * vec(0, 1))[1] << endl;
  }
  return 0;
}