#include <algorithm>
#include <iostream>
using namespace std;

unsigned long long ans;
unsigned long long p(int a, int b, int m)
{
  unsigned long long r = 1;
  if (a < b) swap(a, b);
  bool j[31];
  fill(j + 1, j + 31, true);
  for (int i = m; i > a; i--) {
#ifdef DEBUG
    auto t = r;
#endif
    r *= i;
#ifdef DEBUG
    if (r / i != t) cout << "Overflow with a=" << a << " b=" << b << endl;
#endif
    for (int k = b; k > 0; k--)
      if (j[k] && (r % k == 0)) {
        r /= k;
        j[k] = false;
      }
  }
  return r;
}
int main()
{
  int n, m;
  cin >> n >> m;
  for (int i = 0; i <= m; i++)
    if ((m - 2 * i) % n == 0) ans += p(i, m - i, m);
  cout << ans;
  return 0;
}
