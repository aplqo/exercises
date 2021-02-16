#ifdef APDEBUG
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <queue>
#include <utility>
using namespace std;
typedef pair<int, int> stop;  // first distance,second amount
const int maxn = 10010;

stop s[maxn];
int n, l, p;
int ans;
priority_queue<int> heap;

int main()
{
  ios_base::sync_with_stdio(false);
  cin >> n;
  for (stop* i = s; i < s + n; ++i)
    cin >> i->first >> i->second;
  cin >> l >> p;
  for (stop* i = s; i < s + n; ++i)
    i->first = l - i->first;
  s[n] = make_pair(l, 0);
  ++n;
  sort(s, s + n);

  int lst = 0;
  for (stop* i = s; i < s + n; ++i) {
    p -= i->first - lst;
    if (p < 0) {
      while (p < 0 && !heap.empty()) {
        ++ans;
        p += heap.top();
        heap.pop();
      }
      if (p < 0) {
        cout << "-1" << endl;
        return 0;
      }
    }
    lst = i->first;
    heap.push(i->second);
  }
  cout << ans << endl;
  return 0;
}
