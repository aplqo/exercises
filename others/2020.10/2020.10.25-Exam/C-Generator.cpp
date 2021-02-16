#include <algorithm>
#include <cstring>
#include <fstream>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 300000, maxa = 1e9;

void createQuery(ofstream& in, const unsigned int n, const int mna,
                 const int mxa)
{
  uniform_int_distribution<unsigned int> diso(0, 5);
  uniform_int_distribution<int> disa(mna, mxa);
  unsigned int pos = 1, siz = 1;
  in << "I " << disa(default_rnd) << "\n";
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int po;
    do {
      po = diso(default_rnd);
    } while ((po == 1 && pos == 0) || (po > 3 && !siz));
    switch (po) {
      case 0:
        in << "I " << disa(default_rnd) << "\n";
        ++pos;
        ++siz;
        break;
      case 1:
        in << "D\n";
        --pos;
        --siz;
        break;
      case 2:
        in << "L\n";
        if (pos) --pos;
        break;
      case 3:
        in << "R\n";
        ++pos;
        pos = min(pos, siz);
        break;
      case 4:
        in << "Q ";
        {
          const unsigned int u = GetRandom(1u, siz), v = GetRandom(1u, siz);
          in << min(u, v) << " " << max(u, v) << "\n";
        }
        break;
      case 5:
        in << "C " << GetRandom(1u, siz) << " " << disa(default_rnd) << "\n";
        break;
    }
  }
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  ofstream in(argv[4]);
  in << n << "\n";
  createQuery(in, n, MayBe(atoi(argv[2]), -maxa), MayBe(atoi(argv[3]), maxa));
  return 0;
}
