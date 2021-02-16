#include <algorithm>
#include <cstring>
#include <fstream>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using apdebug::generator::default_rnd;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const int maxn = 100000, maxm = 100000, maxa = 1e6;

void createWeight(ofstream& f, const unsigned int n, const int ma)
{
  uniform_int_distribution<int> dis(-ma, ma);
  generate_n(ostream_iterator<int>(f, " "), n,
             [&dis]() { return dis(default_rnd); });
  f << endl;
}
void createTree(ofstream& f, const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i)
    f << GetRandom(1u, i - 1) << " " << i << endl;
}
void createQuery(ofstream& f, const unsigned int n, const unsigned int m,
                 const int ma)
{
  uniform_int_distribution<unsigned int> disop(1, 3), disx(1, n);
  uniform_int_distribution<int> disa(-ma, ma);
  for (unsigned int i = 0; i < m; ++i) {
    const unsigned int op = disop(default_rnd), x = disx(default_rnd);
    f << op << " " << x;
    if (op == 1 || op == 2) f << " " << disa(default_rnd);
    f << endl;
  }
}

int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
  const int ma = MayBe(atoi(argv[3]), maxa);
  ofstream in(argv[4]);
  in << n << " " << m << endl;
  createWeight(in, n, ma);
  createTree(in, n);
  createQuery(in, n, m, ma);
  return 0;
}