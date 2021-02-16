#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 50000;

int a[maxn * 2 + 10];

int cal(const unsigned int n)
{
  int absSum = 0, sum = 0;
  for (unsigned int i = 0; i <= n * 2; i += 2) {
    absSum += abs(a[i] - a[i + 1]);
    sum += a[i] - a[i + 1];
  }
  return absSum - abs(sum);
}
unsigned long long enumerate(const unsigned int n, const int k)
{
  unsigned long long ret = 0;
  iota(a, a + n * 2, 1);
  do {
    if (cal(n) == 2 * k) {
      copy(a, a + n * 2, ostream_iterator<unsigned int>(cout, " "));
      cout << "\n";
      ++ret;
    }
  } while (next_permutation(a, a + n * 2));
  return ret;
}
int main()
{
  while (true) {
    unsigned int n, k;
    cout << "359B-Print>" << flush;
    cin >> n >> k;
    if (cin.fail()) break;
    const unsigned long long cnt = enumerate(n, k);
    cout << "Count: " << cnt << "\n";
  }
  return 0;
}