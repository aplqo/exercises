#include <fstream>
using namespace std;
const unsigned long long mod = 998244353;

class Number {
 public:
  Number(const unsigned long long v = 0) : val(v % mod) {}
#define decl(x)                                                    \
  inline friend Number operator x(const Number l, const Number r); \
  inline Number& operator x##=(const Number r) { return *this = *this x r; }
  decl(+);
  decl(*);
  decl(-);
#undef decl
  inline friend ostream& operator<<(ostream& os, const Number v);

 private:
  unsigned long long val;
};
#define impl(x)                                            \
  inline Number operator x(const Number l, const Number r) \
  {                                                        \
    return (l.val + mod) x r.val;                          \
  }
impl(+);
impl(-);
impl(*);
#undef impl
inline ostream& operator<<(ostream& os, const Number v) { return os << v.val; }
Number quickPow(Number a, unsigned long long e)
{
  Number ret = 1;
  for (unsigned long long i = 1; e; i <<= 1) {
    if (e & i) {
      ret *= a;
      e ^= i;
    }
    a *= a;
  }
  return ret;
}

Number f[max]

    Number
    solve(const unsigned int n, const unsigned long long t, const Number inv)
{
  Number one = 1, other = 0;
  for (unsigned long long i = 1; i <= t; ++i) {
    const Number lstone = one, lstother = other;
    one = lstother;
    other = (lstone + lstother * (n - 2)) * inv;
  }
  return one;
}
int main(int, char* argv[])
{
  ifstream in(argv[1]);
  ofstream fans(argv[2]);
  unsigned long long n, q;
  in >> n >> q;
  const Number inv = quickPow(n - 1, mod - 2);
  for (unsigned int i = 0; i < q; ++i) {
    unsigned long long t;
    in >> t;
    fans << solve(n, t, inv) << endl;
  }
  return 0;
}
