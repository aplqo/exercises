#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
typedef unsigned int num;
constexpr int maxn = 1e5 + 5, maxl = 25;

int n;
num dat[maxn], table[maxn][maxl];
num lg[maxn];

inline void getLog(int x)
{
  for (num i = 2; i <= x; ++i)
    lg[i] = lg[i >> 1] + 1;
}
void init()
{
  for (int i = 0; i < n; ++i)
    table[i][0] = dat[i];
  for (num j = 1; j <= lg[n]; ++j)
    for (int i = 0; i + (1 << j) <= n; ++i)
      table[i][j] = max(
          table[i][j - 1],
          table[i + (1 << j - 1)][j - 1]);  // i + 1 <<j - 1 = (i + 1)<<(j -1 )
}
num query(int l, int r)
{
  num d = lg[r - l + 1];
  return max(table[l][d], table[r - (1 << d) + 1][d]);
}

int main()
{
  ios_base::sync_with_stdio(false);
  int m;
  cin >> n >> m;
  getLog(n);
  for_each(dat, dat + n, [](num &t) -> void { cin >> t; });
  init();
  for (int i = 0; i < m; ++i) {
    int l, r;
    cin >> l >> r;
    cout << query(--l, --r) << endl;
  }
  return 0;
}