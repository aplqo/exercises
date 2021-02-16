#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
using namespace std;
using num_t = unsigned int;
const int maxn = 1e5, maxm = 1e5;

num_t sum[maxn + 10];
unsigned int n, m;

num_t dfs(const unsigned int beg, const unsigned int grp)
{
  num_t ret = 0;
  if (grp == 1) return sum[n] - sum[beg];
  for (unsigned int i = beg + 1; i <= n; ++i)
    ret = max(ret, min(sum[i] - sum[beg], dfs(i, grp - 1)));
  return ret;
}

int main(int argc, char* argv[])
{
  ifstream in(argv[1]), out(argv[2]);
  in >> n >> m;
  for (num_t* i = sum + 1; i < sum + 1 + n; ++i)
    in >> *i;
  partial_sum(sum + 1, sum + 1 + n, sum + 1);
  in.close();
  num_t ans = dfs(0, m), res;
  out >> res;
  if (ans != res) {
    cout << "Test failed! Read=" << res << " Expect=" << ans << endl;
    return 1;
  }
  else {
    cout << "Test passed" << endl;
    return 0;
  }
}