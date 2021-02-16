#include <algorithm>
#include <fstream>
#include <functional>
#include <iterator>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 100, maxm = 500, maxv = 1000;

void createSeq(ofstream& in, const unsigned int n, const unsigned int mv)
{
  uniform_int_distribution<unsigned int> dis(1, mv);
  generate_n(ostream_iterator<unsigned int>(in, " "), n,
             bind(dis, ref(default_rnd)));
  in.put('\n');
}
void createDep(ofstream& in, const unsigned int n)
{
  uniform_int_distribution<unsigned int> dis(0, n);
  for (unsigned int i = 1; i <= n; ++i) {
    unsigned int v;
    do {
      v = dis(default_rnd);
    } while (v == i);
    in << v << " ";
  }
  in.put('\n');
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(0, MayBe(atoi(argv[1]), maxn));
  const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
  ofstream in(argv[4]);
  in << n << " " << m << "\n";
  createSeq(in, n, m);
  createSeq(in, n, MayBe(atoi(argv[3]), maxv));
  createDep(in, n);
  return 0;
}