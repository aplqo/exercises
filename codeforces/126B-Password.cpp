#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1e6;

char str[maxn + 10];
unsigned int z[maxn + 10];

void ZAlgorithm(const char str[], unsigned int z[], const unsigned int len)
{
  unsigned int l = 0, r = 0;
  for (unsigned int i = 1; i < len; ++i) {
    if (i < r) z[i] = z[i - l];
    if (i + z[i] < r) continue;
    z[i] = min(z[i], r - i);
    while (i + z[i] < len && str[z[i]] == str[i + z[i]])
      ++z[i];
    if (i + z[i] > r) {
      r = i + z[i];
      l = i;
    }
  }
}
pair<unsigned int, unsigned int> find(const unsigned int len)
{
  static bool exist[maxn + 10];
  static unsigned int lst[maxn + 10];
  unsigned int pos = 0, le = 0;
  const auto update = [&pos, &le](const unsigned int p,
                                  const unsigned int cand) {
    if (exist[cand] && lst[cand] > le) {
      pos = p;
      le = lst[cand];
    }
  };
  for (unsigned int i = 0; i < len; ++i)
    if (i + z[i] == len) {
      exist[z[i]] = true;
      if (z[i] > i) exist[z[i] - i] = true;
    }
  {
    unsigned int lp = 0;
    for (unsigned int i = 0; i < len; ++i) {
      if (exist[i]) lp = i;
      lst[i] = lp;
      exist[i] = lp;
    }
  }
  for (unsigned int i = 1; i < len; ++i) {
    if (z[i] + i < len) update(i, z[i]);
    if (z[i] > i) {
      update(i, i);
      update(i, z[i] - i);
    }
  }
  return make_pair(pos, pos + le);
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  cin >> str;
  const size_t len = strlen(str);
  ZAlgorithm(str, z, len);
  const auto [l, r] = find(len);
  if (l == r)
    cout << "Just a legend";
  else
    copy(str + l, str + r, ostreambuf_iterator<char>(cout));
  cout.put('\n');
  return 0;
}