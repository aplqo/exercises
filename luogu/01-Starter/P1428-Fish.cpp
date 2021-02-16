#include <iostream>
using namespace std;
int main()
{
  int num[100], n, ans[100] = {0};
  cin >> n;
  for (int i = 0; i < n; i++)
    cin >> num[i];
  for (int i = i; i < n; i++)
    for (int j = 0; j < i; j++)
      if (num[j] < num[i]) ans[i]++;
  for (int i = 0; i < n; i++)
    cout << ans[i] << ' ';
  return 0;
}
