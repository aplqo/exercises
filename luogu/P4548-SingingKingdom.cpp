#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
const unsigned int mod = 1e4, maxl = 1e5;

unsigned long long powN[maxl + 10];

static void initPow(const unsigned int n, const unsigned int l)
{
  powN[0] = 1;
  for (unsigned int i = 1; i <= l; ++i)
    powN[i] = (powN[i - 1] * n) % mod;
}
void getNext(const unsigned int a[], const unsigned int n, unsigned int nxt[])
{
  unsigned int j = 0;
  for (unsigned int i = 1; i < n; ++i) {
    while (j && a[j + 1] != a[i + 1])
      j = nxt[j];
    if (a[j + 1] == a[i + 1]) ++j;
    nxt[i + 1] = j;
  }
}
unsigned int solve(const unsigned int n, const unsigned int name[])
{
  static unsigned int next[maxl + 10];
  getNext(name, n, next);
  unsigned long long ret = 0;
  for (unsigned int i = n; i; i = next[i])
    ret = (ret + powN[i]) % mod;
  return ret;
}

int main()
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::cout.fill('0');
  unsigned int n, t;
  std::cin >> n >> t;
  initPow(n, maxl);
  for (unsigned int i = t; i; --i) {
    static unsigned int str[maxl + 10];
    unsigned int m;
    std::cin >> m;
    std::copy_n(std::istream_iterator<unsigned int>(std::cin), m, str + 1);
    std::cout.width(4);
    std::cout << solve(m, str) << "\n";
  }
  return 0;
}