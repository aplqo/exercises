#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <unordered_set>
#include <vector>
using namespace std;
const unsigned int maxq = 40000;
typedef unsigned long long sum_t;

unsigned int fac[maxq * 2 + 10];
unordered_set<sum_t> result[maxq * 2 + 10];

unsigned int factor(const unsigned int n)
{
  unsigned int* ptr = fac;
  for (unsigned int i = 1; i * i <= n; ++i) {
    if (n % i) continue;
    *(ptr++) = i;
    if (i != n / i) *(ptr++) = n / i;
  }
  return ptr - fac;
}
vector<sum_t> findSolution(const unsigned int n)
{
  const unsigned int cd = factor(n);
  sort(fac, fac + cd);
  result[0].insert(0);
  for (unsigned int i = 0; i < cd; ++i)
    for (unsigned int j = 0; j < i; ++j)
      if (fac[i] % fac[j] == 0)
        transform(result[j].cbegin(), result[j].cend(),
                  inserter(result[i], result[i].end()),
                  [t = fac[i] / fac[j]](sum_t i) { return i + t - 1; });
  vector<sum_t> ret(result[cd - 1].size());
  copy(result[cd - 1].cbegin(), result[cd - 1].cend(), ret.begin());
  sort(ret.begin(), ret.end());
  return move(ret);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  const vector<sum_t>& s = findSolution(n);
  cout << s.size() << "\n";
  copy(s.cbegin(), s.cend(), ostream_iterator<unsigned int>(cout, " "));
  cout.put('\n');
  return 0;
}