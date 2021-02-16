#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
constexpr unsigned int mod = 9973;

inline unsigned int QuickPow(unsigned int a, unsigned int ex)
{
  unsigned int ret = 1;
  a %= mod;
  for (unsigned int i = 1; ex; i <<= 1) {
    if (ex & i) {
      ret = (ret * a) % mod;
      ex ^= i;
    }
    a = (a * a) % mod;
  }
  return ret;
}

int main()
{
  ios_base::sync_with_stdio(false);
  unsigned int t;
  cin >> t;
  for (unsigned int i = 0; i < t; ++i) {
    unsigned int a, b, ans = 1;
    cin >> a >> b;
    cout << (a * QuickPow(b, mod - 2)) % mod << endl;
  }
  return 0;
}