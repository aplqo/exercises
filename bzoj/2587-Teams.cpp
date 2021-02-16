#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
using namespace std;
const unsigned int maxn = 1000000;
constexpr unsigned int inf = UINT_MAX / 2;

struct Child {
  unsigned int id, member;
} a[maxn + 10];
struct TeamFront {
  unsigned int end;
  bool valid = false;
} teml[maxn + 10];
vector<unsigned int> seq[maxn * 2 + 10];

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
static pair<unsigned int, unsigned int> dp(const unsigned int n)
{
  static unsigned int team[maxn + 10], maxTeam[maxn + 10];
  sortChild(n);
  for (unsigned int i = a[1].member + 1; i <= n; ++i)
    seq[a[i].member + i].push_back(i);
  team[a[1].member] = 1;
  maxTeam[a[1].member] = a[1].member;
  for (unsigned int i = a[1].member + 1; i <= n; ++i) {
    unsigned int mnF = inf, mnM = inf, from = 0;
    for (const unsigned int j : seq[i + 1])
      if (team[j - 1] == team[i - 1]) {
        const unsigned int tp = max(maxTeam[j - 1], i + 1 - j);
        if (tp < mnF) {
          mnF = tp;
          from = j;
        }
      }
      else if (team[j - 1] + 1 == team[i - 1])
        mnM = min(mnM, max(maxTeam[j - 1], i + 1 - j));
    if (from) {
      team[i] = team[i - 1] + 1;
      teml[from] = TeamFront{i, true};
      maxTeam[i] = mnF;
    }
    else {
      team[i] = team[i - 1];
      maxTeam[i] =
          min(mnM, maxTeam[i - 1] +
                       (1ull * team[i - 1] * (maxTeam[i - 1]) == i - 1));
    }
  }
  teml[1] = TeamFront{a[1].member, true};
  return make_pair(team[n], maxTeam[n]);
}

typedef vector<unsigned int> Team;
inline void printTeam(const Team& t)
{
  cout << t.size() << " ";
  copy(t.cbegin(), t.cend(), ostream_iterator<unsigned int>(cout, " "));
  cout.put('\n');
}
static void printSolution(const unsigned int n, const unsigned int tem,
                          const unsigned int mxT)
{
  Team *t = new Team[tem], *fnt = t, *ptr = t;
  for (unsigned int i = 1; i <= n; ++i) {
    if (teml[i].valid) {
      for_each(a + i, a + teml[i].end + 1,
               [ptr](const Child& i) { ptr->push_back(i.id); });
      ++ptr;
      i = teml[i].end;
      continue;
    }
    while (fnt->size() == mxT)
      printTeam(*(fnt++));
    fnt->push_back(a[i].id);
  }
  while (fnt < ptr)
    printTeam(*(fnt++));
  delete[] t;
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
  const auto [tem, mxt] = dp(n);
  cout << tem << "\n";
  printSolution(n, tem, mxt);
  return 0;
}