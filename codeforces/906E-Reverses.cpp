#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
const int maxN = 5e5, maxC = 26;
const int maxLength = maxN * 2;
const int inf = INT_MAX / 2;

struct ValFrom {
  int val, from;

  inline bool operator<(const ValFrom &r) const { return val < r.val; }
};
struct Node {
  Node *link, *slink, *next[maxC + 10];
  int diff, len;
  ValFrom min;
} nodes[maxLength + 10], oddRoot{&oddRoot, nullptr, {}, inf, -1, {inf, 0}},
    evenRoot{&oddRoot, nullptr, {}, inf, 0, {inf, 0}}, *cur = nodes;
int str[maxLength + 10];
ValFrom f[maxLength + 2];

Node *insertChar(Node *const last, const int ch, const int pos)
{
  Node *fa = last;
  while (str[pos - fa->len - 1] != ch)
    fa = fa->link;
  if (!fa->next[ch]) {
    Node *const ptr = cur++;
    Node *lk = fa->link;
    while (str[pos - lk->len - 1] != ch)
      lk = lk->link;
    lk = lk->next[ch] ? lk->next[ch] : &evenRoot;
    ptr->len = fa->len + 2;
    ptr->link = lk;
    ptr->diff = ptr->len - lk->len;
    ptr->slink = ptr->diff == lk->diff ? lk->slink : lk;
    fa->next[ch] = ptr;
  }
  return fa->next[ch];
}
int dp(const int n)
{
  std::fill(f, f + n + 1, ValFrom{inf, 0});
  f[0] = ValFrom{0, 0};
  Node *ptr = &evenRoot;
  for (int i = 1; i <= n; ++i) {
    ptr = insertChar(ptr, str[i], i);
    for (Node *j = ptr; j->len > 0; j = j->slink) {
      j->min = ValFrom{f[i - (j->slink->len + j->diff)].val,
                       i - (j->slink->len + j->diff)};
      if (j->link->diff == j->diff) j->min = std::min(j->min, j->link->min);
    }
    if (!(i & 0x01)) {
      if (str[i] == str[i - 1]) f[i] = ValFrom{f[i - 2].val, i - 2};
      for (const Node *j = ptr; j; j = j->slink)
        f[i] = std::min(f[i], ValFrom{j->min.val + 1, j->min.from});
    }
  }
  return f[n].val;
}
int buildString(const char s1[], const char s2[])
{
  const int len = std::strlen(s1);
  int *ptr = str + 1;
  for (int i = 0; i < len; ++i) {
    *(ptr++) = s1[i] + 1 - 'a';
    *(ptr++) = s2[i] + 1 - 'a';
  }
  return ptr - 1 - str;
}
void printSolution(const int x)
{
  if (!x) return;
  const int from = f[x].from;
  if (x - from > 2) std::cout << (from / 2 + 1) << " " << x / 2 << "\n";
  printSolution(from);
}

int main()
{
  std::ios::sync_with_stdio(false);
  static char s1[maxN + 10], s2[maxN + 10];
  std::cin >> s1 >> s2;
  const int size = buildString(s1, s2);
  const int ans = dp(size);
  if (ans == inf)
    std::cout << "-1\n";
  else {
    std::cout << ans << "\n";
    printSolution(size);
  }
  return 0;
}