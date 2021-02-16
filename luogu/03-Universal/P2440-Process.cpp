#ifdef APDEBUG
#define APINPROG
#include "time.cpp"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
typedef unsigned int numType;
const int maxn = 100005;

numType wod[maxn], k;
unsigned int n;

bool test(numType d)
{
  numType ans = 0;
  for (numType *i = wod; i < wod + n; ++i)
    ans += (*i) / d;
  return ans >= k;
}
int main()
{
  cin >> n >> k;
  for (numType *i = wod; i < wod + n; ++i)
    cin >> *i;
  numType l = 0, r = *max_element(wod, wod + n);
  while (l <= r) {
    numType mid = (l + r) >> 1;
    if (mid == 0) {
      r = l;
      break;
    }
    if (test(mid))
      l = mid + 1;
    else
      r = mid - 1;
  }
  cout << r;
  return 0;
}
