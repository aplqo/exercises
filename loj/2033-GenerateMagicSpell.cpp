#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <unordered_map>
using namespace std;
const unsigned int maxn = 2e5;

struct SAMNode {
  SAMNode* link;
  unordered_map<unsigned int, SAMNode*> next;
  unsigned int len;
} nodes[maxn * 2 + 10], root{};

unsigned int insertChar(const unsigned int c)
{
  static SAMNode *cur = nodes, *last = &root;
  SAMNode* const ptr = cur++;
  new (ptr) SAMNode{};
  ptr->len = last->len + 1;
  SAMNode* from = last;
  while (from && !from->next.count(c)) {
    from->next[c] = ptr;
    from = from->link;
  }
  if (!from)
    ptr->link = &root;
  else {
    SAMNode* const fNxt = from->next[c];
    if (fNxt->len == from->len + 1)
      ptr->link = fNxt;
    else {
      SAMNode* const cloned = cur++;
      *cloned = *fNxt;
      cloned->len = from->len + 1;
      for (SAMNode* i = from; i; i = i->link)
        if (const auto it = i->next.find(c);
            it != i->next.end() && it->second == fNxt)
          it->second = cloned;
        else
          break;
      fNxt->link = ptr->link = cloned;
    }
  }
  last = ptr;
  return ptr->len - ptr->link->len;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  unsigned long long ans = 0;
  cin >> n;
  for (unsigned int i = 0; i < n; ++i) {
    unsigned int c;
    cin >> c;
    ans += insertChar(c);
    cout << ans << "\n";
  }
  return 0;
}