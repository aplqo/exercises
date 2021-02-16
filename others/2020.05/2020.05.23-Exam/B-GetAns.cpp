#include <algorithm>
#include <fstream>
#include <limits>
using namespace std;
using num = unsigned long long;
const unsigned int maxn = 30, maxm = 16;
constexpr num inf = numeric_limits<num>::max();

num ans = inf;
unsigned int n;
bool vis[(1 << maxm) + 1], fail[(1 << maxm) + 1];
struct {
  unsigned int apply(const unsigned int v) const { return (v & del) | add; }
  unsigned int add = 0, del = 0;
} ch[maxn + 10];

bool dfs(const unsigned int p, const num d = 0)
{
  if (d >= ans || vis[p]) return fail[p];
  if (p == 0) {
    ans = min(ans, d);
    return false;
  }
  vis[p] = true;
  fail[p] = true;
  for (const auto* i = ch; i < ch + n; ++i)
    fail[p] &= dfs(i->apply(p), d + 1);
  vis[p] = false;
  return fail[p];
}
unsigned int read(const char* c)
{
  ifstream in(c);
  unsigned int m;
  in >> n >> m;
  unsigned int ini = 0;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int t;
    in >> t;
    if (t) ini |= 1 << i;
  }
  for (auto* i = ch; i < ch + n; ++i) {
    for (unsigned int j = 0; j < m; ++j) {
      int t;
      in >> t;
      if (t == 1)
        i->add |= 1 << j;
      else if (t == -1)
        i->del |= 1 << j;
    }
    i->del = ~(i->del);
  }
  return ini;
}

int main(int argc, char* argv[])
{
  ofstream out(argv[2]);
  dfs(read(argv[1]));
  if (ans == inf)
    out << "-1" << endl;
  else
    out << ans << endl;
  return 0;
}