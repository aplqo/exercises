#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <utility>
using namespace std;
typedef int num_t;
const num_t m = 23 * 28 * 33;

template <num_t a, num_t b>
class inverse {
 public:
  enum : num_t {
    X = inverse<b, a % b>::Y,
    Y = inverse<b, a % b>::X - (a / b) * inverse<b, a % b>::Y
  };
};
template <num_t a>
class inverse<a, 0> {
 public:
  enum { X = 1, Y = 0 };
};
template <num_t a>
const num_t getInverse()
{
  return inverse<m / a, a>::X * (m / a);
}

inline num_t solve(const num_t p, const num_t e, const num_t i, const num_t d)
{
  num_t ans =
      p * getInverse<23>() + e * getInverse<28>() + i * getInverse<33>();
  return ans + (ans <= d ? (d - ans + m) / m : -((ans - d - 1) / m)) * m - d;
}
int main()
{
  ios_base::sync_with_stdio(false);
  unsigned int t = 1;
  while (true) {
    num_t p, e, i, d;
    cin >> p >> e >> i >> d;
    if (p == -1 && i == -1 && e == -1 && d == -1) return 0;
    cout << "Case " << t << ": the next triple peak occurs in ";
    cout << solve(p, e, i, d) << " days." << endl;
    ++t;
  }
  return 0;
}