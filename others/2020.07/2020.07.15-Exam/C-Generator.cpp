#include <chrono>
#include <cstring>
#include <fstream>
#include <random>
using namespace std;
const unsigned int maxn = 2000;

mt19937 rnd;
unsigned int maybe(const unsigned int v, const unsigned int def)
{
  return v ? v : def;
}
unsigned int getRandom(const unsigned int l, const unsigned int r)
{
  uniform_int_distribution<unsigned int> dis(l, r);
  return dis(rnd);
}

int main(int, char* argv[])
{
  rnd.seed(std::chrono::steady_clock::now().time_since_epoch().count());
  ofstream in(argv[2]);
  const unsigned int mn = maybe(atoi(argv[1]), maxn);
  const unsigned int n = getRandom(1u, mn), m = getRandom(1u, mn);
  uniform_int_distribution<char> dis('0', '1');
  in << n << " " << m << endl;
  for (unsigned int i = 0; i < n; ++i) {
    for (unsigned int j = 0; j < m; ++j)
      in << dis(rnd);
    in << endl;
  }
  return 0;
}
