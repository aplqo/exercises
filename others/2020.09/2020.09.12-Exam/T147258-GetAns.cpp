#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 10000000;
constexpr unsigned long long mod = 1e9 + 7;

unsigned int seq[maxn + 1];

bool isGood(const unsigned int s[], const unsigned int n)
{
  static unsigned int f[maxn + 10];
  for (unsigned int i = 0; i < n; ++i) {
    f[i] = 1;
    for (unsigned int j = 0; j < i; ++j)
      if (s[j] > s[i]) f[i] = max(f[j] + 1, f[i]);
    if (f[i] > 2) return false;
  }
  return true;
}
unsigned long long enumerate(const unsigned int n, const unsigned int x,
                             const unsigned int y)
{
  static unsigned int seq[maxn + 10];
  unsigned long long ret = 0;
  iota(seq, seq + n, 1);
  do {
    if (isGood(seq, n) && seq[x - 1] == y) {
      ret = (ret + 1) % mod;
#ifdef PrintLog
      copy(seq, seq + n, ostream_iterator<unsigned int>(cout, " "));
      cout << "\n";
#endif
    }
  } while (next_permutation(seq, seq + n));
#ifdef PrintLog
  cout << endl;
#endif
  return ret;
}
int main(int, char* argv[])
{
  ifstream in(argv[1]);
  ofstream fans(argv[2]);
  unsigned int t;
  in >> t;
  for (unsigned int i = 0; i < t; ++i) {
    unsigned int n, x, y;
    in >> n >> x >> y;
    fans << enumerate(n, x, y) << "\n";
  }
  return 0;
}