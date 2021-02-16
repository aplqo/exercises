#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const int maxk = 105, maxn = 205;

struct fod {
  int fat;
  int* li;
} f[maxn];
int lim[maxk], m, n, k, ans;

int main()
{
  ios_base::sync_with_stdio(false);
  cin >> n >> m >> k;
  for_each(lim + 1, lim + 1 + k, [](int& t) -> void { cin >> t; });
  for (fod* i = f; i < f + n; ++i) {
    int b;
    cin >> i->fat >> b;
    i->li = lim + b;
  }
  sort(f, f + n,
       [](const fod& a, const fod& b) -> bool { return a.fat > b.fat; });
  for (fod* i = f; i < f + n && m; ++i) {
    if (*(i->li) == 0) continue;
    ans += i->fat;
    --m;
    --(*(i->li));
  }
  cout << ans;
  return 0;
}