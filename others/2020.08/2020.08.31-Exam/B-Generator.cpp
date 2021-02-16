#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iterator>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxm = 2e6, maxn = 5, maxq = 5e4;

void createMatrix(ofstream& in, const unsigned int n, const unsigned int m)
{
  uniform_int_distribution<unsigned int> dis(0, 1);
  for (unsigned int i = 0; i < n; ++i) {
    generate_n(ostream_iterator<unsigned int>(in, " "), m,
               bind(dis, ref(default_rnd)));
    in << "\n";
  }
}
void createQuery(ofstream& in, const unsigned int n, const unsigned int m,
                 const unsigned int q)
{
  uniform_int_distribution<unsigned int> diso(1, 2), disx(1, n), disy(1, m);
  for (unsigned int i = 0; i < q; ++i) {
    const unsigned int op = diso(default_rnd), y1 = disy(default_rnd);
    in << op << " " << disx(default_rnd) << " ";
    if (op == 2) {
      const unsigned int y2 = disy(default_rnd);
      in << min(y1, y2) << " " << disx(default_rnd) << " " << max(y1, y2);
    }
    else
      in << y1;
    in << "\n";
  }
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
  const unsigned int q = GetRandom(1, MayBe(atoi(argv[3]), maxq));
  ofstream in(argv[4]);
  in << n << " " << m << " " << q << "\n";
  createMatrix(in, n, m);
  createQuery(in, n, m, q);
  return 0;
}