#include <algorithm>
#include <cstring>
#include <fstream>
#include <numeric>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e4, maxv = 1e9, maxk = 100, maxm = 1e4;

struct Edge {
  unsigned int from, to;
};
vector<Edge> ed;

void createPath(const unsigned int n, const unsigned int len,
                const unsigned int s, const unsigned int t)
{
  uniform_int_distribution<unsigned int> dis(1, n);
  unsigned int lst = s;
  for (unsigned int i = 1; i < len; ++i) {
    const unsigned int u = dis(default_rnd);
    ed.push_back(Edge{lst, u});
    lst = u;
  }
  ed.push_back(Edge{lst, t});
}
void createEdge(const unsigned int n, const unsigned int cnt)
{
  uniform_int_distribution<unsigned int> dis(1, n);
  for (unsigned int i = 0; i < cnt; ++i) {
    const unsigned int u = dis(default_rnd), v = dis(default_rnd);
    ed.push_back(Edge{u, v});
  }
}
void createGraph(const unsigned int n, const unsigned int cnt,
                 const unsigned int s1, const unsigned int s2,
                 const unsigned int t)
{
  unsigned int v1 = GetRandom(1u, cnt), v2 = GetRandom(0u, cnt - 1);
  if (v1 > v2) swap(v1, v2);
  unsigned int d[3] = {v1, v2 - v1, cnt - v2};
  if (!d[0])
    swap(d[0], d[2]);
  else if (!d[1])
    swap(d[1], d[2]);
  createPath(n, d[0], s1, t);
  createPath(n, d[1], s2, t);
  createEdge(n, d[2]);
}
void createWeight(ofstream& in, const unsigned int k, const unsigned int m,
                  const unsigned int mv)
{
  uniform_int_distribution<unsigned int> disw(1, mv);
  shuffle(ed.begin(), ed.end(), default_rnd);
  for (auto it = ed.cbegin(); it != ed.cbegin() + m; ++it)
    in << it->from << " " << it->to << " " << disw(default_rnd) << "\n";
  for (auto it = ed.cbegin() + m; it != ed.cend(); ++it) {
    const unsigned int v1 = disw(default_rnd), v2 = disw(default_rnd);
    in << it->from << " " << it->to << " " << min(v1, v2) << " " << max(v1, v2)
       << "\n";
  }
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
  const unsigned int k = GetRandom(1, MayBe(atoi(argv[3]), maxk));
  ofstream in(argv[5]);
  in << n << " " << m << " " << k << "\n";
  uniform_int_distribution<unsigned int> dis(1, n);
  const unsigned int s1 = dis(default_rnd), s2 = dis(default_rnd),
                     f = dis(default_rnd);
  in << s1 << " " << s2 << " " << f << "\n";
  createGraph(n, k + m, s1, s2, f);
  createWeight(in, k, m, MayBe(atoi(argv[4]), maxv));
  return 0;
}