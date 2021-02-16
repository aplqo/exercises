#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iterator>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 3e5, maxm = 3e5, maxw = 1e9;

void createTree(ofstream& in, const unsigned int n, const unsigned int mw)
{
  uniform_int_distribution<unsigned int> dis(1, mw);
  for (unsigned int i = 2; i <= n; ++i)
    in << i << " " << GetRandom(1u, i - 1) << " " << dis(default_rnd) << "\n";
}
void createArmy(ofstream& in, const unsigned int n, const unsigned int m)
{
  uniform_int_distribution<unsigned int> dis(2, n);
  generate_n(ostream_iterator<unsigned int>(in, " "), m,
             bind(dis, ref(default_rnd)));
  in.put('\n');
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(2, MayBe(atoi(argv[2]), maxn));
  const unsigned int m =
      GetRandom(1u, min<unsigned int>(MayBe(atoi(argv[2]), maxm), n - 1));
  ofstream in(argv[4]);
  in << n << "\n";
  createTree(in, n, MayBe(atoi(argv[3]), maxw));
  in << m << "\n";
  createArmy(in, n, m);
  return 0;
}