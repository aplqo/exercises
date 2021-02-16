#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;
const int maxn = 300;

struct course {
  void AddChild(course* c) { chl[++cnt] = c; }
  void dp(const unsigned int m)
  {
    for (unsigned int i = 1; i <= cnt; ++i)
      chl[i]->dp(m);
    memset(tmp, 0, sizeof(tmp));
    for (unsigned int i = 1; i <= cnt; ++i)
      for (unsigned int j = 1; j <= m; ++j) {
        tmp[i][j] = tmp[i - 1][j];
        for (unsigned int k = 0; k <= j; ++k)
          tmp[i][j] = max(tmp[i][j], tmp[i - 1][j - k] + chl[i]->f[k]);
      }
    count();
    for (unsigned int i = 1; i <= m; ++i)
      f[i] = tmp[cnt][i - 1] + s;
  }
  void count()
  {
    siz = 1;
    for (unsigned int i = 1; i <= cnt; ++i)
      this->siz += chl[i]->siz;
  }

  course* chl[maxn + 10];
  unsigned int cnt = 0, siz = 0;
  unsigned int s = 0, k;
  unsigned int f[maxn + 10];

 private:
  static unsigned int tmp[maxn + 10][maxn + 10];
} cl[maxn + 10];
unsigned int course::tmp[maxn + 10][maxn + 10];

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (course* i = cl + 1; i < cl + 1 + n; ++i) {
    cin >> i->k >> i->s;
    cl[i->k].AddChild(i);
  }
  cl[0].dp(m + 1);
  cout << cl[0].f[m + 1] << endl;
  return 0;
}