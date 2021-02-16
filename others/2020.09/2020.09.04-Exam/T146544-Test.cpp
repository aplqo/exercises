#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>

#include "debug_tools/tester.h"
using namespace std;
using apdebug::tester::ReadOutput;
using apdebug::tester::exceptions::ReadFail;
constexpr unsigned int prc = numeric_limits<long double>::digits10;

int main(int, char* argv[])
{
  ifstream out(argv[1]), ans(argv[2]);
  long double val;
  ans >> val;
  out.precision(prc);
  ans.precision(prc);
  try {
    const long double r = ReadOutput<long double>(out);
    if (fabs(r - val) > 1e-6) {
      cout << fixed << "Wrong answer. Read=" << r << " Expect=" << val << "\n";
      return 1;
    }
  }
  catch (ReadFail& e) {
    cout << "Wrong output format!\n";
    return 2;
  }
  return 0;
}