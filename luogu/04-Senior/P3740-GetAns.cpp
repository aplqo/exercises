#include <algorithm>
#include <fstream>
using namespace std;
const unsigned int maxn = 1e7;

unsigned int dat[maxn + 1];

int main(int argc, char* argv[])
{
  ifstream in(argv[1]);
  ofstream ans(argv[2]);
  unsigned int n, m;
  in >> n >> m;
  for (unsigned int i = 1; i <= m; ++i) {
    unsigned int a, b;
    in >> a >> b;
    fill(dat + a, dat + b + 1, i);
  }
  sort(dat, dat + 1 + n);
  ans << (unique(dat, dat + 1 + n) - dat - 1) << endl;
  return 0;
}
