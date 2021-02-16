#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxa = 2e9, maxn = 1000000;

void createSeq(ofstream& in, const unsigned int n, const int ma)
{
  uniform_int_distribution<unsigned int> disa(0, ma);
  generate_n(ostream_iterator<unsigned int>(in, " "), n,
             [&disa]() { return disa(default_rnd); });
  in << endl;
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  ofstream in(argv[3]);
  in << n << endl;
  createSeq(in, n, MayBe(atoi(argv[2]), maxa));
  return 0;
}