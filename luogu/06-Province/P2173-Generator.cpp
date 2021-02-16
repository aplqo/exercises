#include <algorithm>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <set>
#include <sstream>
#include <utility>
#include <vector>

#include "debug_tools/generator.h"

using namespace std;
using namespace apdebug::generator;
typedef pair<unsigned int, unsigned int> Edge;
typedef uniform_int_distribution<unsigned int> Distribution;
const int maxn = 1e4, maxm = 1e5, maxc = 10, maxk = 1e5, maxv = 1e5;

class Disjoint {
 public:
  void init(const unsigned int n)
  {
    iota(fa, fa + 1 + n, 0);
    fill(siz, siz + 1 + n, 1);
  }
  unsigned int find(const unsigned int x)
  {
    return fa[x] == x ? x : fa[x] = find(fa[x]);
  }
  bool isConnect(const unsigned int x, const unsigned int y)
  {
    return find(x) == find(y);
  }
  void merge(unsigned int x, unsigned int y)
  {
    x = find(x);
    y = find(y);
    if (siz[x] < siz[y]) swap(x, y);
    fa[y] = x;
    siz[x] += siz[y];
  }

 private:
  unsigned int fa[maxn + 10], siz[maxn + 10];
};
Disjoint gra[maxc + 10];
vector<unsigned int> sel[maxc + 10];
unsigned int degree[maxn + 10], cnt[maxc + 10];
set<Edge> eds;

void initGraph(const unsigned int n, const unsigned int c)
{
  sel[0].reserve(n * 2);
  for (unsigned int i = 1; i <= n; ++i) {
    sel[0].push_back(i);
    sel[0].push_back(i);
  }
  gra[0].init(n);
  for (unsigned int i = 1; i < c; ++i) {
    sel[i] = sel[0];
    gra[i] = gra[0];
  }
}
bool checkColor(const unsigned int c, const unsigned int cn,
                const unsigned int n)
{
  const auto it =
      remove_if(sel[c].begin(), sel[c].end(),
                [n](const unsigned int i) { return degree[i] == n - 1; });
  sel[c].erase(it, sel[c].end());
  if (!sel[c].empty()) {
    for (unsigned int tc = 0; tc < cn; ++tc) {
      const unsigned int beg = gra[tc].find(sel[c][0]);
      for (unsigned int i : sel[c])
        if (gra[tc].find(i) != beg) goto Pass;
      return false;
    Pass:;
    }
  }
  return sel[c].size() > 1 && cnt[c] < n - 1;
}
inline Edge addEdge(unsigned int u, unsigned int v, const unsigned int c)
{
  if (u > v) swap(u, v);
  ++degree[u];
  ++degree[v];
  ++cnt[c];
  gra[c].merge(u, v);
  eds.emplace(u, v);
  return Edge(u, v);
}
pair<bool, Edge> generateEdge(const unsigned int c)
{
  Distribution disp(0, sel[c].size() - 1);
  unsigned int u = disp(default_rnd), v = disp(default_rnd);
  if (sel[c][u] > sel[c][v]) swap(u, v);
  if (gra[c].isConnect(sel[c][u], sel[c][v]) ||
      eds.count(Edge{sel[c][u], sel[c][v]}))
    return make_pair(false, Edge());
  const Edge ret = addEdge(sel[c][u], sel[c][v], c);
  sel[c].erase(sel[c].begin() + u);
  sel[c].erase(sel[c].begin() + v - (u < v));
  return make_pair(true, ret);
}

void createGraph(ostream& in, const unsigned int n, const unsigned int m,
                 const unsigned int c)
{
  static unsigned long long fail[maxc + 10];
  vector<unsigned int> colors;
  initGraph(n, c);
  uniform_int_distribution<unsigned int> disc(0, c - 1);
  colors.resize(c);
  iota(colors.begin(), colors.end(), 0);
  for (unsigned int i = 0; i < m && !colors.empty();) {
    unsigned int co;
    while (!colors.empty()) {
      co = GetRandom(0ul, colors.size() - 1);
      if (!checkColor(colors[co], c, n))
        colors.erase(colors.begin() + co);
      else
        break;
    }
    if (colors.empty()) break;
    const auto [suc, e] = generateEdge(colors[co]);
    if (suc) {
      ++i;
      fail[colors[co]] = 0;
      in << e.first << " " << e.second << " " << colors[co] << endl;
    }
    ++fail[colors[co]];
    if (fail[colors[co]] >
        sel[colors[co]].size() * (sel[colors[co]].size() - 1) / 2)
      colors.erase(colors.begin() + co);
  }
}
void createWeight(ostream& in, const unsigned int n, const unsigned int mv)
{
  Distribution dis(1, mv);
  generate_n(ostream_iterator<unsigned int>(in, "\n"), n,
             bind(dis, ref(default_rnd)));
}
void createQuery(ostream& in, const unsigned int n, const unsigned int k,
                 const unsigned int m, const unsigned int c,
                 const unsigned int mv)
{
  const static auto genNeq = [](Distribution& dis, const unsigned int v) {
    unsigned int ret;
    do {
      ret = dis(default_rnd);
    } while (ret == v);
    return ret;
  };
  Distribution disp(1, n), diso(0, 2), disv(1, mv), disc(0, c - 1);
  for (unsigned int i = 0; i < k; ++i) {
    const unsigned int op = diso(default_rnd), p = disp(default_rnd);
    in << op << " ";
    switch (op) {
      case 0:
        in << p << " " << disv(default_rnd);
        break;
      case 1:
        in << p << " " << genNeq(disp, p) << " " << disc(default_rnd);
        break;
      case 2:
        in << disc(default_rnd) << " " << p << " " << genNeq(disp, p);
        break;
    }
    in.put('\n');
  }
}
int main(int, char* argv[])
{
  Init();
  const static auto genRange = [](const int l, const char* str, const int def) {
    return GetRandom(l, MayBe(atoi(str), def));
  };
  const unsigned int n = genRange(2, argv[1], maxn);
  const unsigned int c = genRange(1, argv[4], maxc);
  const unsigned int m = GetRandom<uint64_t>(
      1, min<uint64_t>({1ull * (n - 1) * c, 1ull * n * (n - 1) / 2,
                        MayBe<uint64_t>(atoi(argv[2]), maxm)}));
  const unsigned int k = genRange(1, argv[3], maxk);
  const unsigned int mv = MayBe(atoi(argv[5]), maxv);
  ofstream in(argv[6]);
  ostringstream buf;
  createGraph(buf, n, m, c);
  in << n << " " << eds.size() << " " << c << " " << k << "\n";
  createWeight(in, n, mv);
  in << buf.str();
  createQuery(in, n, k, m, c, mv);
  return 0;
}