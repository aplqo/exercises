#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <numeric>
using namespace std;
const int maxn = 20005, maxm = 105;

int dat[maxn], m, n, su[maxn], ans;

int main()
{
  ios_base::sync_with_stdio(false);
  cin >> m >> n;
  fill(dat, dat + n, INT_MIN);
  for (int i = 0; i < m; ++i)
    for (int j = 0; j < n; ++j) {
      int t;
      cin >> t;
      dat[j] = max(dat[j], t);
    }
  partial_sum(dat, dat + n, su);
  int i = 0;
  for (int* j = su; j < su + n; ++j) {
    ans = max(ans, *j - i);
    if (*j < i) i = *j;
  }
  cout << ans;
  return 0;
}