#include <cstring>
#include <fstream>
#include <random>

#include "debug_tools/generator.h"
using namespace apdebug::generator;

void createPoint(std::ofstream &in, const int n, const int mv)
{
  std::uniform_int_distribution<int> dis(1, mv);
  for (int i = 0; i < n; ++i)
    in << dis(default_rnd) << " " << dis(default_rnd) << "\n";
}
void createConstraint(std::ofstream &in, const int m, const int n, const int mv)
{
  std::uniform_int_distribution<int> disp(1, mv), disv(1, 2), disc(1, n);
  for (int i = 0; i < m; ++i)
    in << disv(default_rnd) << " " << disp(default_rnd) << " "
       << disc(default_rnd) << "\n";
}
void createCost(std::ofstream &in, const int mc)
{
  std::uniform_int_distribution<int> dis(1, mc);
  in << dis(default_rnd) << " " << dis(default_rnd) << "\n";
}

int main(int, char *argv[])
{
  Init();
  const int n = GetRandom(1, atoi(argv[1])), m = GetRandom(1, atoi(argv[2]));
  const int mv = atoi(argv[3]);
  std::ofstream in(argv[5]);
  in << n << " " << m << "\n";
  createCost(in, atoi(argv[4]));
  createPoint(in, n, mv);
  createConstraint(in, m, n, mv);
  return 0;
}