#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
const int maxn = 20005, maxm = 100005;
typedef unsigned int resType;

struct relate {
  int x, y;
  resType val;
} re[maxm];
int fa[maxn * 2];

int GetFather(int x) { return fa[x] == x ? x : fa[x] = GetFather(fa[x]); }
inline void merge(int x, int y) { fa[GetFather(x)] = GetFather(y); }
int main()
{
  ios_base::sync_with_stdio(false);
  int n, m;
  cin >> n >> m;
  for (relate* i = re; i < re + m; ++i)
    cin >> i->x >> i->y >> i->val;
  iota(fa + 1, fa + 1 + n * 2, 1);
  sort(re, re + m,
       [](const relate& a, const relate& b) -> bool { return a.val > b.val; });
  for (relate* i = re; i < re + m; ++i) {
    if (GetFather(i->x) == GetFather(i->y)) {
      cout << i->val << endl;
      return 0;
    }
    merge(i->x + n, i->y);
    merge(i->x, i->y + n);
  }
  cout << "0" << endl;
  return 0;
}
