#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxa = (1 << 20);

long long ans[maxa + 1] = {1};
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  long long n;
  cin >> n;
  if (n == 1) {
    cout << "None" << endl;
    return 0;
  }
  long long* pos = ans + 1;
  for (long long i = 1; i * i <= n; ++i)
    if (n % i == 0) {
      const long long a = i, b = n / i;
      for (long long t = b; t <= n; t += b) {
        if (t + 1 <= n && (t + 2) % a == 0) *(pos++) = t + 1;
        if (t - 1 > 0 && (t - 2) % a == 0) *(pos++) = t - 1;
      }
    }
  sort(ans, pos);
  pos = unique(ans, pos);
  copy(ans, pos, ostream_iterator<long long>(cout, "\n"));
  return 0;
}