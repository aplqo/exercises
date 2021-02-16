#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
using namespace std;
const unsigned int maxk = 11, maxv = 1 << maxk, maxe = maxv * maxv;

bool mat[maxv + 10][maxv + 10];
unsigned int msk;
vector<unsigned int> seq;
unsigned int ans[maxv * 2 + 10];

void dfs(const unsigned int x)
{
  for (unsigned int i = 0; i < 2; ++i) {
    const unsigned int to = ((x << 1) | i) & msk;
    if (mat[x][to]) continue;
    mat[x][to] = true;
    dfs(to);
  }
  seq.push_back(x);
}
void GetSeq()
{
  unsigned int pos = 0;
  for (auto it = seq.rbegin(); it != seq.rend(); ++it) {
    ans[pos] = (*it) & 0x01;
    pos = (pos + 1) % (seq.size() - 1);
  }
}
unsigned int findLex(const unsigned int n)
{
  unsigned int pos = 0;
  copy(ans, ans + n, ans + n);
  for (unsigned int i = 0; i < n; ++i)
    if (lexicographical_compare(ans + i, ans + i + n, ans + pos, ans + pos + n))
      pos = i;
  return pos;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int k;
  cin >> k;
  msk = (1 << (k - 1)) - 1;
  dfs(0);
  GetSeq();
  const unsigned int p = findLex(seq.size() - 1);
  cout << seq.size() - 1 << " ";
  copy(ans + p, ans + p + seq.size() - 1, ostream_iterator<unsigned int>(cout));
  cout << "\n";
  return 0;
}