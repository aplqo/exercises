#ifdef APDEBUG
#define APINPROG
#define COLOR
#include "debug_tools/time.cpp"
#endif
#include <deque>
#include <iostream>
#include <limits>
using namespace std;
const int maxn = 505;

struct node {
  node(int nx, int ny) : x(nx), y(ny) {}
  template <class T>
  inline T& operator[](T dat[maxn][maxn])
  {
    return dat[x][y];
  };
  int x, y, val = 1;
};
const int dx[4] = {1, 1, -1, -1}, dy[4] = {1, -1, 1, -1};
bool dat[maxn][maxn][4], vis[maxn][maxn];
int ans[maxn][maxn];
int n, m;
deque<node> que;

inline void bfs()
{
  que.push_back(node(0, 0));
  que.front()[ans] = 1;
  while (!que.empty()) {
    node cur = que.front();
    que.pop_front();
    if (cur[ans] < cur.val) continue;
    cur[ans] = cur.val;
    for (int i = 0; i < 4; ++i) {
      int nx = cur.x + dx[i], ny = cur.y + dy[i];
      if (nx < 0 || ny < 0 || nx > n || ny > m) continue;
      node t(nx, ny);
      if (cur[dat][i]) {
        if (t[ans] && t[ans] <= cur[ans]) continue;
        t[ans] = cur[ans];
        t.val = cur[ans];
        que.push_front(t);
      }
      else {
        if (t[ans] && t[ans] <= cur[ans] + 1) continue;
        t[ans] = cur[ans] + 1;
        t.val = cur[ans] + 1;
        que.push_back(t);
      }
      if (nx == n && ny == m) return;
    }
  }
}
int main()
{
  ios_base::sync_with_stdio(false);
  cin >> n >> m;
  for (int i = 0; i < n; ++i) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    for (int j = 0; j < m; ++j) {
      if (cin.get() == '/') {
        dat[i + 1][j][2] = true;
        dat[i][j + 1][1] = true;
      }
      else {
        dat[i][j][0] = true;
        dat[i + 1][j + 1][3] = true;
      }
    }
  }
  bfs();
  if (ans[n][m])
    cout << ans[n][m] - 1;
  else
    cout << "NO SOLUTION";
  return 0;
}