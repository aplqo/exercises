#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <tuple>
using namespace std;
const int maxn = 100000;
using mon = tuple<int, int, unsigned int>;

mon m[maxn + 10];

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  long long z;
  cin >> n >> z;
  for_each(m, m + n, [](mon& i) -> void { cin >> get<0>(i) >> get<1>(i); });
  for_each(m, m + n, [t = 0](mon& i) mutable -> void { get<2>(i) = ++t; });
  auto t = partition(m, m + n, [](mon i) { return get<0>(i) < get<1>(i); });
  sort(m, t, [](mon a, mon b) { return get<0>(a) < get<0>(b); });
  sort(t, m + n, [](mon a, mon b) { return get<1>(a) > get<1>(b); });
  for (unsigned int i = 0; i < n; ++i) {
    z -= get<0>(m[i]);
    if (z <= 0) {
      cout << "NIE" << endl;
      return 0;
    }
    z += get<1>(m[i]);
  }
  cout << "TAK" << endl;
  for (unsigned int i = 0; i < n; ++i)
    cout << get<2>(m[i]) << " ";
  cout << endl;
  return 0;
}