#include <fstream>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 2e5, maxx = 1e9;

void createQuery(ofstream& in, const unsigned int n, const unsigned int k,
                 const unsigned int ma)
{
  uniform_int_distribution<unsigned int> disx(1, ma), diso(1, 2);
  unsigned int cnt = 0;
  for (unsigned int i = 0; i < n; ++i) {
    const unsigned int o = cnt < k ? 1 : diso(default_rnd);
    if (o == 1) {
      in << "1 " << disx(default_rnd) << "\n";
      ++cnt;
    }
    else {
      in << "2\n";
      --cnt;
    }
  }
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  const unsigned int k = GetRandom(1u, n);
  ofstream in(argv[3]);
  in << n << " " << k << " 0\n";
  createQuery(in, n, k, MayBe(atoi(argv[2]), maxx));
  return 0;
}