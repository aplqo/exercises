#include <iostream>
using std::cin;
using std::cout;
using std::endl;
int main()
{
  int n, m, ans = 0;
  cin >> n >> m;
  for (int i = 0; i < n; i++) {
    int t;
    cin >> t;
    if (t == m) ans++;
  }
  cout << ans;
  return 0;
}
