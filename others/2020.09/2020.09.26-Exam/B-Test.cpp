#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "debug_tools/tester.h"
using namespace std;
using namespace apdebug::tester;

int main(int, char* argv[])
{
  ifstream out(argv[1]), fans(argv[2]);
  long double ans;
  fans >> setprecision(10) >> ans;
  out.precision(10);
  try {
    const long double v = ReadOutput<long double>(out);
    if (fabs(v - ans) > 1e-6) {
      cout.precision(10);
      cout << "Wrong answer. Read=" << v << " Expect=" << ans << "\n";
      return 2;
    }
  }
  catch (exceptions::ReadFail& e) {
    cout << "Output invalid!\n";
    return 1;
  }
  return 0;
}