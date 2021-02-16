#include <algorithm>
#include <cstring>
#include <fstream>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 5e4, maxm = 1e5, maxv = 5e4;

void createGraph(ofstream& in, const unsigned int n, const unsigned int m,
                 const unsigned int mv)
{
  uniform_int_distribution<unsigned int> disp(1, n), disb(1, mv);
  for (unsigned int i = 0; i < m; ++i)
    in << disp(default_rnd) << " " << disp(default_rnd) << " "
       << disb(default_rnd) << " " << disb(default_rnd) << "\n";
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(2, MayBe(atoi(argv[1]), maxn));
  const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
  ofstream in(argv[4]);
  in << n << " " << m << "\n";
  createGraph(in, n, m, MayBe(atoi(argv[1]), maxv));
  return 0;
}