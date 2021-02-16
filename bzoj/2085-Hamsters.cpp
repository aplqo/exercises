#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 200, maxl = 1e5, maxc = 26;
constexpr unsigned long long inf = ULLONG_MAX / 2;

unsigned int n;
char str[maxn + 1][maxl + 10];
unsigned int nxt[maxn + 1][maxl + 10];
size_t len[maxn + 1];
struct Matrix {
  void reset()
  {
    for (unsigned int i = 0; i <= n; ++i)
      fill(val[i], val[i] + n + 1, inf);
  }
  Matrix operator*(const Matrix& r) const
  {
    Matrix ret;
    ret.reset();
    for (unsigned int i = 0; i <= n; ++i)
      for (unsigned int j = 0; j <= n; ++j)
        for (unsigned int k = 0; k <= n; ++k)
          ret.val[i][k] = std::min(ret.val[i][k], val[i][j] + r.val[j][k]);
    return ret;
  }
  unsigned long long min(const unsigned int col) const
  {
    return *min_element(val[col], val[col] + n + 1);
  }

  unsigned long long val[maxn + 1][maxn + 1];
};
Matrix operator^(Matrix a, unsigned long long e)
{
  Matrix ret;
  ret.reset();
  ret.val[0][0] = 0;
  for (; e; e >>= 1) {
    if (e & 0x01) ret = ret * a;
    a = a * a;
  }
  return ret;
}
void prefix(const unsigned int s)
{
  const char* const st = str[s];
  unsigned int* const nx = nxt[s];
  unsigned int j = 0;
  for (size_t i = 1; i <= len[s]; ++i) {
    while (j && st[j + 1] != st[i + 1])
      j = nx[j];
    if (st[j + 1] == st[i + 1]) ++j;
    nx[i + 1] = j;
  }
}
unsigned int kmp(const unsigned int p, const unsigned int s)
{
  const char *const sp = str[p], *const ss = str[s];
  unsigned int k = 0;
  for (size_t i = 1; i <= len[s]; ++i) {
    while (k && ss[i] != sp[k + 1])
      k = nxt[p][k];
    if (sp[k + 1] == ss[i]) ++k;
    if (k == len[p]) return k;
  }
  return k;
}
static Matrix buildMatrix(const unsigned int n)
{
  Matrix ret;
  ret.reset();
  for (unsigned int i = 1; i <= n; ++i)
    prefix(i);
  for (unsigned int i = 1; i <= n; ++i) {
    for (unsigned int j = 1; j <= n; ++j)
      ret.val[i][j] = len[j] - kmp(j, i);
    ret.val[i][i] = len[i] - nxt[i][len[i]];
    ret.val[0][i] = len[i];
  }
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned long long m;
  cin >> n >> m;
  for (unsigned int i = 1; i <= n; ++i) {
    cin >> (str[i] + 1);
    len[i] = strlen(str[i] + 1);
  }
  cout << (buildMatrix(n) ^ m).min(0) << "\n";
  return 0;
}