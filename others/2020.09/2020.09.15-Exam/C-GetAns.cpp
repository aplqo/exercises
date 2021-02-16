#include <algorithm>
#include <climits>
#include <fstream>
using namespace std;
const unsigned int maxn = 5000, maxm = 5000, maxk = 10;

struct Edge {
  unsigned int to, key;
  const Edge* pre;
} ed[maxm + 10];
const Edge* head[maxn + 10];
unsigned int key[maxn + 10];
bool vis[maxn + 10][1 << maxk];
unsigned int ans = UINT_MAX, n;

inline void addEdge(const unsigned int from, const unsigned int to,
                    const unsigned int key)
{
  static Edge* cur = ed;
  new (cur) Edge{to, key, head[from]};
  head[from] = cur++;
}
unsigned int readKey(istream& is, const unsigned int k)
{
  unsigned int ret = 0;
  for (unsigned int i = 0; i < k; ++i) {
    unsigned int v;
    is >> v;
    ret |= v << i;
  }
  return ret;
}
void dfs(const unsigned int x, const unsigned int k, const unsigned int dist)
{
  if (vis[x][k]) return;
  if (x == n) {
    ans = min(ans, dist);
    return;
  }
  vis[x][k] = true;
  for (const Edge* i = head[x]; i; i = i->pre)
    if ((i->key & k) == i->key) dfs(i->to, k | key[i->to], dist + 1);
  vis[x][k] = false;
}

int main(int, char* argv[])
{
  ifstream in(argv[1]);
  ofstream fans(argv[2]);
  unsigned int m, k;
  in >> n >> m >> k;
  for (unsigned int i = 1; i <= n; ++i)
    key[i] = readKey(in, k);
  for (unsigned int j = 0; j < m; ++j) {
    unsigned int u, v;
    in >> u >> v;
    addEdge(u, v, readKey(in, k));
  }
  dfs(1, key[1], 0);
  if (ans != UINT_MAX)
    fans << ans << "\n";
  else
    fans << "No Solution\n";
  return 0;
}