#include <iostream>
#include <utility>
using namespace std;
const int maxn = 100000;
typedef unsigned int num;

num a[maxn];

void QuickSort(int l, int r)
{
  int i = l, j = r;
  num m = a[(l + r) >> 1];  // Prevent from being swapped
  do {
    while (a[i] < m)
      i++;
    while (a[j] > m)
      j--;
    if (i <= j) {
      swap(a[i], a[j]);
      i++;
      j--;
    }
  } while (i <= j);
  if (l < j) QuickSort(l, j);
  if (i < r) QuickSort(i, r);
}
int main()
{
  int n;
  cin >> n;
  for (num *i = a; i < a + n; i++)
    cin >> *i;
  QuickSort(0, n - 1);
  for (num *i = a; i < a + n; i++)
    cout << *i << ' ';
  return 0;
}
