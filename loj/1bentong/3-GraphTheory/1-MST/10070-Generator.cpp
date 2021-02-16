#include <algorithm>
#include <fstream>
#include <map>
#include <random>
#include <set>
#include <utility>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 100, maxm = 1000, maxc = 10000000;

bool tryInsert(const unsigned int from, const unsigned int to)
{
  static set<pair<unsigned int, unsigned int>> ed;
  const auto t = from > to ? make_pair(to, from) : make_pair(from, to);
  if (ed.find(t) == ed.end()) {
    ed.insert(t);
    return true;
  }
  return false;
}
bool valid(const unsigned int c)
{
  static map<unsigned int, unsigned int> mp;
  if (mp[c] <= 10) {
    ++mp[c];
    return true;
  }
  return false;
}
void createGraph(ostream& in, const unsigned int n, const unsigned int m,
                 const unsigned int mc)
{
  uniform_int_distribution<unsigned int> dis(1, n), disc(1, mc);
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int f, t, c;
    do {
      f = dis(default_rnd);
      t = dis(default_rnd);
    } while (f == t || !tryInsert(f, t));
    do {
      c = disc(default_rnd);
    } while (!valid(c));
    in << f << " " << t << " " << c << endl;
  }
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(2, MayBe(atoi(argv[1]), maxn));
  const unsigned int m =
      GetRandom(1, min<int>(n * (n - 1) / 2, MayBe(atoi(argv[2]), maxm)));
  ofstream in(argv[4]);
  in << n << " " << m << endl;
  createGraph(in, n, m, MayBe(atoi(argv[3]), maxc));
  return 0;
}