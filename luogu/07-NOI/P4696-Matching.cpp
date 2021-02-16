#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
using namespace std;
const unsigned int maxn = 1000000;

struct ListNode {
  unsigned int val;
  ListNode *prev, *next;

  void remove()
  {
    prev->next = next;
    next->prev = prev;
  }
};
struct Near {
  unsigned int prev, next;
} np[maxn + 10];
unsigned int p[maxn + 10], h[maxn + 10];
unsigned int nxt[maxn + 10];

void initNear(const unsigned int n, unsigned int v[], Near res[])
{
  static ListNode nodes[maxn + 10];
  static unsigned int rev[maxn + 10];
  for (unsigned int i = 1; i <= n; ++i)
    rev[v[i]] = i;
  rev[0] = rev[n + 1] = 0;
  for (ListNode* i = nodes; i < nodes + 2 + n; ++i)
    *i = ListNode{static_cast<unsigned int>(i - nodes), i - 1, i + 1};
  nodes[0].prev = nodes[n + 1].next = nullptr;
  for (unsigned int i = n; i; --i) {
    ListNode* const p = nodes + rev[i];
    res[i] = Near{v[p->prev->val], v[p->next->val]};
    p->remove();
  }
  copy(rev, rev + n + 1, v);
}
inline bool match(const unsigned int prefix, const unsigned int cur,
                  const unsigned int cval[])
{
  const unsigned int* const base = cval + cur - prefix;
  const Near npre = np[prefix];
  return (!npre.prev || base[npre.prev] < cval[cur]) &&
         (!npre.next || cval[cur] < base[npre.next]);
}
void getNext(const unsigned int n)
{
  initNear(n, p, np);
  unsigned int j = 0;
  for (unsigned int i = 1; i <= n; ++i) {
    while (j && !match(j + 1, i + 1, p))
      j = nxt[j];
    if (match(j + 1, i + 1, p)) ++j;
    nxt[i + 1] = j;
  }
}
vector<unsigned int> kmp(const unsigned int m, const unsigned int n)
{
  vector<unsigned int> ret;
  ret.reserve(m);
  unsigned int k = 0;
  for (unsigned int i = 1; i <= m; ++i) {
    while (k && !match(k + 1, i, h))
      k = nxt[k];
    if (match(k + 1, i, h)) ++k;
    if (k == n) {
      ret.push_back(i + 1 - n);
      k = nxt[k];
    }
  }
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  copy_n(istream_iterator<unsigned int>(cin), n, p + 1);
  copy_n(istream_iterator<unsigned int>(cin), m, h + 1);
  getNext(n);
  const vector<unsigned int> v = kmp(m, n);
  cout << v.size() << "\n";
  copy(v.cbegin(), v.cend(), ostream_iterator<unsigned int>(cout, " "));
  cout.put('\n');
  return 0;
}