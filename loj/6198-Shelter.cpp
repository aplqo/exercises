#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
#include <numeric>
const int maxn = 1e5, maxl = 17;

char str[maxn + 10];
unsigned int w[maxn + 10];

namespace SA {
int height[maxn + 10], sa[maxn + 19], rank[maxn + 10];

void radixSort(const int l, const int rk[], const int tp[])
{
  static int buk[maxn + 10];
  std::memset(buk, 0, sizeof(buk));
  for (const int* i = tp + 1; i < tp + l + 1; ++i)
    ++buk[rk[*i]];
  std::partial_sum(buk + 1, buk + std::max(l, 26) + 1, buk + 1);
  for (const int* i = tp + l; i > tp; --i)
    sa[buk[rk[*i]]--] = *i;
}
void build(const char str[], const int n)
{
  static int tmp[maxn + 10];
  int *rk = rank, *tp = tmp;
  for (int i = 1; i <= n; ++i) {
    rk[i] = str[i] + 1 - 'a';
    tp[i] = i;
  }
  radixSort(n, rk, tp);
  for (int i = 1, p = 0; p < n; i <<= 1) {
    {
      int e = 1;
      for (int j = n + 1 - i; j <= n; ++j)
        tp[e++] = j;
      for (int j = 1; j <= n; ++j)
        if (sa[j] > i) tp[e++] = sa[j] - i;
    }
    radixSort(n, rk, tp);
    std::swap(rk, tp);
    rk[sa[1]] = p = 1;
    for (int j = 2; j <= n; ++j)
      if (tp[sa[j]] == tp[sa[j - 1]] && tp[sa[j] + i] == tp[sa[j - 1] + i])
        rk[sa[j]] = p;
      else
        rk[sa[j]] = ++p;
  }
  for (int i = 1; i <= n; ++i)
    rank[sa[i]] = i;
  for (int i = 1, k = 0; i <= n; ++i) {
    if (k) --k;
    while (str[i + k] == str[sa[rank[i] - 1] + k])
      ++k;
    height[rank[i]] = k;
  }
}
}  // namespace SA
namespace Trie {
struct Node {
  int cnt;
  const Node* child[2];
} nod[(maxn + 1) * (maxl + 10)];
const Node nil{0, {&nil, &nil}};
typedef const Node Root;

Root* insert(Root* old, const unsigned int x)
{
  static Node* cur = nod;
  const Node* ret = cur;
  Node* ptr = cur++;
  for (unsigned int msk = 1u << maxl; msk; msk >>= 1) {
    const bool d = x & msk;
    ptr->cnt = old->cnt + 1;
    ptr->child[!d] = old->child[!d];
    ptr->child[d] = cur;
    old = old->child[d];
    ptr = cur++;
  }
  ptr->cnt = old->cnt + 1;
  return ret;
}
unsigned int maxXor(Root* cur, Root* old, const unsigned int x)
{
  unsigned int ret = 0;
  for (unsigned int msk = 1u << maxl; msk; msk >>= 1) {
    bool d = x & msk;
    if (cur->child[!d]->cnt - old->child[!d]->cnt) {
      ret |= msk;
      d = !d;
    }
    cur = cur->child[d];
    old = old->child[d];
  }
  return ret;
}
}  // namespace Trie
Trie::Root* root[maxn + 10];
unsigned int ans;

unsigned int maxXor(const int vl, const int vr, Trie::Root* root,
                    Trie::Root* old)
{
  unsigned int ret = 0;
  for (int i = vl; i < vr; ++i)
    ret = std::max(ret, Trie::maxXor(root, old, w[SA::sa[i]]));
  return ret;
}
namespace CartesianTree {
struct Node {
  int l, r, size;
} nodes[maxn + 10];

int build(const int n, const int dat[])
{
  static int stk[maxn + 10];
  int *top = stk, root = 1;
  *(top++) = 1;
  for (int i = 2; i <= n; ++i) {
    int* ptr = top;
    while (ptr > stk && dat[*(ptr - 1)] > dat[i])
      --ptr;
    if (ptr > stk)
      nodes[*(ptr - 1)].r = i;
    else
      root = i;
    if (ptr < top) nodes[i].l = *ptr;
    top = ptr;
    *(top++) = i;
  }
  return root;
}
void dfs(const int x)
{
  if (!x) return;
  Node &cur = nodes[x], &lc = nodes[cur.l], &rc = nodes[cur.r];
  dfs(cur.l);
  dfs(cur.r);
  cur.size = lc.size + rc.size + 1;

  using SA::sa, SA::height;
  if (cur.l && cur.r) {
    if (lc.size < rc.size)
      ans = std::max(ans, height[x] + maxXor(x - lc.size - 1, x,
                                             root[x + rc.size], root[x - 1]));
    else
      ans = std::max(ans, height[x] + maxXor(x, x + rc.size + 1, root[x - 1],
                                             root[x - lc.size - 2]));
  }
  else if (cur.l)
    ans = std::max(
        ans,
        height[x] + Trie::maxXor(root[x - 1], root[x - 2 - lc.size], w[sa[x]]));
  else if (cur.r)
    ans = std::max(ans, height[x] + Trie::maxXor(root[x + rc.size], root[x - 1],
                                                 w[sa[x - 1]]));
  else
    ans = std::max(ans, height[x] + (w[sa[x]] ^ w[sa[x - 1]]));
}
}  // namespace CartesianTree

static int init(const int n)
{
  SA::build(str, n);
  root[0] = &Trie::nil;
  for (int i = 1; i <= n; ++i)
    root[i] = Trie::insert(root[i - 1], w[SA::sa[i]]);
  return CartesianTree::build(n, SA::height);
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n;
  std::cin >> n >> (str + 1);
  std::copy_n(std::istream_iterator<unsigned int>(std::cin), n, w + 1);
  CartesianTree::dfs(init(n));
  std::cout << ans << "\n";
  return 0;
}