#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <numeric>
using namespace std;

int main()
{
  unsigned int n, k;
  cin >> n >> k;
  const unsigned int g = gcd(n, k);
  n /= g;
  k /= g;
  cout << k << "/" << n << endl;
  return 0;
}