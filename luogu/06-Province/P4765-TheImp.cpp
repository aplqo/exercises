#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxn = 1.5e5, maxk = 9;

struct Object {
  long long values, cost;
} obj[maxn + 10];

long long dp(const unsigned int n, const unsigned int k)
{
  static long long f[maxn + 10][maxk + 10];
  sort(obj + 1, obj + 1 + n,
       [](const Object& a, const Object& b) { return a.values < b.values; });
  memset(f[n + 1], 0, sizeof(f[n + 1]));
  for (unsigned int i = n; i; --i)
    f[i][0] = max(f[i + 1][0], obj[i].values - obj[i].cost);
  for (unsigned int i = 1; i <= k; ++i)
    for (unsigned int j = n; j; --j)
      f[j][i] =
          max(f[j + 1][i], min(obj[j].values, f[j + 1][i - 1]) - obj[j].cost);
  return f[1][k];
}
long long solve()
{
  unsigned int n, k;
  cin >> n >> k;
  for (Object* i = obj + 1; i < obj + 1 + n; ++i)
    cin >> i->values >> i->cost;
  return dp(n, k);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int t;
  cin >> t;
  for (unsigned int i = t; i; --i)
    cout << solve() << "\n";
  return 0;
}