#ifdef APDEBUG
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
using namespace std;
typedef int num;
typedef pair<num, num> pairType;
typedef priority_queue<pairType, vector<pairType>, greater<pairType>> que;
const int maxn = 5010, maxa = 50010;
const num inf = numeric_limits<num>::max();

num a[maxn], ans[maxa], s;
bool vis[maxa];
int n, k;

inline void dijkstra()
{
  que q;
  q.push(make_pair(0, 0));
  ans[0] = 0;
  while (!q.empty()) {
    pairType cur = q.top();
    q.pop();
    if (vis[cur.second]) continue;
    vis[cur.second] = true;
    for (num* i = a; i < a + n; ++i) {
      num pos = (cur.second + *i) % s;
      if (ans[pos] > cur.first + *i) {
        ans[pos] = cur.first + *i;
        q.push(make_pair(ans[pos], pos));
      }
    }
  }
}
int main()
{
  ios_base::sync_with_stdio(false);
  cin >> n;
  for (num* i = a; i < a + n; ++i)
    cin >> *i;
  s = *min_element(a, a + n);
  fill(ans, ans + s, inf);
  dijkstra();
  cin >> k;
  for (int i = 0; i < k; ++i) {
    num b;
    cin >> b;
    cout << (ans[b % s] > b ? "NIE" : "TAK") << endl;
  }
  return 0;
}