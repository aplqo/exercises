#include <iostream>
using namespace std;
int main()
{
  int n, m, ans = 0;
  int num[100];
  cin >> n;
  for (int i = 0; i < n; i++)
    cin >> num[i];
  cin >> m;
  for (int i = 0; i < n; i++)
    if (num[i] == m) ans++;
  cout << ans;
  return 0;
}
