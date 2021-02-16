#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <valarray>
using namespace std;
using vec = valarray<unsigned long long>;

struct matrix {
  vec operator*(const vec& r) const
  {
    vec ret(2);
    ret = (r[0] * val[0]) % mod;
    ret += r[1] * val[1] % mod;
    ret %= mod;
    return ret;
  }
  matrix operator*(const matrix r) const
  {
    matrix ret;
    ret.val[0] = (r * val[0]) % mod;
    ret.val[1] = (r * val[1]) % mod;
    return ret;
  }
  matrix operator^(unsigned long long e) const
  {
    matrix ret, a = *this;
    for (unsigned long long i = 1; e; i <<= 1) {
      if (e & i) {
        ret = ret * a;
        e ^= i;
      }
      a = a * a;
    }
    return ret;
  };

  static unsigned long long mod;
  vec val[2] = {vec{1, 0}, vec{0, 1}};
};
unsigned long long matrix::mod;

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  vec ini(2);
  matrix m;
  unsigned long long n;
  cin >> m.val[1][1] >> m.val[0][1] >> ini[0] >> ini[1] >> n >> matrix::mod;
  m.val[1][0] = 1;
  m.val[0][0] = 0;
  cout << ((m ^ (n - 2)) * ini)[1] << endl;
  return 0;
}