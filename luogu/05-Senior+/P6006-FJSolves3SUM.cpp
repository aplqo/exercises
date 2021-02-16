#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <fstream>
#include <iostream>
using namespace std;
const unsigned int maxn = 5000, maxa = 1e6;

int buk[maxa * 4 + 100], a[maxn + 10];
int* const mid = buk + maxa * 2 + 50;
unsigned long long cnt[maxn + 1][maxn + 1];

inline int& index(const int x) { return *(mid + x); }
static void initCnt(const unsigned int n)
{
  for (unsigned int i = 3; i <= n; ++i) {
    ++index(a[i - 1]);
    for (unsigned int j = i - 2; j; --j) {
      cnt[j][i] = index(-(a[i] + a[j]));
      ++index(a[j]);
    }
    for (unsigned int j = 1; j <= i; ++j)
      index(a[j]) = 0;
  }
}
static void initSum(const unsigned int n)
{
  for (unsigned int i = 1; i <= n; ++i)
    for (unsigned int j = 1; j <= n; ++j)
      cnt[i][j] += cnt[i - 1][j] + cnt[i][j - 1] - cnt[i - 1][j - 1];
}
inline unsigned long long sum(const unsigned int a, const unsigned int b)
{
  return cnt[b][b] + cnt[a - 1][a - 1] - cnt[a - 1][b] - cnt[b][a - 1];
}

int main()
{
#if !defined APTEST && !defined APDEBUG
  ios_base::sync_with_stdio(false);
  ifstream in("threesum.in");
  ofstream out("threesum.out");
#else
  istream& in = cin;
  ostream& out = cout;
#endif
  unsigned int n, q;
  in >> n >> q;
  for (int* i = a + 1; i < a + 1 + n; ++i)
    in >> *i;
  initCnt(n);
  initSum(n);
  for (unsigned int i = 0; i < q; ++i) {
    unsigned int a, b;
    in >> a >> b;
    out << sum(a, b) << "\n";
  }
  return 0;
}