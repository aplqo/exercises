#include <chrono>
#include <cstring>
#include <fstream>
#include <random>
using namespace std;
const unsigned int maxn = 16;

mt19937 rnd;
bool edge[maxn + 1][maxn + 1];

unsigned int getRandom(const unsigned int l, const unsigned int r)
{
  uniform_int_distribution<unsigned int> dis(l, r);
  return dis(rnd);
}
unsigned int maybe(const unsigned int v, const unsigned int def)
{
  return v ? v : def;
}

bool tryAdd(const unsigned int x, const unsigned int y)
{
  if (edge[x][y] || x == y) return false;
  edge[x][y] = edge[y][x] = true;
  return true;
}
void createTree(ofstream& f, const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i) {
    const unsigned int t = getRandom(1, i - 1);
    f << t << " " << i << endl;
    tryAdd(t, i);
  }
}
void createEdge(ofstream& f, const unsigned int n, const unsigned int m)
{
  uniform_int_distribution<unsigned int> dis(1, n);
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int x, y;
    do {
      x = dis(rnd);
      y = dis(rnd);
    } while (!tryAdd(x, y));
    f << x << " " << y << endl;
  }
}
int main(int, char* argv[])
{
  rnd.seed(std::chrono::steady_clock::now().time_since_epoch().count());
  const unsigned int n = getRandom(1, maybe(atoi(argv[1]), maxn));
  const unsigned int m = getRandom(n - 1, n * (n - 1) / 2);
  ofstream in(argv[2]);
  in << n << " " << m << " " << getRandom(1, m) << endl;
  createTree(in, n);
  createEdge(in, n, m - (n - 1));
  return 0;
}
