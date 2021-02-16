#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iterator>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 5e5, maxa = (1ull << 32) - 1;

void createSeq(ofstream& in, const unsigned int n, const unsigned int ma)
{
  uniform_int_distribution<unsigned int> disa(1, ma);
  generate_n(ostream_iterator<unsigned int>(in, " "), n,
             bind(disa, ref(default_rnd)));
  in.put('\n');
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  ofstream in(argv[3]);
  in << n << "\n";
  createSeq(in, n, MayBe(atoi(argv[2]), maxa));
  return 0;
}