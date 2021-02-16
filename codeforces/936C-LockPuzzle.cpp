#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
using namespace std;
const unsigned int maxn = 2000;

string s, t;
vector<unsigned int> ans;

void shift(const unsigned int x)
{
  if (!x) return;
  ans.push_back(x);
  reverse(s.end() - x, s.end());
  rotate(s.begin(), s.end() - x, s.end());
}
bool checkAnswer(const string& s, const string& t)
{
  static unsigned int cnt[2][26];
  for (unsigned int i = 0; i < s.length(); ++i) {
    ++cnt[0][s[i] - 'a'];
    ++cnt[1][t[i] - 'a'];
  }
  return equal(cnt[0], cnt[0] + 26, cnt[1]);
}
template <int d>
void buildAnswer(const int l, const int r)
{
  const int len = d * (r - l) + 1;
  if (len < 1)
    return;
  else if (len == 1) {
    shift(s.size() - s.find_last_of(t[l]) - 1);
    shift(1);
    return;
  }
  buildAnswer<(-d)>(r - d, l + d);
  {
    const unsigned int px = s.find_last_of(t[r]);
    shift(s.length() - px);
    if (px + 2 > len) shift(px + 2 - len);
  }
  shift(s.length() - s.find(t[l]) - 1);
  shift(1);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n >> s >> t;
  if (!checkAnswer(s, t)) {
    cout << "-1\n";
    return 0;
  }
  buildAnswer<1>(0, n - 1);
  cout << ans.size() << "\n";
  copy(ans.cbegin(), ans.cend(), ostream_iterator<unsigned int>(cout, " "));
  cout.put('\n');
  return 0;
}