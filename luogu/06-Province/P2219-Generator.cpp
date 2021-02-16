#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const int maxn = 1000, maxv = 100;

int main(int argc, char* argv[])
{
  mt19937_64 rnd;
  Init();
  Init(rnd);
  const unsigned int m = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[2]), maxn));
  ofstream in(argv[4]);
  {
    const unsigned int a = GetRandom(1u, m), b = GetRandom(1u, n);
    in << m << " " << n << " " << a << " " << b << " "
       << (a > 2 ? GetRandom(1u, a - 2) : 1) << " "
       << (b > 2 ? GetRandom(1u, b - 2) : 1) << endl;
  }
  uniform_int_distribution<unsigned int> dis(0, MayBe(atoi(argv[3]), maxv));
  for (unsigned int i = 0; i < m; ++i) {
    generate_n(ostream_iterator<unsigned int>(in, " "), n,
               [&dis, &rnd]() { return dis(rnd); });
    in << endl;
  }
  return 0;
}