#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
const unsigned int maxn = 300000, maxm = 300000, maxl = 25;

unsigned int ch[maxn * maxl + maxm * maxl + 1][2],
    siz[maxn * maxl + maxm * maxl + 1];
unsigned int root[maxn + maxm + 1], xsum[maxn + maxm + 1];

void insert(const unsigned int p, unsigned int old, const unsigned int v)
{
  static unsigned int cnt = 0;
  root[p] = ++cnt;
  unsigned int pc = root[p];
  for (int i = maxl; i >= 0; --i) {
    const bool t = v & (1 << i);
    ch[pc][!t] = ch[old][!t];
    ch[pc][t] = ++cnt;
    siz[pc] = siz[old] + 1;
    pc = ch[pc][t];
    old = ch[old][t];
  }
  siz[pc] = siz[old] + 1;
}
unsigned int query(unsigned int rl, unsigned int rr, const unsigned int val)
{
  if (rl == rr) return val;
  unsigned int ret = 0;
  for (int i = maxl; i >= 0; --i) {
    const bool b = val & (1 << i);
    if (siz[ch[rr][!b]] - siz[ch[rl][!b]]) {
      ret |= 1 << i;
      rl = ch[rl][!b];
      rr = ch[rr][!b];
    }
    else {
      rl = ch[rl][b];
      rr = ch[rr][b];
    }
  }
  return ret;
}
inline void add(const unsigned int p, const unsigned int x)
{
  xsum[p] = xsum[p - 1] ^ x;
  insert(p, root[p - 1], xsum[p]);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int i = 1; i <= n; ++i) {
    unsigned int a;
    cin >> a;
    add(i, a);
  }
  for (unsigned int i = 0; i < m; ++i) {
    char op;
    cin >> op;
    if (op == 'A') {
      unsigned int x;
      cin >> x;
      add(++n, x);
    }
    else {
      unsigned int l, r, x;
      cin >> l >> r >> x;
      cout << query(root[l >= 2 ? l - 2 : 0], root[r - 1], x ^ xsum[n]) << endl;
    }
  }
  return 0;
}