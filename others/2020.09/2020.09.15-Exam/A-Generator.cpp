#include <algorithm>
#include <cstring>
#include <fstream>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 5e4, maxk = 5e4, maxp = 1e9;
const long long maxm = 1e14;

void createObject(ofstream& in, const unsigned int n, const unsigned int mq)
{
  uniform_int_distribution<unsigned int> dis(1, mq);
  for (unsigned int i = 0; i < n; ++i) {
    const unsigned int v1 = dis(default_rnd), v2 = dis(default_rnd);
    in << max(v1, v2) << " " << min(v1, v2) << '\n';
  }
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  ofstream in(argv[5]);
  in << n << " "
     << GetRandom(1u, min<unsigned int>(n, MayBe(atoi(argv[3]), maxn))) << " "
     << GetRandom(1ll, MayBe(atoll(argv[2]), maxm)) << "\n";
  createObject(in, n, MayBe(atoi(argv[4]), maxp));
  return 0;
}