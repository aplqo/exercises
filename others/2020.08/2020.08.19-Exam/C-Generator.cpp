#include <algorithm>
#include <fstream>
#include <functional>
#include <iterator>
#include <random>
#include <sstream>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e5, maxa = 1e5, maxq = 1e5, maxk = 1e5;

unsigned long long createA(ostream& in, const unsigned int n,
                           const unsigned int ma)
{
  uniform_int_distribution<unsigned int> disa(1, ma);
  unsigned long long sum = 0;
  for (unsigned int i = 0; i < n; ++i) {
    unsigned int t = disa(default_rnd);
    in << t << " ";
    sum += t;
  }
  in.put('\n');
  return sum;
}
void createQuery(ostream& in, const unsigned int mq, const unsigned int ma,
                 unsigned long long suma, unsigned int k, unsigned int n)
{
  uniform_int_distribution<unsigned int> disa(1, ma), diso(1, 3);
  for (unsigned int i = 0; i < mq; ++i) {
    const unsigned int op = [&diso, suma, k, n]() {
      unsigned int t;
      do {
        t = diso(default_rnd);
      } while ((t == 1 && k == suma - n) || (t == 2 && k == 0));
      return t;
    }();
    in << op << " ";
    switch (op) {
      case 1:
        ++k;
        break;
      case 2:
        --k;
        break;
      case 3:
        const unsigned int t = disa(default_rnd);
        in << t;
        ++n;
        suma += t;
    }
    in << '\n';
  }
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  const unsigned int q = GetRandom(1, MayBe(atoi(argv[2]), maxq));
  const unsigned int ma = GetRandom(1, MayBe(atoi(argv[4]), maxa));
  ofstream in(argv[5]);
  ostringstream os;
  const unsigned long long sa = createA(os, n, ma);
  const unsigned int ik = GetRandom(
      0ull, min(sa - n, MayBe<unsigned long long>(atoi(argv[3]), maxk)));
  in << n << " " << q << " " << ik << '\n';
  in << os.str();
  createQuery(in, q, ma, sa, ik, n);
  return 0;
}