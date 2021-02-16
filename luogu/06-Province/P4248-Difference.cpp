#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cstring>
#include <iostream>
#include <numeric>
#include <utility>
using namespace std;
const unsigned int maxn = 500000;

char str[maxn + 1];
unsigned int sa[maxn + 1], Rank[maxn + 1], hight[maxn + 1];
struct {
  unsigned int l = 0, r = 0;
} nod[maxn + 1];

void radixSort(const size_t l, unsigned int rk[], unsigned int tp[])
{
  static unsigned int buk[maxn + 1];
  memset(buk, 0, sizeof(buk));
  for (unsigned int i = 1; i <= l; ++i)
    ++buk[rk[tp[i]]];
  partial_sum(buk, buk + max<size_t>(l, 26) + 1, buk);
  for (unsigned int i = l; i; --i)
    sa[buk[rk[tp[i]]]--] = tp[i];
}
void buildSA(const char str[], const size_t l)
{
  static unsigned int tmp[maxn + 1];
  unsigned int *rk = Rank, *tp = tmp;
  for (unsigned int i = 1; i <= l; ++i) {
    rk[i] = str[i] + 1 - 'a';
    tp[i] = i;
  }
  radixSort(l, rk, tp);
  for (unsigned int i = 1, p = 0; p < l; i <<= 1) {
    {
      unsigned int e = 1;
      for (unsigned int j = l + 1 - i; j <= l; ++j)
        tp[e++] = j;
      for (unsigned int j = 1; j <= l; ++j)
        if (sa[j] > i) tp[e++] = sa[j] - i;
    }
    radixSort(l, rk, tp);
    swap(rk, tp);
    rk[sa[1]] = p = 1;
    for (unsigned int j = 2; j <= l; ++j)
      if (tp[sa[j]] == tp[sa[j - 1]] && tp[sa[j] + i] == tp[sa[j - 1] + i])
        rk[sa[j]] = p;
      else
        rk[sa[j]] = ++p;
  }
  for (unsigned int i = 1; i <= l; ++i)
    Rank[sa[i]] = i;
  for (unsigned int i = 1, k = 0; i <= l; ++i) {
    if (k) --k;
    while (str[i + k] == str[sa[Rank[i] - 1] + k])
      ++k;
    hight[Rank[i]] = k;
  }
}
unsigned int build(const size_t l)
{
  static unsigned int stk[maxn + 1], *top = stk;
  *(top++) = 1;
  unsigned int root = 1;
  for (unsigned int i = 2; i <= l; ++i) {
    unsigned int* k = top;
    while (k > stk && hight[*(k - 1)] > hight[i])
      --k;
    if (k > stk)
      nod[*(k - 1)].r = i;
    else
      root = i;
    if (k < top) nod[i].l = *k;
    top = k;
    *(top++) = i;
  }
  return root;
}
pair<unsigned int, unsigned long long> dfs(const unsigned int x)
{
  if (!x) return make_pair(0, 0);
  const auto [sizl, ansl] = dfs(nod[x].l);
  const auto [sizr, ansr] = dfs(nod[x].r);
  const unsigned long long ra = 1ull * sizl * sizr + sizl + sizr + 1;
  return make_pair(sizl + sizr + 1, 2 * ra * hight[x] + ansl + ansr);
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  cin >> (str + 1);
  const size_t l = strlen(str + 1);
  buildSA(str, l);
  unsigned long long ans =
      1ull * (l - 1) * (1 + l) * l / 2 - dfs(build(l)).second;
  cout << ans << endl;
  return 0;
}