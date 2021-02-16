#include <algorithm>
#include <deque>
#include <iostream>
using namespace std;
deque<int> mem;
unsigned int m, ans;

int main()
{
  int n, t;
  cin >> m >> n;
  for (int i = 0; i < n; i++) {
    cin >> t;
    if (find(mem.begin(), mem.end(), t) == mem.end()) {
      if (mem.size() >= m) mem.pop_front();
      ans++;
      mem.push_back(t);
    }
  }
  cout << ans;
  return 0;
}