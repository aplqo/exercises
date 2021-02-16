#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e5, maxm = 1e5, maxc = 1e5;

void createColor(ofstream& f, const unsigned int n, const unsigned int mc)
{
  uniform_int_distribution<unsigned int> disc(1, mc);
  generate_n(ostream_iterator<unsigned int>(f, " "), n,
             [&disc]() { return disc(default_rnd); });
  f << endl;
}
void createTree(ofstream& f, const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i)
    f << i << " " << GetRandom(1u, i - 1) << endl;
}
void createQuery(ofstream& f, const unsigned int n, const unsigned int m)
{
  uniform_int_distribution<unsigned int> disv(1, n), disc(1, n);
  for (unsigned int i = 0; i < m; ++i)
    f << disv(default_rnd) << " " << disc(default_rnd) << endl;
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(2, MayBe(atoi(argv[1]), maxn));
  const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
  ofstream in(argv[4]);
  in << n << " " << m << endl;
  createColor(in, n, MayBe(atoi(argv[3]), maxc));
  createTree(in, n);
  createQuery(in, n, m);
  return 0;
}