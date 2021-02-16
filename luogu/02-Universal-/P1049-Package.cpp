#ifdef APDEBUG
#define APINPROG
#define COLOR
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const int maxn = 35, maxv = 20005;

int ans[maxv][maxn], v, n, obj[maxn];

int main()
{
  ios_base::sync_with_stdio(false);
  cin >> v >> n;
  for_each(obj + 1, obj + 1 + n, [](int &i) -> void { cin >> i; });
  for (int vi = 1; vi <= v; ++vi) {
    ans[vi][0] = vi;
    for (int oi = 1; oi <= n; ++oi)
      if (obj[oi] > vi)
        ans[vi][oi] = ans[vi][oi - 1];
      else
        ans[vi][oi] = min(ans[vi][oi - 1], ans[vi - obj[oi]][oi - 1]);
  }
  cout << *min_element(ans[v], ans[v] + n + 1);
  return 0;
}