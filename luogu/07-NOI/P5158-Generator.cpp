#include <algorithm>
#include <fstream>
#include <iterator>
#include <unordered_set>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 100000, mod = 998244353;

void createPoint(ofstream& in, const unsigned int n)
{
  uniform_int_distribution<unsigned int> disx(0, mod - 1), disy(0, mod - 1);
  unordered_set<unsigned int> st;
  for (unsigned int i = n; i; --i) {
    unsigned int x;
    do {
      x = disx(default_rnd);
    } while (st.contains(x));
    st.insert(x);
    in << x << " " << disy(default_rnd) << "\n";
  }
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = MayBe(atoi(argv[1]), maxn);
  ofstream in(argv[2]);
  in << n << "\n";
  createPoint(in, n);
  return 0;
}