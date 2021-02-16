#include <algorithm>
#include <climits>
#include <fstream>
#include <iterator>
#include <queue>
using namespace std;
const unsigned int maxn = 5e5;
constexpr unsigned int inf = UINT_MAX / 2;

struct Edge {
  unsigned int to;
  const Edge* pre;
} ed[maxn + 1];
const Edge* head[maxn + 1];
unsigned int f[maxn + 1], w[maxn + 1], fa[maxn + 1];
bool ban[maxn + 1];

void addEdge(const unsigned int from, const unsigned int to)
{
  static Edge* cur = ed;
  cur->to = to;
  cur->pre = head[from];
  head[from] = cur++;
}
void bfs(const unsigned int n, const unsigned int k)
{
  static unsigned int dep[maxn + 1];
  static bool vis[maxn + 1];
  queue<unsigned int> q;
  fill(f + 2, f + 1 + n, inf);
  q.push(1);
  unsigned int cdep = 0;
  while (!q.empty()) {
    const unsigned int cur = q.front();
    q.pop();
    vis[cur] = true;
    if (dep[cur] != cdep) {
      unsigned int mv = inf;
      for (unsigned int i = 1; i <= n; ++i)
        if (vis[i] && dep[i] == cdep) mv = min(mv, f[i]);
      for (unsigned int j = 1; j <= n; ++j)
        if (vis[j] && dep[j] == cdep && !ban[j]) f[j] = mv;
      cdep = dep[cur];
    }
    f[cur] = f[fa[cur]] + w[cur];
    if (!ban[cur]) {
      for (unsigned int i = 1; i <= n; ++i)
        if (dep[i] <= dep[cur])
          f[cur] = min(f[cur], f[i] + k * (dep[cur] - dep[i]));
    }
    for (const Edge* i = head[cur]; i; i = i->pre) {
      dep[i->to] = dep[cur] + 1;
      q.push(i->to);
    }
  }
}
int main(int, char* argv[])
{
  ifstream in(argv[1]);
  ofstream out(argv[2]);
  unsigned int n, k;
  in >> n >> k;
  for (unsigned int i = 2; i <= n; ++i) {
    unsigned int b;
    in >> fa[i] >> w[i] >> b;
    ban[i] = b;
    addEdge(fa[i], i);
  }
  bfs(n, k);
  copy(f + 1, f + 1 + n, ostream_iterator<unsigned int>(out, "\n"));
  return 0;
}