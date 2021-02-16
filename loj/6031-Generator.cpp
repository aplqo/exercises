#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e5, maxm = 1e5, maxk = 1e5, maxsl = 1e5;

void createStr(ofstream& in, const unsigned int n, const char* str,
               const size_t len)
{
  uniform_int_distribution<unsigned int> dis(0, len - 1);
  generate_n(ostreambuf_iterator<char>(in), n,
             [&dis, str]() { return str[dis(default_rnd)]; });
}
void createRange(ofstream& in, const unsigned int n, const unsigned int m)
{
  uniform_int_distribution<unsigned int> disp(0, n - 1);
  for (unsigned int i = m; i; --i) {
    unsigned int l = disp(default_rnd), r = disp(default_rnd);
    if (l > r) swap(l, r);
    in << l << " " << r << "\n";
  }
}
void createQuery(ofstream& in, const unsigned int m, const unsigned int q,
                 const unsigned int k, const char* ch, const size_t len)
{
  uniform_int_distribution<unsigned int> disp(0, m - 1);
  for (unsigned int i = q; i; --i) {
    createStr(in, k, ch, len);
    unsigned int l = disp(default_rnd), r = disp(default_rnd);
    if (l > r) swap(l, r);
    in << " " << l << " " << r << "\n";
  }
}

int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
  const unsigned int sl = GetRandom(1, MayBe(atoi(argv[3]), maxsl));
  const unsigned int k =
      GetRandom(1u, min<unsigned int>(sl, MayBe(atoi(argv[4]), maxk)));
  const unsigned int q = sl / k;
  ofstream in(argv[7]);
  in << n << " " << m << " " << q << " " << k << "\n";
  createStr(in, n, argv[5], strlen(argv[5]));
  in.put('\n');
  createRange(in, n, m);
  createQuery(in, m, q, k, argv[6], strlen(argv[6]));
  return 0;
}