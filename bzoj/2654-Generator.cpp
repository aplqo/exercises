#include <cstring>
#include <fstream>
#include <sstream>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxv = 50000, maxe = 100000, maxw = 100;

unsigned int createTree(ostream& in, const unsigned int n,
                        const unsigned int mw)
{
  uniform_int_distribution<unsigned int> dis(1, mw), dist(0, 1);
  unsigned int cnt = 0;
  for (unsigned int i = 1; i < n; ++i) {
    const unsigned int t = dist(default_rnd);
    in << i << " " << GetRandom(0u, i - 1) << " " << dis(default_rnd) << " "
       << t << endl;
    cnt += !t;
  }
  return cnt;
}
unsigned int createEdge(ostream& in, const unsigned int n, const unsigned int m,
                        const unsigned int mw)
{
  uniform_int_distribution<unsigned int> disv(0, n - 1), dist(0, 1),
      disw(1, mw);
  unsigned int cnt = 0;
  for (unsigned int i = 0; i < m; ++i) {
    const unsigned int t = dist(default_rnd);
    unsigned int u, v;
    do {
      u = disv(default_rnd);
      v = disv(default_rnd);
    } while (u == v);
    in << u << " " << v << " " << disw(default_rnd) << " " << t << endl;
    cnt += !t;
  }
  return cnt;
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(2, MayBe(atoi(argv[1]), maxv));
  const unsigned int m =
      GetRandom(n - 1, max(n - 1, MayBe<unsigned int>(atoi(argv[2]), maxv)));
  const unsigned int mw = MayBe(atoi(argv[3]), maxw);
  ostringstream os;
  ofstream in(argv[4]);
  in << n << " " << m;
  const unsigned int cnt1 = createTree(os, n, mw);
  const unsigned int cnt2 = createEdge(os, n, m - (n - 1), mw);
  in << " " << cnt1 << endl;
  in << os.str() << endl;
  return 0;
}