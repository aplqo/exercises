#include <algorithm>
#include <fstream>
using namespace std;
const int maxn = 1000;
constexpr unsigned long long mod = 1000000009;

bool gra[maxn + 10][maxn + 10];
bool vis[maxn + 10];
unsigned long long ans;

void dfs(const unsigned int x, const unsigned int n)
{
  vis[x] = true;
  for (unsigned int i = 0; i < n; ++i)
    if (gra[x][i] && !vis[i]) dfs(i, n);
}
bool test(const unsigned int n)
{
  fill(vis, vis + n, false);
  dfs(0, n);
  return count(vis, vis + n, true) == n;
}
void funx(const unsigned int x, const unsigned int n);
void funy(const unsigned int x, const unsigned int y, const unsigned int n)
{
  if (y >= n) {
    funx(x + 1, n);
    return;
  }
  gra[x][y] = gra[y][x] = true;
  funy(x, y + 1, n);
  gra[x][y] = gra[y][x] = false;
  funy(x, y + 1, n);
}
void funx(const unsigned int x, const unsigned int n)
{
  if (x >= n) {
    if (test(n)) ans = (ans + 1) % mod;
    return;
  }
  funy(x, x + 1, n);
}
int main(int argc, char* argv[])
{
  ifstream in(argv[1]);
  ofstream fans(argv[2]);
  unsigned int n;
  in >> n;
  funx(0, n);
  fans << ans << endl;
  return 0;
}