#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <functional>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
const unsigned int maxn = 3e5;

priority_queue<unsigned long long, vector<unsigned long long>,
               greater<unsigned long long>>
    q;

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  unsigned long long ans = 0;
  cin >> n;
  {
    unsigned int t;
    cin >> t;
    q.push(t);
  }
  for (unsigned int i = 1; i < n; ++i) {
    unsigned long long v;
    cin >> v;
    if (v > q.top()) {
      ans += v - q.top();
      q.pop();
      q.push(v);
      q.push(v);
    }
    else
      q.push(v);
  }
  cout << ans << endl;
  return 0;
}