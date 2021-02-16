#ifdef APDEBUG
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
using namespace std;
const int maxn = 5000;

unsigned int w[maxn + 10], n;
// clang-format off
priority_queue<unsigned int, vector<unsigned int>, greater<unsigned int> > heap;
// clang-format on

int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cin >> n;
  for (unsigned int* i = w; i < w + n; ++i)
    cin >> *i;
  sort(w, w + n);
  heap.push(1);
  for (unsigned int* i = w + 1; i < w + n; ++i) {
    if (heap.top() > *i)
      heap.push(1);
    else {
      unsigned int nxt = heap.top() + 1;
      heap.pop();
      heap.push(nxt);
    }
  }
  cout << heap.size() << endl;
  return 0;
}