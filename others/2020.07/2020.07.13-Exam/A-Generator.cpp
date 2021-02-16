#include <algorithm>
#include <chrono>
#include <climits>
#include <cstring>
#include <fstream>
#include <numeric>
#include <random>
using namespace std;
const int maxn = 1e5, maxv = INT_MAX;

mt19937 rnd;
unsigned int shf[maxn + 1];

int maybe(const int v, const int def) { return v ? v : def; }
unsigned int get_random(const unsigned int l, const unsigned int r)
{
  uniform_int_distribution<unsigned int> dis(l, r);
  return dis(rnd);
}
void createShf(const unsigned int n)
{
  iota(shf, shf + 1 + n, 0);
  shuffle(shf + 1, shf + 1 + n, rnd);
}
void createTree(ofstream& f, const unsigned int n, const int mv)
{
  uniform_int_distribution<int> dis(-mv, mv);
  for (unsigned int i = 2; i <= n; ++i)
    f << dis(rnd) << " " << shf[i] << " " << shf[get_random(1, i - 1)] << endl;
}

int main(int, char* argv[])
{
  rnd.seed(std::chrono::steady_clock::now().time_since_epoch().count());
  const unsigned int n = get_random(1, maybe(atoi(argv[1]), maxn));
  ofstream in(argv[3]);
  in << n << endl;
  createShf(n);
  createTree(in, n, maybe(atoi(argv[2]), maxv));
  return 0;
}
