#include <algorithm>
#include <iostream>
using namespace std;
#define it(v, n) v + 1, v + 1 + n

struct c {
  int num, pos;
} col[1001], row[1001];
int m, n, k, l;
void fun(c ar[], int num)
{
  int t = 1;
  for (c *i = ar + 1; i <= ar + num; i++, t++)
    i->pos = t;
}
int main()
{
  int d, x, y, i, j;
  auto cnum = [](const c &a, const c &b) -> bool { return a.num > b.num; };
  auto cpos = [](const c &a, const c &b) -> bool { return a.pos < b.pos; };
  auto pri = [](const c &a) { cout << a.pos << ' '; };
  cin >> m >> n >> k >> l >> d;
  fun(col, m);
  fun(row, n);
  for (int t = 0; t < d; t++) {
    cin >> y >> x >> j >> i;
    if (x == i)
      row[min(y, j)].num++;
    else if (y == j)
      col[min(x, i)].num++;
  }
  sort(it(col, m), cnum);
  sort(it(col, l), cpos);
  sort(it(row, n), cnum);
  sort(it(row, k), cpos);
  for_each(it(row, k), pri);
  cout << endl;
  for_each(it(col, l), pri);
  return 0;
}
