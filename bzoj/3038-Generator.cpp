#include <algorithm>
#include <fstream>
#include <iterator>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using apdebug::generator::default_rnd;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const int maxn = 100000, maxm = maxn;
const long long maxv = 1e12;

void createSeq(ofstream& f, const unsigned int n, const unsigned long long mv)
{
  uniform_int_distribution<unsigned long long> dis(1, mv);
  generate_n(ostream_iterator<long long>(f, " "), n,
             [&dis]() { return dis(default_rnd); });
  f << endl;
}
void createQuery(ofstream& f, const unsigned int n, const unsigned int m)
{
  uniform_int_distribution<unsigned int> disop(0, 1), disr(1, n);
  for (unsigned int i = 0; i < m; ++i)
    f << disop(default_rnd) << " " << disr(default_rnd) << " "
      << disr(default_rnd) << endl;
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
  ofstream in(argv[4]);
  in << n << endl;
  createSeq(in, n, MayBe(atoll(argv[3]), maxv));
  in << m << endl;
  createQuery(in, n, m);
  return 0;
}