/* Luogu team T129700: equal*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cctype>
#include <cstring>
#include <iostream>
using namespace std;
const int maxl = 100000;

char str1[maxl + 10], str2[maxl + 10];
char tmp[maxl + 10];

void Convert(char* out)
{
  size_t len = 0;
  const size_t l = strlen(tmp);
  for (unsigned int i = 0; i < l; ++i)
    if (!isspace(tmp[i])) out[len++] = tolower(tmp[i]);
  out[len] = '\0';
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  cin.getline(tmp, maxl + 10);
  Convert(str1);
  cin.getline(tmp, maxl + 10);
  Convert(str2);
  cout << (!strcmp(str1, str2) ? "YES" : "NO") << endl;
  return 0;
}
