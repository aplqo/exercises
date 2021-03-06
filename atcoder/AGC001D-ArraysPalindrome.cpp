#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
const int maxn = 1e5;

int a[maxn + 10], b[maxn + 10];

bool shuffle(const int n)
{
  int cnt = 0;
  if (a[0] & 0x01) ++cnt;
  if (a[n - 1] & 0x01) ++cnt;
  for (int i = 1; i + 1 < n; ++i)
    if (a[i] & 0x01) {
      switch (cnt) {
        case 0:
          std::swap(a[i], a[0]);
          break;
        case 1:
          std::swap(a[i], a[n - 1]);
          break;
        default:
          return false;
      }
      ++cnt;
    }
  return true;
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n, m;
  std::cin >> n >> m;
  std::copy_n(std::istream_iterator<int>(std::cin), m, a);
  if (m == 1) {
    std::cout << a[0] << "\n";
    if (a[0] > 1)
      std::cout << "2\n" << a[0] - 1 << " " << 1 << "\n";
    else
      std::cout << "1\n1\n";
    return 0;
  }
  if (!shuffle(m)) {
    std::cout << "Impossible\n";
    return 0;
  }
  std::copy(a, a + m, std::ostream_iterator<int>(std::cout, " "));
  std::cout.put('\n');
  --a[0];
  ++a[m - 1];
  std::cout << m - (!a[0]) << "\n";
  std::copy(a + (!a[0]), a + m, std::ostream_iterator<int>(std::cout, " "));
  std::cout.put('\n');
  return 0;
}