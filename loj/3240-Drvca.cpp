#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
using namespace std;
const unsigned int maxn = 1e5;

int h[maxn + 10], dif[maxn + 10], epos;
vector<int> seq1, seq2;

static void init(const unsigned int n)
{
  sort(h + 1, h + 1 + n);
  adjacent_difference(h + 1, h + 1 + n, dif + 1);
  seq1.reserve(n);
  seq2.reserve(n);
  for (unsigned int i = n; i > 0; --i)
    if (dif[i] == dif[n])
      epos = i - 1;
    else
      break;
}
inline bool checkEnd(const int i, const unsigned int n, const int d2)
{
  switch (seq2.size()) {
    case 0:
      return true;
    case 1:
      return i - seq2.back() == dif[n];
    default:
      return i - seq2.back() == d2 && d2 == dif[n];
  }
}
template <unsigned int s1, unsigned int s2, unsigned int lst>
bool buildSeq(const unsigned int n)
{
  const int d1 = h[s2] - h[s1];
  seq1.clear();
  seq2.clear();
  seq1.push_back(h[s1]);
  if constexpr (lst < 3)
    seq2.push_back(h[lst]);
  else
    seq1.push_back(h[s2]);
  for (const int* i = h + 3; i < h + n + 1; ++i) {
    if (i >= h + epos && checkEnd(*i, n, seq2[1] - seq2[0])) {
      copy<decltype(i)>(i, h + n + 1, back_inserter(seq2));
      return true;
    }
    if (*i - seq1.back() == d1)
      seq1.push_back(*i);
    else {
      if (seq2.size() > 1 && *i - seq2.back() != seq2[1] - seq2[0])
        return false;
      seq2.push_back(*i);
    }
  }
  return true;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  copy_n(istream_iterator<int>(cin), n, h + 1);
  if (n < 3) {
    cout << "1\n" << h[1] << "\n";
    cout << "1\n" << h[2] << "\n";
    return 0;
  }
  init(n);
  if (buildSeq<1, 2, 3>(n) || buildSeq<1, 3, 2>(n) || buildSeq<2, 3, 1>(n)) {
    if (seq2.empty()) {
      seq2.push_back(seq1.back());
      seq1.pop_back();
    }
    cout << seq1.size() << "\n";
    copy(seq1.cbegin(), seq1.cend(), ostream_iterator<int>(cout, " "));
    cout.put('\n');
    cout << seq2.size() << "\n";
    copy(seq2.cbegin(), seq2.cend(), ostream_iterator<int>(cout, " "));
    cout.put('\n');
  }
  else
    cout << "-1\n";
  return 0;
}