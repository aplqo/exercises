#include <algorithm>
#include <climits>
#include <cstring>
#include <fstream>
#include <iterator>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 20000, maxq = 25000;
const long long maxa = LLONG_MAX;

void createSeq(ofstream& f, const unsigned int n, const long long mv)
{
  uniform_int_distribution<long long> dis(0, mv);
  generate_n(ostream_iterator<long long>(f, "\n"), n,
             [&dis]() { return dis(default_rnd); });
}
void createQuery(ofstream& f, const unsigned int n, const unsigned int q)
{
  uniform_int_distribution<unsigned int> dis(0, n - 1);
  static unsigned int vis[maxn + 1];
  for (unsigned int i = 0; i < q; ++i) {
    for (unsigned int j = 0; j < 4; ++j) {
      unsigned int v = dis(default_rnd);
      while (vis[v] == i + 1)
        v = dis(default_rnd);
      vis[v] = i + 1;
      f << v << " ";
    }
    f << endl;
  }
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(4, MayBe(atoi(argv[1]), maxn));
  const unsigned int q = GetRandom(1, MayBe(atoi(argv[2]), maxq));
  ofstream in(argv[4]);
  in << n << endl;
  createSeq(in, n, MayBe(atoll(argv[3]), maxa));
  in << q << endl;
  createQuery(in, n, q);
  return 0;
}