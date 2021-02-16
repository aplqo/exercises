#include <fstream>
using namespace std;

unsigned long long factor(const unsigned long long v)
{
  unsigned long long ret = 0;
  for (unsigned long long i = 1; i <= v; ++i)
    if (v % i == 0) ++ret;
  return ret;
}
unsigned long long sum(const unsigned long long n)
{
  unsigned long long ret = 0;
  for (unsigned long long i = 1; i <= n; ++i)
    ret += factor(i);
  return ret;
}

int main(int, char* argv[])
{
  ifstream in(argv[1]);
  ofstream ans(argv[2]);
  unsigned long long n;
  in >> n;
  ans << sum(n) << endl;
  return 0;
}