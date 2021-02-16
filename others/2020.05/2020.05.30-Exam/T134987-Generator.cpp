#include <cstring>
#include <fstream>
#include <random>
#include <set>
#include <utility>

#include "debug_tools/generator.h"
using namespace std;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const int maxn = 1e5, maxm = 3e5;

set<pair<unsigned int, unsigned int>> ed;

inline bool tryInsert(const unsigned int x, const unsigned int y)
{
  const auto e = make_pair(x, y);
  if (ed.find(e) != ed.end()) return false;
  ed.insert(e);
  return true;
}
void createGraph(ofstream& f, const unsigned int n, const unsigned int m)
{
  mt19937_64 rnd;
  uniform_int_distribution<unsigned int> dis(1, n);
  rnd.seed(random_device()());
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int x, y;
    do {
      x = dis(rnd);
      y = dis(rnd);
    } while (x == y || !tryInsert(x, y));
    f << x << " " << y << endl;
  }
}
int main(int argc, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  const unsigned int m = GetRandom(
      1ull,
      min<unsigned long long>(MayBe(atoi(argv[2]), maxm), 1ull * n * (n - 1)));
  ofstream in(argv[3]);
  if (n != 1) {
    in << n << " " << m << endl;
    createGraph(in, n, m);
  }
  else
    in << 1 << " " << 0 << endl;
  return 0;
}