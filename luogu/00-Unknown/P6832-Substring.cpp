#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iostream>
using namespace std;

unsigned int cnt[26];

void count()
{
  char c = getchar();
  while (!isalpha(c))
    c = getchar();
  while (isalpha(c)) {
    ++cnt[c - 'a'];
    c = getchar();
  }
}
int main()
{
  count();
  cout << *max_element(cnt, cnt + 26) << "\n";
  return 0;
}