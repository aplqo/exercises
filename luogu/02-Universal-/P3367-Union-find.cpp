#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <iostream>
#include <numeric>
using namespace std;
const int maxn = 10005;

int fa[maxn];
int GetFather(int x) { return fa[x] == x ? x : fa[x] = GetFather(fa[x]); }

int main()
{
  ios_base::sync_with_stdio(false);
  int n, m;
  cin >> n >> m;
  iota(fa + 1, fa + n + 1, 1);
  for (int i = 0; i < m; ++i) {
    char z;
    int x, y, fx, fy;
    cin >> z >> x >> y;
    fx = GetFather(x);
    fy = GetFather(y);
    if (z == '1')
      fa[fx] = fy;
    else
      cout << (fx == fy ? "Y" : "N") << endl;
  }
  return 0;
}