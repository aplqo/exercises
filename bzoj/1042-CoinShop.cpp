#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <type_traits>
using namespace std;
const unsigned int maxn = 100000, maxc = 4;

long long f[maxn + 1][maxc + 1] = {{1, 1, 1, 1, 1}};
unsigned int c[maxc], d[maxc];

template <unsigned int p, unsigned int sel>
class expr {
 public:
  static inline long long eval(unsigned int s)
  {
    if (1ull * c[p] * (d[p] + 1) > s)
      return expr<p + 1, sel>::eval(s);
    else
      return expr<p + 1, sel>::eval(s) +
             expr<p + 1, sel + 1>::eval(s - c[p] * (d[p] + 1));
  }
};
template <unsigned int sel>
class expr<4, sel> {
 public:
  static inline long long eval(unsigned int s)
  {
    constexpr long long r = sel % 2 ? -1 : 1;
    return r * f[s][maxc];
  }
};
template <unsigned int p, unsigned int lim>
class read {
 public:
  static inline void doRead(unsigned int s[])
  {
    cin >> s[p];
    read<p + 1, lim>::doRead(s);
  }
};
template <unsigned int lim>
class read<lim, lim> {
 public:
  static inline void doRead(unsigned int[]) {}
};

void init(const unsigned int s)
{
  for (unsigned int i = 1; i <= s; ++i)
    for (unsigned int j = 1; j <= maxc; ++j)
      f[i][j] = (i >= c[j - 1] ? f[i - c[j - 1]][j] : 0) + f[i][j - 1];
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int t;
  read<0, 4>::doRead(c);
  init(maxn);
  cin >> t;
  for (unsigned int i = 0; i < t; ++i) {
    unsigned int s;
    read<0, 4>::doRead(d);
    cin >> s;
    cout << expr<0, 0>::eval(s) << endl;
  }
  return 0;
}