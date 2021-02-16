#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iterator>
#include <random>
#include <utility>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e5, maxa = 1e9, maxx = 1e9;

unsigned long long quickPow(unsigned long long a, unsigned long long e,
                            const unsigned long long mod)
{
  unsigned long long ret = 1;
  for (; e; e >>= 1) {
    if (e & 0x01) ret = (ret * a) % mod;
    a = (a * a) % mod;
  }
  return ret;
}
pair<unsigned int, unsigned long long> lowbit(unsigned long long x)
{
  unsigned int ret = 0;
  while (!(x & 0x01)) {
    ++ret;
    x >>= 1;
  }
  return make_pair(ret, x);
}
bool millerRubin(const unsigned long long n)
{
  static constexpr unsigned int testA[] = {2,  3,  5,  7,  11, 13,
                                           17, 19, 23, 29, 31, 37};
  const auto [r, d] = lowbit(n);
  for (unsigned long long a : testA) {
    if (a > n - 2) break;
    unsigned long long x = quickPow(a, d, n);
    if (x == 1 || x == n - 1) continue;
    for (unsigned int i = 1; i < r; ++i) {
      x = (x * x) % n;
      if (x == n - 1) goto End;
    }
    return false;
  End:;
  }
  return true;
}
unsigned int createPrime(const unsigned int mx)
{
  uniform_int_distribution<unsigned int> dis(2, mx);
  unsigned int x;
  do {
    x = dis(default_rnd);
  } while (!millerRubin(x));
  return x;
}
void createA(ofstream& in, const unsigned int n, const unsigned int ma)
{
  uniform_int_distribution<unsigned int> dis(0, ma);
  generate_n(ostream_iterator<unsigned int>(in, " "), n,
             bind(dis, ref(default_rnd)));
  in << "\n";
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  ofstream in(argv[4]);
  in << n << " " << createPrime(GetRandom(2, MayBe(atoi(argv[2]), maxx)))
     << "\n";
  createA(in, n, MayBe(atoi(argv[3]), maxa));
  return 0;
}