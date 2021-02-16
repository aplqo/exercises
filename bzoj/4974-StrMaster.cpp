#ifdef APDEBUG
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const int maxn = 100000;

int nxt[maxn + 10];
char s[maxn + 10];
bool neq[27];

int main()
{
  ios_base::sync_with_stdio(false);
  int n;
  cin >> n;
  for (int i = 1; i <= n; ++i) {
    cin >> nxt[i];
    nxt[i] = i - nxt[i];
  }
  for (int i = 2; i <= n; ++i) {
    if (nxt[i])
      s[i] = s[nxt[i]];
    else {
      int j = nxt[i - 1];
      while (j) {
        neq[s[j + 1]] = true;
        j = nxt[j];
      }
      if (!j) neq[0] = true;
      s[i] = find(neq, neq + 26, false) - neq;
      fill(neq, neq + 26, false);
    }
  }
  for (int i = 1; i <= n; ++i)
    cout << char(s[i] + 'a');
  return 0;
}