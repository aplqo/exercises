#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
using namespace std;
const unsigned int maxm = 200;

unsigned int ans[maxm + 10];
map<unsigned int, unsigned int> mp;

inline void addFactor(const unsigned int x)
{
  if (const auto it = mp.find(x); it != mp.end())
    ++(it->second);
  else
    mp[x] = 1;
}
void factor(const unsigned int v, const unsigned int n)
{
  for (unsigned int i = 1; i * i <= v && i <= n; ++i) {
    if (v % i) continue;
    addFactor(i);
    if (v / i <= n && i * i != v) addFactor(v / i);
  }
}
void count(const unsigned int n)
{
  for (const auto i : mp)
    ++ans[i.second];
  ans[0] = n - mp.size();
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int v;
    cin >> v;
    factor(v, n);
  }
  count(n);
  copy(ans, ans + m + 1, ostream_iterator<unsigned int>(cout, "\n"));
  return 0;
}