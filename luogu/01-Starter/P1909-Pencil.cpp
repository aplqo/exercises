#include <iostream>
using namespace std;
int main()
{
  int n, num, price;
  int ans = 1e11;
  cin >> n;
  for (int i = 0; i < 3; i++) {
    cin >> num >> price;
    int p = (n / num) * price;
    if (n % num) p += price;
    if (p < ans) ans = p;
  }
  cout << ans;
  return 0;
}
