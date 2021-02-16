#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;
const int maxn = 1000;
typedef unsigned int numType;

char s1[maxn], s2[maxn];
numType f[maxn][maxn];
numType l1, l2, ans;

void solve()
{
  l1 = strlen(s1 + 1);
  l2 = strlen(s2 + 1);
  for_each(f, f + l1 + 1,
           [](numType(&i)[maxn]) -> void { fill(i, i + l2 + 1, 0); });
  ans = 0;
  for (int i = 1; i <= l1; ++i)
    for (int j = 1; j <= l2; ++j)
      if (s1[i] == s2[j])
        f[i][j] = f[i - 1][j - 1] + 1;
      else
        f[i][j] = max(f[i - 1][j], f[i][j - 1]);
  for_each(f + 1, f + l1 + 1, [](numType(&i)[maxn]) -> void {
    ans = *max_element(i + 1, i + 1 + l2);
  });
  cout << ans << endl;
}
int main()
{
  ios_base::sync_with_stdio(false);
  while (cin >> (s1 + 1) >> (s2 + 1))
    solve();
  return 0;
}