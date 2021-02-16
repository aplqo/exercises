#ifdef APDEBUG
#define APINPROG
#define COLOR
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
using namespace std;
const int maxn = 205;

int pos[maxn], dif[maxn];
int m, s, c, ans;

int main()
{
  ios_base::sync_with_stdio(false);
  cin >> m >> s >> c;
  m = min(m, c);
  for (int* i = pos; i < pos + c; ++i)
    cin >> *i;
  sort(pos, pos + c);
  adjacent_difference(pos, pos + c, dif);
  partial_sort(dif + 1, dif + m, dif + c, greater<int>());
  ans = pos[c - 1] - pos[0] + 1;
  for_each(dif + 1, dif + m, [](int t) { ans -= (t - 1); });
  cout << ans;
  return 0;
}