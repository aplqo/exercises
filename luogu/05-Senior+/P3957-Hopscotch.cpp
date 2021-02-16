#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <climits>
#include <cstdio>
#include <deque>
#include <iterator>
#include <numeric>
using namespace std;
const int maxn = 500000;
constexpr long long ninf = LLONG_MIN / 2;

template <class T>
T read()
{
  T ret = 0;
  bool sig = false;
  char c = getchar();
  while (!isdigit(c) && c != '-')
    c = getchar();
  if (c == '-') {
    sig = true;
    c = getchar();
  }
  while (isdigit(c)) {
    ret = ret * 10 + c - '0';
    c = getchar();
  }
  return sig ? -ret : ret;
}
class mqueue {
 public:
  void push(const long long p, const long long val)
  {
    while (l <= r && r->second < val)
      --r;
    *(++r) = make_pair(p, val);
  }
  void pop(const long long p)
  {
    while (l <= r && l->first < p)
      ++l;
  }
  long long front() const { return l <= r ? l->second : ninf; }
  void clear()
  {
    l = dat;
    r = dat - 1;
  }

 private:
  using val_type = pair<long long, long long>;
  val_type dat[maxn + 1], *l = dat, *r = dat - 1;
};
long long s[maxn + 1], x[maxn + 1];
unsigned int n;
long long d;

bool test(const long long g, const long long k)
{
  static mqueue q;
  static long long f[maxn + 1] = {};
  unsigned int p = 0;
  q.clear();
  const long long tl = d + g, tr = g >= d ? 1 : d - g;
  for (unsigned int i = 1; i <= n; ++i) {
    for (const long long pl = x[i] - tr; p < i && x[p] <= pl; ++p)
      q.push(x[p], f[p]);
    q.pop(x[i] - tl);
    f[i] = q.front() + s[i];
  }
  return *max_element(f + 1, f + 1 + n) >= k;
}
inline long long maxDis(const unsigned int n)
{
  long long ret = 0;
  unsigned int lst = 0;
  for (unsigned int i = 1; i <= n; ++i) {
    ret = max(ret, x[i] - x[lst]);
    if (s[i] > 0) lst = i;
  }
  return ret;
}
long long solve(const unsigned int n, const long long k)
{
  if (accumulate(s, s + n + 1, 0ll, [](long long acc, long long v) {
        return acc + max(v, 0ll);
      }) < k)
    return -1;
  long long l = 0, r = maxDis(n);
  while (l <= r) {
    const long long m = (l + r) / 2;
    if (test(m, k))
      r = m - 1;
    else
      l = m + 1;
  }
  return l;
}
int main()
{
  n = read<unsigned int>();
  d = read<long long>();
  const long long k = read<long long>();
  for (unsigned int i = 1; i <= n; ++i) {
    x[i] = read<long long>();
    s[i] = read<long long>();
  }
  printf("%lld\n", solve(n, k));
  return 0;
}