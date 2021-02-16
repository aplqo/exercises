#include <algorithm>
#include <fstream>
#include <iterator>
using namespace std;
const unsigned int maxn = 100000;
constexpr unsigned long long mod = 1e9 + 7;

unsigned long long t[maxn + 1], f[maxn + 1];

unsigned long long solve(const unsigned int n, const unsigned long long a,
                         const unsigned long long b)
{
  for (unsigned int i = 2; i <= n; ++i) {
    f[1] = t[i];
    for (unsigned int j = 2; j <= n; ++j)
      f[j] = ((f[j] * b) % mod + (f[j - 1] * a) % mod) % mod;
  }
  return f[n];
}
int main(int argc, char* argv[])
{
  ifstream in(argv[1]);
  ofstream ans(argv[2]);
  unsigned int n, a, b;
  in >> n >> a >> b;
  copy_n(istream_iterator<unsigned long long>(in), n, t + 1);
  copy_n(istream_iterator<unsigned long long>(in), n, f + 1);
  ans << solve(n, a, b) << endl;
  return 0;
}