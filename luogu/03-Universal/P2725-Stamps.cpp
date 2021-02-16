#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;

int stamps[200], f[10000 * 200];

int main()
{
  int n, k;
  cin >> n >> k;
  for (int *i = stamps; i < stamps + k; i++) {
    cin >> *i;
    f[*i] = 1;
  }
  fill(f + 1, f + 10000 * 200, INT_MAX);
  for (int i = 1;; i++) {
    for (int *j = stamps; j < stamps + k; j++)
      if (*j <= i) f[i] = min(f[i], f[i - *j] + 1);
    if (f[i] > n) {
      cout << i - 1;
      return 0;
    }
  }
  return 0;
}
