#include <fstream>
#include <random>
#include <set>
#include <string>
#include <utility>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e4, maxm = 1e5, maxq = 1e4, maxk = 1e9;

mt19937_64 rnd;
set<pair<unsigned int, unsigned int>> vec;

void createGraph(ofstream& f, const unsigned int n, const unsigned int m,
                 const unsigned int mv)
{
  uniform_int_distribution<unsigned int> disv(1, n), disw(1, mv);
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int u, v;
    do {
      u = disv(rnd);
      v = disv(rnd);
      if (u > v) swap(u, v);
    } while (u == v || vec.find(make_pair(u, v)) != vec.end());
    vec.insert(make_pair(u, v));
    f << u << " " << v << " " << disw(rnd) << endl;
  }
}
void createQuery(ofstream& f, const unsigned int n, const unsigned int q)
{
  uniform_int_distribution<unsigned int> dis(1, n);
  f << q << endl;
  for (unsigned int i = 0; i < q; ++i)
    f << dis(rnd) << " " << dis(rnd) << endl;
}
int main(int argc, char* argv[])
{
  Init();
  rnd.seed(random_device()());
  const unsigned int n = GetRandom(1, MayBe(stoi(argv[1]), maxn));
  const unsigned int m = GetRandom(
      1u, min<unsigned int>(MayBe(stoi(argv[2]), maxm), n * (n - 1) / 2));
  ofstream in(argv[5]);
  in << n << " " << m << endl;
  createGraph(in, n, m, MayBe(stoi(argv[4]), maxk));
  createQuery(in, n, GetRandom(1, MayBe(stoi(argv[3]), maxq)));
  return 0;
}