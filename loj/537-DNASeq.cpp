#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxn = 5e6, maxs = (1 << 20);

unsigned int cnt[maxs + 1], str[maxn + 1];

void count(const size_t l, const unsigned int k)
{
  const unsigned int msk = (1 << k * 2) - 1;
  unsigned int c = 0;
  for (unsigned int i = 0; i < k; ++i)
    c = (c << 2) | str[i];
  ++cnt[c];
  for (size_t i = k; i < l; ++i) {
    c = ((c << 2) | str[i]) & msk;
    ++cnt[c];
  }
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  static char buf[maxn + 10];
  unsigned int k;
  cin >> buf >> k;
  const size_t l = strlen(buf);
  for (unsigned int i = 0; i < l; ++i)
    switch (buf[i]) {
      case 'A':
        str[i] = 0;
        break;
      case 'T':
        str[i] = 1;
        break;
      case 'G':
        str[i] = 2;
        break;
      case 'C':
        str[i] = 3;
        break;
    }
  count(l, k);
  cout << *max_element(cnt, cnt + (1 << 2 * k)) << endl;
  return 0;
}