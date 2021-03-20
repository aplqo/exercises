#include <algorithm>
#include <fstream>
#include <functional>
#include <iterator>
#include <vector>

#include "debug_tools/generator.h"
using namespace apdebug::generator;

std::vector<int> factor(const int s)
{
  std::vector<int> ret;
  for (long long i = 1; i * i <= s; ++i)
    if (!(s % i)) ret.push_back(i);
  return ret;
}
void createMatrix(std::ofstream &in, const int n, const int m, const int mv)
{
  std::uniform_int_distribution<int> dis(2, mv);
  for (int i = 0; i < n; ++i) {
    std::generate_n(std::ostream_iterator<int>(in, " "), m,
                    std::bind(dis, std::ref(default_rnd)));
    in.put('\n');
  }
}
void createTestcase(std::ofstream &in, const int size, const int mv)
{
  const auto p = factor(size);
  const int n = p[GetRandom<size_t>(0, p.size() - 1)], m = size / n;
  in << n << " " << m << "\n";
  createMatrix(in, n, m, mv);
}
void getSize(const int sum, const int n, int dest[])
{
  int *const tmp = new int[n + 2];
  tmp[0] = 0;
  tmp[n] = sum - n;
  createArray(0, sum - n, n - 1, tmp + 1);
  std::sort(tmp + 1, tmp + n);
  for (int i = 0; i < n; ++i)
    dest[i] = tmp[i + 1] - tmp[i] + 1;
  delete[] tmp;
}

int main(int, char *argv[])
{
  Init();
  const int sum = GetRandom(1, atoi(argv[2]));
  const int t = GetRandom(1, std::min(sum, atoi(argv[1])));
  const int mv = GetRandom(1, atoi(argv[3]));
  std::ofstream in(argv[4]);
  int *const size = new int[t];
  getSize(sum, t, size);
  in << t << "\n";
  std::for_each(
      size, size + t,
      std::bind(createTestcase, std::ref(in), std::placeholders::_1, mv));
  delete[] size;
  return 0;
}