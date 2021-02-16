#include <algorithm>
#include <iostream>
using namespace std;
int main()
{
  int n;
  long long num, dif[3000] = {0}, t;
  cin >> n >> num;
  if (n == 1) goto j;
  for (int i = 1; i < n; i++) {
    cin >> t;
    dif[i] = t > num ? t - num : num - t;
    num = t;
  }
  sort(dif + 1, dif + n);
  for (int i = 0; i < n - 2; i++) {
    if (dif[i + 1] - dif[i] != 1) {
      cout << "Not jolly";
      return 0;
    }
  }
j:
  cout << "Jolly";
  return 0;
}
