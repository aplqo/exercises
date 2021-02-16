#ifdef APDEBUG
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
using namespace std;
const int maxn = 1005;

struct isl {
  int a, b;
} is[maxn];
int n, d, ans;

int main()
{
  ios_base::sync_with_stdio(false);
  cin >> n >> d;
  for (isl* i = is; i < is + n; ++i) {
    int x, y, dx;
    cin >> x >> y;
    if (y > d) {
      cout << "-1" << endl;
      return 0;
    }
    dx = sqrt(d * d - y * y);
    i->a = x - dx;
    i->b = x + dx;
  }
  sort(is, is + n,
       [](const isl& a, const isl& b) -> bool { return a.b < b.b; });
  int c = INT_MIN;
  for (isl* i = is; i < is + n; ++i) {
    if (i->a <= c) continue;
    ++ans;
    c = i->b;
  }
  cout << ans;
  return 0;
}