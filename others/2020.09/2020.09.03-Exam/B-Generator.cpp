#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iterator>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 300000, maxm = 300000;

void createTree(ofstream& in, const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i)
    in << i << " " << GetRandom(1u, i - 1) << "\n";
}
void createGraph(ofstream& in, const unsigned int n, const unsigned int m)
{
  uniform_int_distribution<unsigned int> dis(1, n);
  for (unsigned int i = 0; i < m; ++i) {
    const unsigned int x = dis(default_rnd);
    unsigned int y;
    do {
      y = dis(default_rnd);
    } while (y == x);
    in << x << " " << y << "\n";
  }
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(2, MayBe(atoi(argv[1]), maxn));
  const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
  ofstream in(argv[3]);
  in << n << " " << m << "\n";
  createTree(in, n);
  createGraph(in, n, m);
  return 0;
}