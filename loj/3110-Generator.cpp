#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
#include <random>

#include "debug_tools/generator.h"
using namespace apdebug::generator;
const int maxa = 1e9;

void createOperate(std::ostream& os, const int n, const int q, const int mnv,
                   const int mxv)
{
  std::uniform_int_distribution<int> disp(1, n), disv(mnv, mxv), diso(1, 6);
  for (int i = q; i; --i) {
    const int op = diso(default_rnd);
    os << op;
    switch (op) {
      case 1:
        os << " " << disp(default_rnd) << " " << disv(default_rnd);
        break;
      case 5:
        os << " " << disp(default_rnd);
        break;
      case 6:
        break;
      default:
        os << " " << disv(default_rnd);
        break;
    }
    os.put('\n');
  }
}
void createRound(std::ostream& os, const int t)
{
  std::uniform_int_distribution<unsigned int> dis(0, maxa);
  for (int i = t; i; --i)
    os << dis(default_rnd) << " " << dis(default_rnd) << "\n";
}

int main(int, char* argv[])
{
  Init();
  const int n = GetRandom(1, atoi(argv[1])), q = GetRandom(1, atoi(argv[2])),
            t = GetRandom(1, atoi(argv[3]));
  std::ofstream in(argv[6]);
  in << n << " " << q << "\n";
  createOperate(in, n, q, atoi(argv[4]), atoi(argv[5]));
  in << t << "\n";
  createRound(in, t);
  return 0;
}