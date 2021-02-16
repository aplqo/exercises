#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;
const unsigned int maxn = 1e6;

struct SAMNode {
  SAMNode* link;
  unordered_map<unsigned int, SAMNode*> next;
  vector<SAMNode*> chl;
  unsigned int len, count;
} nodes[maxn * 3 + 10]{}, *cur = nodes, root{}, *last = &root;

void insertChar(const unsigned int c)
{
  SAMNode *const ptr = cur++, *from = last;
  new (ptr) SAMNode{nullptr, {}, {}, last->len + 1, 1};
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
      cloned->count = 0;
      for (SAMNode* i = from; i; i = i->link) {
        const auto it = i->next.find(c);
        if (it != i->next.end() && it->second == fNxt)
          it->second = cloned;
        else
          break;
      }
      fNxt->link = ptr->link = cloned;
    }
  }
  last = ptr;
}
void buildTree()
{
  for (SAMNode* i = nodes; i < cur; ++i)
    i->link->chl.push_back(i);
}
unsigned long long dfs(SAMNode* const x)
{
  unsigned long long ret = 0;
  for (auto i : x->chl) {
    ret = max(ret, dfs(i));
    x->count += i->count;
  }
  if (x->count > 1)
    ret = max(ret, x->count * static_cast<unsigned long long>(x->len));
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  static char str[maxn + 10];
  cin >> str;
  const size_t len = strlen(str);
  for (const char* i = str; i < str + len; ++i)
    insertChar(*i - 'a');
  buildTree();
  cout << dfs(&root) << "\n";
  return 0;
}