#include <fstream>
#include <iostream>
using namespace std;
const int maxn = 2000, maxm = maxn * maxn;

struct edge {
  unsigned int to;
  edge* pre = nullptr;
} ed[maxm + 10];
edge* head[maxn + 10];
unsigned int vis[maxn + 10];

inline void AddEdge(const unsigned int from, const unsigned int to)
{
  static edge* cur = ed;
  cur->to = to;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
unsigned int dfs(const unsigned int x, const unsigned int sta)
{
  unsigned int ret = 1;
  vis[x] = sta;
  for (edge* i = head[x]; i; i = i->pre)
    if (vis[i->to] != sta) ret += dfs(i->to, sta);
  return ret;
}
unsigned int GetAnswer(const char* dat)
{
  ifstream in(dat);
  unsigned int n, ans = 0;
  in >> n;
  for (unsigned int i = 1; i <= n; ++i) {
    in.ignore(2, '\n');
    for (unsigned int j = 1; j <= n; ++j)
      if (in.get() == '1') AddEdge(i, j);
  }
  for (unsigned int i = 1; i <= n; ++i)
    ans += dfs(i, i);
  in.close();
  return ans;
}

int main(int argc, char* argv[])
{
  unsigned int ex = GetAnswer(argv[1]);
  unsigned int res;
  ifstream out(argv[2]);
  ofstream ans(argv[3]);
  out >> res;
  out.close();
  ans << ex << endl;
  ans.close();
  if (res != ex) {
    cout << "Test failed! Expect " << ex << " Read " << res << endl;
    return 1;
  }
  return 0;
}