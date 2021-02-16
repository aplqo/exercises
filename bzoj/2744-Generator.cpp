#include <algorithm>
#include <climits>
#include <fstream>
#include <functional>
#include <iterator>
#include <set>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxa = 200, maxb = 3000, maxt = 6;

void createGraph(ofstream& in, const unsigned int m, const unsigned int na,
                 const unsigned int nb)
{
  set<unsigned int> ed[maxa + 10];
  uniform_int_distribution<unsigned int> disa(1, na), disb(1, nb);
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int u, v;
    do {
      u = disa(default_rnd);
      v = disb(default_rnd);
    } while (ed[u].find(v) != ed[u].end());
    in << u << " " << v << "\n";
    ed[u].insert(v);
  }
}
void createSeq(ofstream& in, const unsigned int len)
{
  uniform_int_distribution<unsigned int> dis(0, INT_MAX);
  generate_n(ostream_iterator<unsigned int>(in, " "), len,
             bind(dis, ref(default_rnd)));
  in.put('\n');
}

int main(int, char* argv[])
{
  Init();
  const unsigned int t = GetRandom(1, MayBe(atoi(argv[1]), maxt));
  uniform_int_distribution<unsigned int> disa(1, MayBe(atoi(argv[2]), maxa)),
      disb(1, MayBe(atoi(argv[3]), maxb));
  ofstream in(argv[5]);
  in << t << "\n";
  for (unsigned int i = 0; i < t; ++i) {
    const unsigned int a = disa(default_rnd), b = disb(default_rnd);
    const unsigned int m =
        GetRandom(0u, min(MayBe<unsigned int>(atoi(argv[4]), a * b), a * b));
    in << a << " " << b << " " << m << "\n";
    createSeq(in, a);
    createSeq(in, b);
    createGraph(in, m, a, b);
  }
  return 0;
}