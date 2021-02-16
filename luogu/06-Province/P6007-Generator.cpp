#include <cstring>
#include <fstream>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxp = 1e5, maxn = 1e9;

void createPath(ofstream& in, const unsigned int n, const unsigned int p)
{
  uniform_int_distribution<unsigned int> dis(0, n);
  for (unsigned int i = 0; i < p; ++i) {
    unsigned int x1 = dis(default_rnd), y1 = dis(default_rnd),
                 x2 = dis(default_rnd), y2 = dis(default_rnd);
    in << min(x1, x2) << " " << min(y1, y2) << " " << max(x1, x2) << " "
       << max(y1, y2) << endl;
  }
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  const unsigned int p = GetRandom(1, MayBe(atoi(argv[2]), maxp));
  ofstream in(argv[3]);
  in << n << " " << p << endl;
  createPath(in, n, p);
  return 0;
}