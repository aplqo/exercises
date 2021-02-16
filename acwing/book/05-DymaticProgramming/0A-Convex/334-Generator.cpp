#include <algorithm>
#include <fstream>
#include <functional>
#include <iterator>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 500000, maxk = 20, maxv = 500000;

void createSeq(ofstream& in, const unsigned int n, const unsigned int mv)
{
  static unsigned int buf[maxn];
  uniform_int_distribution<unsigned int> dis(1, mv);
  generate(buf, buf + n, bind(dis, ref(default_rnd)));
  sort(buf, buf + n);
  copy(buf, buf + n, ostream_iterator<unsigned int>(in, " "));
  in << "\n";
}
int main(int, char* argv[])
{
  Init();
  const unsigned int t = GetRandom(1, MayBe(atoi(argv[1]), maxk));
  const unsigned int mv = MayBe(atoi(argv[3]), maxv);
  uniform_int_distribution<unsigned int> disn(2, MayBe(atoi(argv[2]), maxn));
  ofstream in(argv[4]);
  in << t << "\n";
  for (unsigned int i = 0; i < t; ++i) {
    const unsigned int n = disn(default_rnd), k = GetRandom(2u, n);
    in << n << " " << GetRandom(2u, n) << "\n";
    createSeq(in, n, mv);
  }
  return 0;
}
