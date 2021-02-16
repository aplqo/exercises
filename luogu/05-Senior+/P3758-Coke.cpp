#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <valarray>
using namespace std;
const unsigned int maxn = 30;
constexpr unsigned long long mod = 2017;

unsigned int n;
using Vector = valarray<unsigned long long>;
struct Matrix {
  Matrix()
  {
    for (unsigned int i = 0; i <= n; ++i)
      val[i].resize(n + 2);
  }
  Vector operator*(const Vector& v) const;
  Matrix operator*(const Matrix& r) const;
  Matrix operator^(unsigned int e) const;
  void unit();

  Vector val[maxn + 1];
};
Vector Matrix::operator*(const Vector& v) const
{
  Vector ret(n + 2);
  for (unsigned int i = 0; i <= n; ++i)
    ret = (ret + (v[i] * val[i]) % mod) % mod;
  return ret;
}
Matrix Matrix::operator*(const Matrix& r) const
{
  Matrix ret;
  for (unsigned int i = 0; i <= n; ++i)
    ret.val[i] = *this * r.val[i];
  return ret;
}
void Matrix::unit()
{
  for (unsigned int i = 0; i <= n; ++i)
    val[i][i] = 1;
}
Matrix Matrix::operator^(unsigned int e) const
{
  Matrix ret, c = *this;
  ret.unit();
  for (; e; e >>= 1) {
    if (e & 0x01) ret = ret * c;
    c = c * c;
  }
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int m, t;
  cin >> n >> m;
  Matrix ma;
  ma.unit();
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int u, v;
    cin >> u >> v;
    ma.val[u][v] = 1;
    ma.val[v][u] = 1;
  }
  for (unsigned int i = 1; i <= n; ++i)
    ma.val[i][0] = 1;
  cin >> t;
  Matrix&& r = ma ^ t;
  unsigned long long ret = 0;
  for (unsigned int i = 0; i <= n; ++i)
    ret = (ret + r.val[1][i]) % mod;
  cout << ret << endl;
  return 0;
}