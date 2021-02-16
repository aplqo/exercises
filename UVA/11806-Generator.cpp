#include <fstream>
#include <random>
#include <string>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxt = 50, maxn = 20, maxk = 500;

mt19937_64 rnd;

int main(int argc, char* argv[])
{
  Init();
  rnd.seed(random_device()());
  ofstream in(argv[4]);
  const unsigned int t = GetRandom(1, MayBe(stoi(argv[1]), maxt));
  const unsigned int mn = MayBe(stoi(argv[2]), maxn);
  const unsigned int mk = MayBe(stoi(argv[3]), min<int>(mn * mn, maxk));
  in << t << endl;
  uniform_int_distribution<unsigned int> disc(2, mn), disk(1, mk);
  for (unsigned int i = 0; i < t; ++i)
    in << disc(rnd) << " " << disc(rnd) << " " << disk(rnd) << endl;
  return 0;
}