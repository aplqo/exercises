#include <cstring>
#include <fstream>
#include <set>

#include "debug_tools/generator.h"
using namespace apdebug::generator;

std::set<std::pair<int, int>> edge;
void createTree(std::ofstream &in, const int n, const int mv)
{
  std::uniform_int_distribution<int> dis(1, mv);
  for (int i = 2; i <= n; ++i) {
    const int p = GetRandom(1, i - 1);
    edge.emplace(p, i);
    in << p << " " << i << " " << dis(default_rnd) << "\n";
  }
}
void createGraph(std::ofstream &in, const int n, const int mv)
{
  createTree(in, n, mv);
  {
    std::uniform_int_distribution<int> disp(1, n);
    int u, v;
    do {
      u = disp(default_rnd);
      v = disp(default_rnd);
      if (u > v) std::swap(u, v);
    } while (u == v || edge.contains(std::pair<int, int>(u, v)));
    in << u << " " << v << " " << GetRandom(1, mv) << "\n";
  }
}

int main(int, char *argv[])
{
  Init();
  const int n = GetRandom(3, atoi(argv[1]));
  std::ofstream in(argv[3]);
  in << n << "\n";
  createGraph(in, n, atoi(argv[2]));
  return 0;
}