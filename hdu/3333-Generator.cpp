#include <fstream>
#include <functional>
#include <iterator>
#include <random>
#include <string>

#include "debug_tools/generator.h"
using namespace std;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const unsigned long long maxa = 1e9;
const int maxn = 30000, maxt = 10, maxq = 100000;

mt19937_64 rnd;

void createA(ofstream& f, const unsigned int n, const unsigned long long ma)
{
  uniform_int_distribution<unsigned long long> dis(0, ma);
  generate_n(ostream_iterator<unsigned long long>(f, " "), n, bind(dis, rnd));
  f << endl;
}
void createQuery(ofstream& f, const unsigned int n, const unsigned int q)
{
  uniform_int_distribution<unsigned int> dis(1, n);
  for (unsigned int i = 0; i < q; ++i) {
    const unsigned int l = dis(rnd);
    const unsigned int r = dis(rnd, decltype(dis)::param_type(l, n));
    f << l << " " << r << endl;
  }
}
int main(int argc, char* argv[])
{
  Init();
  Init(rnd);
  const unsigned int t = GetRandom(1, MayBe(atoi(argv[1]), maxt));
  const unsigned int ma = MayBe(stoull(argv[4]), maxa);
  ofstream in(argv[5]);
  in << t << endl;
  uniform_int_distribution<unsigned int> disn(1, MayBe(atoi(argv[2]), maxn));
  uniform_int_distribution<unsigned int> disq(1, MayBe(atoi(argv[3]), maxq));
  for (unsigned int i = 0; i < t; ++i) {
    const unsigned int n = disn(rnd), q = disq(rnd);
    in << n << endl;
    createA(in, n, ma);
    in << q << endl;
    createQuery(in, n, q);
  }
  return 0;
}