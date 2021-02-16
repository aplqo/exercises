#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <cstdint>
#include <iostream>
using namespace std;
typedef int_fast32_t num;
const int maxn = 100005, maxl = 30;

num a[maxn];
int f[maxn], t[maxl + 5];

int main()
{
  ios_base::sync_with_stdio(false);
  int n;
  cin >> n;
  for (num* i = a; i < a + n; ++i)
    cin >> *i;
  for (int i = 0; i < n; ++i) {
    num v = 1 << maxl;
    for (int j = maxl; j >= 0; --j, v >>= 1)
      if (a[i] & v) f[i] = max(f[i], t[j] + 1);
    v = 1 << maxl;
    for (int j = maxl; j >= 0; --j, v >>= 1)
      if (a[i] & v) t[j] = max(t[j], f[i]);
  }
  cout << *max_element(f, f + n) << endl;
  return 0;
}