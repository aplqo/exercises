#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <queue>
using namespace std;
const unsigned int maxn = 5e5, maxl = 20;

struct MaxVal {
  inline bool operator<(const MaxVal& r) const { return val < r.val; }
  long long val;
  unsigned int pos;
};
class STTable {
 public:
  void init(const unsigned int n, const long long val[]);
  MaxVal max(const unsigned int l, const unsigned int r) const;

  static void getLog(const unsigned int n);

 private:
  MaxVal val[maxn + 10][maxl + 10];
  static unsigned int lg[maxn + 10];
} st;
unsigned int STTable::lg[maxn + 10];
struct Range {
  long long val;
  unsigned int leftPos, maxPos, l, r;

  inline bool operator<(const Range& r) const { return val < r.val; }
};
long long sum[maxn + 10];

void STTable::init(const unsigned int n, const long long v[])
{
  for (unsigned int i = 1; i <= n; ++i)
    this->val[i][0] = MaxVal{v[i], i};
  for (unsigned int i = 1; i <= lg[n] + 1; ++i)
    for (unsigned int j = 1; j + (1 << i) <= n + 1; ++j)
      val[j][i] = std::max(val[j][i - 1], val[j + (1 << (i - 1))][i - 1]);
}
MaxVal STTable::max(const unsigned int l, const unsigned int r) const
{
  const unsigned int d = lg[r + 1 - l];
  return std::max(val[l][d], val[r + 1 - (1 << d)][d]);
}
void STTable::getLog(const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i)
    lg[i] = lg[i >> 1] + 1;
}

static void init(const unsigned int n)
{
  partial_sum(sum + 1, sum + n + 1, sum + 1);
  STTable::getLog(n);
  st.init(n, sum);
}
inline Range maxRange(const unsigned int lpos, const unsigned int rl,
                      const unsigned int rr)
{
  const MaxVal v = st.max(rl, rr);
  return Range{v.val - sum[lpos - 1], lpos, v.pos, rl, rr};
}
long long solve(const unsigned int l, const unsigned int r,
                const unsigned int n, const unsigned int k)
{
  priority_queue<Range> q;
  long long ret = 0;
  for (unsigned int i = 1; i + l <= n + 1; ++i)
    q.push(maxRange(i, i + l - 1, min(i + r - 1, n)));
  for (unsigned int i = 0; i < k; ++i) {
    const Range t = q.top();
    q.pop();
    ret += t.val;
    if (t.maxPos > t.l) q.push(maxRange(t.leftPos, t.l, t.maxPos - 1));
    if (t.maxPos < t.r) q.push(maxRange(t.leftPos, t.maxPos + 1, t.r));
  }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, k, l, r;
  cin >> n >> k >> l >> r;
  copy_n(istream_iterator<long long>(cin), n, sum + 1);
  init(n);
  cout << solve(l, r, n, k) << "\n";
  return 0;
}