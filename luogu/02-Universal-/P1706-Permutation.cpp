#include <algorithm>
#include <iostream>
using namespace std;

bool vis[10];
int n, ans[10];
void f(int* cur)
{
  if (cur == ans + n) {
    for_each(ans, cur, [](int& i) -> void {
      cout.width(5);
      cout << i;
    });
    cout << endl;
    return;
  }
  for (int i = 1; i <= n; i++)
    if (!vis[i]) {
      vis[i] = true;
      *cur = i;
      f(cur + 1);
      vis[i] = false;
    }
}
int main()
{
  cin >> n;
  f(ans);
  return 0;
}
