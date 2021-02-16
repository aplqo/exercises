#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 3e5;

unsigned int str[maxn * 2 + 109];

unsigned int minRepresent(unsigned int str[], const unsigned int len)
{
  copy(str, str + len, str + len);
  unsigned int p1 = 0, p2 = 1, l = 0;
  while (p1 < len && p2 < len && l < len) {
    if (str[p1 + l] == str[p2 + l])
      ++l;
    else {
      if (str[p1 + l] > str[p2 + l])
        p1 += l + 1;
      else
        p2 += l + 1;
      p1 += p1 == p2;
      l = 0;
    }
  }
  return min(p1, p2);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  copy_n(istream_iterator<unsigned int>(cin), n, str);
  copy_n(str + minRepresent(str, n), n,
         ostream_iterator<unsigned int>(cout, " "));
  cout.put('\n');
  return 0;
}