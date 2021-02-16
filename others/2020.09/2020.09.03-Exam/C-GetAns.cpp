#include <algorithm>
#include <cstring>
#include <fstream>
using namespace std;
const unsigned int maxn = 1e6;
constexpr unsigned long long mod = 998244353;

char str[maxn + 10];

unsigned long long solve(const size_t len)
{
  static char stk[maxn + 10];
  unsigned long long ret = 0;
  for (unsigned int i = 1; i < len; ++i) {
    char* top = stk;
    *(top++) = str[i];
    for (int j = i - 1; j >= 0; --j) {
      if (top > stk && *(top - 1) == str[j])
        --top;
      else
        *(top++) = str[j];
      if (top == stk) ++ret;
    }
  }
  return ret;
}
int main(int, char* argv[])
{
  ifstream in(argv[1]);
  ofstream ans(argv[2]);
  in >> str;
  ans << solve(strlen(str)) % mod << "\n";
  return 0;
}