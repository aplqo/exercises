#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;
const unsigned int maxn = 1000000;
constexpr long long ninf = INT_MIN / 2, inf = INT_MAX / 2;

struct Child {
  unsigned int id, member;
} a[maxn + 10];
vector<unsigned int> seq[maxn * 2 + 10];
long long team[maxn + 10], maxTeam[maxn + 10];

static void sortChild(const unsigned int n)
{
  static Child tmp[maxn + 10];
  static unsigned int buk[maxn + 10];
  copy(a + 1, a + n + 1, tmp + 1);
  for (unsigned int i = 1; i <= n; ++i)
    ++buk[a[i].member];
  for (unsigned int i = n; i; --i)
    buk[i] += buk[i + 1];
  for (unsigned int i = n; i; --i)
    a[buk[tmp[i].member]--] = tmp[i];
}
static unsigned int dp(const unsigned int n)
{
  sortChild(n);
  for (unsigned int i = 1; i <= n; ++i)
    seq[a[i].member + i].push_back(i);
  fill(team + 1, team + 1 + n, ninf);
  fill(maxTeam + 1, maxTeam + 1 + n, inf);
  maxTeam[0] = ninf;
  team[a[1].member] = 1;
  maxTeam[a[1].member] = a[1].member;
  for (unsigned int i = a[1].member + 1; i <= n; ++i) {
    bool found = false;
    for (const unsigned int j : seq[i + 1])
      if (j <= i && team[j - 1] == team[i - 1]) {
        maxTeam[i] = min(maxTeam[i], max<long long>(maxTeam[j - 1], i + 1 - j));
        found = true;
      }
    if (found)
      team[i] = team[i - 1] + 1;
    else {
      team[i] = team[i - 1];
      maxTeam[i] = maxTeam[i - 1] + (team[i - 1] * maxTeam[i - 1] == i - 1);
    }
  }
  return team[n];
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  for (unsigned int i = 1; i <= n; ++i) {
    cin >> a[i].member;
    a[i].id = i;
  }
  cout << dp(n) << "\n";
  return 0;
}