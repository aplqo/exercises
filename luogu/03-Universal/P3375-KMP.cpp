#ifdef APDEBUG
#include "debug_tools/program.h"
#endif
#include <cstring>
#include <iostream>
using namespace std;
const int maxn = 1e6;

char s1[maxn + 10], s2[maxn + 10];
int l1, l2, nxt[maxn + 10];

int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cin >> (s1 + 1) >> (s2 + 1);
  l1 = strlen(s1 + 1);
  l2 = strlen(s2 + 1);

  int j = 0;
  for (int i = 1; i <= l2; ++i) {
    while (j && s2[i + 1] != s2[j + 1])
      j = nxt[j];
    if (s2[j + 1] == s2[i + 1]) ++j;
    nxt[i + 1] = j;
  }

  int k = 0;
  for (int i = 1; i <= l1; ++i) {
    while (k && s1[i] != s2[k + 1])
      k = nxt[k];
    if (s1[i] == s2[k + 1]) ++k;
    if (k == l2) {
      cout << i - k + 1 << endl;
      k = nxt[k];
    }
  }
  for (int i = 1; i <= l2; ++i)
    cout << nxt[i] << " ";
  return 0;
}