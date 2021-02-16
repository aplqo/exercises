#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
using res_t = unsigned long long;
constexpr res_t mod = 1e9 + 7;

res_t QuickPow(res_t a, res_t ex)
{
  res_t ret = 1;
  a %= mod;
  for (res_t t = 1; ex; t <<= 1) {
    if (ex & t) {
      ret = (ret * a) % mod;
      ex ^= t;
    }
    a = (a * a) % mod;
  }
  return ret;
}
int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  res_t n, k, ans = 1;
  cin >> n >> k;
  ans = QuickPow(n - k, n - k);
  ans = (ans * QuickPow(k, k - 1)) % mod;
  cout << ans << endl;
  return 0;
}