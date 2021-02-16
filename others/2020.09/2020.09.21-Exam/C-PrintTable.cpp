#include <algorithm>
#include <iostream>
#include <map>
using namespace std;
const unsigned int maxn = 1e6;

unsigned long long f(const unsigned long long x)
{
  static map<unsigned long long, unsigned long long> mem;
  if (x == 1) return 1;
  if (const auto it = mem.find(x); it != mem.end()) return it->second;
  unsigned long long acc = 1;
  for (unsigned long long i = 2; i * i <= x; ++i)
    if (x % i == 0) acc += f(i) + (x / i != i ? f(x / i) : 0);
  return mem[x] = x - acc;
}
int main()
{
  while (true) {
    unsigned long long v;
    cout << "print>" << flush;
    cin >> v;
    if (cin.fail()) break;
    cout << f(v) << endl;
  }
  return 0;
}