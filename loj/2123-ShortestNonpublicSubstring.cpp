#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
using namespace std;
const unsigned int maxn = 2000, maxc = 26;

template <class T>
struct NodeBase {
  unsigned int id;
  T* next[maxc];
};
class SAM {
 public:
  struct Node : public NodeBase<Node> {
    unsigned int len;
    Node* link;
  } nodes[maxn * 2 + 10]{};
  Node* root = nodes;

  void build(const char buf[], const size_t len);

 private:
  Node *last = root, *cur = nodes + 1;

  void insertChar(const unsigned int ch);
} samA, samB;
class SeqAM {
 public:
  struct Node : public NodeBase<Node> {
  } nodes[maxn + 10];
  Node* root = nodes;

  void build(const char buf[], const size_t len);

 private:
  Node* cur = nodes + 1;
} sqA, sqB;

void SAM::build(const char buf[], const size_t len)
{
  for (const char* i = buf; i < buf + len; ++i)
    insertChar(*i - 'a');
}
void SAM::insertChar(const unsigned int ch)
{
  Node* const ptr = cur++;
  ptr->id = ptr - nodes;
  Node* from = last;
  while (from && !from->next[ch]) {
    from->next[ch] = ptr;
    from = from->link;
  }
  if (!from)
    ptr->link = root;
  else {
    Node* const fNxt = from->next[ch];
    if (fNxt->len == from->len + 1)
      ptr->link = fNxt;
    else {
      Node* const cloned = cur++;
      *cloned = *fNxt;
      cloned->id = cloned - nodes;
      cloned->len = from->len + 1;
      for (Node* i = from; i && i->next[ch] == fNxt; i = i->link)
        i->next[ch] = cloned;
      fNxt->link = ptr->link = cloned;
    }
  }
  last = ptr;
}
void SeqAM::build(const char buf[], const size_t len)
{
  Node* last[maxc]{};
  for (const char* i = buf + len - 1; i >= buf; --i) {
    Node* const ptr = cur++;
    ptr->id = ptr - nodes;
    copy(last, last + maxc, ptr->next);
    last[*i - 'a'] = ptr;
  }
  copy(last, last + maxc, root->next);
  root->id = 0;
}
template <class T, class U>
struct State {
  NodeBase<typename T::Node>* pa;
  NodeBase<typename U::Node>* pb;
  unsigned int dist;
};
template <class T, class U>
int bfs(const T& ma, const U& mb)
{
  static bool vis[maxn * 2 + 10][maxn * 2 + 10];
  queue<State<T, U>> q;
  q.push(State<T, U>{ma.root, mb.root, 0});
  vis[0][0] = true;
  while (!q.empty()) {
    const State<T, U> cur = q.front();
    q.pop();
    const auto pa = cur.pa, pb = cur.pb;
    for (unsigned int i = 0; i < maxc; ++i) {
      const auto npa = pa->next[i], npb = pb->next[i];
      if (!npa) continue;
      if (!npb) return cur.dist + 1;
      if (!vis[npa->id][npb->id]) {
        vis[npa->id][npb->id] = true;
        q.push(State<T, U>{npa, npb, cur.dist + 1});
      }
    }
  }
  return -1;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  string sa, sb;
  cin >> sa >> sb;
  samA.build(sa.c_str(), sa.size());
  sqA.build(sa.c_str(), sa.size());
  samB.build(sb.c_str(), sb.size());
  sqB.build(sb.c_str(), sb.size());
  cout << bfs(samA, samB) << "\n"
       << bfs(samA, sqB) << "\n"
       << bfs(sqA, samB) << "\n"
       << bfs(sqA, sqB) << "\n";
  return 0;
}