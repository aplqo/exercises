#include <algorithm>
#include <iostream>
using namespace std;
const int maxn = 100005;

unsigned long long hos[maxn];
unsigned int n, c;

bool test(unsigned long long d)
{
  unsigned long long num = 1, lst = hos[0];
  for (unsigned long long *i = hos + 1; i < hos + n; i++) {
    if (*i - lst < d) continue;
    lst = *i;
    num++;
  }
  return num >= c;
}
int main()
{
  cin >> n >> c;
  for (unsigned long long *i = hos; i < hos + n; i++)
    cin >> *i;
  sort(hos, hos + n);
  unsigned long long l = 1, r = hos[n - 1] - hos[0];
  while (l <= r) {
    unsigned long long mid = (l + r) >> 1;
    if (test(mid))
      l = mid + 1;
    else
      r = mid - 1;
  }
  cout << r;
  return 0;
}
