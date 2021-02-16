#include <fstream>
using namespace std;
const int maxn = 50000;

char str[maxn + 10][6];
unsigned int n, m;

unsigned int calc(const char str1[], const char str2[])
{
  unsigned int ret = 0;
  for (unsigned int i = 0; i < 4; ++i)
    if (str1[i] != str2[i]) ++ret;
  return ret;
}
unsigned int solve(const unsigned int n, const unsigned int m)
{
  unsigned int ret = 0;
  for (unsigned int i = 0; i < n; ++i)
    for (unsigned int j = 0; j < n; ++j)
      ret += calc(str[i], str[j]) == m;
  ret /= 2;
  return ret;
}

int main(int argc, char* argv[])
{
  unsigned int n, m;
  {
    ifstream in(argv[1]);
    in >> n >> m;
    for (unsigned int i = 0; i < n; ++i)
      in >> str[i];
  }
  {
    ofstream ans(argv[2]);
    ans << solve(n, m) << endl;
  }
  return 0;
}