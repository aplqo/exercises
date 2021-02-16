#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
using namespace std;

struct stu {
  char name[21];
  int py, et, at;
  long long sum;
  char west, gb;
} num[100];

int main()
{
  int n;
  cin >> n;
  stu *ans = num;
  for (stu *i = num; i < num + n; i++) {
    cin >> i->name >> i->et >> i->py;
    cin >> i->gb >> i->west >> i->at;
    i->sum = 0;
    if (i->et > 80 && i->at >= 1) i->sum += 8000;
    if (i->et > 85 && i->py > 80) i->sum += 4000;
    if (i->et > 90) i->sum += 2000;
    if (i->et > 85 && i->west == 'Y') i->sum += 1000;
    if (i->py > 80 && i->gb == 'Y') i->sum += 850;
    if (i->sum > ans->sum) ans = i;
  }
  cout << ans->name << endl << ans->sum << endl;
  cout << accumulate(num, num + n, 0ll,
                     [](long long a, const stu &b) { return a + b.sum; });
  return 0;
}
