#include <algorithm>
#include <fstream>
using namespace std;
const unsigned int maxl = 1e5;
constexpr unsigned long long mod = 1e9 + 7;

bool sum(const bool* l, const bool* r)
{
  bool ret = *l;
  for (const bool* i = l + 1; i < r; ++i)
    ret = !(ret || *i);
  return ret;
}
unsigned long long enumerate(const unsigned int n, const unsigned int m,
                             const bool g)
{
  static bool buf[maxl * 2 + 10];
  fill(buf + m, buf + m + n, true);
  unsigned long long ret = 0;
  do {
    ret += sum(buf, buf + n + m) == g;
  } while (next_permutation(buf, buf + n + m));
  return ret;
}
int main(int, char* argv[])
{
  ifstream in(argv[1]);
  ofstream ans(argv[2]);
  unsigned int n, m, g;  // n : number 1, m: number 0
  in >> m >> n >> g;
  ans << enumerate(n, m, g) << endl;
  return 0;
}
