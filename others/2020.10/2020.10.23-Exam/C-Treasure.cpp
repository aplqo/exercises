#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <bitset>
#include <iostream>
#include <limits>
using namespace std;
const unsigned int maxn = 500, maxl = 64;

unsigned int n;
struct Matrix {
  typedef bitset<maxn> Bits;
  void unit()
  {
    for (unsigned int i = 0; i < n; ++i)
      val[i].set(i);
  }
  inline bool check() const { return val[0].any(); }
  inline const Bits& operator[](const unsigned int i) const { return val[i]; }
  inline Bits& operator[](const unsigned int i) { return val[i]; }

  Bits val[maxn];
};
Matrix mat[maxl + 1][2];

inline Matrix operator*(const Matrix& a, const Matrix& b)
{
  Matrix ret;
  for (unsigned int i = 0; i < n; ++i)
    for (unsigned int j = 0; j < n; ++j)
      if (a[i].test(j)) ret[i] |= b[j];
  return ret;
}
static void initPow()
{
  for (unsigned int i = 1; i <= maxl; ++i) {
    mat[i][0] = mat[i - 1][0] * mat[i - 1][1];
    mat[i][1] = mat[i - 1][1] * mat[i - 1][0];
  }
}
unsigned long long binarySearch()
{
  static Matrix m;
  m.unit();
  unsigned long long ret = 0;
  unsigned int typ = 0;
  for (unsigned int i = maxl; i; --i) {
    const unsigned int mid = i - 1;
    const Matrix& tp = m * mat[mid][typ];
    if (tp.check()) {
      m = tp;
      ret |= 1ull << mid;
      typ = !typ;
    }
  }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int m;
  cin >> n >> m;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int x, y, w;
    cin >> x >> y >> w;
    mat[0][w][x - 1].set(y - 1);
  }
  initPow();
  const unsigned long long v = binarySearch();
  if (v > 1e18)
    cout << "-1\n";
  else
    cout << v << "\n";
  return 0;
}