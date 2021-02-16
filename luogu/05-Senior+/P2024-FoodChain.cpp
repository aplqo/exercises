#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
const int maxn = 5e4;

int fa[maxn * 3], ans;
int GetFather(int x) { return fa[x] == x ? x : fa[x] = GetFather(fa[x]); }
inline void merge(int x, int y) { fa[GetFather(x)] = GetFather(y); }
int main()
{
  ios_base::sync_with_stdio(false);
  int n, k;
  cin >> n >> k;
  const int enem = n * 2, tar = n;
  iota(fa + 1, fa + 1 + n * 3, 1);
  for (int i = 0; i < k; ++i) {
    int op, x, y;
    cin >> op >> x >> y;
    if (x > n || y > n) {
      ++ans;
      continue;
    }
    if (op == 1) {
      if (GetFather(x + tar) == GetFather(y) ||
          GetFather(x + enem) == GetFather(y)) {
        ++ans;
        continue;
      }
      merge(x, y);
      merge(x + tar, y + tar);
      merge(x + enem, y + enem);
    }
    else {
      if (GetFather(x) == GetFather(y) || GetFather(x + enem) == GetFather(y)) {
        ++ans;
        continue;
      }
      merge(x + tar, y);
      merge(y + enem, x);
      merge(x + enem, y + tar);
    }
  }
  cout << ans;
  return 0;
}