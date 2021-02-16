#include <algorithm>
#include <fstream>
#include <iterator>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 50, maxk = 10, maxv = 1000;

void createMatrix(ofstream& in, const unsigned int n, const unsigned int mv)
{
  uniform_int_distribution<unsigned int> dis(0, mv);
  for (unsigned int i = 0; i < n; ++i) {
    generate_n(ostream_iterator<unsigned int>(in, " "), n,
               [&dis]() { return dis(default_rnd); });
    in << endl;
  }
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  ofstream in(argv[4]);
  in << n << " " << GetRandom(1, MayBe(atoi(argv[2]), maxk)) << endl;
  createMatrix(in, n, MayBe(atoi(argv[3]), maxk));
  return 0;
}