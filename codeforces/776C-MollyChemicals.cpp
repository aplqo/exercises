#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <map>
using namespace std;
const unsigned int maxn = 1e5, maxl = 50;
const long long maxa = 1e9;
typedef map<long long, unsigned int> Map;

long long a[maxn + 10], po[maxl + 10];

static unsigned int initPow(const unsigned int n, const long long k)
{
  po[0] = 1;
  if (abs(k) == 1) {
    po[1] = po[0] * k;
    return k < 0 ? 2 : 1;
  }
  const long long mv = n * maxa;
  unsigned int cnt = 1;
  while (abs(po[cnt - 1] * k) <= mv) {
    po[cnt] = po[cnt - 1] * k;
    ++cnt;
  }
  return cnt;
}
unsigned long long count(const unsigned int n, const unsigned int ck)
{
  Map sums;
  unsigned long long ret = 0;
  long long acc = 0;
  sums[0] = 1;
  for (const long long* i = a; i < a + n; ++i) {
    acc += *i;
    for (const long long* j = po; j < po + ck; ++j)
      if (auto it = sums.find(acc - *j); it != sums.end()) ret += it->second;
    auto it = sums.find(acc);
    if (it == sums.end()) it = sums.insert({acc, 0}).first;
    ++(it->second);
  }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  long long k;
  cin >> n >> k;
  copy_n(istream_iterator<long long>(cin), n, a);
  cout << count(n, initPow(n, k)) << "\n";
  return 0;
}