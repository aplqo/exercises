#include <algorithm>
#include <fstream>
#include <functional>
#include <iterator>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e5, maxa = (1u << 31) - 1;

void createArray(ofstream& in, const unsigned int n, const int ma)
{
  uniform_int_distribution<int> dis(-ma, ma);
  generate_n(ostream_iterator<int>(in, " "), n, bind(dis, ref(default_rnd)));
  in.put('\n');
}
void createQuery(ofstream& in, const unsigned int n, const int ma)
{
  uniform_int_distribution<int> disa(-ma, ma);
  uniform_int_distribution<unsigned int> disp(1, n);
  for (unsigned int i = 0; i < n; ++i) {
    const unsigned int p1 = disp(default_rnd), p2 = disp(default_rnd);
    in << min(p1, p2) << " " << max(p1, p2) << " " << disa(default_rnd) << "\n";
  }
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  const int a = MayBe(atoi(argv[2]), maxa);
  ofstream in(argv[3]);
  in << n << "\n";
  createArray(in, n, a);
  createQuery(in, n, a);
  return 0;
}