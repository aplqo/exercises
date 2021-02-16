/* Optmiize: -march=native -fvectorize -ftree-vectorize -Ofast */
#include <algorithm>
#include <fstream>
#include <iterator>
using namespace std;
const unsigned int maxn = 100000;

unsigned int a[maxn + 1];
unsigned int n;

unsigned int query(const unsigned int x, const unsigned int y)
{
  unsigned int ret = 0;
  for (unsigned int i = 0; i < n; ++i)
    ret += (a[i] + x) & y;
  return ret;
}
int main(int argc, char* argv[])
{
  ifstream in(argv[1]);
  ofstream fans(argv[2]);
  unsigned int q;
  in >> n >> q;
  copy_n(istream_iterator<unsigned int>(in), n, a);
  for (unsigned int i = 0; i < q; ++i) {
    unsigned int op, x, y;
    in >> op >> x >> y;
    if (op == 1)
      a[x - 1] = y;
    else
      fans << query(x, y) << endl;
  }
  return 0;
}