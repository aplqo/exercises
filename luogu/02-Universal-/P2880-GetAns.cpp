#include <algorithm>
#include <fstream>
#include <iterator>
using namespace std;
const unsigned int maxn = 50000;

unsigned int val[maxn + 1];

int main(int, char* argv[])
{
  ifstream in(argv[1]);
  ofstream ans(argv[2]);
  unsigned int n, q;
  in >> n >> q;
  copy_n(istream_iterator<unsigned int>(in), n, val + 1);
  for (unsigned int i = 0; i < q; ++i) {
    unsigned int a, b;
    in >> a >> b;
    ans << (*max_element(val + a, val + b + 1) -
            *min_element(val + a, val + b + 1))
        << endl;
  }
  return 0;
}