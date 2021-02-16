#include <algorithm>
#include <chrono>
#include <fstream>
#include <random>
#include <string>
using namespace std;
const unsigned int maxn = 1e7, maxm = 1000;

mt19937 rnd;

unsigned int maybe(const unsigned int v, const unsigned int def)
{
  return v ? v : def;
}
unsigned int get_random(const unsigned int l, const unsigned int r)
{
  uniform_int_distribution<unsigned int> dis(l, r);
  return dis(rnd);
}

void createPost(ofstream& f, const unsigned int n, const unsigned int m)
{
  uniform_int_distribution<unsigned int> dis(1, n);
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int t1 = dis(rnd), t2 = dis(rnd);
    if (t1 > t2) swap(t1, t2);
    f << t1 << " " << t2 << endl;
  }
}
int main(int argc, char* argv[])
{
  rnd.seed(std::chrono::steady_clock::now().time_since_epoch().count());
  ofstream in(argv[3]);
  const unsigned int n = get_random(1, maybe(atoi(argv[1]), maxn));
  const unsigned int m = get_random(1, maybe(atoi(argv[2]), maxm));
  in << n << " " << m << endl;
  createPost(in, n, m);
  return 0;
}
