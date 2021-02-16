#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <deque>
#include <iostream>
#include <utility>
using namespace std;
const int maxn = 1e6;

class myqueue {
 public:
  void push(const int i, const int val)
  {
    while (!q.empty() && q.back().second < val)
      q.pop_back();
    q.emplace_back(i, val);
  }
  void pop(const int i)
  {
    while (!q.empty() && q.front().first < i)
      q.pop_front();
  }
  int front() const { return q.front().second; }

 private:
  deque<pair<int, int>> q;
};
int a[maxn * 2 + 10];

void init(const unsigned int n) { copy(a, a + n, a + n); }
int dp(const unsigned int n)
{
  int ans = 0;
  myqueue q;
  q.push(0, a[0]);
  for (int i = 1; i < n * 2; ++i) {
    q.pop(i - n / 2 + n % 2);
    ans = max(ans, q.front() + i + a[i]);
    q.push(i, a[i] - i);
  }
  return ans;
}

int main()
{
  unsigned int n;
  cin >> n;
  for (int* i = a; i < a + n; ++i)
    cin >> *i;
  init(n);
  cout << dp(n) << endl;
  return 0;
}