#include <algorithm>
#include <fstream>
using namespace std;
const int maxn = 1e6;

unsigned int val[maxn + 10];

int main(int argc, char* argv[])
{
  ifstream in(argv[1]);
  unsigned int n, m;
  in >> n >> m;
  for_each(val, val + n, [&in](unsigned int& i) -> void { in >> i; });

  ofstream ans(argv[2]);
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int a, b, c;
    in >> a >> b >> c;
    --a;
    auto p = find_if(val + a, val + b,
                     [&c](const unsigned int i) { return i <= c; });
    ans << (p == val + b ? -1 : p + 1 - val) << endl;
  }
  return 0;
}
