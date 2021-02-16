#include <algorithm>
#include <iostream>
using namespace std;
const int maxm = 5005;

struct farm {
  unsigned int p, a;
} f[maxm];

unsigned long long ans;
int main()
{
  unsigned int n, m;
  cin >> n >> m;
  for (farm *i = f; i < f + m; i++)
    cin >> i->p >> i->a;
  sort(f, f + m,
       [](const farm &a, const farm &b) -> bool { return a.p < b.p; });
  for (farm *i = f; i < f + m && n; i++) {
    unsigned int t = min(i->a, n);
    ans += t * i->p;
    n -= t;
  }
  cout << ans;
  return 0;
}