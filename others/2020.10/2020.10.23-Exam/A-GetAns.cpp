#include <algorithm>
#include <bitset>
#include <fstream>
#include <iterator>
using namespace std;
const unsigned int maxn = 100000, maxa = 100000, maxp = 10000;
typedef bitset<maxp> Factors;

unsigned int mindiv[maxa + 10], primes[maxa + 10], *pcur = primes;
unsigned int pid[maxa + 10];
unsigned int a[maxn + 10];
Factors fac[maxn + 10];

static void eular(const unsigned int ma)
{
  for (unsigned long long i = 2; i <= ma; ++i) {
    if (!mindiv[i]) {
      pid[i] = pcur - primes;
      mindiv[i] = i;
      *(pcur++) = i;
    }
    for (const unsigned int* j = primes;
         j <= pcur && *j * i <= ma && *j <= mindiv[i]; ++j)
      mindiv[*j * i] = *j;
  }
}
void factor(Factors& f, unsigned int v)
{
  while (v > 1) {
    f.flip(pid[mindiv[v]]);
    v /= mindiv[v];
  }
}
static void initFactor(const unsigned int n)
{
  for (unsigned int i = 0; i < n; ++i)
    factor(fac[i], a[i]);
}
unsigned long long count(const unsigned int n)
{
  unsigned long long ret = 0;
  for (unsigned int i = 0; i < n; ++i) {
    Factors acc;
    for (unsigned int r = i; r < n; ++r) {
      acc ^= fac[r];
      if (acc.none()) ++ret;
    }
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
  eular(maxa);
  initFactor(n);
  ans << count(n) << "\n";
  return 0;
}