#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <climits>
#include <functional>
#include <iostream>
#include <queue>
#include <utility>
using namespace std;
typedef unsigned long long numType;
typedef pair<numType, int> pairType;
const numType inf = numeric_limits<numType>::max();
const int maxn = 100005, maxm = 500005, maxk = 25;

struct edge {
  int to;
  numType c;
  edge* pre = nullptr;
} lin[maxm];
edge* head[maxn];
numType ans[maxn][maxk];
bool vis[maxn][maxk];
int n, m, k;

inline void AddEdge(int a, int b, numType c)
{
  static edge* cur = lin;
  --a;
  cur->to = b - 1;
  cur->c = c;
  cur->pre = head[a];
  head[a] = cur;
  ++cur;
}
inline void dijkstra()
{
  priority_queue<pairType, vector<pairType>, greater<pairType>> q;
  for_each(ans, ans + n,
           [](numType(&t)[maxk]) -> void { fill(t, t + k + 1, inf); });
  q.push(make_pair(0, 0));
  ans[0][0] = 0;
  while (!q.empty()) {
    pairType cur = q.top();
    q.pop();
    const int lay = cur.second / n, pos = cur.second % n;
    if (vis[pos][lay]) continue;
    vis[pos][lay] = true;
    for (edge* i = head[pos]; i != nullptr; i = i->pre) {
      if (ans[i->to][lay] > cur.first + i->c) {
        ans[i->to][lay] = cur.first + i->c;
        q.push(make_pair(ans[i->to][lay], i->to + n * lay));
      }
      if (lay < k && ans[i->to][lay + 1] > cur.first) {
        ans[i->to][lay + 1] = cur.first;
        q.push(make_pair(ans[i->to][lay + 1], i->to + n * (lay + 1)));
      }
    }
  }
}
int main()
{
  ios_base::sync_with_stdio(false);
  cin >> n >> m >> k;
  if (k >= m) {
    cout << "0";
    return 0;
  }
  for (int i = 0; i < m; ++i) {
    int a, b;
    numType c;
    cin >> a >> b >> c;
    AddEdge(a, b, c);
    AddEdge(b, a, c);
  }
  dijkstra();
  cout << *min_element(ans[n - 1], ans[n - 1] + 1 + k) << endl;
  return 0;
}