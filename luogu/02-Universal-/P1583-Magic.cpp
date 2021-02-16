#include <algorithm>
#include <iostream>
using namespace std;
const int maxn = 20005;

struct per {
  int w, n;
} peo[maxn];
int e[11];

int main()
{
  int n, k;
  const auto cmp = [](const per &a, const per &b) {
    return a.w != b.w ? a.w > b.w : a.n < b.n;
  };
  cin >> n >> k;
  for (int *i = e + 1; i < e + 11; i++)
    cin >> *i;
  for (int t = 1; t < n + 1; ++t)
    peo[t].n = t;
  for (per *i = peo + 1; i < peo + n + 1; i++)
    cin >> i->w;
  sort(peo + 1, peo + n + 1, cmp);
  for (int i = 1; i < n + 1; i++)
    peo[i].w += e[(i - 1) % 10 + 1];
  sort(peo + 1, peo + n + 1, cmp);
  for (per *i = peo + 1; i < peo + 1 + k; i++)
    cout << i->n << ' ';
  return 0;
}