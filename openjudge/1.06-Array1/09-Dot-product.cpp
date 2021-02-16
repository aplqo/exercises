#include <iostream>
#define rep(n) for (int i = 0; i < n; i++)
using namespace std;
int main()
{
  int n, a[1000], b[1000];
  long long ans = 0;
  cin >> n;
  rep(n) cin >> a[i];
  rep(n) cin >> b[i];
  rep(n) ans += a[i] * b[i];
  cout << ans;
  return 0;
}
