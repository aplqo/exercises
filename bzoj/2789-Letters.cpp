#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;
const unsigned int maxn = 1e6;

char orig[maxn + 10], ex[maxn + 10];
unsigned int val[maxn + 10];

unsigned long long msort(unsigned int* const l, unsigned int* const r)
{
  static unsigned int tmp[maxn + 10];
  if (r - l < 2) return 0;
  unsigned int* const mid = l + ((r - l) >> 1);
  unsigned long long ret = msort(l, mid) + msort(mid, r);
  copy(l, r, tmp);
  unsigned int *pl = tmp, *const el = tmp + (mid - l);
  unsigned int *pr = el, *const er = tmp + (r - l);
  for (unsigned int* i = l; i < r; ++i) {
    if (pl == el || (pr != er && *pr < *pl)) {
      *i = *(pr++);
      ret += el - pl;
    }
    else
      *i = *(pl++);
  }
  return ret;
}
static void buildSeq(const unsigned int n)
{
  queue<unsigned int> q[26];
  for (const char* i = ex; i < ex + n; ++i)
    q[*i - 'A'].push(i - ex);
  for (unsigned int i = 0; i < n; ++i) {
    const unsigned int ch = orig[i] - 'A';
    val[i] = q[ch].front();
    q[ch].pop();
  }
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n >> orig >> ex;
  buildSeq(n);
  cout << msort(val, val + n) << "\n";
  return 0;
}