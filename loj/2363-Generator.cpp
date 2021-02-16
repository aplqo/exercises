#include <cstring>
#include <fstream>
#include <random>
#include <utility>
using namespace std;
using point = pair<unsigned int, unsigned int>;
const int maxn = 1000;

bool vis[maxn + 10][maxn + 10];
mt19937_64 genx, geny;
uniform_int_distribution<unsigned int> disx(1, 1000), disy(1, 1000);

point generate()
{
  while (true) {
    unsigned int x = disx(genx), y = disy(geny);
    if (!vis[x][y]) {
      vis[x][y] = true;
      return make_pair(x, y);
    }
  }
}
int main(int argc, char* argv[])
{
  ofstream out(argv[2]);
  unsigned int n = atoi(argv[1]);
  genx.seed(random_device()());
  geny.seed(random_device()());
  out << 1 << endl;
  out << n << " " << 0 << endl;
  for (unsigned int i = 0; i < n; ++i) {
    point p = generate();
    out << p.first / 100.0 << " " << p.second / 100.0 << endl;
  }
  return 0;
}