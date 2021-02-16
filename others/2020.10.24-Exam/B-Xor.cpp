#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1e6, maxl = 31;

struct Node {
  unsigned int siz = 0;
  Node* chl[2] = {};
} nod[maxn * (maxl + 1)];
Node* root = nod;
unsigned int a[maxn + 10];

unsigned int countXor(const unsigned int v, const unsigned int k)
{
  const Node* ptr = root;
  unsigned int ret = 0;
  for (unsigned int i = 1u << maxl; i && ptr; i >>= 1) {
    bool dir;
    if (k & i)
      dir = !(v & i);
    else {
      dir = v & i;
      ret += ptr->chl[!dir] ? ptr->chl[!dir]->siz : 0;
    }
    ptr = ptr->chl[dir];
  }
  return ret + (ptr ? ptr->siz : 0);
}
void insert(const unsigned int v)
{
  static Node* cur = nod;
  Node* ptr = root;
  for (unsigned int i = 1u << maxl; i; i >>= 1) {
    const bool dir = v & i;
    ++(ptr->siz);
    if (!ptr->chl[dir]) ptr->chl[dir] = ++cur;
    ptr = ptr->chl[dir];
  }
  ++(ptr->siz);
}
unsigned long long count(const unsigned int n, const unsigned int k)
{
  unsigned int acc = 0;
  unsigned long long ret = 0;
  insert(0);
  for (const unsigned int* i = a; i < a + n; ++i) {
    acc ^= *i;
    ret += countXor(acc, k);
    insert(acc);
  }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, k;
  cin >> n >> k;
  copy_n(istream_iterator<unsigned int>(cin), n, a);
  cout << count(n, k) << "\n";
  return 0;
}