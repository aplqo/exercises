#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxn = 5e5, maxc = 26;

struct EERNode {
  EERNode *link, *next[maxc];
  int len;
  unsigned int count;
} nodes[maxn + 10]{}, *cur = nodes;
EERNode oddRoot{&oddRoot, {}, -1, 0}, evenRoot{&oddRoot, {}, 0, 0},
    *last = &evenRoot;
unsigned int str[maxn + 10] = {UINT_MAX};

void insertChar(const unsigned int ch, const int pos)
{
  EERNode* fa = last;
  str[pos] = ch;
  while (str[pos - fa->len - 1] != ch)
    fa = fa->link;
  if (!fa->next[ch]) {
    EERNode* const ptr = cur++;
    EERNode* lk = fa->link;
    while (str[pos - lk->len - 1] != ch)
      lk = lk->link;
    lk = lk->next[ch] ? lk->next[ch] : &evenRoot;
    ptr->link = lk;
    ptr->len = fa->len + 2;
    ptr->count = lk->count + 1;
    fa->next[ch] = ptr;
  }
  last = fa->next[ch];
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  static char str[maxn + 10];
  cin >> (str + 1);
  const size_t len = strlen(str + 1);
  unsigned int ans = 0;
  for (const char* i = str + 1; i < str + len + 1; ++i) {
#ifndef NoEncode
    insertChar((*i - 97 + ans) % 26, i - str);
#else
    insertChar(*i - 97, i - str);
#endif
    cout << (ans = last->count) << " ";
  }
  return 0;
}