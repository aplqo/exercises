#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxl = 18;

unsigned long long f[maxl + 10][4], po[maxl + 10];

static void GetCount(const unsigned int ml)
{
  po[0] = 1;
  f[0][0] = 1;
  for (unsigned int i = 1; i <= ml; ++i) {
    f[i][0] = (f[i - 1][0] + f[i - 1][1] + f[i - 1][2]) * 9;
    f[i][1] = f[i - 1][0];
    f[i][2] = f[i - 1][1];
    f[i][3] = f[i - 1][2] + 10 * f[i - 1][3];
    po[i] = po[i - 1] * 10;
  }
}
unsigned int GetDigit(const unsigned long long k)
{
  unsigned int ret = 1;
  while (f[ret][3] < k)
    ++ret;
  return ret;
}
unsigned int dp(unsigned long long k, unsigned int res[])
{
  const unsigned int d = GetDigit(k);
  unsigned int lst = 0;
  for (unsigned int i = 0; i < d; ++i) {
    const unsigned int cd = d - i;
    for (unsigned int j = 0; j < 10; ++j) {
      const unsigned long long mv = [](unsigned int cd, int j, int lst) {
        if (lst == 3)
          return po[cd - 1];
        else if (j == 6)
          return accumulate(f[cd - 1] + max(3 - lst - (j == 6), 0),
                            f[cd - 1] + 4, 0ull);
        else
          return f[cd - 1][3];
      }(cd, j, lst);
      res[i] = j;
      if (mv >= k) {
        if (lst < 3) lst = j == 6 ? min(lst + 1, 3u) : 0;
        break;
      }
      k -= mv;
    }
  }
  return d;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  GetCount(maxl);
  unsigned int t;
  cin >> t;
  for (unsigned int i = 0; i < t; ++i) {
    static unsigned int res[maxl + 10];
    unsigned long long n;
    cin >> n;
    transform(res, res + dp(n, res), ostreambuf_iterator<char>(cout),
              [](unsigned int i) { return i + '0'; });
    cout.put('\n');
  }
  return 0;
}