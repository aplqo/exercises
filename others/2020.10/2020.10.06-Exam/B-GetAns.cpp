#include <algorithm>
#include <fstream>
#include <iterator>
#include <numeric>
#include <utility>
#include <vector>
using namespace std;
const unsigned int maxn = 5e5;
typedef pair<unsigned int, unsigned int> Range;

unsigned int a[maxn + 10];

bool check(const unsigned int* l, const unsigned int* r)
{
  const unsigned int g = accumulate(
      l, r, *l, [](unsigned int a, unsigned int b) { return gcd(a, b); });
  for (const unsigned int* i = l; i < r; ++i)
    if (g % *i == 0) return true;
  return false;
}
vector<Range> enumerate(const unsigned int n)
{
  vector<Range> ret;
  unsigned int v = 0;
  for (unsigned int i = 0; i < n; ++i)
    for (unsigned int j = i + 1; j < n + 1; ++j)
      if (check(a + i, a + j)) {
        if (j - i > v) {
          v = j - i;
          ret.clear();
          ret.emplace_back(i, j);
        }
        else if (j - i == v)
          ret.emplace_back(i, j);
      }
  return ret;
}
int main(int, char* argv[])
{
  ifstream in(argv[1]);
  ofstream ans(argv[2]);
  unsigned int n;
  in >> n;
  copy_n(istream_iterator<unsigned int>(in), n, a);
  const vector<Range> v = enumerate(n);
  ans << v.size() << " " << v.front().second - v.front().first - 1 << "\n";
  for (const auto i : v)
    ans << i.first + 1 << " ";
  ans.put('\n');
  return 0;
}