#include <cmath>
#include <iostream>
using namespace std;

const int a = 7;
int x, s;
double bef, aft;
int main()
{
  auto getS = [](int d) -> double {
    double t = 1 - ((double)d * 0.02) / a;
    return log(t) / log(0.98);
  };
  cin >> s >> x;
  aft = getS(s + x);
  bef = getS(s - x);
  if (aft - bef <= 1)
    cout << 'n';
  else
    cout << 'y';
  return 0;
}
