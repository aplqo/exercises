#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
const unsigned int maxn = 3000;

bool mat[maxn + 10][maxn + 10];
bool invalid[maxn + 10];

void mark(const unsigned int n)
{
  for (unsigned int i = 1; i <= n; ++i)
    if (!invalid[i]) {
      for (unsigned int j = i + 1; j <= n && !invalid[i]; ++j)
        if (!invalid[j] && !mat[i][j]) invalid[i] = invalid[j] = true;
    }
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int a, b;
    cin >> a >> b;
    mat[a][b] = mat[b][a] = true;
  }
  mark(n);
  for (unsigned int i = 1, c = n / 3; i <= n && c; ++i)
    if (!invalid[i]) {
      cout << i << " ";
      --c;
    }
  cout << "\n";
  return 0;
}