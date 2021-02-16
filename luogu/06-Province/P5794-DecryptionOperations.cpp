#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <utility>
using namespace std;
const unsigned int maxn = 200000;

pair<unsigned int, unsigned int> seq[maxn + 10];

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (auto i = seq; i < seq + n + 1; ++i) {
    cin >> i->first;
    i->second = i - seq;
  }
  sort(seq, seq + n + 1);
  {
    unsigned int pos = 0;
    for (unsigned int i = 0; i < n; ++i) {
      pos = seq[pos].second;
      cout << seq[pos].first << " ";
    }
  }
  return 0;
}