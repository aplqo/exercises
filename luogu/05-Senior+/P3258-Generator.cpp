#include <algorithm>
#include <fstream>
#include <iterator>
#include <numeric>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using apdebug::generator::default_rnd;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const int maxn = 300000;

void createSeq(ofstream& f, const unsigned int n)
{
  static unsigned int seq[maxn + 1];
  iota(seq, seq + n, 1);
  shuffle(seq, seq + n, default_rnd);
  copy_n(seq, n, ostream_iterator<unsigned int>(f, " "));
  f << endl;
}
void createTree(ofstream& f, const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i)
    f << GetRandom(1u, i - 1) << " " << i << endl;
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(2, MayBe(atoi(argv[1]), maxn));
  ofstream in(argv[2]);
  in << n << endl;
  createSeq(in, n);
  createTree(in, n);
  return 0;
}