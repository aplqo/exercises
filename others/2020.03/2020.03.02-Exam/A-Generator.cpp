#include <algorithm>
#include <cstring>
#include <fstream>
#include <numeric>
#include <random>
#include <set>
#include <utility>
using namespace std;
const unsigned int maxn = 1e5, maxm = 1e5;

set<pair<unsigned int, unsigned int>> vis;
unsigned int v[maxn + 10];
mt19937_64 rnd;
struct limit {
  static void Create(unsigned int f, unsigned int t)
  {
    cur->from = f + 1;
    cur->to = t;
    cur->val = v[t] - v[f];
    vis.insert(make_pair(f, t));
    ++cur;
  }
  static limit* cur;
  unsigned int from, to;
  unsigned int val;
} l[maxm + 10];
limit* limit::cur = l;

inline unsigned int GetRandom(const unsigned int a, const unsigned int b)
{
  uniform_int_distribution<unsigned int> dis(a, b);
  return dis(rnd);
}
inline void CreateAns(const unsigned int n)
{
  generate(v + 1, v + 1 + n, []() { return GetRandom(0, 1); });
  partial_sum(v + 1, v + 1 + n, v + 1);
}
inline unsigned int CreateRangeC(
    const unsigned int n, const unsigned int m)  // create contious ranges
{
  unsigned int beg = 0, ret = 0;
  for (unsigned int i = 0; i < m - 1 && beg < n; ++i) {
    unsigned int end;
    do {
      end = GetRandom(beg + 1, n);
    } while (vis.find(make_pair(beg, end)) != vis.end());
    limit::Create(beg, end);
    beg = end;
    ++ret;
  }
  if (beg < n) {
    limit::Create(beg, n);
    ++ret;
  }
  return ret;
}
inline void CreateRangeN(const unsigned int n, const unsigned int m)
{
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int beg, end;
    do {
      beg = GetRandom(0, n - 1);
      end = GetRandom(beg + 1, n);
    } while (vis.find(make_pair(beg, end)) != vis.end());
    limit::Create(beg, end);
  }
}

int main(int argc, char* argv[])
{
  rnd.seed(random_device()());
  unsigned int limn = atoi(argv[1]);
  ofstream in(argv[2]), ans(argv[3]);
  unsigned int n = GetRandom(1, limn);
  unsigned int m = GetRandom(1, min(n * n / 2, maxm));
  if (n == 1) {
    unsigned int v = GetRandom(0, 1);
    in << n << " " << 1 << endl;
    in << 1 << " " << 1 << " " << v << endl;
    in.close();
    ans << v << endl;
    ans.close();
    return 0;
  }
  in << n << " " << m << endl;

  CreateAns(n);
  m -= CreateRangeC(n, GetRandom(1, m));
  CreateRangeN(n, m);
  shuffle(l, limit::cur, rnd);

  for (limit* i = l; i < limit::cur; ++i)
    in << i->from << " " << i->to << " " << i->val << endl;
  in.close();
  ans << v[n] << endl;
  ans.close();
  return 0;
}
