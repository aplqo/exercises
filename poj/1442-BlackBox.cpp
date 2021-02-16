#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <functional>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
typedef int num;
const int maxm = 200005;

priority_queue<num> gheap;
priority_queue<num, vector<num>, greater<num>> lheap;
num a[maxm];
int u[maxm], n, m;

inline void add(num x)
{
  lheap.push(x);
  if (gheap.empty()) return;
  while (gheap.top() > lheap.top()) {
    num tl = lheap.top(), tg = gheap.top();
    gheap.pop();
    lheap.pop();
    lheap.push(tg);
    gheap.push(tl);
  }
}
inline num get()
{
  num t = lheap.top();
  gheap.push(t);
  lheap.pop();
  return t;
}

int main()
{
  ios_base::sync_with_stdio(false);
  cin >> n >> m;
  for (num* i = a + 1; i < a + 1 + n; ++i)
    cin >> *i;
  for (int* i = u; i < u + m; ++i)
    cin >> *i;
  int* pos = u;
  for (int i = 1; i <= n; ++i) {
    add(a[i]);
    while (pos < u + m && i == *pos) {
      cout << get() << endl;
      ++pos;
    }
  }
  return 0;
}
