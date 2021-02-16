#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;
const int maxn = 1e6, maxt = 1000;

char str[(maxn + 10) * 2];
bool ans[maxn * 2 + 10];
size_t l;

inline void pre(char str[], const size_t len)
{
  static int nxt[(maxn + 10) * 2];
  static bool cval[maxn * 2 + 10];
  nxt[0] = nxt[1] = 0;
  int j = 0;
  for (int i = 1; i <= len; ++i) {
    while (j && str[j + 1] != str[i + 1])
      j = nxt[j];
    if (str[j + 1] == str[i + 1]) ++j;
    nxt[i + 1] = j;
  }

  const size_t il = min(len - (str[1] == str[len]), l - 1);
  fill(cval, cval + il + 1, true);
  for (unsigned int j = nxt[len]; j; j = nxt[j]) {
    const size_t t = len - j;
    for (size_t i = 0; i + 1 <= il; i += t)
      cval[i + 1] = false;
  }
  for (unsigned int i = 1; i <= il; ++i)
    ans[l - i] |= cval[i];
}
void solve(int id)
{
  cin >> (str + 1);
  if (cin.fail()) exit(0);
  l = strlen(str + 1);
  fill(ans + 1, ans + l + 1, false);
  ans[0] = true;
  if (l == 1) goto pr;
  str[0] = str[l];
  copy(str + 1, str + 1 + l, str + 1 + l);

  {
    ans[l - 1] = true;
    int lst = 0;
    for (int i = 1; i <= l * 2; ++i)
      if (str[i] == str[i - 1]) {
        ans[0] = false;
        pre(str + lst - 1, i - lst);
        lst = i;
      }
    if (lst < l * 2) pre(str + lst - 1, l * 2 + 1 - lst);
  }

pr:
  cout << "Case " << id << ": ";
  for (int i = 0; i < l; ++i)
    cout.put(ans[i] ? '1' : '0');
  cout << endl;
}
int main()
{
  ios_base::sync_with_stdio(false);
  int i = 0;
  while (true)
    solve(++i);
  return 0;
}