#include <iostream>
using namespace std;
bool trees[10000];
int ans;
int main()
{
  int n, m;
  int l, r;
  cin >> n >> m;
  for (int i = 0; i < m; i++) {
    cin >> l >> r;
    for (int j = l; j <= r; j++)
      trees[j] = true;
  }
  for (int i = 0; i <= n; i++)
    if (!trees[i]) ans++;
  cout << ans;
  return 0;
}
