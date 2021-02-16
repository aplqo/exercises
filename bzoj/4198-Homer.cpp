#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <utility>
using namespace std;
typedef unsigned long long num;
typedef pair<num, int> pairType;
typedef greater<pairType> cmp;
const int maxn = 1e5 + 5, maxk = 10;

int n, k;
num dat[maxn], dep[maxk * maxn];
pairType heap[maxn * 2], *en = heap;
int fa[maxn * maxk], c;

int GetFather(int x)
{
  if (fa[x] == x) return x;
  int f = GetFather(fa[x]);
  dep[x] += dep[fa[x]];
  return fa[x] = f;
}
inline void con(int from, int to)
{
  fa[from] = to;
  dep[from] = 1;
}

int main()
{
  ios_base::sync_with_stdio(false);
  cin >> n >> k;
  for (int i = 0; i < n; ++i) {
    cin >> heap[i].first;
    heap[i].second = i;
    dat[i] = heap[i].first;
  }
  {
    // init
    iota(fa, fa + n * (k + 1), 0);
    en = heap + n;
    int m = (n - 1) % (k - 1);
    if (m) {
      en += k - 1 - m;
      fill(heap + n, en, make_pair(0, 0));
    }
    make_heap(heap, en, cmp());
    c = n;
  }
  while (en - heap > 1) {
    num v = 0;
    for (int i = 0; i < k; ++i) {
      pairType cur = heap[0];
      pop_heap(heap, en, cmp());
      --en;
      if (cur.first == 0) continue;
      v += cur.first;
      con(cur.second, c);
    }
    *en = make_pair(v, c);
    ++en;
    push_heap(heap, en, cmp());
    ++c;
  }
  num ans = 0, d = 0;
  for (int i = 0; i < n; ++i) {
    GetFather(i);
    ans += dat[i] * dep[i];
    d = max(d, dep[i]);
  }
  cout << ans << endl << d << endl;
  return 0;
}