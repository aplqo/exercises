#include <cstring>
#include <fstream>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using apdebug::generator::default_rnd;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const int maxt = 5000, maxm = 1e9;
const long long maxn = 1e18, maxk = 1e18;

int main(int argc, char* argv[])
{
  Init();
  const unsigned int t = GetRandom(1, MayBe(atoi(argv[1]), maxt));
  ofstream in(argv[5]);
  in << t << endl;
  uniform_int_distribution<unsigned long long> disa(
      1, MayBe(atoll(argv[2]), maxn)),
      disk(0, MayBe(atoll(argv[3]), maxk));
  for (unsigned int i = 0; i < t; ++i)
    in << disa(default_rnd) << " " << disa(default_rnd) << " "
       << disk(default_rnd) << " " << MayBe(atoi(argv[4]), GetRandom(1, maxm))
       << endl;
  return 0;
}