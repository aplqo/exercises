#include <algorithm>
#include <iostream>
using namespace std;
const int maxn = 1005;
typedef unsigned int numType;

numType a[maxn][maxn];

int main()
{
  ios_base::sync_with_stdio(false);
  int r;
  cin >> r;
  for (int i = 1; i <= r; ++i)
    for (int j = 1; j <= i; ++j)
      cin >> a[i][j];
  for (int i = r; i > 0; --i)
    for (int j = 1; j <= i; ++j)
      a[i][j] = max(a[i + 1][j], a[i + 1][j + 1]) + a[i][j];
  cout << a[1][1];
  return 0;
}