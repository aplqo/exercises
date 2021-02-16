#include <algorithm>
#include <fstream>
#include <functional>
#include <iterator>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 10000, maxl = maxn, maxq = 100, maxa = 1e9;

void createSeq(ofstream& in, const unsigned int n, const unsigned int ma)
{
  uniform_int_distribution<unsigned int> dis(1, ma);
  generate_n(ostream_iterator<unsigned int>(in, " "), n,
             bind(dis, ref(default_rnd)));
  in.put('\n');
}
void createQuery(ofstream& in, const unsigned int l, const unsigned int q)
{
  uniform_int_distribution<unsigned int> dis(0, l);
  generate_n(ostream_iterator<unsigned int>(in, "\n"), q,
             bind(dis, ref(default_rnd)));
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn)),
                     q = GetRandom(1, MayBe(atoi(argv[2]), maxq));
  const unsigned int l = GetRandom(1u, n);
  ofstream in(argv[4]);
  in << n << " " << l << "\n";
  createSeq(in, n, MayBe(atoi(argv[3]), maxa));
  in << q << "\n";
  createQuery(in, l, q);
  return 0;
}