#ifdef APDEBUG
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
using namespace std;
typedef int num;
const int maxn = 50010, maxl = 20;

int lg[maxn];
template <class T, class cmp>
class table {
 public:
  void init(T val[maxn], int len)
  {
    for (int i = 0; i < len; ++i)
      dat[i][0] = val[i];
    for (int j = 1; j <= lg[len]; ++j)
      for (int i = 0; i + (1 << j) <= len; ++i)
        dat[i][j] = max(dat[i][j - 1], dat[i + (1 << j - 1)][j - 1], cmp());
  }
  T query(int l, int r)
  {
    int d = lg[r - l + 1];
    return max(dat[l][d], dat[r - (1 << d) + 1][d], cmp());
  }

 private:
  T dat[maxn][maxl];
};
// clang-format off
table<num, greater<num> > lst;
table<num, less<num> > gst;
// clang-format on
num dat[maxn];
int n, q;

inline void getLog(int v)
{
  for (int i = 2; i <= v; ++i)
    lg[i] = lg[i >> 1] + 1;
}

int main()
{
  ios_base::sync_with_stdio(false);
  cin >> n >> q;
  for (num* i = dat; i < dat + n; ++i)
    cin >> *i;
  getLog(n);
  lst.init(dat, n);
  gst.init(dat, n);
  for (int i = 0; i < q; ++i) {
    int a, b;
    cin >> a >> b;
    cout << gst.query(a - 1, b - 1) - lst.query(a - 1, b - 1) << endl;
  }
  return 0;
}