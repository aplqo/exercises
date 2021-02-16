#ifdef APDEBUG
#define APINPROG
#include "time.cpp"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
const int maxn = 70, maxl = 51;

struct stick {
  int len = 0, cnt = 0;
  stick *pre, *nxt;
} st[maxl + 1];
int n, per, num;

bool solve(stick *it, int k, int lst)
{
  if (k == num - 1) {
    cout << per << endl;
    return true;
  }
  int nk, nlst;
  stick *ni;
  for (; it > st; it = it->pre) {
    if (it->cnt == 0 || it->len > lst) continue;
    nk = k;
    nlst = lst - it->len;
    ni = it;
    --(it->cnt);
    if (nlst == 0) {
      ++nk;
      nlst = per;
      ni = st[maxl].pre;
    }
    if (solve(ni, nk, nlst)) return true;
    ++(it->cnt);
    if (lst == per) return false;
  }
  return false;
}
int main()
{
  cin >> n;
  for (int i = 0; i < n; ++i) {
    int t;
    cin >> t;
    st[t].cnt++;
  }
  for (int i = 0; i < maxl; ++i)
    st[i].len = i;
  for (stick *i = st; i < st + maxl;) {
    i->nxt = find_if(i + 1, st + maxl,
                     [](const stick &t) -> bool { return t.cnt != 0; });
    i->nxt->pre = i;
    i = i->nxt;
  }
  int b = st[maxl].pre->len, sum = 0;
  for (stick *i = st; i < st + maxl; i = i->nxt)
    sum += (i->cnt) * (i->len);
  for (per = b; per <= sum; ++per) {
    if (sum % per) continue;
    num = sum / per;
    if (solve(st[maxl].pre, 0, per)) break;
  }
  return 0;
}