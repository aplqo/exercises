#include <cstring>
#include <fstream>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const int maxn = 10, maxm = 1000;

mt19937_64 rnd;

void createStr(ofstream& f, const unsigned int l)
{
  static uniform_int_distribution<char> dis('A', 'Z');
  for (unsigned i = 0; i < l; ++i)
    f << dis(rnd);
  f << endl;
}
int main(int argc, char* argv[])
{
  Init();
  rnd.seed(random_device()());
  ofstream in(argv[3]);
  const unsigned int n = 1 << GetRandom(1, MayBe(atoi(argv[1]), maxn));
  const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
  in << n << " " << m << endl;
  for (unsigned int i = 0; i < n; ++i)
    createStr(in, m);
  return 0;
}