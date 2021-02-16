#include <fstream>
#include <random>
#include <string>
using namespace std;
using num_t = unsigned long long;
const unsigned int maxn = 1e5;
const num_t maxa = 1e8;

mt19937_64 rnd;

template <class T>
T GetRandom(T a, T b)
{
  uniform_int_distribution<T> dis(a, b);
  return dis(rnd);
}
template <class T>
T MayBe(const T val, const T def)
{
  return val ? val : def;
}

void CreateData(ofstream& of, const unsigned int n, const num_t ma)
{
  uniform_int_distribution<num_t> dis(1, ma);
  for (unsigned int i = 0; i < n; ++i)
    of << dis(rnd) << " ";
  of << endl;
}
void CreateQuery(ofstream& of, const unsigned int n, const unsigned int m,
                 const num_t ma)
{
  uniform_int_distribution<unsigned int> disl(1, n);
  uniform_int_distribution<num_t> disx(1, ma);
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int l = disl(rnd), r = GetRandom(l, n);
    num_t x = disx(rnd);
    of << x << " " << l << " " << r << endl;
  }
}

int main(int argc, char* argv[])
{
  rnd.seed(random_device()());
  ofstream in(argv[4]);
  unsigned int n = MayBe<unsigned int>(stoi(argv[1]), GetRandom(1u, maxn));
  unsigned int m = MayBe<unsigned int>(stoi(argv[2]), GetRandom(1u, maxn));
  num_t a = MayBe<num_t>(stoull(argv[3]), maxa);
  in << n << " " << m << endl;
  CreateData(in, n, a);
  CreateQuery(in, n, m, a);
  in.close();
  return 0;
}