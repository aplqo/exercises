#ifdef APTEST
#include "debug_tools/program/h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
using namespace std;
const unsigned int maxn = 1e6, maxm = 1e6, maxa = 1e6;

vector<unsigned int> nxt[maxa + 10];
unsigned int tmp[maxm + 10];

bool checkSeq(const unsigned int* beg, const unsigned int* end)
{
  if (nxt[*beg].empty()) return false;
  unsigned int pos = nxt[*beg].front();
  for (const unsigned int* i = beg + 1; i < end; ++i) {
    const auto nit = upper_bound(nxt[*i].cbegin(), nxt[*i].cend(), pos);
    if (nit == nxt[*i].cend()) return false;
    pos = *nit;
  }
  return true;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, q;
  cin >> n;
  for (unsigned int i = 0; i < n; ++i) {
    unsigned int a;
    cin >> a;
    nxt[a].push_back(i);
  }
  cin >> q;
  for (unsigned int i = 0; i < q; ++i) {
    unsigned int m;
    cin >> m;
    copy_n(istream_iterator<unsigned int>(cin), m, tmp);
    cout << (checkSeq(tmp, tmp + m) ? "TAK" : "NIE") << "\n";
  }
  return 0;
}
