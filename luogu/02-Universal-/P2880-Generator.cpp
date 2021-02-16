#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using apdebug::generator::default_rnd;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const int maxn = 50000, maxq = 180000, maxv = 1000000;

void createSeq(ofstream& in, const unsigned int n, const unsigned int mv)
{
  uniform_int_distribution<unsigned int> dis(1, mv);
  generate_n(ostream_iterator<unsigned int>(in, " "), n,
             [&dis]() { return dis(default_rnd); });
  in << endl;
}
void createQuery(ofstream& in, const unsigned int n, const unsigned int q)
{
  uniform_int_distribution<unsigned int> dis(1, n);
  for (unsigned int i = 0; i < q; ++i) {
    unsigned int l = dis(default_rnd), r = dis(default_rnd);
    in << min(l, r) << " " << max(l, r) << endl;
  }
}

int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  const unsigned int q = GetRandom(1, MayBe(atoi(argv[2]), maxq));
  ofstream in(argv[4]);
  in << n << " " << q << endl;
  createSeq(in, n, MayBe(atoi(argv[3]), maxv));
  createQuery(in, n, q);
  return 0;
}