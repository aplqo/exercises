#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <cmath>
#include <iostream>

int main()
{
  unsigned int a, b;
  std::cin >> a >> b;
  if (a < b) std::swap(a, b);
  std::cout << (static_cast<unsigned int>((sqrt(5) + 1) * (a - b) / 2) != b)
            << "\n";
  return 0;
}