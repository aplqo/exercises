#include <algorithm>
#include <cstring>
#include <fstream>
#include <numeric>
#include <sstream>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 600, maxm = 1e5, maxp = 1e9 + 7;

unsigned int shf[maxn + 10], indeg[maxn + 10], outdeg[maxn + 10], cnt;
inline bool isPrime(const unsigned int p)
{
  for (unsigned int i = 2; i * i <= p; ++i)
    if (p % i == 0) return false;
  return true;
}
unsigned int createPrime(const unsigned int mp)
{
  uniform_int_distribution<unsigned int> disp(2, mp);
  unsigned int ret;
  do {
    ret = disp(default_rnd);
  } while (!isPrime(ret));
  return ret;
}
inline void addEdge(ostream& in, const unsigned int from, const unsigned int to)
{
  in << shf[from] << " " << shf[to] << "\n";
  ++indeg[to];
  ++outdeg[from];
  ++cnt;
}
inline void createDAG(ostream& in, const unsigned int n,
                      const unsigned int endpoint, const unsigned int m)
{
  uniform_int_distribution<unsigned int> disf(0, n - 1 - endpoint);
  for (unsigned int i = 0; i < m; ++i) {
    const unsigned int u = disf(default_rnd);
    addEdge(in, u, GetRandom(max(u + 1, endpoint), n - 1));
  }
}
void createGraph(ostream& in, const unsigned int n, const unsigned int endpoint,
                 const unsigned int ignore, const unsigned int m)
{
  iota(shf, shf + n, 1);
  shuffle(shf, shf + n, default_rnd);
  createDAG(in, n - ignore, endpoint, m);
  for (unsigned int i = 0; i < n - ignore - endpoint; ++i)
    if (!outdeg[i])
      addEdge(in, i, GetRandom(max(endpoint, i + 1), n - 1 - ignore));
  for (unsigned int i = endpoint; i < n - ignore; ++i)
    if (!indeg[i])
      addEdge(in, GetRandom(0u, min(i - 1, n - 1 - ignore - endpoint)), i);
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(2, MayBe(atoi(argv[1]), maxn));
  const unsigned int ignore = GetRandom(0u, n - 2),
                     endpoint = GetRandom(1u, (n - ignore) / 2);
  const unsigned int m = GetRandom<unsigned int>(0, MayBe(atoi(argv[2]), maxm));
  ofstream in(argv[4]);
  ostringstream os;
  createGraph(os, n, endpoint, ignore, m);
  in << n << " " << cnt << " " << createPrime(MayBe(atoi(argv[3]), maxp))
     << "\n";
  in << os.str();
  return 0;
}