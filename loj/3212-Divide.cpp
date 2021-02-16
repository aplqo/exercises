#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 4e7, maxm = 1e5;
typedef __uint128_t data_t;

unsigned long long sum[maxn + 10];

void readInput(istream& is, const unsigned int n, unsigned long long dat[])
{
  static constexpr unsigned int msk = (1 << 30) - 1;
  static unsigned int b[maxn + 10];
  static struct Tuple {
    unsigned int p, l, r;
  } tup[maxm + 10];
  unsigned int m;
  {
    unsigned long long x, y, z;
    is >> x >> y >> z >> b[1] >> b[2] >> m;
    for (unsigned int i = 3; i <= n; ++i)
      b[i] = (((x * b[i - 1]) & msk) + ((y * b[i - 2]) & msk) + z) & msk;
  }
  for (Tuple* i = tup + 1; i < tup + m + 1; ++i)
    is >> i->p >> i->l >> i->r;
  tup[0].p = 0;
  const Tuple* ptr = tup + 1;
  for (unsigned int i = 1; i <= n; ++i) {
    while (ptr < tup + m + 1 && !((ptr - 1)->p < i && i <= ptr->p))
      ++ptr;
    dat[i] = (b[i] % (ptr->r + 1 - ptr->l)) + ptr->l;
  }
}
template <class T>
void writeUInt(ostream& os, T val)
{
  static char buf[40];
  char* pos = buf;
  do {
    *(pos++) = (val % 10) + '0';
    val /= 10;
  } while (val);
  while (pos > buf)
    os.put(*(--pos));
}

data_t dp(const unsigned int n)
{
  static unsigned int q[maxn + 10], lst[maxn + 10];
  const auto segSum = [](const unsigned int i) {
    return 2 * sum[i] - sum[lst[i]];
  };
  unsigned int *front = q, *back = q;
  *(back++) = 0;
  for (unsigned int i = 1; i <= n; ++i) {
    while (back - front > 1 &&
           segSum(*(front + 1)) <= sum[i])  // (pre,front] > (front,i]
      ++front;
    lst[i] = *front;
    while (back - front >= 1 && segSum(*(back - 1)) >= segSum(i))
      --back;
    *(back++) = i;
  }
  data_t ret = 0;
  for (unsigned int pos = n; pos; pos = lst[pos])
    ret += static_cast<data_t>(sum[pos] - sum[lst[pos]]) *
           (sum[pos] - sum[lst[pos]]);
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  ifstream in("partition.in");
  ofstream ans("partition.out");
  unsigned int n;
  {
    unsigned int type;
    in >> n >> type;
    if (type)
      readInput(in, n, sum);
    else
      copy_n(istream_iterator<unsigned int>(in), n, sum + 1);
  }
  partial_sum(sum, sum + n + 1, sum);
  writeUInt(ans, dp(n));
  ans.put('\n');
  return 0;
}