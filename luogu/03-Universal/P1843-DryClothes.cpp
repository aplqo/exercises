#include <algorithm>
#include <cmath>
#include <iostream>
using namespace std;
typedef unsigned int resType;
const int maxn = 500005;

unsigned int a, b, n, clo[maxn];

bool test(resType t)
{
  resType mt = t, bas = a * t;
  for (unsigned int *i = clo; i < clo + n; ++i) {
    if (*i > bas) {
      if ((*i - bas) > mt * b) return false;
      mt -= ceil(double(*i - bas) / b);
    }
  }
  return true;
}
int main()
{
  cin >> n >> a >> b;
  for_each(clo, clo + n, [](unsigned int &i) -> void { cin >> i; });
  resType l = 0, r = *max_element(clo, clo + n) / a + 1;
  while (l <= r) {
    resType mid = (l + r) >> 1;
    if (test(mid))
      r = mid - 1;
    else
      l = mid + 1;
  }
  cout << l;
  return 0;
}