#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
using num_t = unsigned int;
#define rebase(v, p, o) ((v) + ((p) - (o)))
const int maxn = 1e5;
constexpr num_t mod = 99999997;

unsigned int a1[maxn + 10], a2[maxn + 10];
unsigned int tmp[maxn + 10], num[maxn + 10];
num_t ans = 0;

inline void discretize(unsigned int a[], const unsigned int n)
{
  copy(a, a + n, tmp);
  sort(tmp, tmp + n);
  unsigned int* e = unique(tmp, tmp + n);
  for (unsigned int i = 0; i < n; ++i) {
    unsigned int v = lower_bound(tmp, e, a[i]) - tmp;
    a[i] = v;
    num[v] = i;
  }
}
void MergeSort(unsigned int* l, unsigned int* r)
{
  if (r - l <= 1) return;
  unsigned int* m = l + (distance(l, r) >> 1);
  MergeSort(l, m);
  MergeSort(m, r);
  copy(l, r, rebase(a2, l, tmp));
  unsigned int *cm = rebase(a2, m, tmp), *cr = rebase(a2, r, tmp);
  unsigned int *i = rebase(a2, l, tmp), *j = cm;
  for (unsigned int* k = l; k < r; ++k) {
    if (i >= cm)
      (*k = *j), ++j;
    else if (j >= cr)
      (*k = *i), ++i;
    else if (*i > *j) {
      *k = *j;
      ++j;
      ans = (ans + distance(i, cm)) % mod;
    }
    else
      (*k = *i), ++i;
  }
}

int main()
{
  ios_base::sync_with_stdio(false);
  unsigned int n;
  cin >> n;
  for_each(a1, a1 + n, [](unsigned int& i) -> void { cin >> i; });
  for_each(a2, a2 + n, [](unsigned int& i) -> void { cin >> i; });

  discretize(a2, n);
  discretize(a1, n);
  transform(a2, a2 + n, a2, [](const unsigned int i) { return num[i]; });

  MergeSort(a2, a2 + n);
  cout << ans << endl;
  return 0;
}