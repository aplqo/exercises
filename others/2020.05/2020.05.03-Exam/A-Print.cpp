#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
using namespace std;
using real = long double;
using distribution = uniform_int_distribution<unsigned int>;
const int maxn = 100000;
const unsigned int samplen = 5000, swapn = 50, avgn = 15;

mt19937_64 rnd;
unsigned int t1[maxn + 10], t2[maxn + 10];

unsigned int compare(unsigned int n)
{
  unsigned int ret = 0;
  for (unsigned int i = 0; i < n; ++i)
    ret += t1[i] == t2[i];
  return ret;
}
unsigned int change(unsigned int seq[], unsigned int lv, const unsigned int n)
{
  uniform_int_distribution<unsigned int> sdis(1, min(n * (n - 1) / 2, swapn)),
      dis(0, n - 1);
  const auto testb = [](const unsigned int p) { return t1[p] == t2[p]; };
  const unsigned int sn = sdis(rnd);
  for (unsigned int i = 0; i < sn; ++i) {
    unsigned int p1 = dis(rnd), p2 = dis(rnd);
    while (p2 == p1)
      p2 = dis(rnd);
    lv -= testb(p1) + testb(p2);
    swap(seq[p1], seq[p2]);
    lv += testb(p1) + testb(p2);
  }
  return lv;
}
real fImpl(const unsigned int n)
{
  unsigned int v = 0, lv = 0;
  iota(t1, t1 + n, 0);
  iota(t2, t2 + n, 0);
  shuffle(t1, t1 + n, rnd);
  shuffle(t2, t2 + n, rnd);
  lv = compare(n);
  for (unsigned int i = 0; i < samplen; ++i) {
    lv = change(t1, lv, n);
    v += lv;
    lv = change(t2, lv, n);
    v += lv;
  }
  return real(1) * v / (2 * samplen);
}
real f(const unsigned int n)
{
  real ret = 0;
  for (unsigned int i = 0; i < avgn; ++i)
    ret += fImpl(n);
  ret /= avgn;
  return ret;
}
int main(int argc, char* argv[])
{
  rnd.seed(random_device()());
  unsigned int beg = stoi(argv[1]), end = stoi(argv[2]);
  cout << "F[1] = 1" << endl;
  for (unsigned int i = beg; i <= beg + end; ++i)
    cout << "F[" << i << "] = " << f(i) << endl;
  return 0;
}