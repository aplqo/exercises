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
const int maxn = 35000, maxk = 50;

int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  ofstream in(argv[3]);
  in << n << " " << GetRandom(1, min(MayBe(atoi(argv[2]), maxk), maxn)) << endl;
  uniform_int_distribution<unsigned int> dis(1, n);
  generate_n(ostream_iterator<unsigned int>(in, " "), n,
             [&dis]() { return dis(default_rnd); });
  in << endl;
  return 0;
}