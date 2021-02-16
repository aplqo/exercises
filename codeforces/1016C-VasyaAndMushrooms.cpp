#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 3e5;

enum { Pre, Suf };
unsigned long long val[2][maxn + 10], sum[2][maxn + 10];
unsigned long long gain[2][maxn + 10][2];

template <unsigned int typ>
static void initGain(const unsigned int n)
{
  for (unsigned int i = n; i; --i) {
    gain[typ][i][Suf] = gain[typ][i + 1][Suf] + val[typ][i] * (n + 1 - i);
    gain[typ][i][Pre] = gain[typ][i + 1][Pre] + sum[typ][i + 1] + val[typ][i];
  }
}
static void initSum(const unsigned int n, const unsigned long long a[],
                    unsigned long long res[])
{
  for (unsigned int i = n; i > 0; --i)
    res[i] = res[i + 1] + a[i];
}
unsigned long long findMax(const unsigned int n)
{
  unsigned long long ret = 0, acc = 0;
  for (unsigned int i = 0; i < n * 2; ++i) {
    const unsigned int x = (i & 0x03) == 1 || (i & 0x03) == 2, y = i / 2 + 1,
                       typ = i & 0x01;
    acc += val[x][y] * i;
    ret = max(ret, acc + i * sum[x][y + 1] + gain[x][y + 1][Pre] +
                       (n + i - y) * sum[!x][y + typ] + gain[!x][y + typ][Suf]);
  }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  copy_n(istream_iterator<unsigned int>(cin), n, val[0] + 1);
  copy_n(istream_iterator<unsigned int>(cin), n, val[1] + 1);
  initSum(n, val[0], sum[0]);
  initSum(n, val[1], sum[1]);
  initGain<0>(n);
  initGain<1>(n);
  cout << findMax(n) << "\n";
  return 0;
}