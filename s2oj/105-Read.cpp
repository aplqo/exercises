#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cctype>
#include <cstdio>
#include <iostream>
using namespace std;
const unsigned int maxn = 3e6;

char inbuf[maxn * 67 + 1000], *inpos = inbuf;

unsigned long long readUInt()
{
  unsigned long long ret = 0;
  char c = *(inpos++);
  while (!isdigit(c))
    c = *(inpos++);
  while (isdigit(c)) {
    ret = ret * 10 + (c - '0');
    c = *(inpos++);
  }
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  cin.read(inbuf, sizeof(inbuf));
  unsigned long long ret = 0;
  for (unsigned int i = maxn; i; --i)
    ret ^= readUInt();
  cout << ret << "\n";
  return 0;
}