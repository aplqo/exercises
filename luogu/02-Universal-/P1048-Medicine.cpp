#ifdef APDEBUG
#define APINPROG
#include "time.cpp"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const int maxm = 10005, maxt = 100005;
typedef unsigned long long numType;

struct object {
  int t, w;
} obj[maxm];
numType ans[maxt];

int main()
{
  ios_base::sync_with_stdio(false);
  int t, m;
  cin >> t >> m;
  for (object *i = obj + 1; i < obj + 1 + m; ++i)
    cin >> i->t >> i->w;
  for (int mi = 1; mi <= m; ++mi)
    for (int ti = t; ti >= obj[mi].t; --ti)
      ans[ti] = max(ans[ti], ans[ti - obj[mi].t] + obj[mi].w);
  cout << *max_element(ans + 1, ans + 1 + t);
  return 0;
}
