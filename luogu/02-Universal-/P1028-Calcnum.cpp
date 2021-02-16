#include <iostream>
using namespace std;
int ans = 1;
int f[1001] = {1};
int main()
{
  int n;
  cin >> n;
  for (int i = 1; i <= n; i++) {
    if (i % 2 == 0)
      f[i] += f[i - 1] + f[i / 2];
    else
      f[i] += f[i - 1];
  }
  cout << f[n];
  return 0;
}
