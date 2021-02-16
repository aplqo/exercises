#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cstring>
#include <iostream>
#include <queue>
using namespace std;
const unsigned int maxn = 60, maxl = 100, maxc = 26;
const unsigned long long mod = 1e4 + 7;

unsigned int ch[maxn * maxl + 1][maxc], fail[maxn * maxl + 1];
unsigned int cur = 0;
unsigned long long f[maxn * maxl + 1][maxl + 1];
bool b[maxn * maxl + 1];

void add(const char* s)
{
  const size_t l = strlen(s);
  unsigned int pos = 0;
  for (unsigned int i = 0; i < l; ++i) {
    const unsigned int c = s[i] - 'A';
    if (!ch[pos][c]) ch[pos][c] = ++cur;
    pos = ch[pos][c];
  }
  b[pos] = true;
}
void make()
{
  queue<unsigned int> q;
  for (unsigned int i = 0; i < maxc; ++i)
    if (ch[0][i]) q.push(ch[0][i]);
  while (!q.empty()) {
    const unsigned int cur = q.front();
    q.pop();
    for (unsigned int i = 0; i < maxc; ++i) {
      if (!ch[cur][i])
        ch[cur][i] = ch[fail[cur]][i];
      else {
        fail[ch[cur][i]] = ch[fail[cur]][i];
        b[ch[cur][i]] |= b[fail[ch[cur][i]]];
        q.push(ch[cur][i]);
      }
    }
  }
}
unsigned long long quickPow(unsigned long long v, unsigned int e)
{
  unsigned long long ret = 1;
  for (; e; e >>= 1) {
    if (e & 0x01) ret = (ret * v) % mod;
    v = (v * v) % mod;
  }
  return ret;
}
unsigned long long dp(const unsigned int m)
{
  f[0][0] = 1;
  for (unsigned int j = 0; j < m; ++j)
    for (unsigned int i = 0; i <= cur; ++i) {
      if (b[i]) continue;
      for (unsigned int k = 0; k < maxc; ++k)
        if (!b[ch[i][k]])
          f[ch[i][k]][j + 1] = (f[ch[i][k]][j + 1] + f[i][j]) % mod;
    }
  unsigned long long ret = 0;
  for (unsigned int i = 0; i <= cur; ++i)
    ret = (ret + f[i][m]) % mod;
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int i = 0; i < n; ++i) {
    char str[maxl + 10];
    cin >> str;
    add(str);
  }
  make();
  cout << (quickPow(26, m) + mod - dp(m)) % mod << endl;
  return 0;
}