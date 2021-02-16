#include <iostream>
using std::cin;
using std::cout;
constexpr float d = 0;
int main()
{
  int a;
  cin >> a;
  float walk, bike, t;
  walk = a / 1.2;
  bike = 27 + 23 + a / 3.0;
  t = walk - bike;
  if (t > d)
    cout << "Bike";
  else if (t < (0 - d))
    cout << "Walk";
  else
    cout << "All";
  return 0;
}
