#include <fstream>
#include <random>
#include <string>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 30, maxm = 16;

int main(int argc, char* argv[])
{
  Init();
  ofstream in(argv[3]);
  const unsigned int n = GetRandom(1, MayBe(stoi(argv[1]), maxn));
  const unsigned int m = GetRandom(1, MayBe(stoi(argv[2]), maxm));
  in << n << " " << m << endl;
  uniform_int_distribution<int> dis(-1, 1);
  for (unsigned int i = 0; i <= n; ++i) {
    for (unsigned int j = 0; j < m; ++j)
      in << dis(default_rnd) << " ";
    in << endl;
  }
  return 0;
}