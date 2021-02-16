#include <iostream>
using namespace std;
bool trees[10000];
int main()
{
  int l, m;
  int a, b, ans = 0;
  cin >> l >> m;
  for (int i = 0; i < m; i++) {
    cin >> a >> b;
    for (int j = a; j <= b; j++)
      trees[j] = true;
  }
  for (int i = 0; i <= l; i++)
    if (!trees[i]) ans++;
  cout << ans;
  return 0;
}
