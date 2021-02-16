#include <algorithm>
#include <fstream>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const unsigned int maxc = 3;
const int maxn = 2.5e5, maxq = 2.5e5, maxv = 998244353 - 1;

void createPair(ofstream& in, const unsigned int n, const unsigned int mv)
{
  uniform_int_distribution<unsigned int> dis(0, mv);
  for (unsigned int i = 0; i < n; ++i) {
    for (unsigned int j = 0; j < maxc; ++j)
      in << dis(default_rnd) << " ";
    in << "\n";
  }
}
void createQuery(ofstream& in, const unsigned int q, const unsigned int n,
                 const unsigned int mv)
{
  uniform_int_distribution<unsigned int> diso(1, 7), disp(1, n), disv(0, mv);
  for (unsigned int i = 0; i < q; ++i) {
    const unsigned int op = diso(default_rnd), p1 = disp(default_rnd),
                       p2 = disp(default_rnd);
    in << op << " " << min(p1, p2) << " " << max(p1, p2);
    if (4 <= op && op < 7) in << " " << disv(default_rnd);
    in.put('\n');
  }
}

int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn)),
                     q = GetRandom(1, MayBe(atoi(argv[2]), maxq));
  const unsigned int mv = MayBe(atoi(argv[3]), maxv);
  ofstream in(argv[4]);
  in << n << "\n";
  createPair(in, n, mv);
  in << q << "\n";
  createQuery(in, q, n, mv);
  return 0;
}
