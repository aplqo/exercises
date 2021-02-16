#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const int maxn = 1e5 + 5;

struct range {
  int s, t;
} r[maxn];
int ans, n;

int main()
{
  ios_base::sync_with_stdio(false);
  cin >> n;
  for (range* i = r; i < r + n; ++i)
    cin >> i->s >> i->t;
  sort(r, r + n, [](const range& a, const range& b) -> bool {
    return a.s == b.s ? a.t > b.t : a.s < b.s;
  });
  int lr = 0;
  for (range* i = r; i < r + n; ++i) {
    if (i->s > lr) {
      lr = i->t;
      ans += i->t - i->s + 1;
    }
    else if (i->t > lr) {
      ans += i->t - lr;
      lr = i->t;
    }
  }
  cout << ans;
  return 0;
}