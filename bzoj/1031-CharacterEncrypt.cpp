#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 100000;

char str[maxn * 2 + 10];
unsigned int sa[maxn * 2 + 10], Rank[maxn * 2 + 10];

void radixSort(const size_t l, unsigned int tp[], unsigned int rk[])
{
  static unsigned int buk[maxn * 2 + 10];
  memset(buk, 0, sizeof(buk));
  for (unsigned int i = 1; i <= l; ++i)
    ++buk[rk[tp[i]]];
  partial_sum(buk, buk + maxn * 2 + 1, buk);
  for (unsigned int i = l; i; --i)
    sa[buk[rk[tp[i]]]--] = tp[i];
}
void buildSA(const size_t l)
{
  static unsigned int tmp[maxn * 2 + 10];
  unsigned int *rk = Rank, *tp = tmp;
  iota(tp + 1, tp + 1 + l, 1);
  copy(str + 1, str + 1 + l, rk + 1);
  radixSort(l, tp, rk);
  for (unsigned int i = 1, p = 0; p < l; i <<= 1) {
    {
      unsigned int e = 1;
      for (unsigned int j = l + 1 - i; j <= l; ++j)
        tp[e++] = j;
      for (unsigned int j = 1; j <= l; ++j)
        if (sa[j] > i) tp[e++] = sa[j] - i;
    }
    radixSort(l, tp, rk);
    swap(tp, rk);
    rk[sa[1]] = p = 1;
    for (unsigned int j = 2; j <= l && sa[j]; ++j)
      if (tp[sa[j]] == tp[sa[j - 1]] && tp[sa[j] + i] == tp[sa[j - 1] + i])
        rk[sa[j]] = p;
      else
        rk[sa[j]] = ++p;
  }
  for (unsigned int i = 1; i <= l; ++i)
    Rank[sa[i]] = i;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  cin >> (str + 1);
  const size_t hl = strlen(str + 1);
  copy(str + 1, str + hl + 1, str + hl + 1);
  buildSA(hl * 2);
  for (unsigned int i = 1; i <= hl * 2; ++i)
    if (sa[i] + hl - 1 < hl * 2) cout.put(str[sa[i] + hl - 1]);
  cout << endl;
  return 0;
}